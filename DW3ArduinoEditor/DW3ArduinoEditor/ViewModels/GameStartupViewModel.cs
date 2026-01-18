using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class GameStartupViewModel : ViewModelBase
   {
      private uint _playerStartTileMapIndex;
      public uint PlayerStartTileMapIndex
      {
         get => _playerStartTileMapIndex;
         set => SetProperty( ref _playerStartTileMapIndex, value );
      }

      private uint _playerStartTileIndex;
      public uint PlayerStartTileIndex
      {
         get => _playerStartTileIndex;
         set => SetProperty( ref _playerStartTileIndex, value );
      }

      private Direction _playerStartDirection;
      public Direction PlayerStartDirection
      {
         get => _playerStartDirection;
         set => SetProperty( ref _playerStartDirection, value );
      }

      public GameStartupViewModel() { }

      public GameStartupViewModel( GameStartupSaveData saveData )
      {
         _playerStartTileMapIndex = saveData.PlayerStartTileMapIndex;
         _playerStartTileIndex = saveData.PlayerStartTileIndex;
         _playerStartDirection = saveData.PlayerStartDirection;
      }
   }
}
