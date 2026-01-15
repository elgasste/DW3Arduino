using System.IO;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.Graphics;

struct TileDataRun
{
   public int Start;
   public int Count;
   public ushort Value;
}

struct TextureDataRun
{
   public int Start;
   public int Count;
   public int Value;
}

namespace DW3ArduinoEditor.SaveData
{
   public class GameDataGenerator
   {
      private GameSaveData? _gameSaveData;
      private Palette? _palette;
      private TileTexturePool? _tileTexturePool;
      private StaticSpriteTexturePool? _staticSpriteTexturePool;
      private ActiveSpriteTexturePool? _activeSpriteTexturePool;
      private ActiveSpriteTexturePool? _playerSpriteTexturePool;

      public void WriteGameDataSourceFile( GameSaveData? saveData,
                                           Palette? palette,
                                           TileTexturePool? tileTexturePool,
                                           StaticSpriteTexturePool? staticSpriteTexturePool,
                                           ActiveSpriteTexturePool? activeSpriteTexturePool,
                                           ActiveSpriteTexturePool? playerSpriteTexturePool )
      {
         _gameSaveData = saveData;
         _palette = palette;
         _tileTexturePool = tileTexturePool;
         _staticSpriteTexturePool = staticSpriteTexturePool;
         _activeSpriteTexturePool = activeSpriteTexturePool;
         _playerSpriteTexturePool = playerSpriteTexturePool;

         WriteTextBitFieldsHeader();
         WriteTileTexturesHeader();
         WriteStaticSpriteTexturesHeader();
         WriteActiveSpriteTexturesHeader( Constants.GameDataActiveSpriteTexturesHeaderPath, "active", _activeSpriteTexturePool, _gameSaveData?.HeaderGuids.ActiveSpriteTexturesHeaderGuid );
         WriteActiveSpriteTexturesHeader( Constants.GameDataPlayerSpriteTexturesHeaderPath, "player", _playerSpriteTexturePool, _gameSaveData?.HeaderGuids.PlayerSpriteTexturesHeaderGuid );
         WriteTileMapsHeader( Constants.GameDataTileMapsHeaderPath );

         using FileStream fs = File.Create( Constants.GameDataSourceFilePath );
         WriteHeaderSection( fs );
         WriteHelperFunctions( fs );
         WritePaletteFunction( fs );
         WriteTextTilesFunction( fs );
         WriteTileTextureIndexesFunction( fs );
         WriteStaticSpriteTextureIndexesFunction( fs );
         WriteActiveSpriteTextureIndexesFunction( fs );
         WritePlayerSpritesFunction( fs );
         WriteTileMapFunction( fs );
         WriteGameResetFunction( fs );
      }

