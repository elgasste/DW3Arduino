using System.ComponentModel;

namespace DW3ArduinoEditor.Enums
{
   public enum TileEditTool
   {
      [Description( "Change Tile Texture" )]
      ChangeTexture = 0,

      [Description( "Flood Fill Texture" )]
      FloodFillTexture
   }
}
