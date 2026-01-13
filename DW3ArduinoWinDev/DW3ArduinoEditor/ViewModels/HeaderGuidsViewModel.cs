using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class HeaderGuidsViewModel : ViewModelBase
   {
      private string _textBitFieldsHeaderGuid = string.Empty;
      public string TextBitFieldsHeaderGuid
      {
         get => _textBitFieldsHeaderGuid;
         set => SetProperty( ref _textBitFieldsHeaderGuid, value );
      }

      private string _tileTexturesHeaderGuid = string.Empty;
      public string TileTexturesHeaderGuid
      {
         get => _tileTexturesHeaderGuid;
         set => SetProperty( ref _tileTexturesHeaderGuid, value );
      }

      private string _staticSpriteTexturesHeaderGuid = string.Empty;
      public string StaticSpriteTexturesHeaderGuid
      {
         get => _staticSpriteTexturesHeaderGuid;
         set => SetProperty( ref _staticSpriteTexturesHeaderGuid, value );
      }

      private string _activeSpriteTexturesHeaderGuid = string.Empty;
      public string ActiveSpriteTexturesHeaderGuid
      {
         get => _activeSpriteTexturesHeaderGuid;
         set => SetProperty( ref _activeSpriteTexturesHeaderGuid, value );
      }

      private string _playerSpriteTexturesHeaderGuid = string.Empty;
      public string PlayerSpriteTexturesHeaderGuid
      {
         get => _playerSpriteTexturesHeaderGuid;
         set => SetProperty( ref _playerSpriteTexturesHeaderGuid, value );
      }

      private string _mapTilesHeaderGuid = string.Empty;
      public string MapTilesHeaderGuid
      {
         get => _mapTilesHeaderGuid;
         set => SetProperty( ref _mapTilesHeaderGuid, value );
      }

      public HeaderGuidsViewModel() { }

      public HeaderGuidsViewModel( HeaderGuidsSaveData saveData )
      {
         TextBitFieldsHeaderGuid = saveData.TextBitFieldsHeaderGuid;
         TileTexturesHeaderGuid = saveData.TileTexturesHeaderGuid;
         StaticSpriteTexturesHeaderGuid = saveData.StaticSpriteTexturesHeaderGuid;
         ActiveSpriteTexturesHeaderGuid = saveData .ActiveSpriteTexturesHeaderGuid;
         PlayerSpriteTexturesHeaderGuid= saveData .PlayerSpriteTexturesHeaderGuid;
         MapTilesHeaderGuid = saveData .MapTilesHeaderGuid;
      }
   }
}
