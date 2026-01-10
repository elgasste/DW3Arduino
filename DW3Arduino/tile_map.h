#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "sprite.h"
#include "entity.h"
#include "npc.h"

#define TILEMAP_TILE_SIZE                       16

#define TILEMAP_MAX_TILE_TEXTURES               32
#define TILEMAP_MAX_STATIC_SPRITE_TEXTURES      32
#define TILEMAP_MAX_STATIC_SPRITES              64
#define TILEMAP_MAX_ACTIVE_SPRITE_TEXTURES      16
#define TILEMAP_MAX_ACTIVE_SPRITES              32
#define TILEMAP_MAX_PORTALS                     32
#define TILEMAP_MAX_ENTITIES                    32
#define TILEMAP_MAX_NPCS                        24

#define TILEMAP_MAX_TILES_X                     256
#define TILEMAP_MAX_TILES_Y                     256

#define TILEMAP_SWAP_FADE_SECONDS               0.3f
#define TILEMAP_SWAP_PAUSE_SECONDS              0.2f

#define TILE_WALK_SPEED_NORMAL                  56.0f
#define TILE_WALK_SPEED_SLOW                    48.0f
#define TILE_WALK_SPEED_VERY_SLOW               40.0f
#define TILE_WALK_SPEED_CRAWL                   24.0f

#define TILE_GET_TEXTURE_INDEX( tile )       ( ( tile ) & 0x1F )
#define TILE_GET_IS_PASSABLE( tile )         ( ( ( tile ) >> 5 ) & 0x01 )
#define TILE_GET_WALKING_SPEED( tile )       ( ( ( tile ) >> 6 ) & 0x03 )
#define TILE_GET_ENCOUNTER_RATE( tile )      ( ( EncounterRate_t  )( ( ( tile ) >> 8  ) & 0x03 ) )
#define TILE_GET_DAMAGE_RATE( tile )         ( ( TileDamageRate_t )( ( ( tile ) >> 10 ) & 0x03 ) )

typedef struct Entity_t Entity_t;

typedef struct TileTexture_t
{
   u8 paletteIndexes[TILEMAP_TILE_SIZE * TILEMAP_TILE_SIZE];
}
TileTexture_t;

typedef struct Portal_t
{
   u32 sourceTileIndex;
   u32 destTileMapIndex;
   u32 destTileIndex;
   Direction_t destDirection;
}
Portal_t;

typedef enum EncounterRate_t
{
   EncounterRate_None = 0,
   EncounterRate_Low,
   EncounterRate_Medium,
   EncounterRate_High,

   EncounterRate_Count
}
EncounterRate_t;

typedef enum TileDamageRate_t
{
   TileDamageRate_None = 0,
   TileDamageRate_Low,
   TileDamageRate_Medium,
   TileDamageRate_High,

   TileDamageRate_Count
}
TileDamageRate_t;

typedef struct TileMap_t
{
   TileTexture_t tileTextures[TILEMAP_MAX_TILE_TEXTURES];
   StaticSpriteTexture_t staticSpriteTextures[TILEMAP_MAX_STATIC_SPRITE_TEXTURES];
   ActiveSpriteTexture_t activeSpriteTextures[TILEMAP_MAX_ACTIVE_SPRITE_TEXTURES];
   ActiveSpriteTexture_t playerSpriteTextures[MAX_PLAYERS];

   // bits 0-4:   tile texture index
   // bit  5:     is passable
   // bits 6-7:   walking speed
   // bits 8-9:   encounter rate
   // bits 10-11: damage rate
   // bits 12-15: reserved
   u16 tiles[TILEMAP_MAX_TILES_X * TILEMAP_MAX_TILES_Y];
   u32 tilesX;
   u32 tilesY;
   Bool_t wraps;
   Bool_t affectsDaylight;
   Bool_t isUnderground;
   Bool_t hasEncounters;

   Vector4i32_t viewport;
   Vector2u32_t viewportScreenPos;

   StaticSprite_t staticSprites[TILEMAP_MAX_STATIC_SPRITES];
   u32 staticSpriteCount;

   ActiveSprite_t activeSprites[TILEMAP_MAX_ACTIVE_SPRITES];
   u32 activeSpriteCount;

   ActiveSprite_t playerSprites[MAX_PLAYERS];
   Entity_t playerEntities[MAX_PLAYERS];
   u32 ( *getPlayerCountFunc )( void* );
   void* playerCountProvider;

   Portal_t portals[TILEMAP_MAX_PORTALS];
   u32 portalCount;

   Bool_t hasEdgePortal;
   Portal_t edgePortal;

   Entity_t entities[TILEMAP_MAX_ENTITIES];
   u32 entityCount;

   Npc_t npcs[TILEMAP_MAX_NPCS];
   u32 npcCount;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap, u32 ( *getPlayerCountFunc )( void* ), void* playerCountProvider );
void TileMap_Tic( TileMap_t* tileMap );
void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity );
void TileMap_CenterEntityOnTile( TileMap_t* tileMap, Entity_t* entity, u32 tileIndex );
u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, u32 x, u32 y );
void TileMap_GetPositionOfTileIndex( TileMap_t* tileMap, u32 tileIndex, u32* x, u32* y );
Bool_t TileMap_TileIndexIsEdgeTile( TileMap_t* tileMap, u32 tileIndex );
r32 TileMap_GetTileVelocity( TileMap_t* tileMap, u32 tileIndex );

// game_data.c
void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
