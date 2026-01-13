using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class HeaderGuidsSaveData
   {
      public string TextBitFieldsHeaderGuid { get; set; } = string.Empty;
      public string TileTexturesHeaderGuid { get; set; } = string.Empty;
      public string StaticSpriteTexturesHeaderGuid { get; set; } = string.Empty;
      public string ActiveSpriteTexturesHeaderGuid { get; set; } = string.Empty;
      public string PlayerSpriteTexturesHeaderGuid { get; set; } = string.Empty;
      public string MapTilesHeaderGuid { get; set; } = string.Empty;

      public HeaderGuidsSaveData() { }

      public HeaderGuidsSaveData( HeaderGuidsViewModel viewModel )
      {
         TextBitFieldsHeaderGuid = viewModel.TextBitFieldsHeaderGuid;
         TileTexturesHeaderGuid = viewModel.TileTexturesHeaderGuid;
         StaticSpriteTexturesHeaderGuid = viewModel.StaticSpriteTexturesHeaderGuid;
         ActiveSpriteTexturesHeaderGuid= viewModel.ActiveSpriteTexturesHeaderGuid;
         PlayerSpriteTexturesHeaderGuid = viewModel.PlayerSpriteTexturesHeaderGuid;
         MapTilesHeaderGuid = viewModel.MapTilesHeaderGuid;
      }
   }
}
