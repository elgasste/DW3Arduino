using System.IO;
using System.Text;

namespace DW3ArduinoEditor.SaveData
{
   public class GameDataGenerator
   {
      public static void WriteGameDataSourceFile()
      {
         using FileStream fs = File.Create( Constants.GameDataSourceFilePath );
         WriteHeaderSection( fs );
         WritePaletteFunction( fs );
         WriteTileTexturesFunction( fs );
         WriteTileMapFunction( fs );
      }

      private static void WriteHeaderSection( FileStream fs )
      {
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"game.h\"\n" );
         WriteToFileStream( fs, "#include \"random.h\"\n" );
      }

      private static void WritePaletteFunction( FileStream fs )
      {
         string code = string.Empty;

         code += "\nvoid Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index )\n";
         code += "{\n";
         code += "   u32 i;\n\n";
         code += "   UNUSED_PARAM( index );\n\n";
         code += "   for ( i = 0; i < SCREEN_PALETTE_SIZE; i++ )\n";
         code += "   {\n";
         code += "      screen->palette[i] = COLOR16_BLACK;\n";
         code += "   }\n\n";
         code += "   screen->palette[1] = 0x0640;\n";
         code += "   screen->palette[2] = 0x0420;\n";
         code += "   screen->palette[3] = 0x0838;\n";
         code += "   screen->palette[4] = 0x0838;\n";
         code += "   screen->palette[5] = COLOR16_MAGENTA;\n";
         code += "   screen->palette[6] = COLOR16_CYAN;\n";
         code += "}\n";

         WriteToFileStream( fs, code );
      }

      private static void WriteTileTexturesFunction( FileStream fs )
      {
         string code = string.Empty;

         code += "\nvoid TileMap_LoadTileTextures( TileMap_t* tileMap )\n";
         code += "{\n";
         code += "   u32 i, j;\n\n";
         code += "   // we can just use solid-colored textures for now\n";
         code += "   for ( i = 0; i < 7; i++ )\n";
         code += "   {\n";
         code += "      for ( j = 0; j < TILEMAP_TILE_SIZE * TILEMAP_TILE_SIZE; j++ )\n";
         code += "      {\n";
         code += "         tileMap->tileTextures[i].paletteIndexes[j] = (u8)i;\n";
         code += "      }\n";
         code += "   }\n";
         code += "}\n";

         WriteToFileStream ( fs, code );
      }

