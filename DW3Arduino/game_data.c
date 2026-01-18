// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!

#include "text_bit_fields.h"
#include "tile_textures.h"
#include "static_sprite_textures.h"
#include "active_sprite_textures.h"
#include "player_sprite_textures.h"
#include "tile_maps.h"
#include "game.h"
#include "random.h"

internal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index );
internal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );
internal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );
internal void TileMap_LoadPlayerSprites( TileMap_t* tileMap );

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
         TileMap_LoadPortalData( tileMap->portals + 0, 582, 2, 115, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 583, 2, 116, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 2, 913, 4, 17, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 3, 152, 6, 884, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 4, 153, 6, 885, Direction_Left );
         tileMap->edgePortal.destTileMapIndex = 0; tileMap->edgePortal.destTileIndex = 55980; tileMap->edgePortal.destDirection = Direction_Down;
         TileMap_LoadEntityData( tileMap->entities + 0, 13000, 40300, 1200, 1200, tileMap->activeSprites + 0 );
         Npc_Init( tileMap->npcs + 0, tileMap->entities + 0, Direction_Right, True );
         memcpy( tileMap->tiles, g_mapTiles1, sizeof( u16 ) * 1360 );
         break;
      case 2: // Aliahan Eatery 1F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 8, 15, False, False, False, False, 7, 0, 3, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 18, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 84, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 4, 22, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 16, 42, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 16, 44, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 17, 43, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 15, 78, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 115, 1, 582, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 116, 1, 583, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 2, 22, 3, 30, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles2, sizeof( u16 ) * 120 );
         break;
      case 3: // Aliahan Eatery 2F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 11, 18, False, False, False, False, 13, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 30, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 46, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 16, 123, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 16, 125, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 16, 127, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 16, 129, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 16, 156, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 16, 160, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 16, 162, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 17, 124, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 10, 17, 128, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 11, 17, 157, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 12, 17, 161, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 30, 2, 22, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles3, sizeof( u16 ) * 198 );
         break;
      case 4: // Aliahan House 1F
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 5, 4, False, False, False, False, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 4, 8, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 17, 1, 913, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 8, 5, 14, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles4, sizeof( u16 ) * 20 );
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
         TileMap_LoadPortalData( tileMap->portals + 0, 14, 4, 8, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles5, sizeof( u16 ) * 64 );
         break;
      case 6: // Aliahan Castle 1F Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 30, 30, False, False, False, False, 22, 0, 7, True, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 19, 403, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 19, 406, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 19, 433, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 19, 436, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 19, 463, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 19, 466, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 4, 285, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 20, 99, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 20, 425, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 20, 537, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 10, 20, 833, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 11, 21, 245, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 12, 17, 590, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 13, 17, 592, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 14, 17, 680, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 15, 17, 682, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 16, 15, 212, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 17, 15, 213, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 18, 15, 242, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 19, 15, 243, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 20, 15, 272, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 21, 15, 273, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 883, 1, 152, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 884, 1, 152, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 2, 885, 1, 153, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 3, 886, 1, 153, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 4, 99, 7, 39, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 5, 110, 8, 30, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 6, 285, 11, 408, Direction_Left );
         tileMap->edgePortal.destTileMapIndex = 1; tileMap->edgePortal.destTileIndex = 152; tileMap->edgePortal.destDirection = Direction_Left;
         memcpy( tileMap->tiles, g_mapTiles6, sizeof( u16 ) * 900 );
         break;
      case 7: // Aliahan Castle 1F NW
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 10, 6, False, False, False, False, 4, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 10, 11, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 11, 12, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 16, 25, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 17, 26, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 39, 6, 99, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles7, sizeof( u16 ) * 60 );
         break;
      case 8: // Aliahan Castle 1F Hallway
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 10, 24, False, False, False, False, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 226, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 30, 6, 110, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 226, 9, 94, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles8, sizeof( u16 ) * 240 );
         break;
      case 9: // Aliahan Castle Prison Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 12, 11, False, False, True, False, 3, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 4, 94, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 22, 63, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 22, 68, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 94, 8, 226, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 122, 10, 14, Direction_Down );
         memcpy( tileMap->tiles, g_mapTiles9, sizeof( u16 ) * 132 );
         break;
      case 10: // Aliahan Castle Prison Hallway
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 12, 3, False, False, True, False, 2, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 20, 17, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 5, 22, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 2, 9, 110, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 1, 22, 19, 1887, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles10, sizeof( u16 ) * 36 );
         break;
      case 11: // Aliahan Castle Throne Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 19, 24, False, False, False, False, 1, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 5, 408, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 408, 6, 285, Direction_Left );
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
         TileMap_LoadPortalData( tileMap->portals + 13, 960, 13, 12, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 14, 1001, 19, 153, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles12, sizeof( u16 ) * 2058 );
         break;
      case 13: // Reeve Hidden Area Inside
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 1 );
         TileMap_LoadInitialData( tileMap, 5, 4, False, False, False, False, 0, 0, 1, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 17, 12, 1009, Direction_Down );
         memcpy( tileMap->tiles, g_mapTiles13, sizeof( u16 ) * 20 );
         break;
      case 14: // Promontory Cave B1
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 10, 12, False, False, True, True, 2, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 7, 27, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 8, 92, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 27, 0, 57247, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 92, 15, 1109, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles14, sizeof( u16 ) * 120 );
         break;
      case 15: // Promontory Cave B2 SW
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 46, 29, False, False, True, True, 2, 0, 9, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 7, 1109, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 359, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 1109, 14, 92, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 1, 1085, 16, 41, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 2, 1131, 16, 51, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 3, 42, 17, 182, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 4, 43, 17, 183, Direction_Up );
         TileMap_LoadPortalData( tileMap->portals + 5, 597, 17, 493, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 6, 643, 17, 515, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 7, 137, 18, 13, Direction_Right );
         TileMap_LoadPortalData( tileMap->portals + 8, 183, 18, 19, Direction_Right );
         memcpy( tileMap->tiles, g_mapTiles15, sizeof( u16 ) * 1334 );
         break;
      case 16: // Promontory Cave B2 South Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 10, 10, False, False, True, True, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 46, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 40, 15, 1084, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 50, 15, 1130, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles16, sizeof( u16 ) * 100 );
         break;
      case 17: // Promontory Cave B2 NE
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 22, 26, False, False, True, True, 1, 0, 4, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 69, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 204, 15, 88, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 1, 205, 15, 89, Direction_Down );
         TileMap_LoadPortalData( tileMap->portals + 2, 492, 15, 596, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 3, 514, 15, 642, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles17, sizeof( u16 ) * 572 );
         break;
      case 18: // Promontory Cave B2 Stairs
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 6, 6, False, False, True, True, 1, 0, 3, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 7, 21, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 12, 15, 136, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 18, 15, 182, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 2, 21, 19, 2219, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles18, sizeof( u16 ) * 36 );
         break;
      case 19: // Najima B1 Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 54, 46, False, False, True, True, 12, 0, 8, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 13, 153, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 13, 1887, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 2, 13, 2235, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 3, 14, 2219, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 4, 9, 580, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 5, 10, 581, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 6, 11, 634, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 7, 12, 635, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 8, 9, 558, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 9, 10, 559, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 10, 11, 612, False );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 11, 12, 613, False );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 2219, 18, 21, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 1887, 10, 22, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 2, 153, 12, 1001, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 3, 558, 20, 39, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 4, 612, 20, 47, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 5, 634, 21, 4, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 6, 635, 21, 5, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 7, 2235, 22, 1343, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles19, sizeof( u16 ) * 2484 );
         break;
      case 20: // Najima B1 West Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 8, 10, False, False, True, True, 1, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 27, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 39, 19, 558, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 47, 19, 612, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles20, sizeof( u16 ) * 80 );
         break;
      case 21: // Najima B1 East Room
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 10, 10, False, False, True, True, 2, 0, 2, False, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 6, 63, True );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 1, 6, 65, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 4, 19, 634, Direction_Left );
         TileMap_LoadPortalData( tileMap->portals + 1, 5, 19, 635, Direction_Left );
         memcpy( tileMap->tiles, g_mapTiles21, sizeof( u16 ) * 100 );
         break;
      case 22: // Najima 1F Main
         TileMap_LoadTileTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadInitialData( tileMap, 34, 44, False, False, False, True, 1, 0, 1, True, 0, 0 );
         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, 2 );
         TileMap_LoadStaticSpriteData( tileMap->staticSprites + 0, 14, 1343, True );
         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, 0 );
         TileMap_LoadPortalData( tileMap->portals + 0, 1343, 19, 2235, Direction_Left );
         tileMap->edgePortal.destTileMapIndex = 0; tileMap->edgePortal.destTileIndex = 55973; tileMap->edgePortal.destDirection = Direction_Down;
         memcpy( tileMap->tiles, g_mapTiles22, sizeof( u16 ) * 1496 );
         break;
   }
}

