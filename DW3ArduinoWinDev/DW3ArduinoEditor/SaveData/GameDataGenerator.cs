using System.IO;
using System.Text;

struct TileDataRun
{
   public int Start;
   public int Count;
   public ushort Value;
}

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
         WriteToFileStream( fs, "   u32 i;\n" );
         WriteToFileStream( fs, "   u16* m = tileMap->tiles;\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _gameSaveData.TileMaps.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}: // {1}\n", _gameSaveData.TileMaps[i].Index, _gameSaveData.TileMaps[i].Name ) );
            WriteToFileStream( fs, string.Format( "         tileMap->tilesX = {0}; tileMap->tilesY = {1}; tileMap->wraps = {2};\n",
               _gameSaveData.TileMaps[i].TilesX,
               _gameSaveData.TileMaps[i].TilesY,
               _gameSaveData.TileMaps[i].Wraps ? "True" : "False" ) );
            WriteToFileStream( fs, string.Format( "         tileMap->entityCount = 1; tileMap->npcCount = 0; tileMap->portalCount = {0}; tileMap->hasEdgePortal = {1};\n",
               _gameSaveData.TileMaps[i].Portals.Count,
               _gameSaveData.TileMaps[i].EdgePortal is null ? "False" : "True" ) );

            for ( int j = 0; j < _gameSaveData.TileMaps[i].Portals.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         tileMap->portals[{0}].sourceTileIndex = {1}; tileMap->portals[{0}].destTileMapIndex = {2}; tileMap->portals[{0}].destTileIndex = {3};\n", j, _gameSaveData.TileMaps[i].Portals[j].SourceTileIndex, _gameSaveData.TileMaps[i].Portals[j].DestTileMapIndex, _gameSaveData.TileMaps[i].Portals[j].DestTileIndex ) );
            }

            if ( _gameSaveData.TileMaps[i].EdgePortal is not null )
            {
               WriteToFileStream( fs, string.Format( "         tileMap->edgePortal.destTileMapIndex = {0}; tileMap->edgePortal.destTileIndex = {1};\n", _gameSaveData.TileMaps[i].EdgePortal?.DestTileMapIndex, _gameSaveData.TileMaps[i].EdgePortal?.DestTileIndex ) );
            }

            WriteCompressedTileData( fs, _gameSaveData.TileMaps[i] );

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteCompressedTileData( FileStream fs, TileMapSaveData tileMap )
      {
         List<ushort> tileValues = new( tileMap.Tiles.Count );
         Dictionary<ushort, int> valueCounts = [];

         // first pass: compile tile properties into 16-bit values and count their usage
         for ( int i = 0; i < tileMap.Tiles.Count; i++ )
         {
            var tileValue = (ushort)tileMap.Tiles[i].TextureIndex;

            if ( tileMap.Tiles[i].IsPassable )
            {
               tileValue |= ( 0x1 << 5 );
            }

            tileValues.Add( tileValue );

            if ( !valueCounts.ContainsKey( tileValue ) )
            {
               valueCounts.Add( tileValue, 1 );
            }
            else
            {
               valueCounts[tileValue]++;
            }
         }

         // find the most-used value and initialize the entire map with it
         ushort mostUsedValue = valueCounts.OrderByDescending( kvp => kvp.Value ).First().Key;
         WriteToFileStream( fs, string.Format( "         for ( i = 0; i < {0}; i++ ) m[i] = 0x{1};\n", tileValues.Count, mostUsedValue.ToString( "X4" ) ) );

         // second pass: compile a list of runs (a "run" can also have one single value)
         List<TileDataRun> runs = [];
         int runStart = 0, runCount = 0;
         ushort runValue = 0;

         for ( int i = 0; i < tileValues.Count; i++ )
         {
            if ( tileValues[i] == mostUsedValue ) // ran into most-used value, write and restart
            {
               if ( runCount > 0 )
               {
                  runs.Add( new() { Start = runStart, Count = runCount, Value = runValue } );
                  //WriteTileDataRun( fs, runStart, runCount, runValue, ref packCount );
               }

               runCount = 0;
            }
            else
            {
               if ( runCount == 0 ) // starting a new run
               {
                  runStart = i;
                  runCount = 1;
                  runValue = tileValues[i];
               }
               else if ( tileValues[i] != runValue ) // last run has ended, write and restart
               {
                  runs.Add( new() { Start = runStart, Count = runCount, Value = runValue } );
                  //WriteTileDataRun( fs, runStart, runCount, runValue, ref packCount );
                  runStart = i;
                  runCount = 1;
                  runValue = tileValues[i];
               }
               else // run is still going
               {
                  runCount++;
               }

               if ( i == ( tileValues.Count - 1 ) ) // end of the list, write and exit
               {
                  runs.Add( new() { Start = runStart, Count = runCount, Value = runValue } );
                  //WriteTileDataRun( fs, runStart, runCount, runValue, ref packCount );
               }
            }
         }

         int packCount = 0;

         // third pass: write out loops for all the runs with a counter higher than 1, packing 2 per line
         for ( int i = 0; i < runs.Count; i++ )
         {
            if ( runs[i].Count > 1 )
            {
               packCount++;

               if ( packCount == 1 )
               {
                  WriteToFileStream( fs, string.Format( "         for ( i = {0}; i < {1}; i++ ) m[i] = 0x{2};", runs[i].Start, runs[i].Start + runs[i].Count, runs[i].Value.ToString( "X4" ) ) );
               }
               else
               {
                  WriteToFileStream( fs, string.Format( " for ( i = {0}; i < {1}; i++ ) m[i] = 0x{2};", runs[i].Start, runs[i].Start + runs[i].Count, runs[i].Value.ToString( "X4" ) ) );
               }
            }

            if ( packCount == 2 || ( ( packCount > 0 ) && ( i == ( runs.Count - 1 ) ) ) )
            {
               WriteToFileStream( fs, "\n" );
               packCount = 0;
            }
         }

         packCount = 0;

         // fourth pass: write out loops for all the runs with a single count, packing 8 per line
         for ( int i = 0; i < runs.Count; i++ )
         {
            if ( runs[i].Count == 1 )
            {
               packCount++;

               if ( packCount == 1 )
               {
                  WriteToFileStream( fs, string.Format( "         m[{0}] = 0x{1};", runs[i].Start, runs[i].Value.ToString( "X4" ) ) );
               }
               else
               {
                  WriteToFileStream( fs, string.Format( " m[{0}] = 0x{1};", runs[i].Start, runs[i].Value.ToString( "X4" ) ) );
               }
            }

            if ( packCount == 8 || ( ( packCount > 0 ) && ( i == ( runs.Count - 1 ) ) ) )
            {
               WriteToFileStream( fs, "\n" );
               packCount = 0;
            }
         }
      }

      //private static void WriteTileDataRun( FileStream fs, int runStart, int runCount, ushort runValue, ref int packCount )
      //{
      //   if ( runCount == 1 ) // output single value and try to pack it with up to 7 other single values per line
      //   {
      //      packCount++;

      //      if ( packCount == 1 )
      //      {
      //         WriteToFileStream( fs, string.Format( "         m[{0}] = 0x{1};", runStart, runValue.ToString( "X4" ) ) );
      //      }
      //      else
      //      {
      //         WriteToFileStream( fs, string.Format( " m[{0}] = 0x{1};", runStart, runValue.ToString( "X4" ) ) );

      //         if ( packCount >= 8 )
      //         {
      //            WriteToFileStream( fs, "\n" );
      //            packCount = 0;
      //         }
      //      }
      //   }
      //   else
      //   {
      //      if ( packCount > 0 )
      //      {
      //         WriteToFileStream( fs, "\n" );
      //         packCount = 0;
      //      }

      //      WriteToFileStream( fs, string.Format( "         for ( i = {0}; i < {1}; i++ ) m[i] = 0x{2};\n", runStart, runStart + runCount, runValue.ToString( "X4" ) ) );
      //      packCount = 0;
      //   }
      //}

      private static void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
