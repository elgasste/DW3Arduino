#include <unity.h>
#include <game.h>

#include "main.h"

internal u32 getPlayerCountFunc_Mock( Game_t* game )
{
   return 1;
}

void TileMap_Init_Always_InitializesParameters( void )
{
   Player_t player;
   int playerCountProvider;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->staticSpriteCount = 1;
      tileMap->activeSpriteCount = 2;
      tileMap->entityCount = 3;
      tileMap->npcCount = 4;

      TileMap_Init( tileMap, &player, &getPlayerCountFunc_Mock, &playerCountProvider );

      TEST_ASSERT_EQUAL( &player, tileMap->players );
      TEST_ASSERT_EQUAL( &getPlayerCountFunc_Mock, tileMap->getPlayerCountFunc );
      TEST_ASSERT_EQUAL( &playerCountProvider, tileMap->playerCountProvider );

      TEST_ASSERT_EQUAL( 0, tileMap->staticSpriteCount );
      TEST_ASSERT_EQUAL( 0, tileMap->activeSpriteCount );
      TEST_ASSERT_EQUAL( 0, tileMap->entityCount );
      TEST_ASSERT_EQUAL( 0, tileMap->npcCount );

      free( tileMap );
   }
}
