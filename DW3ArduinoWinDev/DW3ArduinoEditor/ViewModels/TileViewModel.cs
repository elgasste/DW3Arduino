using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileViewModel : ViewModelBase
   {
      private uint _textureIndex;
      public uint TextureIndex
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

      public TileViewModel()
      {
         _textureIndex = 0;
         _isPassable = true;
      }

      public TileViewModel( TileSaveData saveData )
      {
         _textureIndex = saveData.TextureIndex;
         _isPassable = saveData.IsPassable;
      }
   }
}
