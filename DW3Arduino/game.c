#include "game.h"
#include "random.h"

#define DIAGONAL_SCALAR 0.707f

internal u32 Game_GetPlayerCount( Game_t* game );
internal void Game_HandleInput( Game_t* game );
internal void Game_HandlePlayerMoved( Game_t* game );
internal void Game_AnchorRearPlayers( Game_t* game );
internal void Game_IncrementDaylightFactor( Game_t* game );
internal void Game_SteppedOnTile( Game_t* game, u32 tileIndex );
internal Bool_t Game_TryEnterPortal( Game_t* game );
internal void Game_EnterPortal( Game_t* game, Portal_t* portal );
internal void Game_ApplyTileDamage( Game_t* game );
internal Bool_t Game_TryEncounter( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   AnimationChain_Init( &game->animationChain, &game->screen );
   TileMap_Init( &game->tileMap, game->players, &Game_GetPlayerCount, game );

   game->tileMap.viewport.w = SCREEN_WIDTH;
   game->tileMap.viewport.h = SCREEN_HEIGHT;
   game->tileMap.viewportScreenPos.x = 0;
   game->tileMap.viewportScreenPos.y = 0;

   game->playerMovedCallback = Game_HandlePlayerMoved;

   Game_Reset( game );
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
      TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );
   }

   Render_DrawGame( game );
}

internal u32 Game_GetPlayerCount( Game_t* game )
{
   return game->playerCount;
}

internal void Game_HandleInput( Game_t* game )
{
   Entity_t* entity = game->players->entity;
   ActiveSprite_t* sprite = game->players->entity->sprite;
   r32 velocity = TileMap_GetTileVelocity( &game->tileMap, game->players->tileIndex );

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
   Player_t* frontPlayer = game->players;

   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].newPos.x = frontPlayer->entity->pos.x;
   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].newPos.y = frontPlayer->entity->pos.y;
   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].newDir = frontPlayer->entity->sprite->direction;
   frontPlayer->moveHistoryIndex++;

   if ( frontPlayer->moveHistoryIndex >= PLAYER_MOVE_HISTORY_SIZE )
   {
      frontPlayer->chainNextPlayer = True;
      frontPlayer->moveHistoryIndex = 0;
   }

   Game_AnchorRearPlayers( game );

   if ( game->tileMap.affectsDaylight )
   {
      Game_IncrementDaylightFactor( game );
   }

   tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap,
                                               (u32)( game->players->entity->pos.x + ( game->players->entity->pos.w / 2 ) ),
                                               (u32)( game->players->entity->pos.y + ( game->players->entity->pos.h / 2 ) ) );

   if ( tileIndex != game->players->tileIndex )
   {
      Game_SteppedOnTile( game, tileIndex );
   }
}

internal void Game_AnchorRearPlayers( Game_t* game )
{
   u32 i;
   Player_t *player, *prevPlayer = game->players;

   for ( i = 1; i < game->playerCount; i++ )
   {
      if ( !prevPlayer->chainNextPlayer )
      {
         break;
      }

      player = game->players + i;

      player->entity->pos.x = prevPlayer->moveHistory[prevPlayer->moveChainIndex].newPos.x;
      player->entity->pos.y = prevPlayer->moveHistory[prevPlayer->moveChainIndex].newPos.y;
      ActiveSprite_SetDirection( player->entity->sprite, prevPlayer->moveHistory[prevPlayer->moveChainIndex].newDir );
      prevPlayer->moveChainIndex++;

      if ( prevPlayer->moveChainIndex >= PLAYER_MOVE_HISTORY_SIZE )
      {
         player->chainNextPlayer = True;
         prevPlayer->moveChainIndex = 0;
      }

      player->moveHistory[player->moveHistoryIndex].newPos.x = player->entity->pos.x;
      player->moveHistory[player->moveHistoryIndex].newPos.y = player->entity->pos.y;
      player->moveHistory[player->moveHistoryIndex].newDir = player->entity->sprite->direction;
      player->moveHistoryIndex++;

      if ( player->moveHistoryIndex >= PLAYER_MOVE_HISTORY_SIZE )
      {
         player->moveHistoryIndex = 0;
      }

      prevPlayer = player;
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
   game->players->tileIndex = tileIndex;

   if ( Game_TryEnterPortal( game ) )
   {
      return;
   }
   
   // TODO: eventually this will be on a per-player basis
   Game_ApplyTileDamage( game );

   // TODO: check if the player has died before going any further

   if ( Game_TryEncounter( game ) )
   {
      // TODO: battle!
   }
}

internal Bool_t Game_TryEnterPortal( Game_t* game )
{
   u32 i;
   Portal_t* checkPortal, * foundPortal = 0;

   // check regular portals first
   for ( i = 0, checkPortal = game->tileMap.portals; i < game->tileMap.portalCount; i++, checkPortal++ )
   {
      if ( checkPortal->sourceTileIndex == game->players->tileIndex )
      {
         foundPortal = checkPortal;
         break;
      }
   }

   // now check for edge portals
   if ( !foundPortal && game->tileMap.hasEdgePortal && TileMap_TileIndexIsEdgeTile( &game->tileMap, game->players->tileIndex ) )
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

      return True;
   }

   return False;
}

internal void Game_EnterPortal( Game_t* game, Portal_t* portal )
{
   u32 i, newPosX, newPosY;
   u32 destTileMapIndex = portal->destTileMapIndex;
   u32 destTileIndex = portal->destTileIndex;
   Direction_t destDirection = portal->destDirection;

   TileMap_LoadFromIndex( &game->tileMap, destTileMapIndex );
   TileMap_GetPositionOfTileIndex( &game->tileMap, destTileIndex, &newPosX, &newPosY );
   game->players->tileIndex = destTileIndex;

   for ( i = 0; i < game->playerCount; i++ )
   {
      TileMap_CenterEntityOnTile( &game->tileMap, game->tileMap.playerEntities + i, destTileIndex );
      ActiveSprite_SetDirection( game->tileMap.playerSprites + i, destDirection );
   }

   TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );
   Random_Seed();
}

internal void Game_ApplyTileDamage( Game_t* game )
{
   u16 tile = game->tileMap.tiles[game->players->tileIndex];

   switch ( TILE_GET_DAMAGE_RATE( tile ) )
   {
      // TODO: actually apply damage to the player based on the rate
      default: return;
   }
}

internal Bool_t Game_TryEncounter( Game_t* game )
{
   u16 tile;

   if ( !game->tileMap.hasEncounters )
   {
      return False;
   }

   tile = game->tileMap.tiles[game->players->tileIndex];

   switch ( TILE_GET_ENCOUNTER_RATE( tile ) )
   {
      case EncounterRate_Low: return ( Random_u32( 1, ENCOUNTER_CHANCE_LOW ) == 1 ) ? True : False;
      case EncounterRate_Medium: return ( Random_u32( 1, ENCOUNTER_CHANCE_MEDIUM ) == 1 ) ? True : False;
      case EncounterRate_High: return ( Random_u32( 1, ENCOUNTER_CHANCE_HIGH ) == 1 ) ? True : False;

      default: return False;
   }
}
