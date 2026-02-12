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

// MUFFINS: add tests for all the other functions:
//
// - void TileMap_Tic( TileMap_t* tileMap )
// - void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity )
// - u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, i32 x, i32 y )
// - void TileMap_GetPositionOfTileIndex( TileMap_t* tileMap, u32 tileIndex, i32* x, i32* y )
// - Bool_t TileMap_TileIndexIsEdgeTile( TileMap_t* tileMap, u32 tileIndex )
// - i32 TileMap_GetTileVelocity( TileMap_t* tileMap, u32 tileIndex )
// - i32 TileMap_GetTileDiagonalVelocity( i32 regularVelocity )

void TileMap_CenterEntityOnTile_Always_UpdatesEntityTileIndex( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      entity.pos.w = 20;
      entity.pos.h = 30;

      TileMap_CenterEntityOnTile( tileMap, &entity, 23 );

      TEST_ASSERT_EQUAL( 23, entity.tileIndex );

      free( tileMap );
   }
}

void TileMap_CenterEntityOnTile_Always_CentersEntityOnCorrectTile( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      entity.pos.w = 20;
      entity.pos.h = 30;

      TileMap_CenterEntityOnTile( tileMap, &entity, 11 );

      TEST_ASSERT_EQUAL( TILEMAP_TILE_SIZE_UNITS + ( ( TILEMAP_TILE_SIZE_UNITS / 2 ) - 10 ), entity.pos.x );
      TEST_ASSERT_EQUAL( TILEMAP_TILE_SIZE_UNITS + ( ( TILEMAP_TILE_SIZE_UNITS / 2 ) - 15 ), entity.pos.y );

      free( tileMap );
   }
}
