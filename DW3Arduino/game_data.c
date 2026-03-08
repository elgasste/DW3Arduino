// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!

#include "text_bit_fields.h"
#include "tile_textures.h"
#include "static_sprite_textures.h"
#include "active_sprite_textures.h"
#include "player_sprite_textures.h"
#include "tile_maps.h"
#include "enemy.h"
#include "game.h"
#include "random.h"

internal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index );
internal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );
internal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );

internal void TileMap_LoadInitialData( TileMap_t* tm, u32 tx, u32 ty, Bool_t w, Bool_t d, Bool_t u, Bool_t he, u32 ssc, u32 asc, u32 pc, Bool_t ep, u32 ec, u32 nc )
{
   tm->tilesX = tx; tm->tilesY = ty; tm->wraps = w; tm->affectsDaylight = d; tm->isUnderground = u; tm->hasEncounters = he, tm->staticSpriteCount = ssc; tm->activeSpriteCount = asc; tm->portalCount = pc, tm->hasEdgePortal = ep, tm->entityCount = ec; tm->npcCount = nc;
}

internal void TileMap_LoadStaticSpriteData( StaticSprite_t* s, u32 txi, u32 ti, Bool_t p )
{
   s->textureIndex = txi; s->tileIndex = ti; s->isPassable = p;
}

internal void TileMap_LoadActiveSpriteData( ActiveSprite_t* s, u32 txi, u32 ox, u32 oy, Direction_t d )
{
   s->textureIndex = txi; s->offset.x = ox; s->offset.y = oy; s->direction = d;
}

internal void TileMap_LoadPortalData( Portal_t* p, u32 st, u32 dm, u32 di, Direction_t dd )
{
   p->sourceTileIndex = st; p->destTileMapIndex = dm; p->destTileIndex = di; p->destDirection = dd;
}

internal void TileMap_LoadEntityData( Entity_t* e, i32 x, i32 y, i32 w, i32 h, ActiveSprite_t* s )
{
   e->pos.x = x; e->pos.y = y; e->pos.w = w; e->pos.h = h; e->sprite = s; e->velocity.x = 0; e->velocity.y = 0;
}

void Screen_LoadPalette( Screen_t* screen )
{
   screen->paletteColorCount = 25;

   screen->palette[0] = 0x9720;
   screen->palette[1] = 0x4CE0;
   screen->palette[2] = 0xFEB3;
   screen->palette[3] = 0xBB60;
   screen->palette[4] = 0x0000;
   screen->palette[5] = 0xFCE0;
   screen->palette[6] = 0x7BEF;
   screen->palette[7] = 0xB5D6;
   screen->palette[8] = 0xFFFF;
   screen->palette[9] = 0x5CFF;
   screen->palette[10] = 0x3AFE;
   screen->palette[11] = 0x4521;
   screen->palette[12] = 0xD1EB;
   screen->palette[13] = 0xFBA5;
   screen->palette[14] = 0x71E0;
   screen->palette[15] = 0x8420;
   screen->palette[16] = 0x44EC;
   screen->palette[17] = 0x6EF1;
   screen->palette[18] = 0x2AE7;
   screen->palette[19] = 0x6F67;
   screen->palette[20] = 0xF81F;
   screen->palette[21] = 0xFE36;
   screen->palette[22] = 0xE663;
   screen->palette[23] = 0x5873;
   screen->palette[24] = 0x9960;
}

void Screen_LoadTextBitFields( Screen_t* screen )
{
   memcpy( screen->textBitFields, g_textBitFields, sizeof( u8 ) * 85 * 8 );
}

