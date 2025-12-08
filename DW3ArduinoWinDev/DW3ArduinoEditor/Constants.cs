namespace DW3ArduinoEditor
{
   public static class Constants
   {
      public const string AssetsBasePath = "..\\..\\..\\assets\\";
      public const string OutputBasePath = "..\\..\\..\\..\\..\\DW3Arduino\\";

      public const string SaveDataFilePath = AssetsBasePath + "EditorData.json";
      public const string GameDataSourceFilePath = OutputBasePath + "game_data.c";

      public const string TileTexturePoolImagePath = AssetsBasePath + "map\\tile_texture_pool.png";

      public const int TileSize = 16;
      public const int TileMapMinTilesX = 3;
      public const int TileMapMinTilesY = 3;
      public const int TileMapMaxTilesX = 256;
      public const int TileMapMaxTilesY = 256;
      public const int TileMapDefaultTilesX = 24;
      public const int TileMapDefaultTilesY = 24;

      public const int PaletteMaxColors = 256;
   }
}
