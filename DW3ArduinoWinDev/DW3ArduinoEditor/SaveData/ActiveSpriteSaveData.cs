using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.Types;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class ActiveSpriteSaveData
   {
      public uint TextureIndex { get; set; }
      public Vector2f Offset { get; set; } = new( 0, 0 );
      public Direction StartDirection { get; set; }

      public ActiveSpriteSaveData() { }

      public ActiveSpriteSaveData( uint textureIndex, Vector2f offset, Direction startDirection )
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
