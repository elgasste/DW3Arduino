#include "game.h"

#define DIAGONAL_SCALAR 0.707f

internal void Game_HandleInput( Game_t* game );
internal void Game_HandlePlayerMoved( Game_t* game );
internal void Game_IncrementDaylightFactor( Game_t* game );
internal void Game_SteppedOnTile( Game_t* game, u32 tileIndex );
internal void Game_EnterPortal( Game_t* game, Portal_t* portal );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   AnimationChain_Init( &game->animationChain, &game->screen );
   TileMap_Init( &game->tileMap );

   TileMap_LoadFromIndex( &game->tileMap, 0 );

   game->tileMap.viewport.w = SCREEN_WIDTH;
   game->tileMap.viewport.h = SCREEN_HEIGHT;
   game->tileMap.viewportScreenPos.x = 0;
   game->tileMap.viewportScreenPos.y = 0;

   Game_LoadPlayerSprites( game );

   game->player.entity = game->tileMap.entities;
   game->player.entity->sprite = game->tileMap.playerSprites;
   game->player.entity->sprite->direction = Direction_Down;
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

   ActiveSprite_SetDirection( game->player.entity->sprite, Direction_Down );
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
      TileMap_ClampViewportToEntity( &game->tileMap, game->player.entity );
   }

   Render_DrawGame( game );
}

internal void Game_HandleInput( Game_t* game )
{
   Entity_t* entity = game->player.entity;
   ActiveSprite_t* sprite = game->player.entity->sprite;
   r32 velocity = TileMap_GetTileVelocity( &game->tileMap, game->player.tileIndex );

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

      if ( !( upIsDown && sprite->direction == Direction_Up ) &&
           !( downIsDown && sprite->direction == Direction_Down ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Left );
      }

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= DIAGONAL_SCALAR;
      }
   }
   else if ( rightIsDown && !leftIsDown )
   {
      entity->velocity.x = velocity;

      if ( !( upIsDown && sprite->direction == Direction_Up ) &&
           !( downIsDown && sprite->direction == Direction_Down ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Right );
      }

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= DIAGONAL_SCALAR;
      }
   }
   if ( upIsDown && !downIsDown )
   {
      entity->velocity.y = -velocity;

      if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
           !( rightIsDown && sprite->direction == Direction_Right ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Up );
      }

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= DIAGONAL_SCALAR;
      }
   }
   else if ( downIsDown && !upIsDown )
   {
      entity->velocity.y = velocity;

      if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
           !( rightIsDown && sprite->direction == Direction_Right ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Down );
      }

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
      Game_IncrementDaylightFactor( game );
   }

   tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap,
                                               (u32)( game->player.entity->pos.x + ( game->player.entity->pos.w / 2 ) ),
                                               (u32)( game->player.entity->pos.y + ( game->player.entity->pos.h / 2 ) ) );

   if ( tileIndex != game->player.tileIndex )
   {
      Game_SteppedOnTile( game, tileIndex );
   }
}

internal void Game_IncrementDaylightFactor( Game_t* game )
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
      AnimationChain_Reset( &game->animationChain );
      AnimationChain_PushWithCallback( &game->animationChain, AnimationType_ActiveFadeOut, TILEMAP_SWAP_FADE_SECONDS, Game_EnterPortal, game, foundPortal );
      AnimationChain_Push( &game->animationChain, AnimationType_Pause, TILEMAP_SWAP_PAUSE_SECONDS );
      AnimationChain_Push( &game->animationChain, AnimationType_ActiveFadeIn, TILEMAP_SWAP_FADE_SECONDS );
      AnimationChain_Start( &game->animationChain );
   }
}

internal void Game_EnterPortal( Game_t* game, Portal_t* portal )
{
   u32 i, newPosX, newPosY;
   u32 destTileMapIndex = portal->destTileMapIndex;
   u32 destTileIndex = portal->destTileIndex;

   TileMap_LoadFromIndex( &game->tileMap, destTileMapIndex );
   TileMap_GetPositionOfTileIndex( &game->tileMap, destTileIndex, &newPosX, &newPosY );
   game->player.tileIndex = destTileIndex;
   TileMap_CenterEntityOnTile( &game->tileMap, game->player.entity, destTileIndex );
   TileMap_ClampViewportToEntity( &game->tileMap, game->player.entity );

   for ( i = 0; i < game->tileMap.playerSpriteCount; i++ )
   {
      ActiveSprite_SetDirection( game->tileMap.playerSprites + i, portal->destDirection );
   }
}
