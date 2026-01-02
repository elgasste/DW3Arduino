using DW3ArduinoEditor.SaveData;
using DW3ArduinoEditor.Types;

namespace DW3ArduinoEditor.ViewModels
{
   public class EntityViewModel : ViewModelBase
   {
      private Vector4f _pos;
      public Vector4f Pos
      {
         get => _pos;
         set => SetProperty( ref _pos, value );
      }

      private uint _spriteIndex;
      public uint SpriteIndex
      {
         get => _spriteIndex;
         set => SetProperty( ref _spriteIndex, value );
      }

      public EntityViewModel( Vector4f pos, uint spriteIndex )
      {
         _pos = pos;
         _spriteIndex = spriteIndex;
      }

      public EntityViewModel( EntitySaveData saveData )
      {
         _pos = saveData.Pos;
         _spriteIndex = saveData.SpriteIndex;
      }
   }
}