internal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index )
{
   switch ( index )
   {
      case 0:
         memcpy( &tileMap->tileTextures[0].paletteIndexes, g_tileTexturePool[0], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[1].paletteIndexes, g_tileTexturePool[1], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[2].paletteIndexes, g_tileTexturePool[2], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[3].paletteIndexes, g_tileTexturePool[3], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[4].paletteIndexes, g_tileTexturePool[4], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[5].paletteIndexes, g_tileTexturePool[5], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[6].paletteIndexes, g_tileTexturePool[6], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[7].paletteIndexes, g_tileTexturePool[7], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[8].paletteIndexes, g_tileTexturePool[8], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[9].paletteIndexes, g_tileTexturePool[9], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[10].paletteIndexes, g_tileTexturePool[10], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[11].paletteIndexes, g_tileTexturePool[11], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[12].paletteIndexes, g_tileTexturePool[12], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[13].paletteIndexes, g_tileTexturePool[13], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[14].paletteIndexes, g_tileTexturePool[14], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[15].paletteIndexes, g_tileTexturePool[15], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[16].paletteIndexes, g_tileTexturePool[16], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[17].paletteIndexes, g_tileTexturePool[17], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[18].paletteIndexes, g_tileTexturePool[18], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[19].paletteIndexes, g_tileTexturePool[19], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[20].paletteIndexes, g_tileTexturePool[20], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[21].paletteIndexes, g_tileTexturePool[21], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[22].paletteIndexes, g_tileTexturePool[22], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[23].paletteIndexes, g_tileTexturePool[23], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[24].paletteIndexes, g_tileTexturePool[24], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[25].paletteIndexes, g_tileTexturePool[25], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[26].paletteIndexes, g_tileTexturePool[26], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[27].paletteIndexes, g_tileTexturePool[27], sizeof( u8 ) * 256 );
         break;
      case 1:
         memcpy( &tileMap->tileTextures[0].paletteIndexes, g_tileTexturePool[0], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[1].paletteIndexes, g_tileTexturePool[1], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[2].paletteIndexes, g_tileTexturePool[3], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[3].paletteIndexes, g_tileTexturePool[9], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[4].paletteIndexes, g_tileTexturePool[28], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[5].paletteIndexes, g_tileTexturePool[29], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[6].paletteIndexes, g_tileTexturePool[30], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[7].paletteIndexes, g_tileTexturePool[31], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[8].paletteIndexes, g_tileTexturePool[32], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[9].paletteIndexes, g_tileTexturePool[33], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[10].paletteIndexes, g_tileTexturePool[34], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[11].paletteIndexes, g_tileTexturePool[35], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[12].paletteIndexes, g_tileTexturePool[36], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[13].paletteIndexes, g_tileTexturePool[37], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[14].paletteIndexes, g_tileTexturePool[38], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[15].paletteIndexes, g_tileTexturePool[39], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[16].paletteIndexes, g_tileTexturePool[40], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[17].paletteIndexes, g_tileTexturePool[41], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[18].paletteIndexes, g_tileTexturePool[42], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[19].paletteIndexes, g_tileTexturePool[8], sizeof( u8 ) * 256 );
         break;
      case 2:
         memcpy( &tileMap->tileTextures[0].paletteIndexes, g_tileTexturePool[9], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[1].paletteIndexes, g_tileTexturePool[43], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[2].paletteIndexes, g_tileTexturePool[44], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[3].paletteIndexes, g_tileTexturePool[45], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[4].paletteIndexes, g_tileTexturePool[46], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[5].paletteIndexes, g_tileTexturePool[47], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[6].paletteIndexes, g_tileTexturePool[48], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[7].paletteIndexes, g_tileTexturePool[49], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[8].paletteIndexes, g_tileTexturePool[50], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[9].paletteIndexes, g_tileTexturePool[8], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[10].paletteIndexes, g_tileTexturePool[51], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[11].paletteIndexes, g_tileTexturePool[52], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[12].paletteIndexes, g_tileTexturePool[53], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[13].paletteIndexes, g_tileTexturePool[54], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[14].paletteIndexes, g_tileTexturePool[55], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[15].paletteIndexes, g_tileTexturePool[56], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[16].paletteIndexes, g_tileTexturePool[57], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[17].paletteIndexes, g_tileTexturePool[58], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[18].paletteIndexes, g_tileTexturePool[59], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[19].paletteIndexes, g_tileTexturePool[60], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[20].paletteIndexes, g_tileTexturePool[61], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[21].paletteIndexes, g_tileTexturePool[62], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[22].paletteIndexes, g_tileTexturePool[63], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[23].paletteIndexes, g_tileTexturePool[64], sizeof( u8 ) * 256 );
         memcpy( &tileMap->tileTextures[24].paletteIndexes, g_tileTexturePool[8], sizeof( u8 ) * 256 );
         break;
   }
}

