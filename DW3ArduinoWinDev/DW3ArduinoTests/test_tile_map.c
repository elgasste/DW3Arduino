#include <unity.h>
#include <game.h>
#include <utility.h>

#include "main.h"

internal u32 getPlayerCountFunc_Mock0( Game_t* game )
{
   return 0;
}

internal u32 getPlayerCountFunc_Mock1( Game_t* game )
{
   return 1;
}

internal u32 getPlayerCountFunc_Mock2( Game_t* game )
{
   return 2;
}

internal Bool_t hasShip_Mock( Game_t* game )
{
   return False;
}

internal Bool_t hasRamia_Mock( Game_t* game )
{
   return False;
}

void TileMap_Init_Always_InitializesParameters( void )
{
   Player_t player;
   int playerCountProvider, hasShipProvider, hasRamiaProvider;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->staticSpriteCount = 1;
      tileMap->activeSpriteCount = 2;
      tileMap->entityCount = 3;
      tileMap->npcCount = 4;

      TileMap_Init( tileMap, &player,
                    &getPlayerCountFunc_Mock1, &playerCountProvider,
                    &hasShip_Mock, &hasShipProvider,
                    &hasRamia_Mock, &hasRamiaProvider );

      TEST_ASSERT_EQUAL( &player, tileMap->players );
      TEST_ASSERT_EQUAL( &getPlayerCountFunc_Mock1, tileMap->getPlayerCountFunc );
      TEST_ASSERT_EQUAL( &playerCountProvider, tileMap->playerCountProvider );
      TEST_ASSERT_EQUAL( &hasShip_Mock, tileMap->hasShipFunc );
      TEST_ASSERT_EQUAL( &hasShipProvider, tileMap->hasShipProvider );
      TEST_ASSERT_EQUAL( &hasRamia_Mock, tileMap->hasRamiaFunc );
      TEST_ASSERT_EQUAL( &hasRamiaProvider, tileMap->hasRamiaProvider );

      TEST_ASSERT_EQUAL( 0, tileMap->staticSpriteCount );
      TEST_ASSERT_EQUAL( 0, tileMap->activeSpriteCount );
      TEST_ASSERT_EQUAL( 0, tileMap->entityCount );
      TEST_ASSERT_EQUAL( 0, tileMap->npcCount );

      free( tileMap );
   }
}

void TileMap_Tic_Always_TicsNpcs( void )
{
   Entity_t npcEntity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->getPlayerCountFunc = getPlayerCountFunc_Mock0;
      tileMap->hasShipFunc = hasShip_Mock;
      tileMap->hasRamiaFunc = hasRamia_Mock;
      tileMap->npcCount = 2;
      tileMap->activeSpriteCount = 0;

      tileMap->npcs[0].entity = &npcEntity;
      tileMap->npcs[0].wanders = True;
      tileMap->npcs[0].elapsedSeconds = 0.0f;
      tileMap->npcs[1].wanders = False;
      tileMap->npcs[1].elapsedSeconds = 0.0f;

      TileMap_Tic( tileMap );

      TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, tileMap->npcs[0].elapsedSeconds );
      TEST_ASSERT_EQUAL( 0.0f, tileMap->npcs[1].elapsedSeconds );

      free( tileMap );
   }
}

void TileMap_Tic_Always_TicsActiveSprites( void )
{
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->getPlayerCountFunc = getPlayerCountFunc_Mock0;
      tileMap->hasShipFunc = hasShip_Mock;
      tileMap->hasRamiaFunc = hasRamia_Mock;
      tileMap->npcCount = 0;
      tileMap->activeSpriteCount = 1;
      tileMap->activeSprites[0].frameSeconds = 0.0f;

      TileMap_Tic( tileMap );

      TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, tileMap->activeSprites[0].frameSeconds );

      free( tileMap );
   }
}

