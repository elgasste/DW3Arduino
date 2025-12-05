#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "vector.h"
#include "entity.h"
#include "npc.h"

#define TILE_SIZE                16

#define TILEMAP_MAX_TEXTURES     32
#define TILEMAP_MAX_ENTITIES     32
#define TILEMAP_MAX_NPCS         24

#define TILEMAP_MAX_TILES_X      256
#define TILEMAP_MAX_TILES_Y      256

#define TILE_GET_TEXTURE_INDEX( tile )   ( ( tile ) & 0x1F )
#define TILE_GET_IS_PASSABLE( tile )     ( ( ( tile ) >> 5 ) & 0x01 )

typedef struct Entity_t Entity_t;

typedef struct TileTexture_t
{
   u8 paletteIndexes[TILE_SIZE * TILE_SIZE];
}
TileTexture_t;

typedef struct TileMap_t
{
   TileTexture_t tileTextures[TILEMAP_MAX_TEXTURES];

   // bits 0-4:  tile texture index
   // bit  5:    is passable
   // bits 6-15: reserved
   u16 tiles[TILEMAP_MAX_TILES_X * TILEMAP_MAX_TILES_Y];
   u32 tilesX;
   u32 tilesY;
   Bool_t wraps;

   Vector4i32_t viewport;
   Vector2u32_t viewportScreenPos;

   Entity_t entities[TILEMAP_MAX_ENTITIES];
   u32 entityCount;
   Npc_t npcs[TILEMAP_MAX_NPCS];
   u32 npcCount;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap );
void TileMap_Tic( TileMap_t* tileMap );
void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity );
u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, u32 x, u32 y );

// game_data.c
void TileMap_LoadTileTextures( TileMap_t* tileMap );
void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