internal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index )
{
   switch ( index )
   {
      case 0:
         memcpy( &tileMap->staticSpriteTextures[0].paletteIndexes, g_staticSpriteTexturePool[0], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[1].paletteIndexes, g_staticSpriteTexturePool[1], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[2].paletteIndexes, g_staticSpriteTexturePool[2], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[3].paletteIndexes, g_staticSpriteTexturePool[3], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[4].paletteIndexes, g_staticSpriteTexturePool[4], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[5].paletteIndexes, g_staticSpriteTexturePool[5], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[6].paletteIndexes, g_staticSpriteTexturePool[6], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[7].paletteIndexes, g_staticSpriteTexturePool[7], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[8].paletteIndexes, g_staticSpriteTexturePool[8], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[9].paletteIndexes, g_staticSpriteTexturePool[9], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[10].paletteIndexes, g_staticSpriteTexturePool[10], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[11].paletteIndexes, g_staticSpriteTexturePool[11], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[12].paletteIndexes, g_staticSpriteTexturePool[13], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[13].paletteIndexes, g_staticSpriteTexturePool[30], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[14].paletteIndexes, g_staticSpriteTexturePool[32], sizeof( u8 ) * 256 );
         break;
      case 1:
         memcpy( &tileMap->staticSpriteTextures[0].paletteIndexes, g_staticSpriteTexturePool[0], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[1].paletteIndexes, g_staticSpriteTexturePool[1], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[2].paletteIndexes, g_staticSpriteTexturePool[2], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[3].paletteIndexes, g_staticSpriteTexturePool[3], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[4].paletteIndexes, g_staticSpriteTexturePool[12], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[5].paletteIndexes, g_staticSpriteTexturePool[13], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[6].paletteIndexes, g_staticSpriteTexturePool[14], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[7].paletteIndexes, g_staticSpriteTexturePool[15], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[8].paletteIndexes, g_staticSpriteTexturePool[16], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[9].paletteIndexes, g_staticSpriteTexturePool[17], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[10].paletteIndexes, g_staticSpriteTexturePool[18], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[11].paletteIndexes, g_staticSpriteTexturePool[19], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[12].paletteIndexes, g_staticSpriteTexturePool[20], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[13].paletteIndexes, g_staticSpriteTexturePool[21], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[14].paletteIndexes, g_staticSpriteTexturePool[22], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[15].paletteIndexes, g_staticSpriteTexturePool[23], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[16].paletteIndexes, g_staticSpriteTexturePool[24], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[17].paletteIndexes, g_staticSpriteTexturePool[25], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[18].paletteIndexes, g_staticSpriteTexturePool[26], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[19].paletteIndexes, g_staticSpriteTexturePool[27], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[20].paletteIndexes, g_staticSpriteTexturePool[28], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[21].paletteIndexes, g_staticSpriteTexturePool[29], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[22].paletteIndexes, g_staticSpriteTexturePool[30], sizeof( u8 ) * 256 );
         break;
      case 2:
         memcpy( &tileMap->staticSpriteTextures[0].paletteIndexes, g_staticSpriteTexturePool[12], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[1].paletteIndexes, g_staticSpriteTexturePool[13], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[2].paletteIndexes, g_staticSpriteTexturePool[28], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[3].paletteIndexes, g_staticSpriteTexturePool[29], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[4].paletteIndexes, g_staticSpriteTexturePool[30], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[5].paletteIndexes, g_staticSpriteTexturePool[23], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[6].paletteIndexes, g_staticSpriteTexturePool[31], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[7].paletteIndexes, g_staticSpriteTexturePool[33], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[8].paletteIndexes, g_staticSpriteTexturePool[34], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[9].paletteIndexes, g_staticSpriteTexturePool[35], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[10].paletteIndexes, g_staticSpriteTexturePool[36], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[11].paletteIndexes, g_staticSpriteTexturePool[37], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[12].paletteIndexes, g_staticSpriteTexturePool[38], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[13].paletteIndexes, g_staticSpriteTexturePool[39], sizeof( u8 ) * 256 );
         memcpy( &tileMap->staticSpriteTextures[14].paletteIndexes, g_staticSpriteTexturePool[40], sizeof( u8 ) * 256 );
         break;
   }
}

