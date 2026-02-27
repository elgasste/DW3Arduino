#include "game.h"
#include "random.h"

internal void Game_TicByState( Game_t* game );
internal u32 Game_GetPlayerCount( Game_t* game );
internal void Game_HandlePlayerMoved( Game_t* game );
internal void Game_AnchorRearPlayers( Game_t* game );
internal void Game_IncrementDaylightFactor( Game_t* game );
internal void Game_UpdateDayFilterIntensity( Game_t* game );
internal void Game_SteppedOnTile( Game_t* game, u32 tileIndex );
internal Bool_t Game_TryEnterPortal( Game_t* game );
internal void Game_EnterPortal( Game_t* game, Portal_t* portal );
internal Bool_t Game_TryEncounter( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   AnimationChain_Init( &game->animationChain, &game->screen );
   TileMap_Init( &game->tileMap, game->players, &Game_GetPlayerCount, game );

   game->tileMap.viewport.w = SCREEN_WIDTH * UNITS_PER_PIXEL;
   game->tileMap.viewport.h = SCREEN_HEIGHT * UNITS_PER_PIXEL;
   game->tileMap.viewportScreenPos.x = 0;
   game->tileMap.viewportScreenPos.y = 0;

   game->playerMovedCallback = Game_HandlePlayerMoved;

   Game_Reset( game );

   // TODO: I'm leaving this here for testing purposes, but it'll move eventually
   if ( Storage_LoadGame( game, 0 ) )
   {
      Program_Log( "save data successfully loaded" );
      TileMap_LoadPlayerSprites( &game->tileMap );
   }
   else
   {
      Program_Log( "save data failed to load" );
      Game_Reset( game );
   }

   game->state = GameState_Overworld_Active;
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
      Input_HandleInput( game );
   }
   
   if ( !AnimationChain_PausesAction( &game->animationChain ) )
   {
      Game_TicByState( game );
   }

   Render_DrawGame( game );
}

internal void Game_TicByState( Game_t* game )
{
   if ( game->state < GameState_Intro_Count )
   {
      // TODO: tic intro stuff
   }
   else if ( game->state < GameState_Overworld_Count )
   {
      if ( game->state == GameState_Overworld_Active )
      {
         Game_UpdateDayFilterIntensity( game );
      }

      TileMap_Tic( &game->tileMap );
      Physics_Tic( game );
      TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );
   }
}

internal u32 Game_GetPlayerCount( Game_t* game )
{
   return game->playerCount;
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
                                               ( game->players->entity->pos.x + ( game->players->entity->pos.w / 2 ) ),
                                               ( game->players->entity->pos.y + ( game->players->entity->pos.h / 2 ) ) );

   if ( tileIndex != game->players->entity->tileIndex )
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

      player = prevPlayer + 1;

      player->entity->pos.x = prevPlayer->moveHistory[player->moveHistoryIndex].newPos.x;
      player->entity->pos.y = prevPlayer->moveHistory[player->moveHistoryIndex].newPos.y;
      ActiveSprite_SetDirection( player->entity->sprite, prevPlayer->moveHistory[player->moveHistoryIndex].newDir );

      player->moveHistory[player->moveHistoryIndex] = prevPlayer->moveHistory[player->moveHistoryIndex];
      player->moveHistoryIndex++;

      if ( player->moveHistoryIndex >= PLAYER_MOVE_HISTORY_SIZE )
      {
         player->chainNextPlayer = True;
         player->moveHistoryIndex = 0;
      }

      Player_ApplyTileDamage( player );

      // TODO: check if the player has died from tile damage

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

internal void Game_UpdateDayFilterIntensity( Game_t* game )
{
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

   // if we're underground, don't go full-nighttime
   if ( game->tileMap.isUnderground && game->screen.dayFilterIntensity < DAY_FACTOR_UNDERGROUND_THRESHOLD )
   {
      game->screen.dayFilterIntensity = DAY_FACTOR_UNDERGROUND_THRESHOLD;
   }
}

internal void Game_SteppedOnTile( Game_t* game, u32 tileIndex )
{
   u16 tile;
   Player_t* frontPlayer = game->players;

   frontPlayer->entity->tileIndex = tileIndex;

   if ( Game_TryEnterPortal( game ) )
   {
      return;
   }
   
   tile = game->tileMap.tiles[tileIndex];
   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].tileDamageRate = TILE_GET_DAMAGE_RATE( tile );
   Player_ApplyTileDamage( frontPlayer );

   // TODO: check if the player has died

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
      if ( checkPortal->sourceTileIndex == game->players->entity->tileIndex )
      {
         foundPortal = checkPortal;
         break;
      }
   }

   // now check for edge portals
   if ( !foundPortal && game->tileMap.hasEdgePortal && TileMap_TileIndexIsEdgeTile( &game->tileMap, game->players->entity->tileIndex ) )
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
   i32 newPosX, newPosY, i;
   u32 destTileMapIndex = portal->destTileMapIndex;
   u32 destTileIndex = portal->destTileIndex;
   Direction_t destDirection = portal->destDirection;
   Player_t* player;

   TileMap_LoadFromIndex( &game->tileMap, destTileMapIndex );
   TileMap_GetPositionOfTileIndex( &game->tileMap, destTileIndex, &newPosX, &newPosY );

   for ( i = 0; i < (i32)game->playerCount; i++ )
   {
      player = game->players + i;
      TileMap_CenterEntityOnTile( &game->tileMap, game->tileMap.playerEntities + i, destTileIndex );
      ActiveSprite_SetDirection( game->tileMap.playerSprites + i, destDirection );

      // this ensures the sprites will be drawn in the correct Z-order on arrival
      player->entity->pos.y += ( destDirection == Direction_Up ) ? i : ( destDirection == Direction_Down ) ? -i : 0;

      Player_ResetChaining( player );
   }

   TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );
   Random_Seed();
}

internal Bool_t Game_TryEncounter( Game_t* game )
{
   u16 tile;

   if ( !game->tileMap.hasEncounters )
   {
      return False;
   }

   tile = game->tileMap.tiles[game->players->entity->tileIndex];

   switch ( TILE_GET_ENCOUNTER_RATE( tile ) )
   {
      case EncounterRate_Low: return ( Random_u32( 1, ENCOUNTER_CHANCE_LOW ) == 1 ) ? True : False;
      case EncounterRate_Medium: return ( Random_u32( 1, ENCOUNTER_CHANCE_MEDIUM ) == 1 ) ? True : False;
      case EncounterRate_High: return ( Random_u32( 1, ENCOUNTER_CHANCE_HIGH ) == 1 ) ? True : False;

      default: return False;
   }
}