      private static void WriteTileMapFunction( FileStream fs )
      {
         string code = string.Empty;

         code += "\nvoid TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )\n";
         code += "{\n";
         code += "   u32 i;\n";
         code += "   u16* tile;\n\n";
         code += "   tileMap->wraps = False;\n";
         code += "   tileMap->hasEdgePortal = False;\n";
         code += "   tileMap->entityCount = 1;\n\n";
         code += "   if ( index == 0 ) // starting room\n";
         code += "   {\n";
         code += "      tileMap->tilesX = 26;\n";
         code += "      tileMap->tilesY = 20;\n\n";
         code += "      for ( i = 0, tile = tileMap->tiles; i < ( tileMap->tilesX * tileMap->tilesY ); i++, tile++ ) // overall tiles\n";
         code += "      {\n";
         code += "         *tile = 2 | ( 0x1 << 5 );\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles; i < tileMap->tilesX; i++, tile++ ) // top wall\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles + ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ); i < tileMap->tilesX; i++, tile++ ) // bottom wall\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles + tileMap->tilesX; i < tileMap->tilesY - 2; i++, tile += tileMap->tilesX ) // left and right walls\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "         *( tile + ( tileMap->tilesX - 1 ) ) = 3;\n";
         code += "      }\n\n";
         code += "      tileMap->tiles[( tileMap->tilesX * 10 )] = 2 | ( 0x1 << 5 );\n";
         code += "      tileMap->tiles[377] = 1 | ( 0x1 << 5 );\n\n";
         code += "      tileMap->tiles[285 - tileMap->tilesX] = 2 | ( 0x1 << 5 );\n";
         code += "      tileMap->tiles[285] = 2 | ( 0x1 << 5 );\n";
         code += "      tileMap->tiles[285 + tileMap->tilesX] = 2 | ( 0x1 << 5 );\n\n";
         code += "      tileMap->portalCount = 2;\n";
         code += "      tileMap->portals[0].sourceTileIndex = 260;\n";
         code += "      tileMap->portals[0].destTileMapIndex = 1;\n";
         code += "      tileMap->portals[0].destTileIndex = 39;\n";
         code += "      tileMap->portals[1].sourceTileIndex = 377;\n";
         code += "      tileMap->portals[1].destTileMapIndex = 2;\n";
         code += "      tileMap->portals[1].destTileIndex = 24;\n\n";
         code += "      tileMap->hasEdgePortal = True;\n";
         code += "      tileMap->edgePortal.destTileMapIndex = 3;\n";
         code += "      tileMap->edgePortal.destTileIndex = 201;\n";
         code += "   }\n";
         code += "   else if ( index == 1 ) // hallway\n";
         code += "   {\n";
         code += "      tileMap->tilesX = 20;\n";
         code += "      tileMap->tilesY = 3;\n\n";
         code += "      tile = tileMap->tiles;\n\n";
         code += "      for ( i = 0, tile = tileMap->tiles; i < tileMap->tilesX * tileMap->tilesY; i++, tile++ ) // top wall\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles + tileMap->tilesX; i < tileMap->tilesX; i++, tile++ ) // hallway\n";
         code += "      {\n";
         code += "         *tile = 2 | ( 0x1 << 5 );\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles + ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ); i < tileMap->tilesX; i++, tile++ ) // bottom wall\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "      }\n\n";
         code += "      tileMap->portalCount = 2;\n";
         code += "      tileMap->portals[0].sourceTileIndex = 39;\n";
         code += "      tileMap->portals[0].destTileMapIndex = 0;\n";
         code += "      tileMap->portals[0].destTileIndex = 260;\n";
         code += "      tileMap->portals[1].sourceTileIndex = 20;\n";
         code += "      tileMap->portals[1].destTileMapIndex = 2;\n";
         code += "      tileMap->portals[1].destTileIndex = 69;\n";
         code += "   }\n";
         code += "   else if ( index == 2 ) // second room\n";
         code += "   {\n";
         code += "      tileMap->tilesX = 10;\n";
         code += "      tileMap->tilesY = 10;\n\n";
         code += "      for ( i = 0, tile = tileMap->tiles; i < ( tileMap->tilesX * tileMap->tilesY ); i++, tile++ ) // overall tiles\n";
         code += "      {\n";
         code += "         *tile = 2 | ( 0x1 << 5 );\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles; i < tileMap->tilesX; i++, tile++ ) // top wall\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles + ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ); i < tileMap->tilesX; i++, tile++ ) // bottom wall\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "      }\n";
         code += "      for ( i = 0, tile = tileMap->tiles + tileMap->tilesX; i < tileMap->tilesY - 2; i++, tile += tileMap->tilesX ) // left and right walls\n";
         code += "      {\n";
         code += "         *tile = 3;\n";
         code += "         *( tile + ( tileMap->tilesX - 1 ) ) = 3;\n";
         code += "      }\n\n";
         code += "      tileMap->tiles[( ( tileMap->tilesX * 7 ) - 1 )] = 2 | ( 0x1 << 5 );\n";
         code += "      tileMap->tiles[24] = 1 | ( 0x1 << 5 );\n\n";
         code += "      tileMap->portalCount = 2;\n";
         code += "      tileMap->portals[0].sourceTileIndex = 69;\n";
         code += "      tileMap->portals[0].destTileMapIndex = 1;\n";
         code += "      tileMap->portals[0].destTileIndex = 20;\n";
         code += "      tileMap->portals[1].sourceTileIndex = 24;\n";
         code += "      tileMap->portals[1].destTileMapIndex = 0;\n";
         code += "      tileMap->portals[1].destTileIndex = 377;\n";
         code += "   }\n";
         code += "   else if ( index == 3 )\n";
         code += "   {\n";
         code += "      tileMap->tilesX = 32;\n";
         code += "      tileMap->tilesY = 32;\n";
         code += "      tileMap->wraps = True;\n\n";
         code += "      for ( i = 0, tile = tileMap->tiles; i < ( tileMap->tilesX * tileMap->tilesY ); i++, tile++ ) // overall tiles\n";
         code += "      {\n";
         code += "         *tile = (u16)( Random_u32( 0, 5 ) == 0 ? 2 : 1 ) | ( 0x1 << 5 );\n";
         code += "      }\n\n";
         code += "      tileMap->tiles[201] = 6 | ( 0x1 << 5 );\n\n";
         code += "      tileMap->portalCount = 1;\n";
         code += "      tileMap->portals[0].sourceTileIndex = 201;\n";
         code += "      tileMap->portals[0].destTileMapIndex = 0;\n";
         code += "      tileMap->portals[0].destTileIndex = 285;\n";
         code += "   }\n";
         code += "}\n";

         WriteToFileStream( fs, code );
      }

      private static void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
