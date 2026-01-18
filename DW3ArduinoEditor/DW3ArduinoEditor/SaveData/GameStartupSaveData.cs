using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class GameStartupSaveData
   {
      public uint PlayerStartTileMapIndex { get; set; }
      public uint PlayerStartTileIndex { get; set; }

      public GameStartupSaveData() { }

      public GameStartupSaveData( GameStartupViewModel viewModel )
      {
         PlayerStartTileMapIndex = viewModel.PlayerStartTileMapIndex;
         PlayerStartTileIndex = viewModel.PlayerStartTileIndex;
      }
   }
}
