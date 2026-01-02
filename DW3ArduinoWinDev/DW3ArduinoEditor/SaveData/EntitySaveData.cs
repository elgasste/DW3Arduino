using DW3ArduinoEditor.Types;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class EntitySaveData
   {
      public Vector4f Pos { get; set; } = new( 0, 0, 0, 0 );
      public uint SpriteIndex { get; set; }

      public EntitySaveData() { }

      public EntitySaveData( EntityViewModel viewModel )
      {
         Pos = viewModel.Pos;
         SpriteIndex = viewModel.SpriteIndex;
      }
   }
}
