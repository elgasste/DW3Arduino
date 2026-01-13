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
         WriteActiveSpriteTexturesHeader( Constants.GameDataActiveSpriteTexturesHeaderPath, "active", _activeSpriteTexturePool );
         WriteActiveSpriteTexturesHeader( Constants.GameDataPlayerSpriteTexturesHeaderPath, "player", _playerSpriteTexturePool );

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

         // TODO: use GUIDs for header defines
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#if !defined( TEXT_BIT_FIELDS_H )\n" );
         WriteToFileStream( fs, "#define TEXT_BIT_FIELDS_H\n\n" );
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

         WriteToFileStream( fs, "#endif // TEXT_BIT_FIELDS_H\n" );
      }

      private void WriteTileTexturesHeader()
      {
         using FileStream fs = File.Create( Constants.GameDataTileTexturesHeaderPath );

         // TODO: use GUIDs for header defines
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#if !defined( TILE_TEXTURES_H )\n" );
         WriteToFileStream( fs, "#define TILE_TEXTURES_H\n\n" );
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

         WriteToFileStream( fs, "#endif // TILE_TEXTURES_H\n" );
      }

      private void WriteStaticSpriteTexturesHeader()
      {
         using FileStream fs = File.Create( Constants.GameDataStaticSpriteTexturesHeaderPath );

         // TODO: use GUIDs for header defines
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#if !defined( STATIC_SPRITE_TEXTURES_H )\n" );
         WriteToFileStream( fs, "#define STATIC_SPRITE_TEXTURES_H\n\n" );
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
         WriteToFileStream( fs, "#endif // STATIC_SPRITE_TEXTURES_H\n" );
      }

      private void WriteActiveSpriteTexturesHeader( string headerPath, string spriteType, ActiveSpriteTexturePool? pool )
      {
         var guid = Guid.NewGuid().ToString( "N" ).ToUpper();
         using FileStream fs = File.Create( headerPath );

         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
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

      private void WriteHeaderSection( FileStream fs )
      {
         // TODO: some of these are defined in Constants.cs, we should load them from there
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"text_bit_fields.h\"\n" );
         WriteToFileStream( fs, "#include \"tile_textures.h\"\n" );
         WriteToFileStream( fs, "#include \"static_sprite_textures.h\"\n" );
         WriteToFileStream( fs, "#include \"active_sprite_textures.h\"\n" );
         WriteToFileStream( fs, "#include \"player_sprite_textures.h\"\n" );
         WriteToFileStream( fs, "#include \"game.h\"\n" );
         WriteToFileStream( fs, "#include \"random.h\"\n\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadPlayerSpriteTextureFromPoolIndex( ActiveSpriteTexture_t* texture, u32 index );\n" );
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

         WriteToFileStream( fs, "\ninternal void TileMap_LoadEntityData( Entity_t* e, r32 x, r32 y, r32 w, r32 h, ActiveSprite_t* s )\n" );
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
         WriteToFileStream( fs, "   u32 i;\n" );
         WriteToFileStream( fs, "   u16* m = tileMap->tiles;\n\n" );
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

            WriteCompressedTileData( fs, _gameSaveData.TileMaps[i] );

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
         WriteToFileStream( fs, "   game->playerCount = 1;\n" );
         WriteToFileStream( fs, "   game->players->playerClass = PlayerClass_Hero;\n" );
         WriteToFileStream( fs, "   game->players->entity = game->tileMap.playerEntities;\n" );
         WriteToFileStream( fs, "   game->players->entity->sprite = game->tileMap.playerSprites;\n" );
         WriteToFileStream( fs, "   game->players->entity->pos.x = 2722.0f;\n" );
         WriteToFileStream( fs, "   game->players->entity->pos.y = 3538.0f;\n" );
         WriteToFileStream( fs, "   game->players->entity->pos.w = 12.0f;\n" );
         WriteToFileStream( fs, "   game->players->entity->pos.h = 12.0f;\n" );
         WriteToFileStream( fs, "   game->players->entity->prevPos = game->players->entity->pos;\n" );
         WriteToFileStream( fs, "   game->players->entity->velocity.x = 0.0f;\n" );
         WriteToFileStream( fs, "   game->players->entity->velocity.y = 0.0f;\n" );
         WriteToFileStream( fs, "   game->players->tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->players->entity->pos.x, (u32)game->players->entity->pos.y );\n" );
         WriteToFileStream( fs, "   TileMap_LoadPlayerSprites( &game->tileMap );\n" );
         WriteToFileStream( fs, "   ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );\n" );
         WriteToFileStream( fs, "   TileMap_ClampViewportToEntity( &game->tileMap, game->players->entity );\n" );
         WriteToFileStream( fs, "   game->isAM = False;\n" );
         WriteToFileStream( fs, "   game->daylightFactor = 1.0f; // noon\n" );
         WriteToFileStream( fs, "   game->screen.dayFilterIntensity = 1.0f;\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteCompressedTileData( FileStream fs, TileMapSaveData tileMap )
      {
         List<ushort> tileValues = new( tileMap.Tiles.Count );
         Dictionary<ushort, int> valueCounts = [];

         // first pass: compile tile properties into 16-bit values and count their usage
         for ( int i = 0; i < tileMap.Tiles.Count; i++ )
         {
            var tileValue = (ushort) (
                  ( tileMap.Tiles[i].TextureIndex ) |
                  ( tileMap.Tiles[i].IsPassable ? ( (uint)0x1 << 5 ) : 0x0 ) |
                  ( (uint)tileMap.Tiles[i].WalkSpeed << 6 ) |
                  ( (uint)tileMap.Tiles[i].EncounterRate << 8 ) |
                  ( (uint)tileMap.Tiles[i].DamageRate << 10 )
               );

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
               }
            }
         }

         int packCount = 0;

         // third pass: write out loops for all the runs with a counter higher than 1, packing 3 per line
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

            if ( packCount == 3 || ( ( packCount > 0 ) && ( i == ( runs.Count - 1 ) ) ) )
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

      private void WriteCompressedTextureData( FileStream fs, List<int> paletteIndexes )
      {
         Dictionary<int, int> valueCounts = [];

         // first pass: count index value usage
         for ( int i = 0; i < paletteIndexes.Count; i++ )
         {
            if ( !valueCounts.ContainsKey( paletteIndexes[i] ) )
            {
               valueCounts.Add( paletteIndexes[i], 1 );
            }
            else
            {
               valueCounts[paletteIndexes[i]]++;
            }
         }

         // find the most-used value and initialize the entire texture with it
         int mostUsedValue = valueCounts.OrderByDescending( kvp => kvp.Value ).First().Key;
         WriteToFileStream( fs, string.Format( "         for ( i = 0; i < {0}; i++ ) m[i] = 0x{1};\n", paletteIndexes.Count, mostUsedValue.ToString( "X2" ) ) );

         // second pass: compile a list of runs (a "run" can also have one single value)
         List<TextureDataRun> runs = [];
         int runStart = 0, runCount = 0, runValue = 0;

         for ( int i = 0; i < paletteIndexes.Count; i++ )
         {
            if ( paletteIndexes[i] == mostUsedValue ) // ran into most-used value, write and restart
            {
               if ( runCount > 0 )
               {
                  runs.Add( new() { Start = runStart, Count = runCount, Value = runValue } );
               }

               runCount = 0;
            }
            else
            {
               if ( runCount == 0 ) // starting a new run
               {
                  runStart = i;
                  runCount = 1;
                  runValue = paletteIndexes[i];
               }
               else if ( paletteIndexes[i] != runValue ) // last run has ended, write and restart
               {
                  runs.Add( new() { Start = runStart, Count = runCount, Value = runValue } );
                  runStart = i;
                  runCount = 1;
                  runValue = paletteIndexes[i];
               }
               else // run is still going
               {
                  runCount++;
               }

               if ( i == ( paletteIndexes.Count - 1 ) ) // end of the list, write and exit
               {
                  runs.Add( new() { Start = runStart, Count = runCount, Value = runValue } );
               }
            }
         }

         int packCount = 0;

         // third pass: write out loops for all the runs with a counter higher than 1, packing 3 per line
         for ( int i = 0; i < runs.Count; i++ )
         {
            if ( runs[i].Count > 1 )
            {
               packCount++;

               if ( packCount == 1 )
               {
                  WriteToFileStream( fs, string.Format( "         for ( i = {0}; i < {1}; i++ ) m[i] = 0x{2};", runs[i].Start, runs[i].Start + runs[i].Count, runs[i].Value.ToString( "X2" ) ) );
               }
               else
               {
                  WriteToFileStream( fs, string.Format( " for ( i = {0}; i < {1}; i++ ) m[i] = 0x{2};", runs[i].Start, runs[i].Start + runs[i].Count, runs[i].Value.ToString( "X2" ) ) );
               }
            }

            if ( packCount == 3 || ( ( packCount > 0 ) && ( i == ( runs.Count - 1 ) ) ) )
            {
               WriteToFileStream( fs, "\n" );
               packCount = 0;
            }
         }

         packCount = 0;

         // fourth pass: write out loops for all the runs with a single count, packing 12 per line
         for ( int i = 0; i < runs.Count; i++ )
         {
            if ( runs[i].Count == 1 )
            {
               packCount++;

               if ( packCount == 1 )
               {
                  WriteToFileStream( fs, string.Format( "         m[{0}] = 0x{1};", runs[i].Start, runs[i].Value.ToString( "X2" ) ) );
               }
               else
               {
                  WriteToFileStream( fs, string.Format( " m[{0}] = 0x{1};", runs[i].Start, runs[i].Value.ToString( "X2" ) ) );
               }
            }

            if ( packCount == 12 || ( ( packCount > 0 ) && ( i == ( runs.Count - 1 ) ) ) )
            {
               WriteToFileStream( fs, "\n" );
               packCount = 0;
            }
         }
      }

      private void WriteToFileStream( FileStream fs, string value )
      {
         byte[] info = new UTF8Encoding( true ).GetBytes( value );
         fs.Write( info, 0, info.Length );
      }
   }
}
