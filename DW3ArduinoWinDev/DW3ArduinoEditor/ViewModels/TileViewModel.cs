using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileViewModel : ViewModelBase
   {
      private int _textureIndex;
      public int TextureIndex
      {
         get => _textureIndex;
         set => SetProperty( ref _textureIndex, value );
      }

      private bool _isPassable;
      public bool IsPassable
      {
         get => _isPassable;
         set => SetProperty( ref _isPassable, value );
      }

      public TileViewModel( int textureIndex, bool isPassable )
      {
         _textureIndex = textureIndex;
         _isPassable = isPassable;
      }

      public TileViewModel( TileSaveData saveData )
      {
         _textureIndex = saveData.TextureIndex;
         _isPassable = saveData.IsPassable;
      }
   }
}
