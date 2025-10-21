#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"

#define TILE_SIZE                16

#define TILEMAP_MAX_TEXTURES     32

#define TILEMAP_MAX_TILES_X      256
#define TILEMAP_MAX_TILES_Y      256

#define TILE_GET_TEXTURE_INDEX( tile )   ( ( tile ) & 0x1F )
#define TILE_GET_IS_PASSABLE( tile )     ( ( ( tile ) >> 5 ) & 0x01 )

typedef struct TileTexture_t
{
   u8 paletteIndexes[TILE_SIZE * TILE_SIZE];
}
TileTexture_t;

typedef struct TileMap_t
{
   TileTexture_t tileTextures[TILEMAP_MAX_TEXTURES];

   // bits 0-4:  tile texture index
   // bit 5:     is passable
   // bits 6-15: reserved
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
void TileMap_LoadTileTextures( TileMap_t* tileMap );
void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
