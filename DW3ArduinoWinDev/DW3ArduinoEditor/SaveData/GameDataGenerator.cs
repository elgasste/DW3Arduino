using System.IO;
using System.Text;

namespace DW3ArduinoEditor.SaveData
{
   public class GameDataGenerator
   {
      private GameSaveData _gameSaveData = new();

      public void WriteGameDataSourceFile( GameSaveData saveData )
      {
         _gameSaveData = saveData;

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

      private void WriteTileMapFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _gameSaveData.TileMaps.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", _gameSaveData.TileMaps[i].Index ) );
            WriteToFileStream( fs, string.Format( "         tileMap->entityCount = 1;\n" ) );
            WriteToFileStream( fs, string.Format( "         tileMap->tilesX = {0}; tileMap->tilesY = {1}; tileMap->wraps = {2};\n", _gameSaveData.TileMaps[i].TilesX, _gameSaveData.TileMaps[i].TilesY, _gameSaveData.TileMaps[i].Wraps ? "True" : "False" ) );

            for ( int j = 0; j < _gameSaveData.TileMaps[i].Tiles.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         tileMap->tiles[{0}] = {1} | ( {2} << 5 );\n", j, _gameSaveData.TileMaps[i].Tiles[j].TextureIndex, _gameSaveData.TileMaps[i].Tiles[j].IsPassable ? "0x1" : "0x0" ) );
            }

            WriteToFileStream( fs, string.Format( "         tileMap->portalCount = {0};\n", _gameSaveData.TileMaps[i].Portals.Count ) );

            for ( int j = 0; j < _gameSaveData.TileMaps[i].Portals.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].sourceTileIndex = {1}; tileMap->portals[{0}].destTileMapIndex = {2}; tileMap->portals[{0}].destTileIndex = {3};\n", j, _gameSaveData.TileMaps[i].Portals[j].SourceTileIndex, _gameSaveData.TileMaps[i].Portals[j].DestTileMapIndex, _gameSaveData.TileMaps[i].Portals[j].DestTileIndex ) );
            }

            if ( _gameSaveData.TileMaps[i].EdgePortal is not null )
            {
               WriteToFileStream( fs, "         tileMap->hasEdgePortal = True;\n" );
               WriteToFileStream( fs, string.Format( "         tileMap->edgePortal.destTileMapIndex = {0}; tileMap->edgePortal.destTileIndex = {1};\n", _gameSaveData.TileMaps[i].EdgePortal?.DestTileMapIndex, _gameSaveData.TileMaps[i].EdgePortal?.DestTileIndex ) );
            }
            else
            {
               WriteToFileStream( fs, "         tileMap->hasEdgePortal = False;\n" );
            }

            WriteToFileStream( fs, "         tileMap->npcCount = 0;\n" );
            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private static void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
