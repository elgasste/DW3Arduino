using System.Numerics;
using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.SaveData;
using DW3ArduinoEditor.Types;

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

      private Vector2f _offset = new( 0, 0 );
      public Vector2f Offset
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
         Offset = new( 0, 0 );
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
