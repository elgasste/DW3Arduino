#include <unity.h>

#include "game_util.h"

Game_t* GameUtil_CreateSimpleGame()
{
   u32 i;
   u16* screenBuffer;
   Game_t* game;

   screenBuffer = (u16*)calloc( SCREEN_PIXELS, sizeof( u16 ) );
   TEST_ASSERT_NOT_NULL( screenBuffer );
   if ( !screenBuffer )
   {
      return 0;
   }

   game = (Game_t*)malloc( sizeof( Game_t ) );
   TEST_ASSERT_NOT_NULL( game );
   if ( !game )
   {
      return 0;
   }

   game->playerCount = 1;
   Game_Init( game, screenBuffer );

   game->tileMap.wraps = False;
   game->tileMap.tilesX = GAMEUTIL_DEFAULT_TILEMAP_TILES_X;
   game->tileMap.tilesY = GAMEUTIL_DEFAULT_TILEMAP_TILES_Y;
   game->tileMap.viewport.w = GAMEUTIL_DEFAULT_TILEMAP_VIEWPORT_W;
   game->tileMap.viewport.h = GAMEUTIL_DEFAULT_TILEMAP_VIEWPORT_H;

   for ( i = 0; i < game->tileMap.tilesX * game->tileMap.tilesY; i++ )
   {
      TILE_MAKE_PASSABLE( game->tileMap.tiles[i] );
      TILE_MAKE_LAND( game->tileMap.tiles[i] );
   }

   for ( i = 0; i < MAX_PLAYERS; i++ )
   {
      Player_Init( game->players + i );
      game->players[i].playerClass = ( i == 0 ) ? PlayerClass_Hero : PlayerClass_Wizard;
      game->players[i].moveHistoryIndex = 0;
      game->tileMap.playerEntities[i].pos.x = 0;
      game->tileMap.playerEntities[i].pos.y = 0;
      game->tileMap.playerEntities[i].pos.w = GENERIC_ENTITY_WIDTH;
      game->tileMap.playerEntities[i].pos.h = GENERIC_ENTITY_HEIGHT;
      game->tileMap.playerEntities[i].prevPos = game->tileMap.playerEntities[i].pos;
      game->players[i].entity = game->tileMap.playerEntities + i;
      TileMap_CenterEntityOnTile( &game->tileMap, &game->tileMap.playerEntities[i], 0 );
      game->tileMap.playerEntities[i].velocity.x = 0;
      game->tileMap.playerEntities[i].velocity.y = 0;
      game->tileMap.playerEntities[i].sprite = game->tileMap.playerSprites + i;

      ActiveSprite_Init( &game->tileMap.playerSprites[i], i, 0, 0, Direction_Down );
   }

   game->players->entity = game->tileMap.playerEntities;
   game->players->entity->sprite = game->tileMap.playerSprites;
   TileMap_ClampViewportToEntity( &game->tileMap, game->tileMap.playerEntities );

   game->isAM = False;
   game->daylightFactor = 1.0f; // noon
   game->screen.dayFilterIntensity = 1.0f;

   return game;
}

void GameUtil_DeleteGame( Game_t* game )
{
   if ( game )
   {
      if ( game->screen.buffer )
      {
         free( game->screen.buffer );
      }

      free( game );
   }
}
