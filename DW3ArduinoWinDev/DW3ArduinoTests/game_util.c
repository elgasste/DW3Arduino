#include <unity.h>

#include "game_util.h"

Game_t* GameUtil_CreateSimpleGame()
{
   u32 i;
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );
   TEST_ASSERT_NOT_NULL( game );

   if ( game == 0 )
   {
      return 0;
   }

   for ( i = 0; i < InputButton_Count; i++ )
   {
      game->input.buttonStates[i].down = False;
      game->input.buttonStates[i].pressed = False;
      game->input.buttonStates[i].released = False;
   }

   game->tileMap.tilesX = GAMEUTIL_DEFAULT_TILEMAP_TILES_X;
   game->tileMap.tilesY = GAMEUTIL_DEFAULT_TILEMAP_TILES_Y;

   for ( i = 0; i < MAX_PLAYERS; i++ )
   {
      game->tileMap.playerEntities[i].pos.w = GENERIC_ENTITY_WIDTH;
      game->tileMap.playerEntities[i].pos.h = GENERIC_ENTITY_HEIGHT;
      game->tileMap.playerEntities[i].velocity.x = 0;
      game->tileMap.playerEntities[i].velocity.y = 0;
   }

   game->playerCount = 1;
   game->players->entity = game->tileMap.playerEntities;
   game->players->entity->sprite = game->tileMap.playerSprites;
   TileMap_CenterEntityOnTile( &game->tileMap, game->tileMap.playerEntities, 0 );

   return game;
}

void GameUtil_DeleteGame( Game_t* game )
{
   if ( game )
   {
      free( game );
   }
}
