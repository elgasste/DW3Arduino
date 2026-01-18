using DW3ArduinoEditor.ViewModels;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.SaveData
{
   public class GameSaveData
   {
      public GameStartupSaveData GameStartup { get; set; } = new();
      public HeaderGuidsSaveData HeaderGuids { get; set; } = new();
      public List<TileMapSaveData> TileMaps { get; set; } = [];
      public List<TextureSetSaveData> TileTextureSets { get; set; } = [];
      public List<TextureSetSaveData> StaticSpriteTextureSets { get; set; } = [];
      public List<TextureSetSaveData> ActiveSpriteTextureSets { get; set; } = [];

      public GameSaveData() { }

      public GameSaveData( GameStartupViewModel gameStartupViewModel,
                           HeaderGuidsViewModel headerGuids,
                           ObservableCollection<TileMapViewModel> tileMapVMs,
                           ObservableCollection<TextureSetViewModel> tileTextureSetVMs,
                           ObservableCollection<TextureSetViewModel> staticSpriteTextureSetVMs,
                           ObservableCollection<TextureSetViewModel> activeSpriteTextureSetVMs )
      {
         GameStartup = new( gameStartupViewModel );
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