void Game_Reset( Game_t* game )
{
   TileMap_LoadFromIndex( &game->tileMap, 0 );
   game->playerCount = 3;

   game->players[0].playerClass = PlayerClass_Hero;
   game->players[0].entity = game->tileMap.playerEntities;
   game->players[0].entity->sprite = game->tileMap.playerSprites;
   game->players[0].entity->pos.x = 2722 * UNITS_PER_PIXEL;
   game->players[0].entity->pos.y = 3538 * UNITS_PER_PIXEL;
   game->players[0].entity->pos.w = 12 * UNITS_PER_PIXEL;
   game->players[0].entity->pos.h = 12 * UNITS_PER_PIXEL;
   game->players[0].entity->prevPos = game->players[0].entity->pos;
   game->players[0].entity->velocity.x = 0;
   game->players[0].entity->velocity.y = 0;
   game->players[0].tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players[0].entity->pos.x, (u32)game->players[0].entity->pos.y );

   game->players[1].playerClass = PlayerClass_Soldier;
   game->players[1].entity = game->tileMap.playerEntities + 1;
   game->players[1].entity->sprite = game->tileMap.playerSprites + 1;
   game->players[1].entity->pos.x = 2722 * UNITS_PER_PIXEL;
   game->players[1].entity->pos.y = 3538 * UNITS_PER_PIXEL;
   game->players[1].entity->pos.w = 12 * UNITS_PER_PIXEL;
   game->players[1].entity->pos.h = 12 * UNITS_PER_PIXEL;
   game->players[1].entity->prevPos = game->players[1].entity->pos;
   game->players[1].entity->velocity.x = 0;
   game->players[1].entity->velocity.y = 0;
   game->players[1].tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players[1].entity->pos.x, (u32)game->players[1].entity->pos.y );

   game->players[2].playerClass = PlayerClass_GoofOff;
   game->players[2].entity = game->tileMap.playerEntities + 2;
   game->players[2].entity->sprite = game->tileMap.playerSprites + 2;
   game->players[2].entity->pos.x = 2722 * UNITS_PER_PIXEL;
   game->players[2].entity->pos.y = 3538 * UNITS_PER_PIXEL;
   game->players[2].entity->pos.w = 12 * UNITS_PER_PIXEL;
   game->players[2].entity->pos.h = 12 * UNITS_PER_PIXEL;
   game->players[2].entity->prevPos = game->players[2].entity->pos;
   game->players[2].entity->velocity.x = 0;
   game->players[2].entity->velocity.y = 0;
   game->players[2].tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players[2].entity->pos.x, (u32)game->players[2].entity->pos.y );

   TileMap_LoadPlayerSprites( &game->tileMap );
   ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );
   TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );
   game->isAM = False;
   game->daylightFactor = 1.0f; // noon
   game->screen.dayFilterIntensity = 1.0f;
}
