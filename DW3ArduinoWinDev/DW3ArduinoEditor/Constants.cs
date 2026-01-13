namespace DW3ArduinoEditor
{
   public static class Constants
   {
      public const string AssetsBasePath = "..\\..\\..\\..\\..\\Assets\\";
      public const string DocsBasePath = "..\\..\\..\\..\\..\\Docs\\";
      public const string OutputBasePath = "..\\..\\..\\..\\..\\DW3Arduino\\";

      public const string SaveDataFilePath = AssetsBasePath + "EditorData.json";
      public const string GameDataSourceFilePath = OutputBasePath + "game_data.c";
      public const string GameDataTextBitFieldsHeaderPath = OutputBasePath + "text_bit_fields.h";
      public const string GameDataTileTexturesHeaderPath = OutputBasePath + "tile_textures.h";

      public const string TileTexturePoolImagePath = AssetsBasePath + "textures\\tile_texture_pool.png";
      public const string StaticSpriteTexturePoolImagePath = AssetsBasePath + "textures\\static_sprite_texture_pool.png";
      public const string ActiveSpriteTexturePoolImagePath = AssetsBasePath + "textures\\active_sprite_texture_pool.png";
      public const string PlayerSpriteTexturePoolImagePath = AssetsBasePath + "textures\\player_sprite_texture_pool.png";
      public const string TextTilesFilePath = AssetsBasePath + "text_tiles.png";

      public const int TileSize = 16;
      public const int TileMapMinTilesX = 3;
      public const int TileMapMinTilesY = 3;
      public const int TileMapMaxTilesX = 256;
      public const int TileMapMaxTilesY = 256;
      public const int TileMapDefaultTilesX = 24;
      public const int TileMapDefaultTilesY = 24;
      public const int TileTextureSetSize = 32;

      public const int StaticSpriteTextureSize = 16;
      public const int StaticSpriteTextureSetSize = 32;
      public const int ActiveSpriteTextureWidth = 32;
      public const int ActiveSpriteTextureHeight = 64;
      public const int ActiveSpriteFrames = 2;
      public const int ActiveSpriteTextureFrameSize = 16;
      public const int ActiveSpriteTextureSetSize = 16;

      public const int PaletteMaxColors = 256;

      public const int TextTileSize = 8;
      public const int TextTileCount = 85;
   }
}
