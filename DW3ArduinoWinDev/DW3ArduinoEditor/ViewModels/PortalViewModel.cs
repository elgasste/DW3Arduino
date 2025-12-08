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

      public PortalViewModel( PortalSaveData saveData )
      {
         _sourceTileIndex = saveData.SourceTileIndex;
         _destTileMapIndex = saveData.DestTileMapIndex;
         _destTileIndex = saveData.DestTileIndex;
      }
   }
}