internal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index )
{
   switch ( index )
   {
      case 0:
         memcpy( &tileMap->activeSpriteTextures[0].paletteIndexes, g_activeSpriteTexturePool[0], sizeof( u8 ) * 2048 );
         break;
   }
}

void TileMap_LoadPlayerSprites( TileMap_t* tileMap )
{
   u32 i;

   for ( i = 0; i < tileMap->getPlayerCountFunc( tileMap->playerCountProvider ); i++ )
   {
      memcpy( tileMap->playerSpriteTextures + i, g_playerSpriteTexturePool[tileMap->players[i].playerClass], sizeof( u8 ) * 2048 );
      TileMap_LoadActiveSpriteData( tileMap->playerSprites + i, i, 2, 4, Direction_Down );
   }
}

void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )
{
   switch( index )
   {
      case 0: // Overworld
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadInitialData( tileMap, 256, 256, True, True, False, True, 7, 0, 5, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 0, 55724, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 1, 55725, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 2, 55980, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 3, 55981, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 6, 57247, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 4, 55717, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 5, 55973, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 55980, 1, 817, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 1, 55981, 1, 817, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 2, 51363, 12, 1250, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 3, 57247, 14, 27, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 4, 55973, 22, 1444, Direction_Up );
         memcpy( tileMap->tiles, g_mapTiles0, sizeof( u16 ) * 65536 );
         break;
      case 1: // Aliahan Town
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 34, 40, False, False, False, False, 10, 1, 5, True, 1, 1 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 759, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 999, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 6, 1075, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 7, 967, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 8, 1180, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 9, 1151, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 0, 118, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 1, 119, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 2, 152, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 3, 153, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         ActiveSprite_Init( tileMap->activeSprites + 0, 0, 2, 4, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 0, 548, 2, 131, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 1, 549, 2, 132, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 2, 879, 4, 22, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 3, 152, 6, 975, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 4, 153, 6, 976, Direction_Up );
         tileMap->edgePortal.destTileMapIndex = 0; tileMap->edgePortal.destTileIndex = 55980; tileMap->edgePortal.destDirection = Direction_Down;
         TileMap_LoadEntityData( tileMap->entities + 0, 13000, 40300, 1200, 1200, tileMap->activeSprites + 0 );
         Npc_Init( tileMap->npcs + 0, tileMap->entities + 0, Direction_Right, True );
         memcpy( tileMap->tiles, g_mapTiles1, sizeof( u16 ) * 1360 );
         break;
      case 2: // Aliahan Eatery 1F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 8, 21, False, False, False, False, 7, 0, 3, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 42, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 108, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 4, 46, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 16, 66, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 16, 68, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 17, 67, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 15, 102, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 139, 1, 582, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 140, 1, 583, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 2, 46, 3, 63, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles2, sizeof( u16 ) * 168 );
         break;
      case 3: // Aliahan Eatery 2F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 11, 24, False, False, False, False, 13, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 63, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 79, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 16, 156, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 16, 158, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 16, 160, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 16, 162, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 16, 189, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 16, 193, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 16, 195, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 17, 157, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 10, 17, 161, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 11, 17, 190, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 12, 17, 194, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 63, 2, 46, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles3, sizeof( u16 ) * 264 );
         break;
      case 4: // Aliahan House 1F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 5, 6, False, False, False, False, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 4, 13, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 27, 1, 913, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 13, 5, 14, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles4, sizeof( u16 ) * 30 );
         break;
      case 5: // Aliahan House 2F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 8, 8, False, False, False, False, 6, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 14, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 10, 9, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 11, 10, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 13, 35, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 13, 37, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 14, 36, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 14, 4, 13, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles5, sizeof( u16 ) * 64 );
         break;
      case 6: // Aliahan Castle 1F Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 32, 32, False, False, False, False, 22, 0, 7, True, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 19, 462, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 19, 465, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 19, 494, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 19, 497, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 19, 526, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 19, 529, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 4, 336, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 20, 138, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 20, 486, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 20, 604, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 10, 20, 920, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 11, 21, 294, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 12, 17, 661, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 13, 17, 663, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 14, 17, 757, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 15, 17, 759, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 16, 15, 259, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 17, 15, 260, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 18, 15, 291, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 19, 15, 292, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 20, 15, 323, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 21, 15, 324, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 1006, 1, 152, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 1007, 1, 152, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 2, 1008, 1, 153, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 3, 1009, 1, 153, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 4, 137, 7, 46, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 5, 150, 8, 49, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 6, 336, 11, 408, Direction_Right );
         tileMap->edgePortal.destTileMapIndex = 1; tileMap->edgePortal.destTileIndex = 152; tileMap->edgePortal.destDirection = Direction_Down;
         memcpy( tileMap->tiles, g_mapTiles6, sizeof( u16 ) * 1024 );
         break;
      case 7: // Aliahan Castle 1F NW
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 12, 6, False, False, False, False, 4, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 10, 14, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 11, 15, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 16, 30, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 17, 31, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 47, 6, 138, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles7, sizeof( u16 ) * 72 );
         break;
      case 8: // Aliahan Castle 1F Hallway
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 12, 26, False, False, False, False, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 282, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 48, 6, 149, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 282, 9, 106, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles8, sizeof( u16 ) * 312 );
         break;
      case 9: // Aliahan Castle Prison Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 12, 13, False, False, True, False, 3, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 4, 106, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 22, 75, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 22, 80, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 106, 8, 282, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 1, 146, 10, 14, Direction_Down );
         memcpy( tileMap->tiles, g_mapTiles9, sizeof( u16 ) * 156 );
         break;
      case 10: // Aliahan Castle Prison Hallway
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 12, 5, False, False, True, False, 2, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 20, 29, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 5, 34, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 2, 9, 134, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 1, 34, 19, 2067, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles10, sizeof( u16 ) * 60 );
         break;
      case 11: // Aliahan Castle Throne Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 19, 24, False, False, False, False, 1, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 408, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 408, 6, 336, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles11, sizeof( u16 ) * 456 );
         break;
      case 12: // Reeve Hidden Area Outside
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 49, 42, False, False, False, False, 2, 0, 15, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 12, 1001, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 13, 1009, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 755, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 757, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 2, 758, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 3, 759, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 4, 761, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 5, 947, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 6, 996, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 7, 1012, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 8, 1143, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 9, 1192, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 10, 1291, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 11, 1295, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 12, 1299, 0, 51363, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 13, 911, 13, 22, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 14, 1001, 19, 269, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles12, sizeof( u16 ) * 2058 );
         break;
      case 13: // Reeve Hidden Area Inside
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 5, 6, False, False, False, False, 0, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 27, 12, 960, Direction_Down );
         memcpy( tileMap->tiles, g_mapTiles13, sizeof( u16 ) * 30 );
         break;
      case 14: // Promontory Cave B1
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 10, 12, False, False, True, True, 2, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 7, 27, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 8, 92, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 27, 0, 57247, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 92, 15, 1307, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles14, sizeof( u16 ) * 120 );
         break;
      case 15: // Promontory Cave B2 SW
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 50, 33, False, False, True, True, 2, 0, 9, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 7, 1307, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 489, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 1307, 14, 92, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 1, 1280, 16, 49, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 2, 1330, 16, 61, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 3, 94, 17, 294, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 4, 95, 17, 295, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 5, 748, 17, 634, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 6, 798, 17, 660, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 7, 248, 18, 17, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 8, 298, 18, 25, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles15, sizeof( u16 ) * 1650 );
         break;
      case 16: // Promontory Cave B2 South Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 12, 10, False, False, True, True, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 55, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 48, 15, 1279, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 60, 15, 1329, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles16, sizeof( u16 ) * 120 );
         break;
      case 17: // Promontory Cave B2 NE
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 26, 30, False, False, True, True, 1, 0, 4, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 135, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 321, 15, 145, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 320, 15, 144, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 2, 633, 15, 747, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 3, 659, 15, 797, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles17, sizeof( u16 ) * 780 );
         break;
      case 18: // Promontory Cave B2 Stairs
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 8, 6, False, False, True, True, 1, 0, 3, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 7, 28, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 16, 15, 247, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 24, 15, 297, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 2, 28, 19, 2413, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles18, sizeof( u16 ) * 48 );
         break;
      case 19: // Najima B1 Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 56, 48, False, False, True, True, 12, 0, 8, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 13, 269, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 13, 2067, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 13, 2429, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 14, 2413, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 9, 712, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 10, 713, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 11, 768, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 12, 769, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 9, 690, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 10, 691, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 10, 11, 746, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 11, 12, 747, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 2413, 18, 28, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 2067, 10, 34, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 2, 269, 12, 1001, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 3, 689, 20, 48, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 4, 745, 20, 58, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 5, 824, 21, 14, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 6, 825, 21, 15, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 7, 2429, 22, 1343, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles19, sizeof( u16 ) * 2688 );
         break;
      case 20: // Najima B1 West Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 10, 10, False, False, True, True, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 34, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 49, 19, 690, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 1, 59, 19, 746, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles20, sizeof( u16 ) * 100 );
         break;
      case 21: // Najima B1 East Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 10, 12, False, False, True, True, 2, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 73, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 75, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 4, 19, 768, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 1, 5, 19, 769, Direction_Up );
         memcpy( tileMap->tiles, g_mapTiles21, sizeof( u16 ) * 120 );
         break;
      case 22: // Najima 1F Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 34, 44, False, False, False, True, 1, 0, 1, True, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 14, 1343, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 1343, 19, 2429, Direction_Left );
         tileMap->edgePortal.destTileMapIndex = 0; tileMap->edgePortal.destTileIndex = 55973; tileMap->edgePortal.destDirection = Direction_Down;
         memcpy( tileMap->tiles, g_mapTiles22, sizeof( u16 ) * 1496 );
         break;
   }
}

