#include "game.h"

void Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index )
{
   u32 i;

   UNUSED_PARAM( index );

   for ( i = 0; i < SCREEN_PALETTE_SIZE; i++ )
   {
      screen->palette[i] = COLOR16_BLACK;
   }

   screen->palette[1]  = COLOR16_WHITE;
   screen->palette[2]  = COLOR16_RED;
   screen->palette[3]  = COLOR16_BLUE;
   screen->palette[4]  = COLOR16_YELLOW;
   screen->palette[5]  = COLOR16_MAGENTA;
   screen->palette[6]  = COLOR16_CYAN;
}

void TileMap_LoadTileTextures( TileMap_t* tileMap )
{
   u32 i, j;

   // we can just use solid-colored textures for now
   for ( i = 0; i < 7; i++ )
   {
      for ( j = 0; j < TILE_SIZE * TILE_SIZE; j++ )
      {
         tileMap->tileTextures[i].paletteIndexes[j] = (u8)i;
      }
   }
}

void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )
{
   u32 row, col;
   u16* tilePos = tileMap->tiles;

   UNUSED_PARAM( index );

   tileMap->tilesX = 32;
   tileMap->tilesY = 32;

   // checkerboard
   for ( row = 0; row < tileMap->tilesY; row++ )
   {
      for ( col = 0; col < tileMap->tilesX; col++ )
      {
         if ( row % 2 == 0 )
         {
            *tilePos = ( col % 2 == 0 ) ? 1 : 2;
         }
         else
         {
            *tilePos = ( col % 2 == 0 ) ? 2 : 1;
         }

         *tilePos |= 0x0 << 5; // passable
         tilePos++;
      }
   }

   // top and bottom borders (not passable)
   for ( col = 0; col < tileMap->tilesX; col++ )
   {
      tileMap->tiles[col] = 3 | ( 0x1 << 5 );
      tileMap->tiles[( ( tileMap->tilesY - 1 ) * tileMap->tilesX ) + col] = 3;
   }

   // left and right borders (not passable)
   for ( row = 0; row < tileMap->tilesY; row++ )
   {
      tileMap->tiles[row * tileMap->tilesX] = 3 | ( 0x1 << 5 );
      tileMap->tiles[( row * tileMap->tilesX ) + ( tileMap->tilesX - 1 )] = 3;
   }
}
