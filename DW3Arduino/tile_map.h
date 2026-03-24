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
   ActiveSpriteTexture_t shipSpriteTextures;
   ActiveSpriteTexture_t ramiaSpriteTextures;

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

   // TODO: these and other flags in here can be consolidated into a single u32
   Bool_t wraps;
   Bool_t affectsDaylight;
   Bool_t isUnderground;
   Bool_t hasEncounters;
   Bool_t allowsShip;
   Bool_t allowsRamia;

   Vector4i32_t viewport;
   Vector2u32_t viewportScreenPos;

   StaticSprite_t staticSprites[TILEMAP_MAX_STATIC_SPRITES];
   u32 staticSpriteCount;
   ActiveSprite_t activeSprites[TILEMAP_MAX_ACTIVE_SPRITES];
   u32 activeSpriteCount;
   ActiveSprite_t playerSprites[MAX_PLAYERS];
   Entity_t playerEntities[MAX_PLAYERS];
   ActiveSprite_t shipSprite;
   ActiveSprite_t ramiaSprite;

   // MUFFINS
   //
   // - if the you're on ramia/ship, update the tile index for the player AND ship/ramia when moving
   // - if you step on the ship's tile index, board the ship
   //    - this will involve checking whether the ship is on a water tile in collision detection
   //    - add an animation for this
   // - if you're on the ship and you step on a passable non-water index, disembark the ship
   //    - same as getting on the ship in the first place
   //    - add an animation for this
   // - if you're on ramia's tile index and you press A, board ramia
   //    - use the same animation as boarding the ship for this
   // - if you're on ramia, you don't need to hold down the direction keys
   // - if you're on ramia and you press A, disembark as long as you're on passable land and not on a portal

   Bool_t isOnShip;
   u32 shipTileIndex;
   Bool_t isOnRamia;
   u32 ramiaTileIndex;

   Player_t* players;
   u32 ( *getPlayerCountFunc )( void* );
   void* playerCountProvider;
   Bool_t ( *hasShipFunc )( void* );
   void* hasShipProvider;
   Bool_t ( *hasRamiaFunc )( void* );
   void* hasRamiaProvider;

   Portal_t portals[TILEMAP_MAX_PORTALS];
   u32 portalCount;
   Bool_t hasEdgePortal;
   Portal_t edgePortal;

   Entity_t entities[TILEMAP_MAX_ENTITIES];
   u32 entityCount;
   Entity_t shipEntity;
   Entity_t ramiaEntity;

   Npc_t npcs[TILEMAP_MAX_NPCS];
   u32 npcCount;
}
TileMap_t;

#if defined( __cplusplus )
extern "C" {
#endif

void TileMap_Init( TileMap_t* tileMap, Player_t* players,
                   u32 ( *getPlayerCountFunc )( void* ), void* playerCountProvider,
                   Bool_t ( *hasShipFunc )( void* ), void* hasShipProvider,
                   Bool_t ( *hasRamiaFunc )( void* ), void* hasRamiaProvider );
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
void TileMap_LoadShipAndRamiaSprites( TileMap_t* tileMap );
void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index );
Bool_t TileMap_AllowsShip( u32 index );
Bool_t TileMap_AllowsRamia( u32 index );

#if defined( __cplusplus )
}
#endif

#endif // TILE_MAP_H
