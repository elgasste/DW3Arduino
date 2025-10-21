#include "tile_map.h"

void TileMap_Init( TileMap_t* tileMap )
{
   u32 i;

   for ( i = 0; i < TILEMAP_MAX_TILES_X * TILEMAP_MAX_TILES_Y; i++ )
   {
      tileMap->tiles[i] = 0;
   }
}
