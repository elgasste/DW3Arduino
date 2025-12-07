using DW3ArduinoEditor.ViewModels;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.SaveData
{
   public class GameSaveData
   {
      public List<TileMapSaveData> TileMaps { get; set; } = [];

      public GameSaveData() { }

      public GameSaveData( ObservableCollection<TileMapViewModel> tileMapVMs )
      {
         foreach ( var tileMapVM in tileMapVMs )
         {
            TileMaps.Add( new( tileMapVM ) );
         }
      }
   }
}
