#include "tile_map.h"

void TileMap_Init( TileMap_t* tileMap )
{
   u32 i, j;

   for ( i = 0; i < TILEMAP_MAX_TEXTURES; i++ )
   {
      for ( j = 0; j < TILE_SIZE * TILE_SIZE; j++ )
      {
         tileMap->tileTextures[i].paletteIndexes[j] = 0;
      }
   }

   for ( i = 0; i < TILEMAP_MAX_TILES_X * TILEMAP_MAX_TILES_Y; i++ )
   {
      tileMap->tiles[i] = 0;
   }
}
