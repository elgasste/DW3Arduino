using System.Runtime.CompilerServices;
using DW3ArduinoEditor.Enums;

namespace DW3ArduinoEditor.SaveData
{
   public static class GameSaveDataSanityChecker
   {
      public static void CheckSanity( GameSaveData saveData,
                                      int tileTexturePoolCount,
                                      int staticSpriteTexturePoolCount,
                                      int activeSpriteTexturePoolCount,
                                      int playerSpriteTexturePoolMaleCount,
                                      int playerSpriteTexturePoolFemaleCount )
      {
         CheckHeaderGuids( saveData.HeaderGuids );
         CheckTextureSets( saveData.TileTextureSets, "Tile", Constants.TileTextureSetSize, tileTexturePoolCount );
         CheckTextureSets( saveData.StaticSpriteTextureSets, "Static sprite", Constants.StaticSpriteTextureSetSize, staticSpriteTexturePoolCount );
         CheckTextureSets( saveData.ActiveSpriteTextureSets, "Active sprite", Constants.ActiveSpriteTextureSetSize, activeSpriteTexturePoolCount );

         if ( playerSpriteTexturePoolMaleCount != (int)PlayerClass.Count )
         {
            throw new Exception( "Male player sprite texture count doesn't match the number of player classes" );
         }

         if ( playerSpriteTexturePoolFemaleCount != (int)PlayerClass.Count )
         {
            throw new Exception( "Female player sprite texture count doesn't match the number of player classes" );
         }

         CheckTileMaps( saveData.TileMaps, saveData.TileTextureSets, saveData.StaticSpriteTextureSets.Count, saveData.ActiveSpriteTextureSets.Count );
         CheckPlayerClassExpTables( saveData.PlayerClassExpTables );
      }

      private static void CheckHeaderGuids( HeaderGuidsSaveData saveData )
      {
         List<string> guids = [
            saveData.TextBitFieldsHeaderGuid,
            saveData.TileTexturesHeaderGuid,
            saveData.StaticSpriteTexturesHeaderGuid,
            saveData.ActiveSpriteTexturesHeaderGuid,
            saveData.PlayerSpriteTexturesHeaderGuid,
            saveData.MapTilesHeaderGuid
         ];

         for ( int i = 0; i < guids.Count; i++ )
         {
            if ( string.IsNullOrEmpty( guids[i] ) )
            {
               throw new Exception( "Header GUID is empty" );
            }
            else
            {
               for ( int j = 0; j < guids.Count; j++ )
               {
                  if ( i != j && guids[i] == guids[j] )
                  {
                     throw new Exception( "Header GUIDs are identical" );
                  }
               }
            }
         }
      }

      private static void CheckTextureSets( List<TextureSetSaveData> textureSets, string setType, int textureSetSize, int texturePoolCount )
      {
         for ( int i = 0; i < textureSets.Count; i++ )
         {
            for ( int j = 0; j < textureSets.Count; j++ )
            {
               if ( i != j && textureSets[i].Index == textureSets[j].Index )
               {
                  throw new Exception( string.Format( "{0} texture set \"{1}\"'s index matches the texture set \"{2}\"", setType, textureSets[i].Name, textureSets[j].Name ) );
               }
            }

            if ( textureSets[i].TexturePoolIndexes.Count > textureSetSize )
            {
               throw new Exception( string.Format( "{0} texture set \"{1}\" contains too many texture pool indexes", setType, textureSets[i].Name ) );
            }

            foreach ( var index in textureSets[i].TexturePoolIndexes )
            {
               if ( index < 0 || index > texturePoolCount )
               {
                  throw new Exception( string.Format( "{0} texture set \"{1}\" contains an invalid texture pool index", setType, textureSets[i].Name ) );
               }
            }
         }
      }

      private static void CheckTileMaps( List<TileMapSaveData> tileMaps,
                                         List<TextureSetSaveData> tileTextureSets,
                                         int staticSpriteTextureSetCount,
                                         int activeSpriteTextureSetCount )
      {
         for ( int i = 0; i < tileMaps.Count; i++ )
         {
            // check for duplicate tile indexes
            for ( int j = 0; j < tileMaps.Count; j++ )
            {
               if ( i != j && tileMaps[i].Index == tileMaps[j].Index )
               {
                  throw new Exception( string.Format( "Tile map \"{0}\"'s index matches the tile map \"{1}\"", tileMaps[i].Name, tileMaps[j].Name ) );
               }
            }

            // check tile map size
            if ( tileMaps[i].TilesX < Constants.TileMapMinTilesX || tileMaps[i].TilesY < Constants.TileMapMinTilesY )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" is too small", tileMaps[i].Name ) );
            }

