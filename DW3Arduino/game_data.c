// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!

#include "game.h"
#include "random.h"

void Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index )
{
   u32 i;

   UNUSED_PARAM( index );

   for ( i = 0; i < SCREEN_PALETTE_SIZE; i++ )
   {
      screen->palette[i] = COLOR16_BLACK;
   }

   screen->palette[1] = 0x0640;
   screen->palette[2] = 0x0420;
   screen->palette[3] = 0x0838;
   screen->palette[4] = 0x0838;
   screen->palette[5] = COLOR16_MAGENTA;
   screen->palette[6] = COLOR16_CYAN;
}

void TileMap_LoadTileTextures( TileMap_t* tileMap )
{
   u32 i, j;

   // we can just use solid-colored textures for now
   for ( i = 0; i < 7; i++ )
   {
      for ( j = 0; j < TILEMAP_TILE_SIZE * TILEMAP_TILE_SIZE; j++ )
      {
         tileMap->tileTextures[i].paletteIndexes[j] = (u8)i;
      }
   }
}

void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )
{
   u32 i;
   u16* tile;

   tileMap->wraps = False;
   tileMap->hasEdgePortal = False;
   tileMap->entityCount = 1;

   if ( index == 0 ) // starting room
   {
      tileMap->tilesX = 26;
      tileMap->tilesY = 20;

      for ( i = 0, tile = tileMap->tiles; i < ( tileMap->tilesX * tileMap->tilesY ); i++, tile++ ) // overall tiles
      {
         *tile = 2 | ( 0x1 << 5 );
      }
      for ( i = 0, tile = tileMap->tiles; i < tileMap->tilesX; i++, tile++ ) // top wall
      {
         *tile = 3;
      }
      for ( i = 0, tile = tileMap->tiles + ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ); i < tileMap->tilesX; i++, tile++ ) // bottom wall
      {
         *tile = 3;
      }
      for ( i = 0, tile = tileMap->tiles + tileMap->tilesX; i < tileMap->tilesY - 2; i++, tile += tileMap->tilesX ) // left and right walls
      {
         *tile = 3;
         *( tile + ( tileMap->tilesX - 1 ) ) = 3;
      }

      tileMap->tiles[( tileMap->tilesX * 10 )] = 2 | ( 0x1 << 5 );
      tileMap->tiles[377] = 1 | ( 0x1 << 5 );

      tileMap->tiles[285 - tileMap->tilesX] = 2 | ( 0x1 << 5 );
      tileMap->tiles[285] = 2 | ( 0x1 << 5 );
      tileMap->tiles[285 + tileMap->tilesX] = 2 | ( 0x1 << 5 );

      tileMap->portalCount = 2;
      tileMap->portals[0].sourceTileIndex = 260;
      tileMap->portals[0].destTileMapIndex = 1;
      tileMap->portals[0].destTileIndex = 39;
      tileMap->portals[1].sourceTileIndex = 377;
      tileMap->portals[1].destTileMapIndex = 2;
      tileMap->portals[1].destTileIndex = 24;

      tileMap->hasEdgePortal = True;
      tileMap->edgePortal.destTileMapIndex = 3;
      tileMap->edgePortal.destTileIndex = 201;
   }
   else if ( index == 1 ) // hallway
   {
      tileMap->tilesX = 20;
      tileMap->tilesY = 3;

      tile = tileMap->tiles;

      for ( i = 0, tile = tileMap->tiles; i < tileMap->tilesX * tileMap->tilesY; i++, tile++ ) // top wall
      {
         *tile = 3;
      }
      for ( i = 0, tile = tileMap->tiles + tileMap->tilesX; i < tileMap->tilesX; i++, tile++ ) // hallway
      {
         *tile = 2 | ( 0x1 << 5 );
      }
      for ( i = 0, tile = tileMap->tiles + ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ); i < tileMap->tilesX; i++, tile++ ) // bottom wall
      {
         *tile = 3;
      }

      tileMap->portalCount = 2;
      tileMap->portals[0].sourceTileIndex = 39;
      tileMap->portals[0].destTileMapIndex = 0;
      tileMap->portals[0].destTileIndex = 260;
      tileMap->portals[1].sourceTileIndex = 20;
      tileMap->portals[1].destTileMapIndex = 2;
      tileMap->portals[1].destTileIndex = 69;
   }
   else if ( index == 2 ) // second room
   {
      tileMap->tilesX = 10;
      tileMap->tilesY = 10;

      for ( i = 0, tile = tileMap->tiles; i < ( tileMap->tilesX * tileMap->tilesY ); i++, tile++ ) // overall tiles
      {
         *tile = 2 | ( 0x1 << 5 );
      }
      for ( i = 0, tile = tileMap->tiles; i < tileMap->tilesX; i++, tile++ ) // top wall
      {
         *tile = 3;
      }
      for ( i = 0, tile = tileMap->tiles + ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ); i < tileMap->tilesX; i++, tile++ ) // bottom wall
      {
         *tile = 3;
      }
      for ( i = 0, tile = tileMap->tiles + tileMap->tilesX; i < tileMap->tilesY - 2; i++, tile += tileMap->tilesX ) // left and right walls
      {
         *tile = 3;
         *( tile + ( tileMap->tilesX - 1 ) ) = 3;
      }

      tileMap->tiles[( ( tileMap->tilesX * 7 ) - 1 )] = 2 | ( 0x1 << 5 );
      tileMap->tiles[24] = 1 | ( 0x1 << 5 );

      tileMap->portalCount = 2;
      tileMap->portals[0].sourceTileIndex = 69;
      tileMap->portals[0].destTileMapIndex = 1;
      tileMap->portals[0].destTileIndex = 20;
      tileMap->portals[1].sourceTileIndex = 24;
      tileMap->portals[1].destTileMapIndex = 0;
      tileMap->portals[1].destTileIndex = 377;
   }
   else if ( index == 3 )
   {
      tileMap->tilesX = 32;
      tileMap->tilesY = 32;
      tileMap->wraps = True;

      for ( i = 0, tile = tileMap->tiles; i < ( tileMap->tilesX * tileMap->tilesY ); i++, tile++ ) // overall tiles
      {
         *tile = (u16)( Random_u32( 0, 5 ) == 0 ? 2 : 1 ) | ( 0x1 << 5 );
      }

      tileMap->tiles[201] = 6 | ( 0x1 << 5 );

      tileMap->portalCount = 1;
      tileMap->portals[0].sourceTileIndex = 201;
      tileMap->portals[0].destTileMapIndex = 0;
      tileMap->portals[0].destTileIndex = 285;
   }
}
