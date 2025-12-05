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

   screen->palette[1]  = 0x0640;
   screen->palette[2]  = 0x0420;
   screen->palette[3]  = 0x0838;
   screen->palette[4]  = 0x0838;
   screen->palette[5]  = COLOR16_MAGENTA;
   screen->palette[6]  = COLOR16_CYAN;
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

      tileMap->portalCount = 1;
      tileMap->portals[0].sourceTileIndex = 260;
      tileMap->portals[0].destTileMapIndex = 1;
      tileMap->portals[0].destTileIndex = 39;
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

      tileMap->portalCount = 1;
      tileMap->portals[0].sourceTileIndex = 69;
      tileMap->portals[0].destTileMapIndex = 1;
      tileMap->portals[0].destTileIndex = 20;
   }

   //u32 row, col, i;
   //u16* tilePos = tileMap->tiles;

   //UNUSED_PARAM( index );

   //tileMap->tilesX = 32;
   //tileMap->tilesY = 32;
   //tileMap->wraps = True;

   //// checkerboard
   //for ( row = 0; row < tileMap->tilesY; row++ )
   //{
   //   for ( col = 0; col < tileMap->tilesX; col++ )
   //   {
   //      if ( row % 2 == 0 )
   //      {
   //         *tilePos = ( col % 2 == 0 ) ? 1 : 2;
   //      }
   //      else
   //      {
   //         *tilePos = ( col % 2 == 0 ) ? 2 : 1;
   //      }

   //      *tilePos |= 0x1 << 5; // passable
   //      tilePos++;
   //   }
   //}

   //// a bunch of random unpassable tiles
   //for ( row = 1; row < tileMap->tilesY - 1; row++ )
   //{
   //   for ( col = 1; col < tileMap->tilesX - 1; col++ )
   //   {
   //      if ( Random_Percent() <= 5 )
   //      {
   //         tileMap->tiles[( row * tileMap->tilesX ) + col] = 3 | ( 0x0 << 5 );
   //      }
   //   }
   //}

   //tileMap->entityCount = TILEMAP_MAX_ENTITIES;

   //for ( i = 0; i < TILEMAP_MAX_ENTITIES; i++ )
   //{
   //   tileMap->entities[i].pos.w = 14.0f;
   //   tileMap->entities[i].pos.h = 14.0f;
   //   tileMap->entities[i].pos.x = (r32)( Random_u32( 1, ( tileMap->tilesX - 2 ) * TILEMAP_TILE_SIZE ) );
   //   tileMap->entities[i].pos.y = (r32)( Random_u32( 1, ( tileMap->tilesY - 2 ) * TILEMAP_TILE_SIZE ) );
   //   tileMap->entities[i].velocity.x = 0.0f;
   //   tileMap->entities[i].velocity.y = 0.0f;
   //}

   //// add the max amount NPCs, all of them wandering
   //tileMap->npcCount = TILEMAP_MAX_NPCS;

   //for ( i = 0; i < TILEMAP_MAX_NPCS; i++ )
   //{
   //   Npc_Init( tileMap->npcs + i, tileMap->entities + ( i + 1 ), True );
   //}
}
