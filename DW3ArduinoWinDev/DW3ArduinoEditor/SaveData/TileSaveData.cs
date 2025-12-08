using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TileSaveData
   {
      public uint Index { get; set; }
      public uint TextureIndex { get; set; }
      public bool IsPassable { get; set; }

      public TileSaveData() { }

      public TileSaveData( TileViewModel viewModel, uint index )
      {
         Index = index;
         TextureIndex = viewModel.TextureIndex;
         IsPassable = viewModel.IsPassable;
      }
   }
}
