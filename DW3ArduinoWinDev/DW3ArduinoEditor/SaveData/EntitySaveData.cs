using System.Numerics;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class EntitySaveData
   {
      public Vector4 Pos { get; set; }
      public uint SpriteIndex { get; set; }

      public EntitySaveData() { }

      public EntitySaveData( EntityViewModel viewModel )
      {
         Pos = viewModel.Pos;
         SpriteIndex = viewModel.SpriteIndex;
      }
   }
}
