using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class PortalViewModel : ViewModelBase
   {
      private uint _sourceTileIndex;
      public uint SourceTileIndex
      {
         get => _sourceTileIndex;
         set => SetProperty( ref _sourceTileIndex, value );
      }

      private uint _destTileMapIndex;
      public uint DestTileMapIndex
      {
         get => _destTileMapIndex;
         set => SetProperty( ref _destTileMapIndex, value );
      }

      private uint _destTileIndex;
      public uint DestTileIndex
      {
         get => _destTileIndex;
         set => SetProperty( ref _destTileIndex, value );
      }

      private Direction _destDirection;
      public Direction DestDirection
      {
         get => _destDirection;
         set => SetProperty( ref _destDirection, value );
      }

      public PortalViewModel( uint sourceTileIndex, uint destTileMapIndex, uint destTileIndex, Direction destDirection )
      {
         _sourceTileIndex = sourceTileIndex;
         _destTileMapIndex = destTileMapIndex;
         _destTileIndex = destTileIndex;
         _destDirection = DestDirection;
      }

      public PortalViewModel( PortalSaveData saveData )
      {
         _sourceTileIndex = saveData.SourceTileIndex;
         _destTileMapIndex = saveData.DestTileMapIndex;
         _destTileIndex = saveData.DestTileIndex;
         _destDirection = saveData.DestDirection;
      }
   }
}