            if ( tileMaps[i].TilesX > Constants.TileMapMaxTilesX || tileMaps[i].TilesY > Constants.TileMapMaxTilesY )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" is too large", tileMaps[i].Name ) );
            }

            if ( tileMaps[i].TilesX * tileMaps[i].TilesY != tileMaps[i].Tiles.Count )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" has a mismatched number of X and Y tiles to its total tile count", tileMaps[i].Name ) );
            }

            // check texture set indexes
            if ( tileMaps[i].TileTextureSetIndex >= tileTextureSets.Count )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" has invalid tile texture set index", tileMaps[i].Name ) );
            }

            if ( tileMaps[i].StaticSpriteTextureSetIndex >= staticSpriteTextureSetCount )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" has invalid static sprite texture set index", tileMaps[i].Name ) );
            }

            if ( tileMaps[i].ActiveSpriteTextureSetIndex >= activeSpriteTextureSetCount )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" has invalid active sprite texture set index", tileMaps[i].Name ) );
            }

            // check tile texture indexes
            foreach ( var tile in tileMaps[i].Tiles )
            {
               if ( tile.TextureIndex >= tileTextureSets[(int)tileMaps[i].TileTextureSetIndex].TexturePoolIndexes.Count )
               {
                  throw new Exception( string.Format( "Tile map \"{0}\" has invalid tile texture index", tileMaps[i].Name ) );
               }
            }

            CheckPortals( tileMaps, tileMaps[i] );
            CheckEntities( tileMaps[i] );
            CheckNpcs( tileMaps[i] );
         }
      }

      private static void CheckPortals( List<TileMapSaveData> tileMaps, TileMapSaveData tileMap )
      {
         // check portal count
         if ( tileMap.Portals.Count > Constants.TileMapMaxPortals )
         {
            throw new Exception( string.Format( "Tile map \"{0}\" contains too many portals", tileMap.Name ) );
         }

         for ( int i = 0; i < tileMap.Portals.Count; i++ )
         {
            // check for duplicate source tile indexes
            for ( int j = 0; j < tileMap.Portals.Count; j++ )
            {
               if ( i != j && tileMap.Portals[i].SourceTileIndex == tileMap.Portals[j].SourceTileIndex )
               {
                  throw new Exception( string.Format( "Tile map \"{0}\" contains portals with duplicate source tile indexes", tileMap.Name ) );
               }
            }

            // check destination tile map indexes
            if ( !tileMaps.Any( tm => tm.Index == tileMap.Portals[i].DestTileMapIndex ) )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" contains a portal with an invalid destination tile map index", tileMap.Name ) );
            }

            // check source tile indexes
            if ( tileMap.Portals[i].SourceTileIndex >= tileMap.Tiles.Count )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" contains a portal with an invalid source tile index", tileMap.Name ) );
            }

            // check destination tile indexes
            if ( tileMap.Portals[i].DestTileIndex >= tileMaps[(int)tileMap.Portals[i].DestTileMapIndex].Tiles.Count )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" contains a portal with an invalid destination tile index", tileMap.Name ) );
            }
         }
      }

      private static void CheckEntities( TileMapSaveData tileMap )
      {
         // check entity count
         if ( tileMap.Entities.Count > Constants.TileMapMaxEntities )
         {
            throw new Exception( string.Format( "Tile map \"{0}\" contains too many entities", tileMap.Name ) );
         }

         foreach ( var entity in tileMap.Entities )
         {
            // check position
            if ( ( entity.Pos.X < 0 ) || ( entity.Pos.X + entity.Pos.W ) >= ( tileMap.TilesX * ( Constants.TileSizePixels * Constants.UnitsPerPixel ) ) ||
                 ( entity.Pos.Y < 0 ) || ( entity.Pos.Y + entity.Pos.H ) >= ( tileMap.TilesY * ( Constants.TileSizePixels * Constants.UnitsPerPixel ) ) )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" contains an entity with an invalid position", tileMap.Name ) );
            }

            // check sprite index
            if ( entity.SpriteIndex >= tileMap.ActiveSprites.Count )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" contains an entity with an invalid sprite index", tileMap.Name ) );
            }
         }
      }

      private static void CheckNpcs( TileMapSaveData tileMap )
      {
         // check NPC count
         if ( tileMap.Npcs.Count > Constants.TileMapMaxNpcs )
         {
            throw new Exception( string.Format( "Tile map \"{0}\" contains too many NPCs", tileMap.Name ) );
         }

         // check entity indexes
         foreach ( var npc in tileMap.Npcs )
         {
            if ( npc.EntityIndex >= tileMap.Entities.Count )
            {
               throw new Exception( string.Format( "Tile map \"{0}\" contains an NPC with an invalid entity index", tileMap.Name ) );
            }
         }
      }

      private static void CheckPlayerClassExpTables( List<PlayerClassExpTableSaveData> tables )
      {
         if ( tables.Count != (int)PlayerClass.Count )
         {
            throw new Exception( string.Format( "Player class experience tables has {0} entries, but should have {1}", tables.Count, (int)PlayerClass.Count ) );
         }

         List<PlayerClass> classesFound = [];

         foreach ( var table in tables )
         {
            if ( table.ExpTable.Count != Constants.PlayerMaxLevel - 1 )
            {
               throw new Exception( string.Format( "Player experience table for class {0} has {1} entries, but should have {2}", table.PlayerClass.ToString(), table.ExpTable.Count, Constants.PlayerMaxLevel - 1 ) );
            }

            if ( classesFound.Contains( table.PlayerClass ) )
            {
               throw new Exception( string.Format( "Player experience table contains multiple entries for class {0}", table.PlayerClass.ToString() ) );
            }

            classesFound.Add( table.PlayerClass );
         }
      }
   }
}