void TileMap_Tic_Always_TicsPlayerSprites( void )
{
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->getPlayerCountFunc = getPlayerCountFunc_Mock2;
      tileMap->hasShipFunc = hasShip_Mock;
      tileMap->hasRamiaFunc = hasRamia_Mock;
      tileMap->npcCount = 0;
      tileMap->activeSpriteCount = 0;
      tileMap->playerSprites[0].frameSeconds = 0.0f;
      tileMap->playerSprites[1].frameSeconds = 0.0f;

      TileMap_Tic( tileMap );

      TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, tileMap->playerSprites[0].frameSeconds );
      TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, tileMap->playerSprites[1].frameSeconds );

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

void TileMap_GetTileVelocity_TileHasNormalSpeedFactor_ReturnsNormalWalkSpeed( void )
{
   i32 velocity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->isOnRamia = False;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->tiles[22] = 0;
      velocity = TileMap_GetTileVelocity( tileMap, 22 );

      TEST_ASSERT_EQUAL( TILE_WALK_SPEED_NORMAL, velocity );

      free( tileMap );
   }
}

void TileMap_GetTileVelocity_TileHasSlowSpeedFactor_ReturnsSlowWalkSpeed( void )
{
   i32 velocity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->isOnRamia = False;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->tiles[22] = 0x1 << 6;
      velocity = TileMap_GetTileVelocity( tileMap, 22 );

      TEST_ASSERT_EQUAL( TILE_WALK_SPEED_SLOW, velocity );

      free( tileMap );
   }
}

void TileMap_GetTileVelocity_TileHasVerySlowSpeedFactor_ReturnsVerySlowWalkSpeed( void )
{
   i32 velocity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->isOnRamia = False;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->tiles[22] = 0x2 << 6;
      velocity = TileMap_GetTileVelocity( tileMap, 22 );

      TEST_ASSERT_EQUAL( TILE_WALK_SPEED_VERY_SLOW, velocity );

      free( tileMap );
   }
}

void TileMap_GetTileVelocity_TileHasCrawlSpeedFactor_ReturnsCrawlWalkSpeed( void )
{
   i32 velocity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->isOnRamia = False;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->tiles[22] = 0x3 << 6;
      velocity = TileMap_GetTileVelocity( tileMap, 22 );

      TEST_ASSERT_EQUAL( TILE_WALK_SPEED_CRAWL, velocity );

      free( tileMap );
   }
}

void TileMap_GetTileVelocity_PlayerIsOnRamiaWithNonNormalSpeedFactor_ReturnsNormalWalkSpeed( void )
{
   i32 velocity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      tileMap->isOnRamia = True;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->tiles[22] = 0x3 << 6;
      velocity = TileMap_GetTileVelocity( tileMap, 22 );

      TEST_ASSERT_EQUAL( TILE_WALK_SPEED_NORMAL, velocity );

      free( tileMap );
   }
}

void TileMap_GetTileDiagonalVelocity_NormalWalkSpeed_ReturnsNormalDiagonalSpeed( void )
{
   i32 velocity = TileMap_GetTileDiagonalVelocity( TILE_WALK_SPEED_NORMAL );

   TEST_ASSERT_EQUAL( TILE_WALK_SPEED_DIAGONAL_NORMAL, velocity );
}

void TileMap_GetTileDiagonalVelocity_SlowWalkSpeed_ReturnsNormalDiagonalSpeed( void )
{
   i32 velocity = TileMap_GetTileDiagonalVelocity( TILE_WALK_SPEED_SLOW );

   TEST_ASSERT_EQUAL( TILE_WALK_SPEED_DIAGONAL_SLOW, velocity );
}

void TileMap_GetTileDiagonalVelocity_VerySlowWalkSpeed_ReturnsNormalDiagonalSpeed( void )
{
   i32 velocity = TileMap_GetTileDiagonalVelocity( TILE_WALK_SPEED_VERY_SLOW );

   TEST_ASSERT_EQUAL( TILE_WALK_SPEED_DIAGONAL_VERY_SLOW, velocity );
}

void TileMap_GetTileDiagonalVelocity_CrawlWalkSpeed_ReturnsNormalDiagonalSpeed( void )
{
   i32 velocity = TileMap_GetTileDiagonalVelocity( TILE_WALK_SPEED_CRAWL );

   TEST_ASSERT_EQUAL( TILE_WALK_SPEED_DIAGONAL_CRAWL, velocity );
}