      private void WriteTextBitFieldsHeader()
      {
         var textTextureMap = new List<byte>();

         var textFileStream = new FileStream( Constants.TextTilesFilePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         BitmapSource bitmap = textDecoder.Frames[0];
         BitmapUtils.CheckTextTilesBitmapFormat( bitmap );

         for ( int row = 0; row < bitmap.PixelHeight; row++ )
         {
            for ( int col = 0; col < bitmap.PixelWidth; col += 8 )
            {
               textTextureMap.Add( 0x00 );

               for ( int i = 0; i < Constants.TextTileSize; i++ )
               {
                  var pixelColor = BitmapUtils.GetBitmapPixelColor( bitmap, col + i, row );

                  if ( !Color.AreClose( pixelColor, Color.FromArgb( 255, 0, 0, 0 ) ) )
                  {
                     textTextureMap[^1] |= (byte)( 1 << ( Constants.TextTileSize - i - 1 ) );
                  }
               }
            }
         }

         using FileStream fs = File.Create( Constants.GameDataTextBitFieldsHeaderPath );

         WriteToFileStream( fs, Constants.GeneratedFileHeaderMessage );
         WriteToFileStream( fs, string.Format( "#if !defined( GEN_{0}_H )\n", _gameSaveData?.HeaderGuids.TextBitFieldsHeaderGuid ) );
         WriteToFileStream( fs, string.Format( "#define GEN_{0}_H\n\n", _gameSaveData?.HeaderGuids.TextBitFieldsHeaderGuid ) );
         WriteToFileStream( fs, "#include \"common.h\"\n\n" );

         WriteToFileStream( fs, string.Format( "const u8 g_textBitFields[{0}][{1}] = {{\n", Constants.TextTileCount, Constants.TextTileSize ) );

         for ( int i = 0; i < Constants.TextTileCount; i++ )
         {
            WriteToFileStream( fs, "   { " );

            for ( int j = 0; j < Constants.TextTileSize; j++ )
            {
               byte b = textTextureMap[i + ( j * Constants.TextTileCount )];

               WriteToFileStream( fs, string.Format( "0x{0}", b.ToString( "X2" ) ) );

               if ( j < Constants.TextTileSize - 1 )
               {
                  WriteToFileStream( fs, "," );
               }

               WriteToFileStream( fs, " " );
            }

            WriteToFileStream( fs, "}" );

            if ( i < Constants.TextTileCount - 1 )
            {
               WriteToFileStream( fs, "," );
            }

            WriteToFileStream( fs, "\n" );
         }

         WriteToFileStream( fs, "};\n\n" );

         WriteToFileStream( fs, string.Format( "#endif // GEN_{0}_H\n", _gameSaveData?.HeaderGuids.TextBitFieldsHeaderGuid ) );
      }

      private void WriteTileTexturesHeader()
      {
         using FileStream fs = File.Create( Constants.GameDataTileTexturesHeaderPath );

         WriteToFileStream( fs, Constants.GeneratedFileHeaderMessage );
         WriteToFileStream( fs, string.Format( "#if !defined( GEN{0}_H )\n", _gameSaveData?.HeaderGuids.TileTexturesHeaderGuid ) );
         WriteToFileStream( fs, string.Format( "#define GEN_{0}_H\n\n", _gameSaveData?.HeaderGuids.TileTexturesHeaderGuid ) );
         WriteToFileStream( fs, "#include \"common.h\"\n\n" );

         WriteToFileStream( fs, string.Format( "const u8 g_tileTexturePool[{0}][{1}] = {{\n", _tileTexturePool?.TilePaletteIndexes.Count, Constants.TileSize * Constants.TileSize ) );

         if ( _tileTexturePool is not null )
         {
            for ( int i = 0; i < _tileTexturePool.TilePaletteIndexes.Count; i++ )
            {
               WriteToFileStream( fs, "   { " );

               for ( int j = 0; j < _tileTexturePool.TilePaletteIndexes[i].Count; j++ )
               {
                  WriteToFileStream( fs, string.Format( "0x{0}", _tileTexturePool.TilePaletteIndexes[i][j].ToString( "X2" ) ) );

                  if ( j < _tileTexturePool.TilePaletteIndexes[i].Count - 1 )
                  {
                     WriteToFileStream( fs, "," );
                  }

                  WriteToFileStream( fs, " " );
               }

               WriteToFileStream( fs, "   }" );

               if ( i < _tileTexturePool.TilePaletteIndexes.Count - 1 )
               {
                  WriteToFileStream( fs, "," );
               }

               WriteToFileStream( fs, "\n" );
            }
         }

         WriteToFileStream( fs, "};\n\n" );

         WriteToFileStream( fs, string.Format( "#endif // GEN_{0}_H\n", _gameSaveData?.HeaderGuids.TileTexturesHeaderGuid ) );
      }

      private void WriteStaticSpriteTexturesHeader()
      {
         using FileStream fs = File.Create( Constants.GameDataStaticSpriteTexturesHeaderPath );

         WriteToFileStream( fs, Constants.GeneratedFileHeaderMessage );
         WriteToFileStream( fs, string.Format( "#if !defined( GEN_{0}_H )\n", _gameSaveData?.HeaderGuids.StaticSpriteTexturesHeaderGuid ) );
         WriteToFileStream( fs, string.Format( "#define GEN_{0}_H\n\n", _gameSaveData?.HeaderGuids.StaticSpriteTexturesHeaderGuid ) );
         WriteToFileStream( fs, "#include \"common.h\"\n\n" );

         WriteToFileStream( fs, string.Format( "const u8 g_staticSpriteTexturePool[{0}][{1}] = {{\n", _staticSpriteTexturePool?.StaticSpritePaletteIndexes.Count, Constants.StaticSpriteTextureSize * Constants.StaticSpriteTextureSize ) );

         if ( _staticSpriteTexturePool is not null )
         {
            for ( int i = 0; i < _staticSpriteTexturePool.StaticSpritePaletteIndexes.Count; i++ )
            {
               WriteToFileStream( fs, "   { " );

               for ( int j = 0; j < _staticSpriteTexturePool.StaticSpritePaletteIndexes[i].Count; j++ )
               {
                  WriteToFileStream( fs, string.Format( "0x{0}", _staticSpriteTexturePool.StaticSpritePaletteIndexes[i][j].ToString( "X2" ) ) );

                  if ( j < _staticSpriteTexturePool.StaticSpritePaletteIndexes[i].Count - 1 )
                  {
                     WriteToFileStream( fs, "," );
                  }

                  WriteToFileStream( fs, " " );
               }

               WriteToFileStream( fs, "   }" );

               if ( i < _staticSpriteTexturePool.StaticSpritePaletteIndexes.Count - 1 )
               {
                  WriteToFileStream( fs, "," );
               }

               WriteToFileStream( fs, "\n" );
            }
         }

         WriteToFileStream( fs, "};\n\n" );
         WriteToFileStream( fs, string.Format( "#endif // GEN_{0}_H\n", _gameSaveData?.HeaderGuids.StaticSpriteTexturesHeaderGuid ) );
      }

      private void WriteActiveSpriteTexturesHeader( string headerPath, string spriteType, ActiveSpriteTexturePool? pool, string? guid )
      {
         using FileStream fs = File.Create( headerPath );

         WriteToFileStream( fs, Constants.GeneratedFileHeaderMessage  );
         WriteToFileStream( fs, string.Format( "#if !defined( GEN_{0}_H )\n", guid ) );
         WriteToFileStream( fs, string.Format( "#define GEN_{0}_H\n\n", guid ) );
         WriteToFileStream( fs, "#include \"common.h\"\n\n" );

         WriteToFileStream( fs, string.Format( "const u8 g_{0}SpriteTexturePool[{1}][{2}] = {{\n", spriteType, pool?.ActiveSpritePaletteIndexes.Count, Constants.ActiveSpriteTextureWidth * Constants.ActiveSpriteTextureHeight ) );

         if ( pool is not null )
         {
            for ( int i = 0; i < pool.ActiveSpritePaletteIndexes.Count; i++ )
            {
               WriteToFileStream( fs, "   { " );

               // rearrange active sprite frames palette indexes to go in order from top-left to bottom-right
               List<int> rearrangedPaletteIndexes = new( pool.ActiveSpritePaletteIndexes[i].Count );

               for ( int dir = 0; dir < (int)Direction.Count; dir++ )
               {
                  int pixelRowStart = Constants.ActiveSpriteTextureFrameSize * dir;

                  for ( int frame = 0; frame < Constants.ActiveSpriteFrames; frame++ )
                  {
                     int pixelColStart = frame * Constants.ActiveSpriteTextureFrameSize;

                     for ( int row = pixelRowStart; row < pixelRowStart + Constants.ActiveSpriteTextureFrameSize; row++ )
                     {
                        for ( int col = pixelColStart; col < pixelColStart + Constants.ActiveSpriteTextureFrameSize; col++ )
                        {
                           rearrangedPaletteIndexes.Add( pool.ActiveSpritePaletteIndexes[i][( ( row * Constants.ActiveSpriteTextureFrameSize * Constants.ActiveSpriteFrames ) + col )] );
                        }
                     }
                  }
               }

               for ( int j = 0; j < rearrangedPaletteIndexes.Count; j++ )
               {
                  WriteToFileStream( fs, string.Format( "0x{0}", rearrangedPaletteIndexes[j].ToString( "X2" ) ) );

                  if ( j < rearrangedPaletteIndexes.Count - 1 )
                  {
                     WriteToFileStream( fs, "," );
                  }

                  WriteToFileStream( fs, " " );
               }

               WriteToFileStream( fs, "   }" );

               if ( i < pool.ActiveSpritePaletteIndexes.Count - 1 )
               {
                  WriteToFileStream( fs, "," );
               }

               WriteToFileStream( fs, "\n" );
            }
         }

         WriteToFileStream( fs, "};\n\n" );
         WriteToFileStream( fs, string.Format( "#endif // GEN_{0}_H\n", guid ) );
      }

      private void WriteTileMapsHeader( string headerPath )
      {
         using FileStream fs = File.Create( headerPath );

         WriteToFileStream( fs, Constants.GeneratedFileHeaderMessage );
         WriteToFileStream( fs, string.Format( "#if !defined( GEN_{0}_H )\n", _gameSaveData?.HeaderGuids.MapTilesHeaderGuid ) );
         WriteToFileStream( fs, string.Format( "#define GEN_{0}_H\n\n", _gameSaveData?.HeaderGuids.MapTilesHeaderGuid ) );
         WriteToFileStream( fs, "#include \"common.h\"\n\n" );

         if ( _gameSaveData is not null )
         {
            for ( int i = 0; i < _gameSaveData.TileMaps.Count; i++ )
            {
               WriteToFileStream( fs, string.Format( "const u16 g_mapTiles{0}[{1}] = {{\n", _gameSaveData.TileMaps[i].Index, _gameSaveData.TileMaps[i].Tiles.Count ) );
               WriteToFileStream( fs, "   " );

               for ( int j = 0; j < _gameSaveData.TileMaps[i].Tiles.Count; j++ )
               {
                  var tileValue = (ushort) (
                     ( _gameSaveData.TileMaps[i].Tiles[j].TextureIndex ) |
                     ( _gameSaveData.TileMaps[i].Tiles[j].IsPassable ? ( (uint)0x1 << 5 ) : 0x0 ) |
                     ( (uint)_gameSaveData.TileMaps[i].Tiles[j].WalkSpeed << 6 ) |
                     ( (uint)_gameSaveData.TileMaps[i].Tiles[j].EncounterRate << 8 ) |
                     ( (uint)_gameSaveData.TileMaps[i].Tiles[j].DamageRate << 10 )
                  );

                  WriteToFileStream( fs, string.Format( "0x{0}", tileValue.ToString( "X4" ) ) );

                  if ( j < _gameSaveData.TileMaps[i].Tiles.Count - 1 )
                  {
                     WriteToFileStream( fs, "," );
                  }

                  WriteToFileStream( fs, " " );
               }

               WriteToFileStream( fs, "\n};\n\n" );
            }
         }

         WriteToFileStream( fs, string.Format( "#endif // GEN_{0}_H\n", _gameSaveData?.HeaderGuids.MapTilesHeaderGuid ) );
      }

      private void WriteHeaderSection( FileStream fs )
      {
         WriteToFileStream( fs, Constants.GeneratedFileHeaderMessage );
         WriteToFileStream( fs, string.Format( "#include \"{0}\"\n", Constants.GameDataTextBitFieldsHeaderFileName ) );
         WriteToFileStream( fs, string.Format( "#include \"{0}\"\n", Constants.GameDataTileTexturesHeaderFileName ) );
         WriteToFileStream( fs, string.Format( "#include \"{0}\"\n", Constants.GameDataStaticSpriteTexturesHeaderFileName ) );
         WriteToFileStream( fs, string.Format( "#include \"{0}\"\n", Constants.GameDataActiveSpriteTexturesHeaderFileName ) );
         WriteToFileStream( fs, string.Format( "#include \"{0}\"\n", Constants.GameDataPlayerSpriteTexturesHeaderFileName ) );
         WriteToFileStream( fs, string.Format( "#include \"{0}\"\n", Constants.GameDataTileMapsHeaderFileName ) );
         WriteToFileStream( fs, "#include \"game.h\"\n" );
         WriteToFileStream( fs, "#include \"random.h\"\n\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadPlayerSprites( TileMap_t* tileMap );\n" );
      }

      private void WriteHelperFunctions( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadInitialData( TileMap_t* tm, u32 tx, u32 ty, Bool_t w, Bool_t d, Bool_t u, Bool_t he, u32 ssc, u32 asc, u32 pc, Bool_t ep, u32 ec, u32 nc )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   tm->tilesX = tx; tm->tilesY = ty; tm->wraps = w; tm->affectsDaylight = d; tm->isUnderground = u; tm->hasEncounters = he, tm->staticSpriteCount = ssc; tm->activeSpriteCount = asc; tm->portalCount = pc, tm->hasEdgePortal = ep, tm->entityCount = ec; tm->npcCount = nc;\n" );
         WriteToFileStream( fs, "}\n" );

         WriteToFileStream( fs, "\ninternal void TileMap_LoadStaticSpriteData( StaticSprite_t* s, u32 txi, u32 ti, Bool_t p )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   s->textureIndex = txi; s->tileIndex = ti; s->isPassable = p;\n" );
         WriteToFileStream( fs, "}\n" );

         WriteToFileStream( fs, "\ninternal void TileMap_LoadActiveSpriteData( ActiveSprite_t* s, u32 txi, u32 ox, u32 oy, Direction_t d )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   s->textureIndex = txi; s->offset.x = ox; s->offset.y = oy; s->direction = d;\n" );
         WriteToFileStream( fs, "}\n" );

         WriteToFileStream( fs, "\ninternal void TileMap_LoadPortalData( Portal_t* p, u32 st, u32 dm, u32 di, Direction_t dd )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   p->sourceTileIndex = st; p->destTileMapIndex = dm; p->destTileIndex = di; p->destDirection = dd;\n" );
         WriteToFileStream( fs, "}\n" );

         WriteToFileStream( fs, "\ninternal void TileMap_LoadEntityData( Entity_t* e, i32 x, i32 y, i32 w, i32 h, ActiveSprite_t* s )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   e->pos.x = x; e->pos.y = y; e->pos.w = w; e->pos.h = h; e->sprite = s;\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WritePaletteFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Screen_LoadPalette( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, string.Format( "   screen->paletteColorCount = {0};\n\n", _palette?.ColorCount ) );

         for ( int i = 0; i < _palette?.ColorCount; i++ )
         {
            WriteToFileStream( fs, string.Format( "   screen->palette[{0}] = 0x{1};\n", i, _palette?.Colors[i].ToString( "X4" ) ) );
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTextTilesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid Screen_LoadTextBitFields( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, string.Format( "   memcpy( screen->textBitFields, g_textBitFields, sizeof( u8 ) * {0} * {1} );\n", Constants.TextTileCount, Constants.TextTileSize ) );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileTextureIndexesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   switch ( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _gameSaveData?.TileTextureSets.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", _gameSaveData.TileTextureSets[i].Index ) );

            for ( int j = 0; j < _gameSaveData.TileTextureSets[i].TileTexturePoolIndexes.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         memcpy( &tileMap->tileTextures[{0}].paletteIndexes, g_tileTexturePool[{1}], sizeof( u8 ) * {2} );\n", j, _gameSaveData.TileTextureSets[i].TileTexturePoolIndexes[j], Constants.TileSize * Constants.TileSize ) );
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteStaticSpriteTextureIndexesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );

         WriteToFileStream( fs, "   switch ( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _gameSaveData?.StaticSpriteTextureSets.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", _gameSaveData.StaticSpriteTextureSets[i].Index ) );

            for ( int j = 0; j < _gameSaveData.StaticSpriteTextureSets[i].StaticSpriteTexturePoolIndexes.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         memcpy( &tileMap->staticSpriteTextures[{0}].paletteIndexes, g_staticSpriteTexturePool[{1}], sizeof( u8 ) * {2} );\n", j, _gameSaveData.StaticSpriteTextureSets[i].StaticSpriteTexturePoolIndexes[j], Constants.StaticSpriteTextureSize * Constants.StaticSpriteTextureSize ) );
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteActiveSpriteTextureIndexesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );

         WriteToFileStream( fs, "   switch ( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _gameSaveData?.ActiveSpriteTextureSets.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", _gameSaveData.ActiveSpriteTextureSets[i].Index ) );

            for ( int j = 0; j < _gameSaveData.ActiveSpriteTextureSets[i].ActiveSpriteTexturePoolIndexes.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         memcpy( &tileMap->activeSpriteTextures[{0}].paletteIndexes, g_activeSpriteTexturePool[{1}], sizeof( u8 ) * {2} );\n", j, _gameSaveData.ActiveSpriteTextureSets[i].ActiveSpriteTexturePoolIndexes[j], Constants.ActiveSpriteTextureWidth * Constants.ActiveSpriteTextureHeight ) );
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WritePlayerSpritesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadPlayerSprites( TileMap_t* tileMap )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   u32 i;\n\n" );
         WriteToFileStream( fs, "   for ( i = 0; i < tileMap->getPlayerCountFunc( tileMap->playerCountProvider ); i++ )\n" );
         WriteToFileStream( fs, "   {\n" );
         WriteToFileStream( fs, string.Format( "      memcpy( tileMap->playerSpriteTextures + i, g_playerSpriteTexturePool[tileMap->players[i].playerClass], sizeof( u8 ) * {0} );\n", Constants.ActiveSpriteTextureWidth * Constants.ActiveSpriteTextureHeight ) );
         WriteToFileStream( fs, "      TileMap_LoadActiveSpriteData( tileMap->playerSprites + i, i, 2, 4, Direction_Down );\n" );
         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileMapFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _gameSaveData?.TileMaps.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}: // {1}\n", _gameSaveData.TileMaps[i].Index, _gameSaveData.TileMaps[i].Name ) );
            WriteToFileStream( fs, string.Format( "         TileMap_LoadTileTexturesFromSetIndex( tileMap, {0} );\n", _gameSaveData.TileMaps[i].TileTextureSetIndex ) );
            WriteToFileStream( fs, string.Format( "         TileMap_LoadInitialData( tileMap, {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11} );\n",
               _gameSaveData.TileMaps[i].TilesX,
               _gameSaveData.TileMaps[i].TilesY,
               _gameSaveData.TileMaps[i].Wraps ? "True" : "False",
               _gameSaveData.TileMaps[i].AffectsDaylight ? "True" : "False",
               _gameSaveData.TileMaps[i].IsUnderground ? "True" : "False",
               _gameSaveData.TileMaps[i].HasEncounters ? "True" : "False",
               _gameSaveData.TileMaps[i].StaticSprites.Count,
               _gameSaveData.TileMaps[i].ActiveSprites.Count,
               _gameSaveData.TileMaps[i].Portals.Count,
               _gameSaveData.TileMaps[i].EdgePortal is null ? "False" : "True",
               _gameSaveData.TileMaps[i].Entities.Count,
               _gameSaveData.TileMaps[i].Npcs.Count ) );

            WriteToFileStream( fs, string.Format( "         TileMap_LoadStaticSpriteTexturesFromSetIndex( tileMap, {0} );\n", _gameSaveData.TileMaps[i].StaticSpriteTextureSetIndex ) );

            for ( int j = 0; j < _gameSaveData.TileMaps[i].StaticSprites.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         TileMap_LoadStaticSpriteData( tileMap->staticSprites + {0}, {1}, {2}, {3} );\n",
                  j,
                  _gameSaveData.TileMaps[i].StaticSprites[j].TextureIndex,
                  _gameSaveData.TileMaps[i].StaticSprites[j].TileIndex,
                  _gameSaveData.TileMaps[i].StaticSprites[j].IsPassable ? "True" : "False" ) );
            }

            WriteToFileStream( fs, string.Format( "         TileMap_LoadActiveSpriteTexturesFromSetIndex( tileMap, {0} );\n", _gameSaveData.TileMaps[i].ActiveSpriteTextureSetIndex ) );

            for ( int j = 0; j < _gameSaveData.TileMaps[i].ActiveSprites.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         ActiveSprite_Init( tileMap->activeSprites + {0}, {1}, {2}, {3}, Direction_{4} );\n",
                  j,
                  _gameSaveData.TileMaps[i].ActiveSprites[j].TextureIndex,
                  _gameSaveData.TileMaps[i].ActiveSprites[j].Offset.X,
                  _gameSaveData.TileMaps[i].ActiveSprites[j].Offset.Y,
                  _gameSaveData.TileMaps[i].ActiveSprites[j].StartDirection.ToString() ) );
            }