void Enemy_LoadFromId( Enemy_t* enemy, u32 id )
{
   UNUSED_PARAM( id );
   enemy->name[0] = '\0';
}

void Game_Reset( Game_t* game )
{
   u32 i;

   TileMap_LoadFromIndex( &game->tileMap, 0 );
   game->playerCount = 1;

   for ( i = 0; i < MAX_PLAYERS; i++ )
   {
      Player_Init( game->players + i );
      game->players[i].playerClass = ( i == 0 ) ? PlayerClass_Hero : PlayerClass_Soldier;
      game->players[i].entity = game->tileMap.playerEntities + i;
      game->players[i].entity->sprite = game->tileMap.playerSprites + i;
      TileMap_CenterEntityOnTile( &game->tileMap, game->players[i].entity, 56746 );
      game->players[i].entity->pos.w = 1200;
      game->players[i].entity->pos.h = 1200;
      game->players[i].entity->prevPos = game->players[0].entity->pos;
      game->players[i].entity->velocity.x = 0;
      game->players[i].entity->velocity.y = 0;
   }

   TileMap_LoadPlayerSprites( &game->tileMap );

   for ( i = 0; i < MAX_PLAYERS; i++ )
   {
      ActiveSprite_SetDirection( game->players[i].entity->sprite, Direction_Down );
   }

   TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );
   game->isAM = False;
   game->daylightFactor = 1.0f; // noon
   game->screen.dayFilterIntensity = 1.0f;
}
