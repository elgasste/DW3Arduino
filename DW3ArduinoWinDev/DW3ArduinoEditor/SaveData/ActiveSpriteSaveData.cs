using System.Numerics;
using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class ActiveSpriteSaveData
   {
      public int TextureIndex { get; set; }
      public Vector2 Offset { get; set; }
      public Direction StartDirection { get; set; }

      public ActiveSpriteSaveData() { }

      public ActiveSpriteSaveData( int textureIndex, Vector2 offset, Direction startDirection )
      {
         TextureIndex = textureIndex;
         Offset = offset;
         StartDirection = startDirection;
      }

      public ActiveSpriteSaveData( ActiveSpriteViewModel vm )
      {
         TextureIndex = vm.TextureIndex;
         Offset = vm.Offset;
         StartDirection = vm.StartDirection;
      }
   }
}