            for ( int j = 0; j < _gameSaveData.TileMaps[i].Portals.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         TileMap_LoadPortalData( tileMap->portals + {0}, {1}, {2}, {3}, Direction_{4} );\n",
                  j,
                  _gameSaveData.TileMaps[i].Portals[j].SourceTileIndex,
                  _gameSaveData.TileMaps[i].Portals[j].DestTileMapIndex,
                  _gameSaveData.TileMaps[i].Portals[j].DestTileIndex,
                  _gameSaveData.TileMaps[i].Portals[j].DestDirection ) );
            }

            if ( _gameSaveData.TileMaps[i].EdgePortal is not null )
            {
               WriteToFileStream( fs, string.Format( "         tileMap->edgePortal.destTileMapIndex = {0}; tileMap->edgePortal.destTileIndex = {1};\n",
                  _gameSaveData.TileMaps[i].EdgePortal?.DestTileMapIndex,
                  _gameSaveData.TileMaps[i].EdgePortal?.DestTileIndex ) );
            }

            for ( int j = 0; j < _gameSaveData.TileMaps[i].Entities.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         TileMap_LoadEntityData( tileMap->entities + {0}, {1}, {2}, {3}, {4}, tileMap->activeSprites + {5} );\n",
                  j,
                  _gameSaveData.TileMaps[i].Entities[j].Pos.X,
                  _gameSaveData.TileMaps[i].Entities[j].Pos.Y,
                  _gameSaveData.TileMaps[i].Entities[j].Pos.W,
                  _gameSaveData.TileMaps[i].Entities[j].Pos.H,
                  _gameSaveData.TileMaps[i].Entities[j].SpriteIndex ) );
            }

            for ( int j = 0; j < _gameSaveData.TileMaps[i].Npcs.Count; j++ )
            {
               WriteToFileStream( fs, string.Format( "         Npc_Init( tileMap->npcs + {0}, tileMap->entities + {1}, {2} );\n",
                  j,
                  _gameSaveData.TileMaps[i].Npcs[j].EntityIndex,
                  _gameSaveData.TileMaps[i].Npcs[j].Wanders ? "True" : "False" ) );
            }

            WriteToFileStream( fs, string.Format( "         memcpy( tileMap->tiles, g_mapTiles{0}, sizeof( u16 ) * {1} );\n", _gameSaveData.TileMaps[i].Index, _gameSaveData.TileMaps[i].Tiles.Count ) );
            WriteToFileStream( fs, "         break;\n" );
         }
                  
         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteGameResetFunction( FileStream fs )
      {
         // TODO: all of this should come from _gameSaveData eventually
         WriteToFileStream( fs, "\nvoid Game_Reset( Game_t* game )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   TileMap_LoadFromIndex( &game->tileMap, 0 );\n" );
         WriteToFileStream( fs, "   game->playerCount = 3;\n\n" );

         WriteToFileStream( fs, "   game->players[0].playerClass = PlayerClass_Hero;\n" );
         WriteToFileStream( fs, "   game->players[0].entity = game->tileMap.playerEntities;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->sprite = game->tileMap.playerSprites;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->pos.x = 2722 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->pos.y = 3538 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->pos.w = 12 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->pos.h = 12 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->prevPos = game->players[0].entity->pos;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->velocity.x = 0;\n" );
         WriteToFileStream( fs, "   game->players[0].entity->velocity.y = 0;\n" );
         WriteToFileStream( fs, "   game->players[0].tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players[0].entity->pos.x, (u32)game->players[0].entity->pos.y );\n\n" );

         WriteToFileStream( fs, "   game->players[1].playerClass = PlayerClass_Soldier;\n" );
         WriteToFileStream( fs, "   game->players[1].entity = game->tileMap.playerEntities + 1;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->sprite = game->tileMap.playerSprites + 1;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->pos.x = 2722 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->pos.y = 3538 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->pos.w = 12 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->pos.h = 12 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->prevPos = game->players[1].entity->pos;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->velocity.x = 0;\n" );
         WriteToFileStream( fs, "   game->players[1].entity->velocity.y = 0;\n" );
         WriteToFileStream( fs, "   game->players[1].tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players[1].entity->pos.x, (u32)game->players[1].entity->pos.y );\n\n" );

         WriteToFileStream( fs, "   game->players[2].playerClass = PlayerClass_GoofOff;\n" );
         WriteToFileStream( fs, "   game->players[2].entity = game->tileMap.playerEntities + 2;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->sprite = game->tileMap.playerSprites + 2;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->pos.x = 2722 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->pos.y = 3538 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->pos.w = 12 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->pos.h = 12 * UNITS_PER_PIXEL;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->prevPos = game->players[2].entity->pos;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->velocity.x = 0;\n" );
         WriteToFileStream( fs, "   game->players[2].entity->velocity.y = 0;\n" );
         WriteToFileStream( fs, "   game->players[2].tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players[2].entity->pos.x, (u32)game->players[2].entity->pos.y );\n\n" );

         WriteToFileStream( fs, "   TileMap_LoadPlayerSprites( &game->tileMap );\n" );
         WriteToFileStream( fs, "   ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );\n" );
         WriteToFileStream( fs, "   TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );\n" );
         WriteToFileStream( fs, "   game->isAM = False;\n" );
         WriteToFileStream( fs, "   game->daylightFactor = 1.0f; // noon\n" );
         WriteToFileStream( fs, "   game->screen.dayFilterIntensity = 1.0f;\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
