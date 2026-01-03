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

      public void WriteGameDataSourceFile( GameSaveData? saveData,
                                           Palette? palette,
                                           TileTexturePool? tileTexturePool,
                                           StaticSpriteTexturePool? staticSpriteTexturePool,
                                           ActiveSpriteTexturePool? activeSpriteTexturePool )
      {
         _gameSaveData = saveData;
         _palette = palette;
         _tileTexturePool = tileTexturePool;
         _staticSpriteTexturePool = staticSpriteTexturePool;
         _activeSpriteTexturePool = activeSpriteTexturePool;

         using FileStream fs = File.Create( Constants.GameDataSourceFilePath );
         WriteHeaderSection( fs );
         WriteHelperFunctions( fs );
         WritePaletteFunction( fs );
         WriteTextTilesFunction( fs );
         WriteTileTexturesPoolFunction( fs );
         WriteTileTextureIndexesFunction( fs );
         WriteStaticSpriteTexturesPoolFunction( fs );
         WriteStaticSpriteTextureIndexesFunction( fs );
         WriteActiveSpriteTexturesPoolFunction( fs );
         WriteActiveSpriteTextureIndexesFunction( fs );
         WritePlayerSpritesFunction( fs );
         WriteTileMapFunction( fs );
         WriteGameResetFunction( fs );
      }

      private void WriteHeaderSection( FileStream fs )
      {
         WriteToFileStream( fs, "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n" );
         WriteToFileStream( fs, "#include \"game.h\"\n" );
         WriteToFileStream( fs, "#include \"random.h\"\n\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadTileTextureFromPoolIndex( TileTexture_t* texture, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadTileTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadStaticSpriteTextureFromPoolIndex( StaticSpriteTexture_t* texture, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadActiveSpriteTextureFromPoolIndex( ActiveSpriteTexture_t* texture, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index );\n" );
         WriteToFileStream( fs, "internal void TileMap_LoadPlayerSprites( TileMap_t* tileMap );\n" );
      }

      private void WriteHelperFunctions( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadInitialData( TileMap_t* tm, u32 tx, u32 ty, Bool_t w, Bool_t d, Bool_t u, u32 ssc, u32 asc, u32 pc, Bool_t ep, u32 ec, u32 nc )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   tm->tilesX = tx; tm->tilesY = ty; tm->wraps = w; tm->affectsDaylight = d; tm->isUnderground = u; tm->staticSpriteCount = ssc; tm->activeSpriteCount = asc; tm->portalCount = pc, tm->hasEdgePortal = ep, tm->entityCount = ec; tm->npcCount = nc;\n" );
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
         WriteToFileStream( fs, string.Format( "screen->paletteColorCount = {0};\n\n", _palette?.ColorCount ) );

         for ( int i = 0; i < _palette?.ColorCount; i++ )
         {
            WriteToFileStream( fs, string.Format( "   screen->palette[{0}] = 0x{1};\n", i, _palette?.Colors[i].ToString( "X4" ) ) );
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTextTilesFunction( FileStream fs )
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

         WriteToFileStream( fs, "\nvoid Screen_LoadTextBitFields( Screen_t* screen )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   uint32_t i, j;\n\n" );

         var byteCounts = new Dictionary<byte, int>();

         for ( int i = 0; i < Constants.TextTileCount; i++ )
         {
            for ( int j = 0; j < Constants.TextTileSize; j++ )
            {
               byte b = textTextureMap[i + ( j * Constants.TextTileCount )];

               if ( byteCounts.TryGetValue( b, out int value ) )
               {
                  byteCounts[b] = ++value;
               }
               else
               {
                  byteCounts[b] = 1;
               }
            }
         }

         int highestCount = 0;
         byte mostCommonValue = 0;

         foreach ( var pair in byteCounts )
         {
            if ( pair.Value > highestCount )
            {
               highestCount = pair.Value;
               mostCommonValue = pair.Key;
            }
         }

         WriteToFileStream( fs, string.Format(
            "   for ( i = 0; i < SCREEN_TEXT_TILE_COUNT; i++ ) for ( j = 0; j < SCREEN_TEXT_TILE_SIZE; j++ ) screen->textBitFields[i][j] = 0x{0};\n",
            mostCommonValue.ToString( "X2" ) ) );

         for ( int i = 0; i < Constants.TextTileCount; i++ )
         {
            for ( int j = 0; j < Constants.TextTileSize; j++ )
            {
               byte b = textTextureMap[i + ( j * Constants.TextTileCount )];

               if ( b != mostCommonValue )
               {
                  WriteToFileStream( fs, string.Format( "   screen->textBitFields[{0}][{1}] = 0x{2};\n", i, j, b.ToString( "X2" ) ) );
               }
            }
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteTileTexturesPoolFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadTileTextureFromPoolIndex( TileTexture_t* texture, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   u32 i;\n" );
         WriteToFileStream( fs, "   u8* m = texture->paletteIndexes;\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _tileTexturePool?.TilePaletteIndexes.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
            WriteCompressedTextureData( fs, _tileTexturePool.TilePaletteIndexes[i] );
            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
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
               WriteToFileStream( fs, string.Format( "         TileMap_LoadTileTextureFromPoolIndex( &tileMap->tileTextures[{0}], {1} );\n", j, _gameSaveData.TileTextureSets[i].TileTexturePoolIndexes[j] ) );
            }

            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteStaticSpriteTexturesPoolFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadStaticSpriteTextureFromPoolIndex( StaticSpriteTexture_t* texture, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   u32 i;\n" );
         WriteToFileStream( fs, "   u8* m = texture->paletteIndexes;\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _staticSpriteTexturePool?.StaticSpritePaletteIndexes.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );
            WriteCompressedTextureData( fs, _staticSpriteTexturePool.StaticSpritePaletteIndexes[i] );
            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteStaticSpriteTextureIndexesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadStaticSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );

         if ( _gameSaveData?.StaticSpriteTextureSets.Count == 0 )
         {
            WriteToFileStream( fs, "   UNUSED_PARAM( tileMap );\n" );
            WriteToFileStream( fs, "   UNUSED_PARAM( index );\n" );
         }
         else
         {

            WriteToFileStream( fs, "   switch ( index )\n" );
            WriteToFileStream( fs, "   {\n" );

            for ( int i = 0; i < _gameSaveData?.StaticSpriteTextureSets.Count; i++ )
            {
               WriteToFileStream( fs, string.Format( "      case {0}:\n", _gameSaveData.StaticSpriteTextureSets[i].Index ) );

               for ( int j = 0; j < _gameSaveData.StaticSpriteTextureSets[i].StaticSpriteTexturePoolIndexes.Count; j++ )
               {
                  WriteToFileStream( fs, string.Format( "         TileMap_LoadStaticSpriteTextureFromPoolIndex( &tileMap->staticSpriteTextures[{0}], {1} );\n", j, _gameSaveData.StaticSpriteTextureSets[i].StaticSpriteTexturePoolIndexes[j] ) );
               }

               WriteToFileStream( fs, "         break;\n" );
            }

            WriteToFileStream( fs, "   }\n" );
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WriteActiveSpriteTexturesPoolFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadActiveSpriteTextureFromPoolIndex( ActiveSpriteTexture_t* texture, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   u32 i;\n" );
         WriteToFileStream( fs, "   u8* m = texture->paletteIndexes;\n\n" );
         WriteToFileStream( fs, "   switch( index )\n" );
         WriteToFileStream( fs, "   {\n" );

         for ( int i = 0; i < _activeSpriteTexturePool?.ActiveSpritePaletteIndexes.Count; i++ )
         {
            WriteToFileStream( fs, string.Format( "      case {0}:\n", i ) );

            // rearrange active sprite frames palette indexes to go in order from top-left to bottom-right
            List<int> rearrangedPaletteIndexes = new( _activeSpriteTexturePool.ActiveSpritePaletteIndexes[i].Count );

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
                        rearrangedPaletteIndexes.Add( _activeSpriteTexturePool.ActiveSpritePaletteIndexes[i][( ( row * Constants.ActiveSpriteTextureFrameSize * Constants.ActiveSpriteFrames ) + col )] );
                     }
                  }
               }
            }

            WriteCompressedTextureData( fs, rearrangedPaletteIndexes );
            WriteToFileStream( fs, "         break;\n" );
         }

         WriteToFileStream( fs, "   }\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteActiveSpriteTextureIndexesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\ninternal void TileMap_LoadActiveSpriteTexturesFromSetIndex( TileMap_t* tileMap, u32 index )\n" );
         WriteToFileStream( fs, "{\n" );

         if ( _gameSaveData?.ActiveSpriteTextureSets.Count == 0 )
         {
            WriteToFileStream( fs, "   UNUSED_PARAM( index );\n" );
            WriteToFileStream( fs, "   tileMap->activeSpriteCount = 0;\n" );
         }
         else
         {
            WriteToFileStream( fs, "   switch ( index )\n" );
            WriteToFileStream( fs, "   {\n" );

            for ( int i = 0; i < _gameSaveData?.ActiveSpriteTextureSets.Count; i++ )
            {
               WriteToFileStream( fs, string.Format( "      case {0}:\n", _gameSaveData.ActiveSpriteTextureSets[i].Index ) );
               WriteToFileStream( fs, string.Format( "         tileMap->activeSpriteCount = {0};\n", _gameSaveData.ActiveSpriteTextureSets[i].ActiveSpriteTexturePoolIndexes.Count ) );

               for ( int j = 0; j < _gameSaveData.ActiveSpriteTextureSets[i].ActiveSpriteTexturePoolIndexes.Count; j++ )
               {
                  WriteToFileStream( fs, string.Format( "         TileMap_LoadActiveSpriteTextureFromPoolIndex( tileMap->activeSpriteTextures + {0}, {1} );\n", j, _gameSaveData.ActiveSpriteTextureSets[i].ActiveSpriteTexturePoolIndexes[j] ) );
               }

               WriteToFileStream( fs, "         break;\n" );
            }

            WriteToFileStream( fs, "   }\n" );
         }

         WriteToFileStream( fs, "}\n" );
      }

      private void WritePlayerSpritesFunction( FileStream fs )
      {
         WriteToFileStream( fs, "\nvoid TileMap_LoadPlayerSprites( TileMap_t* tileMap )\n" );
         WriteToFileStream( fs, "{\n" );

         // TODO: implement multiple party members
         WriteToFileStream( fs, "   TileMap_LoadActiveSpriteTextureFromPoolIndex( tileMap->playerSpriteTextures, 0 );\n" );
         WriteToFileStream( fs, "   tileMap->playerCount = 1;\n" );
         WriteToFileStream( fs, "   TileMap_LoadActiveSpriteData( tileMap->playerSprites, 0, 2, 4, Direction_Down );\n" );

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
            WriteToFileStream( fs, string.Format( "         TileMap_LoadInitialData( tileMap, {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10} );\n",
               _gameSaveData.TileMaps[i].TilesX,
               _gameSaveData.TileMaps[i].TilesY,
               _gameSaveData.TileMaps[i].Wraps ? "True" : "False",
               _gameSaveData.TileMaps[i].AffectsDaylight ? "True" : "False",
               _gameSaveData.TileMaps[i].IsUnderground ? "True" : "False",
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
                  
         WriteToFileStream( fs, "   }\n\n" );
         WriteToFileStream( fs, "   TileMap_LoadPlayerSprites( tileMap );\n" );
         WriteToFileStream( fs, "}\n" );
      }

      private void WriteGameResetFunction( FileStream fs )
      {
         // TODO: all of this should come from _gameSaveData eventually
         WriteToFileStream( fs, "\nvoid Game_Reset( Game_t* game )\n" );
         WriteToFileStream( fs, "{\n" );
         WriteToFileStream( fs, "   TileMap_LoadFromIndex( &game->tileMap, 0 );\n" );
         WriteToFileStream( fs, "   TileMap_LoadPlayerSprites( &game->tileMap );\n" );
         WriteToFileStream( fs, "   game->player.entity = game->tileMap.playerEntities;\n" );
         WriteToFileStream( fs, "   game->player.entity->sprite = game->tileMap.playerSprites;\n" );
         WriteToFileStream( fs, "   game->player.entity->pos.x = 2722.0f;\n" );
         WriteToFileStream( fs, "   game->player.entity->pos.y = 3538.0f;\n" );
         WriteToFileStream( fs, "   game->player.entity->pos.w = 12.0f;\n" );
         WriteToFileStream( fs, "   game->player.entity->pos.h = 12.0f;\n" );
         WriteToFileStream( fs, "   game->player.entity->prevPos = game->player.entity->pos;\n" );
         WriteToFileStream( fs, "   game->player.entity->velocity.x = 0.0f;\n" );
         WriteToFileStream( fs, "   game->player.entity->velocity.y = 0.0f;\n" );
         WriteToFileStream( fs, "   game->player.tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap, (u32)game->player.entity->pos.x, (u32)game->player.entity->pos.y );\n" );
         WriteToFileStream( fs, "   ActiveSprite_SetDirection( game->player.entity->sprite, Direction_Down );\n" );
         WriteToFileStream( fs, "   TileMap_ClampViewportToEntity( &game->tileMap, game->player.entity );\n" );
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
                  ( (uint)tileMap.Tiles[i].WalkSpeed << 6 )
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
