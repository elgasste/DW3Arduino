namespace DW3ArduinoEditor.SaveData
{
   public static class GameSaveDataSanityChecker
   {
      public static void CheckSanity( GameSaveData saveData,
                                      int tileTexturePoolCount,
                                      int staticSpriteTexturePoolCount,
                                      int activeSpriteTexturePoolCount,
                                      int playerSpriteTexturePoolCount )
      {
         CheckHeaderGuids( saveData.HeaderGuids );
         CheckTileTextureSets( saveData.TileTextureSets, tileTexturePoolCount );

         // TODO
         //
         // - make sure there aren't too many textures in each static sprite texture set
         // - make sure there aren't too many textures in each active sprite texture set
         // - make sure the number of player sprite textures matches the number of player classes
         // - make sure no two tile maps have the same index
         // - make sure tile maps are within the accepted size
         // - make sure all tiles in each tile map have a valid texture index
         // - make sure each tile map's portals are valid
         //    - all source tile indexes should be different
         //    - all destination tile map indexes should exist
         //    - the destination tile map tile index should exist
         // - make sure each tile map's entities are valid
         //    - no more than the maximum amount
         //    - should have valid positions
         //    - sprite index should be valid
         // - make sure each tile map's NPCs are valid
         //    - no more than the maximum amount
         //    - entity index should be valid
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

      private static void CheckTileTextureSets( List<TileTextureSetSaveData> tileTextureSets, int tileTexturePoolCount )
      {
         foreach ( var tileTextureSet in tileTextureSets )
         {
            if ( tileTextureSet.TileTexturePoolIndexes.Count >= Constants.TileTextureSetSize )
            {
               throw new Exception( string.Format( "Tile texture set \"{0}\" contains too many texture pool indexes", tileTextureSet.Name ) );
            }

            foreach ( var index in tileTextureSet.TileTexturePoolIndexes )
            {
               if ( index < 0 || index >= tileTexturePoolCount )
               {
                  throw new Exception( string.Format( "Tile texture set \"{0}\" contains an invalid texture pool index", tileTextureSet.Name ) );
               }
            }
         }
      }
   }
}
