using System.Numerics;
using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class ActiveSpriteViewModel : ViewModelBase
   {
      private int _textureIndex;
      public int TextureIndex
      {
         get => _textureIndex;
         set => SetProperty( ref _textureIndex, value );
      }

      private Vector2 _offset;
      public Vector2 Offset
      {
         get => _offset;
         set => SetProperty( ref _offset, value );
      }

      private Direction _startDirection;
      public Direction StartDirection
      {
         get => _startDirection;
         set => SetProperty( ref _startDirection, value );
      }

      public ActiveSpriteViewModel()
      {
         TextureIndex = 0;
         Offset = Vector2.Zero;
         StartDirection = Direction.Down;
      }

      public ActiveSpriteViewModel( ActiveSpriteSaveData saveData )
      {
         TextureIndex = saveData.TextureIndex;
         Offset = saveData.Offset;
         StartDirection = saveData.StartDirection;
      }
   }
}
