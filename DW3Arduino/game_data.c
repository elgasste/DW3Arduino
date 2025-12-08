// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!

#include "game.h"
#include "random.h"

void Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index )
{
   u32 i;

   UNUSED_PARAM( index );

   for ( i = 0; i < SCREEN_PALETTE_SIZE; i++ )
   {
      screen->palette[i] = COLOR16_BLACK;
   }

   screen->palette[1] = 0x0640;
   screen->palette[2] = 0x0420;
   screen->palette[3] = 0x0838;
   screen->palette[4] = 0x0838;
   screen->palette[5] = COLOR16_MAGENTA;
   screen->palette[6] = COLOR16_CYAN;
}

void TileMap_LoadTileTextures( TileMap_t* tileMap )
{
   u32 i, j;

   // we can just use solid-colored textures for now
   for ( i = 0; i < 7; i++ )
   {
      for ( j = 0; j < TILEMAP_TILE_SIZE * TILEMAP_TILE_SIZE; j++ )
      {
         tileMap->tileTextures[i].paletteIndexes[j] = (u8)i;
      }
   }
}

void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )
{
   u32 i;
   u16* m = tileMap->tiles;

   switch( index )
   {
      case 0:
         tileMap->tilesX = 32; tileMap->tilesY = 32; tileMap->wraps = True;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 1; tileMap->hasEdgePortal = False;
         tileMap->portals[0].sourceTileIndex = 201; tileMap->portals[0].destTileMapIndex = 1; tileMap->portals[0].destTileIndex = 285;
         for ( i = 0; i < 1024; i++ ) m[i] = 0x0021;
         m[6] = 0x0002;
         m[7] = 0x0002;
         m[9] = 0x0002;
         m[11] = 0x0002;
         m[13] = 0x0002;
         m[21] = 0x0002;
         m[24] = 0x0002;
         m[25] = 0x0002;
         m[30] = 0x0002;
         m[33] = 0x0002;
         m[37] = 0x0002;
         m[39] = 0x0002;
         m[44] = 0x0002;
         m[45] = 0x0002;
         m[50] = 0x0002;
         m[67] = 0x0002;
         m[69] = 0x0002;
         m[70] = 0x0002;
         m[77] = 0x0002;
         m[78] = 0x0002;
         m[93] = 0x0002;
         m[97] = 0x0002;
         m[117] = 0x0002;
         m[123] = 0x0002;
         m[138] = 0x0002;
         m[140] = 0x0002;
         m[148] = 0x0002;
         m[149] = 0x0002;
         m[159] = 0x0002;
         m[173] = 0x0002;
         m[174] = 0x0002;
         m[186] = 0x0002;
         m[187] = 0x0002;
         m[195] = 0x0002;
         m[201] = 0x0026;
         m[202] = 0x0002;
         m[203] = 0x0002;
         m[204] = 0x0002;
         m[207] = 0x0002;
         m[216] = 0x0002;
         m[223] = 0x0002;
         m[226] = 0x0002;
         m[240] = 0x0002;
         m[247] = 0x0002;
         m[251] = 0x0002;
         m[253] = 0x0002;
         m[256] = 0x0002;
         m[257] = 0x0002;
         m[259] = 0x0002;
         m[260] = 0x0002;
         m[266] = 0x0002;
         m[267] = 0x0002;
         m[271] = 0x0002;
         m[272] = 0x0002;
         m[283] = 0x0002;
         m[302] = 0x0002;
         m[304] = 0x0002;
         m[312] = 0x0002;
         m[314] = 0x0002;
         m[315] = 0x0002;
         m[339] = 0x0002;
         m[346] = 0x0002;
         m[349] = 0x0002;
         m[354] = 0x0002;
         m[355] = 0x0002;
         m[358] = 0x0002;
         m[369] = 0x0002;
         m[371] = 0x0002;
         m[375] = 0x0002;
         m[385] = 0x0002;
         m[410] = 0x0002;
         m[418] = 0x0002;
         m[419] = 0x0002;
         m[422] = 0x0002;
         m[428] = 0x0002;
         m[430] = 0x0002;
         m[431] = 0x0002;
         m[449] = 0x0002;
         m[451] = 0x0002;
         m[456] = 0x0002;
         m[465] = 0x0002;
         m[481] = 0x0002;
         m[483] = 0x0002;
         m[491] = 0x0002;
         m[497] = 0x0002;
         m[501] = 0x0002;
         m[504] = 0x0002;
         m[510] = 0x0002;
         m[520] = 0x0002;
         m[525] = 0x0002;
         m[529] = 0x0002;
         m[532] = 0x0002;
         m[538] = 0x0002;
         m[548] = 0x0002;
         m[550] = 0x0002;
         m[559] = 0x0002;
         m[560] = 0x0002;
         m[564] = 0x0002;
         m[566] = 0x0002;
         m[573] = 0x0002;
         m[577] = 0x0002;
         m[596] = 0x0002;
         m[600] = 0x0002;
         m[601] = 0x0002;
         m[603] = 0x0002;
         m[609] = 0x0002;
         m[611] = 0x0002;
         m[614] = 0x0002;
         m[617] = 0x0002;
         m[618] = 0x0002;
         m[627] = 0x0002;
         m[630] = 0x0002;
         m[635] = 0x0002;
         m[645] = 0x0002;
         m[648] = 0x0002;
         m[650] = 0x0002;
         m[653] = 0x0002;
         m[660] = 0x0002;
         m[662] = 0x0002;
         m[670] = 0x0002;
         m[672] = 0x0002;
         m[674] = 0x0002;
         m[678] = 0x0002;
         m[680] = 0x0002;
         m[684] = 0x0002;
         m[685] = 0x0002;
         m[704] = 0x0002;
         m[706] = 0x0002;
         m[707] = 0x0002;
         m[721] = 0x0002;
         m[726] = 0x0002;
         m[745] = 0x0002;
         m[760] = 0x0002;
         m[766] = 0x0002;
         m[769] = 0x0002;
         m[770] = 0x0002;
         m[772] = 0x0002;
         m[791] = 0x0002;
         m[793] = 0x0002;
         m[795] = 0x0002;
         m[796] = 0x0002;
         m[801] = 0x0002;
         m[821] = 0x0002;
         m[822] = 0x0002;
         m[824] = 0x0002;
         m[828] = 0x0002;
         m[832] = 0x0002;
         m[834] = 0x0002;
         m[837] = 0x0002;
         m[838] = 0x0002;
         m[845] = 0x0002;
         m[847] = 0x0002;
         m[851] = 0x0002;
         m[852] = 0x0002;
         m[862] = 0x0002;
         m[863] = 0x0002;
         m[869] = 0x0002;
         m[876] = 0x0002;
         m[887] = 0x0002;
         m[893] = 0x0002;
         m[899] = 0x0002;
         m[909] = 0x0002;
         m[914] = 0x0002;
         m[922] = 0x0002;
         m[927] = 0x0002;
         m[931] = 0x0002;
         m[937] = 0x0002;
         m[938] = 0x0002;
         m[940] = 0x0002;
         m[953] = 0x0002;
         m[955] = 0x0002;
         m[956] = 0x0002;
         m[968] = 0x0002;
         m[974] = 0x0002;
         m[975] = 0x0002;
         m[983] = 0x0002;
         m[997] = 0x0002;
         m[999] = 0x0002;
         m[1000] = 0x0002;
         m[1003] = 0x0002;
         m[1006] = 0x0002;
         m[1008] = 0x0002;
         m[1010] = 0x0002;
         m[1014] = 0x0002;
         m[1017] = 0x0002;
         break;
      case 1:
         tileMap->tilesX = 26; tileMap->tilesY = 20; tileMap->wraps = False;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 2; tileMap->hasEdgePortal = True;
         tileMap->portals[0].sourceTileIndex = 260; tileMap->portals[0].destTileMapIndex = 3; tileMap->portals[0].destTileIndex = 39;
         tileMap->portals[1].sourceTileIndex = 377; tileMap->portals[1].destTileMapIndex = 2; tileMap->portals[1].destTileIndex = 24;
         tileMap->edgePortal.destTileMapIndex = 0; tileMap->edgePortal.destTileIndex = 201;
         for ( i = 0; i < 520; i++ ) m[i] = 0x0022;
         m[0] = 0x0003;
         m[1] = 0x0003;
         m[2] = 0x0003;
         m[3] = 0x0003;
         m[4] = 0x0003;
         m[5] = 0x0003;
         m[6] = 0x0003;
         m[7] = 0x0003;
         m[8] = 0x0003;
         m[9] = 0x0003;
         m[10] = 0x0003;
         m[11] = 0x0003;
         m[12] = 0x0003;
         m[13] = 0x0003;
         m[14] = 0x0003;
         m[15] = 0x0003;
         m[16] = 0x0003;
         m[17] = 0x0003;
         m[18] = 0x0003;
         m[19] = 0x0003;
         m[20] = 0x0003;
         m[21] = 0x0003;
         m[22] = 0x0003;
         m[23] = 0x0003;
         m[24] = 0x0003;
         m[25] = 0x0003;
         m[26] = 0x0003;
         m[52] = 0x0003;
         m[78] = 0x0003;
         m[104] = 0x0003;
         m[130] = 0x0003;
         m[156] = 0x0003;
         m[182] = 0x0003;
         m[208] = 0x0003;
         m[234] = 0x0003;
         m[286] = 0x0003;
         m[312] = 0x0003;
         m[338] = 0x0003;
         m[364] = 0x0003;
         m[377] = 0x0021;
         m[390] = 0x0003;
         m[416] = 0x0003;
         m[442] = 0x0003;
         m[468] = 0x0003;
         m[494] = 0x0003;
         m[495] = 0x0003;
         m[496] = 0x0003;
         m[497] = 0x0003;
         m[498] = 0x0003;
         m[499] = 0x0003;
         m[500] = 0x0003;
         m[501] = 0x0003;
         m[502] = 0x0003;
         m[503] = 0x0003;
         m[504] = 0x0003;
         m[505] = 0x0003;
         m[506] = 0x0003;
         m[507] = 0x0003;
         m[508] = 0x0003;
         m[509] = 0x0003;
         m[510] = 0x0003;
         m[511] = 0x0003;
         m[512] = 0x0003;
         m[513] = 0x0003;
         m[514] = 0x0003;
         m[515] = 0x0003;
         m[516] = 0x0003;
         m[517] = 0x0003;
         m[518] = 0x0003;
         m[519] = 0x0003;
         break;
      case 2:
         tileMap->tilesX = 10; tileMap->tilesY = 10; tileMap->wraps = False;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 2; tileMap->hasEdgePortal = False;
         tileMap->portals[0].sourceTileIndex = 69; tileMap->portals[0].destTileMapIndex = 3; tileMap->portals[0].destTileIndex = 20;
         tileMap->portals[1].sourceTileIndex = 24; tileMap->portals[1].destTileMapIndex = 1; tileMap->portals[1].destTileIndex = 377;
         for ( i = 0; i < 100; i++ ) m[i] = 0x0022;
         m[0] = 0x0003;
         m[1] = 0x0003;
         m[2] = 0x0003;
         m[3] = 0x0003;
         m[4] = 0x0003;
         m[5] = 0x0003;
         m[6] = 0x0003;
         m[7] = 0x0003;
         m[8] = 0x0003;
         m[9] = 0x0003;
         m[10] = 0x0003;
         m[19] = 0x0003;
         m[20] = 0x0003;
         m[24] = 0x0021;
         m[29] = 0x0003;
         m[30] = 0x0003;
         m[39] = 0x0003;
         m[40] = 0x0003;
         m[49] = 0x0003;
         m[50] = 0x0003;
         m[59] = 0x0003;
         m[60] = 0x0003;
         m[70] = 0x0003;
         m[79] = 0x0003;
         m[80] = 0x0003;
         m[89] = 0x0003;
         m[90] = 0x0003;
         m[91] = 0x0003;
         m[92] = 0x0003;
         m[93] = 0x0003;
         m[94] = 0x0003;
         m[95] = 0x0003;
         m[96] = 0x0003;
         m[97] = 0x0003;
         m[98] = 0x0003;
         m[99] = 0x0003;
         break;
      case 3:
         tileMap->tilesX = 20; tileMap->tilesY = 3; tileMap->wraps = False;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 2; tileMap->hasEdgePortal = False;
         tileMap->portals[0].sourceTileIndex = 20; tileMap->portals[0].destTileMapIndex = 2; tileMap->portals[0].destTileIndex = 69;
         tileMap->portals[1].sourceTileIndex = 39; tileMap->portals[1].destTileMapIndex = 1; tileMap->portals[1].destTileIndex = 260;
         for ( i = 0; i < 60; i++ ) m[i] = 0x0003;
         m[20] = 0x0022;
         m[21] = 0x0022;
         m[22] = 0x0022;
         m[23] = 0x0022;
         m[24] = 0x0022;
         m[25] = 0x0022;
         m[26] = 0x0022;
         m[27] = 0x0022;
         m[28] = 0x0022;
         m[29] = 0x0022;
         m[30] = 0x0022;
         m[31] = 0x0022;
         m[32] = 0x0022;
         m[33] = 0x0022;
         m[34] = 0x0022;
         m[35] = 0x0022;
         m[36] = 0x0022;
         m[37] = 0x0022;
         m[38] = 0x0022;
         m[39] = 0x0022;
         break;
   }
}
