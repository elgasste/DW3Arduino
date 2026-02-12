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

void TileMap_GetTileIndexAtPosition_Always_ReturnsCorrectTileIndex( void )
{
   u32 tileIndex;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileIndex = TileMap_GetTileIndexAtPosition( tileMap, TILEMAP_TILE_SIZE_UNITS + 1, TILEMAP_TILE_SIZE_UNITS + 1 );

      TEST_ASSERT_EQUAL( 11, tileIndex );

      free( tileMap );
   }
}

void TileMap_GetPositionOfTileIndex_Always_ReturnsCorrectTileIndexPosition( void )
{
   i32 x, y;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      TileMap_GetPositionOfTileIndex( tileMap, 32, &x, &y );

      TEST_ASSERT_EQUAL( TILEMAP_TILE_SIZE_UNITS * 2, x );
      TEST_ASSERT_EQUAL( TILEMAP_TILE_SIZE_UNITS * 3, y );

      free( tileMap );
   }
}

void TileMap_TileIndexIsEdgeTile_TileIndexIsNotEdgeTile_ReturnsFalse( void )
{
   Bool_t result;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->wraps = False;
      result = TileMap_TileIndexIsEdgeTile( tileMap, 11 );

      TEST_ASSERT_EQUAL( False, result );

      free( tileMap );
   }
}

void TileMap_TileIndexIsEdgeTile_TileMapWraps_ReturnsFalse( void )
{
   Bool_t result;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->wraps = True;
      result = TileMap_TileIndexIsEdgeTile( tileMap, 0 );

      TEST_ASSERT_EQUAL( False, result );

      free( tileMap );
   }
}

void TileMap_TileIndexIsEdgeTile_TileIndexIsLeftEdgeTile_ReturnsTrue( void )
{
   Bool_t result;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->wraps = False;
      result = TileMap_TileIndexIsEdgeTile( tileMap, 20 );

      TEST_ASSERT_EQUAL( True, result );

      free( tileMap );
   }
}

void TileMap_TileIndexIsEdgeTile_TileIndexIsTopEdgeTile_ReturnsTrue( void )
{
   Bool_t result;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->wraps = False;
      result = TileMap_TileIndexIsEdgeTile( tileMap, 5 );

      TEST_ASSERT_EQUAL( True, result );

      free( tileMap );
   }
}

void TileMap_TileIndexIsEdgeTile_TileIndexIsRightEdgeTile_ReturnsTrue( void )
{
   Bool_t result;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->wraps = False;
      result = TileMap_TileIndexIsEdgeTile( tileMap, 39 );

      TEST_ASSERT_EQUAL( True, result );

      free( tileMap );
   }
}

void TileMap_TileIndexIsEdgeTile_TileIndexIsBottomEdgeTile_ReturnsTrue( void )
{
   Bool_t result;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->wraps = False;
      result = TileMap_TileIndexIsEdgeTile( tileMap, 93 );

      TEST_ASSERT_EQUAL( True, result );

      free( tileMap );
   }
}

// MUFFINS: add tests for all the other functions:
//
// - void TileMap_Tic( TileMap_t* tileMap )
// - void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity )
// - i32 TileMap_GetTileVelocity( TileMap_t* tileMap, u32 tileIndex )
// - i32 TileMap_GetTileDiagonalVelocity( i32 regularVelocity )
