#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"

#define TILE_SIZE_X              16
#define TILEMAP_MAX_TILES_X      256
#define TILEMAP_MAX_TILES_Y      256

typedef struct TileMap_t
{
   // bits 0-7: palette index
   // bits 8-15: reserved
   u16 tiles[TILEMAP_MAX_TILES_X * TILEMAP_MAX_TILES_Y];
   u32 tilesX;
   u32 tilesY;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap );

// game_data.c
void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
