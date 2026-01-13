using DW3ArduinoEditor.ViewModels;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.SaveData
{
   public class GameSaveData
   {
      public HeaderGuidsSaveData HeaderGuids { get; set; } = new();
      public List<TileMapSaveData> TileMaps { get; set; } = [];
      public List<TileTextureSetSaveData> TileTextureSets { get; set; } = [];
      public List<StaticSpriteTextureSetSaveData> StaticSpriteTextureSets { get; set; } = [];
      public List<ActiveSpriteTextureSetSaveData> ActiveSpriteTextureSets { get; set; } = [];

      public GameSaveData() { }

      public GameSaveData( HeaderGuidsViewModel headerGuids,
                           ObservableCollection<TileMapViewModel> tileMapVMs,
                           ObservableCollection<TileTextureSetViewModel> tileTextureSetVMs,
                           ObservableCollection<StaticSpriteTextureSetViewModel> staticSpriteTextureSetVMs,
                           ObservableCollection<ActiveSpriteTextureSetViewModel> activeSpriteTextureSetVMs )
      {
         HeaderGuids = new( headerGuids );

         foreach ( var tileMapVM in tileMapVMs )
         {
            TileMaps.Add( new( tileMapVM ) );
         }

         foreach ( var tileTextureSetVM in tileTextureSetVMs )
         {
            TileTextureSets.Add( new( tileTextureSetVM ) );
         }

         foreach ( var staticSpriteTextureSetVM in staticSpriteTextureSetVMs )
         {
            StaticSpriteTextureSets.Add( new( staticSpriteTextureSetVM ) );
         }

         foreach ( var activeSpriteTextureSetVM in activeSpriteTextureSetVMs )
         {
            ActiveSpriteTextureSets.Add( new( activeSpriteTextureSetVM ) );
         }
      }
   }
}
