using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class StaticSpriteViewModel : ViewModelBase
   {
      private uint _textureIndex;
      public uint TextureIndex
      {
         get => _textureIndex;
         set => SetProperty( ref _textureIndex, value );
      }

      private uint _tileIndex;
      public uint TileIndex
      {
         get => _tileIndex;
         set => SetProperty( ref _tileIndex, value );
      }

      private bool _isPassable;
      public bool IsPassable
      {
         get => _isPassable;
         set => SetProperty( ref _isPassable, value );
      }

      public StaticSpriteViewModel()
      {
         TextureIndex = 0;
         TileIndex = 0;
         _isPassable = true;
      }

      public StaticSpriteViewModel( StaticSpriteSaveData saveData )
      {
         TextureIndex = saveData.TextureIndex;
         TileIndex = saveData.TileIndex;
         IsPassable = saveData.IsPassable;
      }
   }
}
