using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class PortalSaveData
   {
      public uint SourceTileIndex { get; set; }
      public uint DestTileMapIndex { get; set; }
      public uint DestTileIndex { get; set; }
      public Direction DestDirection { get; set; }

      public PortalSaveData() { }

      public PortalSaveData( PortalViewModel viewModel )
      {
         SourceTileIndex = viewModel.SourceTileIndex;
         DestTileMapIndex = viewModel.DestTileMapIndex;
         DestTileIndex = viewModel.DestTileIndex;
         DestDirection = viewModel.DestDirection;
      }
   }
}
