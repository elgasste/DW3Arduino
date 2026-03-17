#if !defined( TILE_MAP_H )
#define TILE_MAP_H

#include "common.h"
#include "sprite.h"
#include "entity.h"
#include "npc.h"

#define TILE_GET_TEXTURE_INDEX( tile )          ( ( tile ) & 0x1F )
#define TILE_GET_IS_PASSABLE( tile )            ( ( ( tile ) >> 5 ) & 0x01 )
#define TILE_GET_WALKING_SPEED( tile )          ( ( ( tile ) >> 6 ) & 0x03 )
#define TILE_GET_ENCOUNTER_RATE( tile )         ( ( EncounterRate_t  )( ( ( tile ) >> 8  ) & 0x03 ) )
#define TILE_GET_DAMAGE_RATE( tile )            ( ( TileDamageRate_t )( ( ( tile ) >> 10 ) & 0x03 ) )
#define TILE_GET_IS_LAND( tile )                ( ( ( tile ) >> 12 ) & 0x01 )

#define TILE_MAKE_PASSABLE( tile )              ( tile |= 0x20 )
#define TILE_MAKE_LAND( tile )                  ( tile |= 0x1000 )

typedef struct Entity_t Entity_t;
typedef struct Player_t Player_t;

typedef struct TileTexture_t
{
   u8 paletteIndexes[TILEMAP_TILE_SIZE_PIXELS * TILEMAP_TILE_SIZE_PIXELS];
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
   // bit  12:    is land
   // bits 13-15: reserved
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

   // MUFFINS
   //
   // - add shipSprite
   // - add ramiaSprite

   Bool_t playerIsOnShip;
   Bool_t playerIsOnRamia;

   Player_t* players;
   u32 ( *getPlayerCountFunc )( void* );
   void* playerCountProvider;
   Bool_t ( *playerHasShipFunc )( void* );
   void* playerHasShipProvider;
   Bool_t ( *playerHasRamiaFunc )( void* );
   void* playerHasRamiaProvider;

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

void TileMap_Init( TileMap_t* tileMap, Player_t* players,
                   u32 ( *getPlayerCountFunc )( void* ), void* playerCountProvider,
                   Bool_t ( *playerHasShipFunc )( void* ), void* playerHasShipProvider,
                   Bool_t ( *playerHasRamiaFunc )( void* ), void* playerHasRamiaProvider );
void TileMap_Tic( TileMap_t* tileMap );
void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity );
void TileMap_CenterEntityOnTile( TileMap_t* tileMap, Entity_t* entity, u32 tileIndex );
u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, i32 x, i32 y );
void TileMap_GetPositionOfTileIndex( TileMap_t* tileMap, u32 tileIndex, i32* x, i32* y );
Bool_t TileMap_TileIndexIsEdgeTile( TileMap_t* tileMap, u32 tileIndex );
i32 TileMap_GetTileVelocity( TileMap_t* tileMap, u32 tileIndex );
i32 TileMap_GetTileDiagonalVelocity( i32 regularVelocity );

// game_data.c
void TileMap_LoadPlayerSprites( TileMap_t* tileMap );
void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
