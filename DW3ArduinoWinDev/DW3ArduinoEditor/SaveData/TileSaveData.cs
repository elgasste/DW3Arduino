using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TileSaveData
   {
      public uint TextureIndex { get; set; }
      public bool IsPassable { get; set; }

      public TileSaveData() { }

      public TileSaveData( TileViewModel viewModel )
      {
         TextureIndex = viewModel.TextureIndex;
         IsPassable = viewModel.IsPassable;
      }
   }
}
