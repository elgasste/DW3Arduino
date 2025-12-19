using DW3ArduinoEditor.ViewModels;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.SaveData
{
   public class GameSaveData
   {
      public List<TileMapSaveData> TileMaps { get; set; } = [];
      public List<TileTextureSetSaveData> TileTextureSets { get; set; } = [];
      public List<StaticSpriteTextureSetSaveData> StaticSpriteTextureSets { get; set; } = [];

      public GameSaveData() { }

      public GameSaveData( ObservableCollection<TileMapViewModel> tileMapVMs,
                           ObservableCollection<TileTextureSetViewModel> tileTextureSetVMs,
                           ObservableCollection<StaticSpriteTextureSetViewModel> staticSpriteTextureSetVMs )
      {
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
      }
   }
}