void TileMap_GetTileDiagonalVelocity_UnknownWalkSpeed_ReturnsCalculatedDiagonalSpeed( void )
{
   i32 velocity = TileMap_GetTileDiagonalVelocity( TILE_WALK_SPEED_NORMAL - 4 );

   TEST_ASSERT_EQUAL( (i32)( ( TILE_WALK_SPEED_NORMAL - 4 ) * DIAGONAL_SCALAR ), velocity );
}

void TileMap_ClampViewportToEntity_EdgeOfMapAndTileMapWraps_ClampsToFocalEntity( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = 1;
      entity.pos.y = 1;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->viewport.w = 1000;
      tileMap->viewport.h = 1000;
      tileMap->wraps = True;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( -474, tileMap->viewport.x );

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_MapIsThinnerThanViewport_ClampsHorizontally( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = 1;
      entity.pos.y = 1;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( -( ( ( TILEMAP_TILE_SIZE_UNITS * 20 ) - ( 10 * TILEMAP_TILE_SIZE_UNITS ) ) / 2 ), tileMap->viewport.x );

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_MapIsShorterThanViewport_ClampsVertically( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = 1;
      entity.pos.y = 1;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( -( ( ( TILEMAP_TILE_SIZE_UNITS * 16 ) - ( 10 * TILEMAP_TILE_SIZE_UNITS ) ) / 2 ), tileMap->viewport.y );

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_CloseToLeftEdge_ClampsToScreen( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = 1;
      entity.pos.y = 1;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 50;
      tileMap->tilesY = 50;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( 0, tileMap->viewport.x );

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_CloseToTopEdge_ClampsToScreen( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = 1;
      entity.pos.y = 1;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 50;
      tileMap->tilesY = 50;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( 0, tileMap->viewport.y );

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_CloseToRightEdge_ClampsToScreen( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = ( 50 * TILEMAP_TILE_SIZE_UNITS ) - 51;
      entity.pos.y = ( 50 * TILEMAP_TILE_SIZE_UNITS ) - 51;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 50;
      tileMap->tilesY = 50;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( ( 50 * TILEMAP_TILE_SIZE_UNITS ) - ( TILEMAP_TILE_SIZE_UNITS * 20 ), tileMap->viewport.x);

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_CloseToBottomEdge_ClampsToScreen( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = ( 50 * TILEMAP_TILE_SIZE_UNITS ) - 51;
      entity.pos.y = ( 50 * TILEMAP_TILE_SIZE_UNITS ) - 51;
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 50;
      tileMap->tilesY = 50;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( ( 50 * TILEMAP_TILE_SIZE_UNITS ) - ( TILEMAP_TILE_SIZE_UNITS * 16 ), tileMap->viewport.y );

      free( tileMap );
   }
}

void TileMap_ClampViewportToEntity_NotCloseToEdge_ClampsToEntity( void )
{
   Entity_t entity;
   TileMap_t* tileMap = (TileMap_t*)malloc( sizeof( TileMap_t ) );
   TEST_ASSERT_NOT_NULL( tileMap );

   if ( tileMap )
   {
      entity.pos.x = ( 22 * TILEMAP_TILE_SIZE_UNITS );
      entity.pos.y = ( 24 * TILEMAP_TILE_SIZE_UNITS );
      entity.pos.w = 50;
      entity.pos.h = 50;
      tileMap->tilesX = 50;
      tileMap->tilesY = 50;
      tileMap->viewport.w = TILEMAP_TILE_SIZE_UNITS * 20;
      tileMap->viewport.h = TILEMAP_TILE_SIZE_UNITS * 16;
      tileMap->wraps = False;

      TileMap_ClampViewportToEntity( tileMap, &entity );

      TEST_ASSERT_EQUAL( ( ( 22 * TILEMAP_TILE_SIZE_UNITS ) + 25 ) - ( 10 * TILEMAP_TILE_SIZE_UNITS ), tileMap->viewport.x );
      TEST_ASSERT_EQUAL( ( ( 24 * TILEMAP_TILE_SIZE_UNITS ) + 25 ) - ( 8 * TILEMAP_TILE_SIZE_UNITS ), tileMap->viewport.y );

      free( tileMap );
   }
}
