// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!

#include "game.h"
#include "random.h"

internal void TileMap_LoadTileTextureFromPoolIndex( TileTexture_t* texture, u32 index );
internal void TileMap_LoadTileTexturesFromIndex( TileMap_t* tileMap, u32 index );

void Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index )
{
   screen->palette[0] = 0x9720;
   screen->palette[1] = 0x4521;
   screen->palette[2] = 0x0000;
   screen->palette[3] = 0xFCE0;
   screen->palette[4] = 0xFEB3;
   screen->palette[5] = 0x7BEF;
   screen->palette[6] = 0xB5D6;
   screen->palette[7] = 0xDA60;
   screen->palette[8] = 0x3AFE;
   screen->palette[9] = 0xFFFF;
}

internal void TileMap_LoadTileTextureFromPoolIndex( TileTexture_t* texture, u32 index )
{
   UNUSED_PARAM( texture );
   UNUSED_PARAM( index );
}

internal void TileMap_LoadTileTexturesFromIndex( TileMap_t* tileMap, u32 index )
{
   UNUSED_PARAM( tileMap );
   UNUSED_PARAM( index );
}

void TileMap_LoadTileTextures( TileMap_t* tileMap, u32 index )
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
      case 0: // Overworld
         TileMap_LoadTileTexturesFromIndex( tileMap, index );
         tileMap->tilesX = 32; tileMap->tilesY = 32; tileMap->wraps = True;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 1; tileMap->hasEdgePortal = False;
         tileMap->portals[0].sourceTileIndex = 201; tileMap->portals[0].destTileMapIndex = 1; tileMap->portals[0].destTileIndex = 285;
         for ( i = 0; i < 1024; i++ ) m[i] = 0x0021;
         for ( i = 6; i < 8; i++ ) m[i] = 0x0002; for ( i = 24; i < 26; i++ ) m[i] = 0x0002;
         for ( i = 44; i < 46; i++ ) m[i] = 0x0002; for ( i = 69; i < 71; i++ ) m[i] = 0x0002;
         for ( i = 77; i < 79; i++ ) m[i] = 0x0002; for ( i = 148; i < 150; i++ ) m[i] = 0x0002;
         for ( i = 173; i < 175; i++ ) m[i] = 0x0002; for ( i = 186; i < 188; i++ ) m[i] = 0x0002;
         for ( i = 202; i < 205; i++ ) m[i] = 0x0002; for ( i = 256; i < 258; i++ ) m[i] = 0x0002;
         for ( i = 259; i < 261; i++ ) m[i] = 0x0002; for ( i = 266; i < 268; i++ ) m[i] = 0x0002;
         for ( i = 271; i < 273; i++ ) m[i] = 0x0002; for ( i = 314; i < 316; i++ ) m[i] = 0x0002;
         for ( i = 354; i < 356; i++ ) m[i] = 0x0002; for ( i = 418; i < 420; i++ ) m[i] = 0x0002;
         for ( i = 430; i < 432; i++ ) m[i] = 0x0002; for ( i = 559; i < 561; i++ ) m[i] = 0x0002;
         for ( i = 600; i < 602; i++ ) m[i] = 0x0002; for ( i = 617; i < 619; i++ ) m[i] = 0x0002;
         for ( i = 684; i < 686; i++ ) m[i] = 0x0002; for ( i = 706; i < 708; i++ ) m[i] = 0x0002;
         for ( i = 769; i < 771; i++ ) m[i] = 0x0002; for ( i = 795; i < 797; i++ ) m[i] = 0x0002;
         for ( i = 821; i < 823; i++ ) m[i] = 0x0002; for ( i = 837; i < 839; i++ ) m[i] = 0x0002;
         for ( i = 851; i < 853; i++ ) m[i] = 0x0002; for ( i = 862; i < 864; i++ ) m[i] = 0x0002;
         for ( i = 937; i < 939; i++ ) m[i] = 0x0002; for ( i = 955; i < 957; i++ ) m[i] = 0x0002;
         for ( i = 974; i < 976; i++ ) m[i] = 0x0002; for ( i = 999; i < 1001; i++ ) m[i] = 0x0002;
         m[9] = 0x0002; m[11] = 0x0002; m[13] = 0x0002; m[21] = 0x0002; m[30] = 0x0002; m[33] = 0x0002; m[37] = 0x0002; m[39] = 0x0002;
         m[50] = 0x0002; m[67] = 0x0002; m[93] = 0x0002; m[97] = 0x0002; m[117] = 0x0002; m[123] = 0x0002; m[138] = 0x0002; m[140] = 0x0002;
         m[159] = 0x0002; m[195] = 0x0002; m[201] = 0x0026; m[207] = 0x0002; m[216] = 0x0002; m[223] = 0x0002; m[226] = 0x0002; m[240] = 0x0002;
         m[247] = 0x0002; m[251] = 0x0002; m[253] = 0x0002; m[283] = 0x0002; m[302] = 0x0002; m[304] = 0x0002; m[312] = 0x0002; m[339] = 0x0002;
         m[346] = 0x0002; m[349] = 0x0002; m[358] = 0x0002; m[369] = 0x0002; m[371] = 0x0002; m[375] = 0x0002; m[385] = 0x0002; m[410] = 0x0002;
         m[422] = 0x0002; m[428] = 0x0002; m[449] = 0x0002; m[451] = 0x0002; m[456] = 0x0002; m[465] = 0x0002; m[481] = 0x0002; m[483] = 0x0002;
         m[491] = 0x0002; m[497] = 0x0002; m[501] = 0x0002; m[504] = 0x0002; m[510] = 0x0002; m[520] = 0x0002; m[525] = 0x0002; m[529] = 0x0002;
         m[532] = 0x0002; m[538] = 0x0002; m[548] = 0x0002; m[550] = 0x0002; m[564] = 0x0002; m[566] = 0x0002; m[573] = 0x0002; m[577] = 0x0002;
         m[596] = 0x0002; m[603] = 0x0002; m[609] = 0x0002; m[611] = 0x0002; m[614] = 0x0002; m[627] = 0x0002; m[630] = 0x0002; m[635] = 0x0002;
         m[645] = 0x0002; m[648] = 0x0002; m[650] = 0x0002; m[653] = 0x0002; m[660] = 0x0002; m[662] = 0x0002; m[670] = 0x0002; m[672] = 0x0002;
         m[674] = 0x0002; m[678] = 0x0002; m[680] = 0x0002; m[704] = 0x0002; m[721] = 0x0002; m[726] = 0x0002; m[745] = 0x0002; m[760] = 0x0002;
         m[766] = 0x0002; m[772] = 0x0002; m[791] = 0x0002; m[793] = 0x0002; m[801] = 0x0002; m[824] = 0x0002; m[828] = 0x0002; m[832] = 0x0002;
         m[834] = 0x0002; m[845] = 0x0002; m[847] = 0x0002; m[869] = 0x0002; m[876] = 0x0002; m[887] = 0x0002; m[893] = 0x0002; m[899] = 0x0002;
         m[909] = 0x0002; m[914] = 0x0002; m[922] = 0x0002; m[927] = 0x0002; m[931] = 0x0002; m[940] = 0x0002; m[953] = 0x0002; m[968] = 0x0002;
         m[983] = 0x0002; m[997] = 0x0002; m[1003] = 0x0002; m[1006] = 0x0002; m[1008] = 0x0002; m[1010] = 0x0002; m[1014] = 0x0002; m[1017] = 0x0002;
         break;
      case 1: // Big Room
         TileMap_LoadTileTexturesFromIndex( tileMap, index );
         tileMap->tilesX = 26; tileMap->tilesY = 20; tileMap->wraps = False;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 2; tileMap->hasEdgePortal = True;
         tileMap->portals[0].sourceTileIndex = 260; tileMap->portals[0].destTileMapIndex = 3; tileMap->portals[0].destTileIndex = 39;
         tileMap->portals[1].sourceTileIndex = 377; tileMap->portals[1].destTileMapIndex = 2; tileMap->portals[1].destTileIndex = 24;
         tileMap->edgePortal.destTileMapIndex = 0; tileMap->edgePortal.destTileIndex = 201;
         for ( i = 0; i < 520; i++ ) m[i] = 0x0022;
         for ( i = 0; i < 27; i++ ) m[i] = 0x0003; for ( i = 494; i < 520; i++ ) m[i] = 0x0003;
         m[52] = 0x0003; m[78] = 0x0003; m[104] = 0x0003; m[130] = 0x0003; m[156] = 0x0003; m[182] = 0x0003; m[208] = 0x0003; m[234] = 0x0003;
         m[286] = 0x0003; m[312] = 0x0003; m[338] = 0x0003; m[364] = 0x0003; m[377] = 0x0021; m[390] = 0x0003; m[416] = 0x0003; m[442] = 0x0003;
         m[468] = 0x0003;
         break;
      case 2: // Small Room
         TileMap_LoadTileTexturesFromIndex( tileMap, index );
         tileMap->tilesX = 10; tileMap->tilesY = 10; tileMap->wraps = False;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 2; tileMap->hasEdgePortal = False;
         tileMap->portals[0].sourceTileIndex = 69; tileMap->portals[0].destTileMapIndex = 3; tileMap->portals[0].destTileIndex = 20;
         tileMap->portals[1].sourceTileIndex = 24; tileMap->portals[1].destTileMapIndex = 1; tileMap->portals[1].destTileIndex = 377;
         for ( i = 0; i < 100; i++ ) m[i] = 0x0022;
         for ( i = 0; i < 11; i++ ) m[i] = 0x0003; for ( i = 19; i < 21; i++ ) m[i] = 0x0003;
         for ( i = 29; i < 31; i++ ) m[i] = 0x0003; for ( i = 39; i < 41; i++ ) m[i] = 0x0003;
         for ( i = 49; i < 51; i++ ) m[i] = 0x0003; for ( i = 59; i < 61; i++ ) m[i] = 0x0003;
         for ( i = 79; i < 81; i++ ) m[i] = 0x0003; for ( i = 89; i < 100; i++ ) m[i] = 0x0003;
         m[24] = 0x0021; m[70] = 0x0003;
         break;
      case 3: // Hallway
         TileMap_LoadTileTexturesFromIndex( tileMap, index );
         tileMap->tilesX = 20; tileMap->tilesY = 3; tileMap->wraps = False;
         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = 2; tileMap->hasEdgePortal = False;
         tileMap->portals[0].sourceTileIndex = 20; tileMap->portals[0].destTileMapIndex = 2; tileMap->portals[0].destTileIndex = 69;
         tileMap->portals[1].sourceTileIndex = 39; tileMap->portals[1].destTileMapIndex = 1; tileMap->portals[1].destTileIndex = 260;
         for ( i = 0; i < 60; i++ ) m[i] = 0x0003;
         for ( i = 20; i < 40; i++ ) m[i] = 0x0022;
         break;
   }
}
