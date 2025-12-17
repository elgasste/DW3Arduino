#include "game.h"

#define DIAGONAL_SCALAR 0.707f

internal void Game_HandleInput( Game_t* game );
internal void Game_HandlePlayerMoved( Game_t* game );
internal void Game_IncrementDayFactor( Game_t* game );
internal void Game_SteppedOnTile( Game_t* game, u32 tileIndex );
internal void Game_EnterPortal( Game_t* game, Portal_t* portal );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   AnimationChain_Init( &game->animationChain, &game->screen );
   TileMap_Init( &game->tileMap );

   Screen_LoadPaletteFromIndex( &game->screen, 0 );
   TileMap_LoadFromIndex( &game->tileMap, 0 );

   game->tileMap.viewport.w = 380;
   game->tileMap.viewport.h = 220;
   game->tileMap.viewportScreenPos.x = 20;
   game->tileMap.viewportScreenPos.y = 10;

   game->player.entity = &game->tileMap.entities[0];
   game->player.entity->pos.x = 2722.0f;
   game->player.entity->pos.y = 3538.0f;
   game->player.entity->pos.w = 12.0f;
   game->player.entity->pos.h = 12.0f;
   game->player.entity->prevPos = game->player.entity->pos;
   game->player.entity->velocity.x = 0.0f;
   game->player.entity->velocity.y = 0.0f;
   game->player.tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap,
                                                            (u32)game->player.entity->pos.x,
                                                            (u32)game->player.entity->pos.y );

   TileMap_ClampViewportToEntity( &game->tileMap, game->player.entity );

   game->isAM = False;
   game->daylightFactor = 1.0f; // noon
   game->screen.dayFilterIntensity = 1.0f;

   game->playerMovedCallback = Game_HandlePlayerMoved;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &game->input );

   if ( game->animationChain.isRunning )
   {
      AnimationChain_Tic( &game->animationChain );
   }

   if ( !AnimationChain_BlocksInput( &game->animationChain ) )
   {
      Game_HandleInput( game );
   }
   
   if ( !AnimationChain_PausesAction( &game->animationChain ) )
   {
      TileMap_Tic( &game->tileMap );
      Physics_Tic( game );
   }

   Render_DrawGame( game );
}

internal void Game_HandleInput( Game_t* game )
{
   r32 velocity = PLAYER_MAX_VELOCITY;
   Entity_t* entity = game->player.entity;

#if defined( VISUAL_STUDIO_DEV )
   if ( g_winDebugFlags.fastWalk )
   {
      velocity = 256;
   }
#endif

   Bool_t leftIsDown = game->input.buttonStates[InputButton_Left].down;
   Bool_t upIsDown = game->input.buttonStates[InputButton_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[InputButton_Right].down;
   Bool_t downIsDown = game->input.buttonStates[InputButton_Down].down;

   if ( leftIsDown && !rightIsDown )
   {
      entity->velocity.x = -velocity;

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= DIAGONAL_SCALAR;
      }
   }
   else if ( rightIsDown && !leftIsDown )
   {
      entity->velocity.x = velocity;

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= DIAGONAL_SCALAR;
      }
   }
   if ( upIsDown && !downIsDown )
   {
      entity->velocity.y = -velocity;

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= DIAGONAL_SCALAR;
      }
   }
   else if ( downIsDown && !upIsDown )
   {
      entity->velocity.y = velocity;

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= DIAGONAL_SCALAR;
      }
   }
}

internal void Game_HandlePlayerMoved( Game_t* game )
{
   u32 tileIndex;

   if ( game->tileMap.affectsDaylight )
   {
      Game_IncrementDayFactor( game );
   }

   tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap,
                                               (u32)( game->player.entity->pos.x + ( game->player.entity->pos.w / 2 ) ),
                                               (u32)( game->player.entity->pos.y + ( game->player.entity->pos.h / 2 ) ) );

   if ( tileIndex != game->player.tileIndex )
   {
      Game_SteppedOnTile( game, tileIndex );
   }

}

internal void Game_IncrementDayFactor( Game_t* game )
{
   game->daylightFactor += game->isAM
      ? ( 1 / ( DAY_FACTOR_TOTAL_SECONDS * (r32)CLOCK_FPS ) )
      : -( 1 / ( DAY_FACTOR_TOTAL_SECONDS * (r32)CLOCK_FPS ) );

   if ( game->daylightFactor > 1.0f )
   {
      game->daylightFactor = 1.0f;
      game->isAM = False;
   }
   else if ( game->daylightFactor < 0.0f )
   {
      game->daylightFactor = 0.0f;
      game->isAM = True;
   }

   // use cutoffs to determine when the sun rises and sets
   if ( game->daylightFactor < DAY_FACTOR_LOW_CUTOFF )
   {
      game->screen.dayFilterIntensity = 0.0f;
   }
   else if ( game->daylightFactor > DAY_FACTOR_HIGH_CUTOFF )
   {
      game->screen.dayFilterIntensity = 1.0f;
   }
   else
   {
      game->screen.dayFilterIntensity = ( game->daylightFactor - DAY_FACTOR_LOW_CUTOFF ) / ( DAY_FACTOR_HIGH_CUTOFF - DAY_FACTOR_LOW_CUTOFF );
   }
}

internal void Game_SteppedOnTile( Game_t* game, u32 tileIndex )
{
   u32 i;
   Portal_t *checkPortal, *foundPortal = 0;

   game->player.tileIndex = tileIndex;

   // check regular portals first
   for ( i = 0, checkPortal = game->tileMap.portals; i < game->tileMap.portalCount; i++, checkPortal++ )
   {
      if ( checkPortal->sourceTileIndex == tileIndex )
      {
         foundPortal = checkPortal;
         break;
      }
   }

   // now check for edge portals
   if ( !foundPortal && game->tileMap.hasEdgePortal && TileMap_TileIndexIsEdgeTile( &game->tileMap, tileIndex ) )
   {
      foundPortal = &game->tileMap.edgePortal;
   }

   if ( foundPortal )
   {
      // TODO: use some #defines for these values
      AnimationChain_Reset( &game->animationChain );
      AnimationChain_PushWithCallback( &game->animationChain, AnimationType_ActiveFadeOut, 0.5f, Game_EnterPortal, game, foundPortal );
      AnimationChain_Push( &game->animationChain, AnimationType_Pause, 0.2f );
      AnimationChain_Push( &game->animationChain, AnimationType_ActiveFadeIn, 0.5f );
      AnimationChain_Start( &game->animationChain );
   }
}

internal void Game_EnterPortal( Game_t* game, Portal_t* portal )
{
   u32 newPosX, newPosY;
   u32 destTileMapIndex = portal->destTileMapIndex;
   u32 destTileIndex = portal->destTileIndex;

   TileMap_LoadFromIndex( &game->tileMap, destTileMapIndex );
   TileMap_GetPositionOfTileIndex( &game->tileMap, destTileIndex, &newPosX, &newPosY );
   game->player.entity->pos.x = (r32)newPosX + ( ( TILEMAP_TILE_SIZE - game->player.entity->pos.w ) / 2 );
   game->player.entity->pos.y = (r32)newPosY + ( ( TILEMAP_TILE_SIZE - game->player.entity->pos.h ) / 2 );
   game->player.tileIndex = destTileIndex;
   TileMap_ClampViewportToEntity( &game->tileMap, game->player.entity );
}
