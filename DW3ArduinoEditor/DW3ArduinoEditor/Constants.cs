using System.DirectoryServices.ActiveDirectory;

namespace DW3ArduinoEditor
{
   public static class Constants
   {
      public const string AssetsBasePath = "..\\..\\..\\..\\..\\Assets\\";
      public const string DocsBasePath = "..\\..\\..\\..\\..\\Docs\\";
      public const string OutputBasePath = "..\\..\\..\\..\\..\\DW3Arduino\\";

      public const string GameDataCommonHeaderFileName = "gen_common.h";
      public const string GameDataCommonHeaderDefine = "GEN_COMMON_H";
      public const string GameDataTextBitFieldsHeaderFileName = "text_bit_fields.h";
      public const string GameDataTileTexturesHeaderFileName = "tile_textures.h";
      public const string GameDataStaticSpriteTexturesHeaderFileName = "static_sprite_textures.h";
      public const string GameDataActiveSpriteTexturesHeaderFileName = "active_sprite_textures.h";
      public const string GameDataPlayerSpriteTexturesHeaderFileName = "player_sprite_textures.h";
      public const string GameDataTileMapsHeaderFileName = "tile_maps.h";

      public const string SaveDataFilePath = AssetsBasePath + "EditorData.json";
      public const string GameDataSourceFilePath = OutputBasePath + "game_data.c";
      public const string GameDataCommonHeaderPath = OutputBasePath + GameDataCommonHeaderFileName;
      public const string GameDataTextBitFieldsHeaderPath = OutputBasePath + GameDataTextBitFieldsHeaderFileName;
      public const string GameDataTileTexturesHeaderPath = OutputBasePath + GameDataTileTexturesHeaderFileName;
      public const string GameDataStaticSpriteTexturesHeaderPath = OutputBasePath + GameDataStaticSpriteTexturesHeaderFileName;
      public const string GameDataActiveSpriteTexturesHeaderPath = OutputBasePath + GameDataActiveSpriteTexturesHeaderFileName;
      public const string GameDataPlayerSpriteTexturesHeaderPath = OutputBasePath + GameDataPlayerSpriteTexturesHeaderFileName;
      public const string GameDataTileMapsHeaderPath = OutputBasePath + GameDataTileMapsHeaderFileName;

      public const string TileTexturePoolImagePath = AssetsBasePath + "textures\\tile_texture_pool.png";
      public const string StaticSpriteTexturePoolImagePath = AssetsBasePath + "textures\\static_sprite_texture_pool.png";
      public const string ActiveSpriteTexturePoolImagePath = AssetsBasePath + "textures\\active_sprite_texture_pool.png";
      public const string PlayerSpriteTexturePoolImagePath = AssetsBasePath + "textures\\player_sprite_texture_pool.png";
      public const string TextTilesFilePath = AssetsBasePath + "text_tiles.png";

      public const int UnitsPerPixel = 100;

      public const int MaxSaveSlots = 3;

      public const int MaxPlayers = 4;
      public const int PlayerMaxNameLength = 8;

      public const int ClockFps = 60;

      public const int MaxAnimations = 32;

      public const float DayFactorTotalSeconds = 29.0f;
      public const float DayFactorLowCutoff = 0.15f;
      public const float DayFactorHighCutoff = 0.85f;
      public const float DayFactorUndergroundThreshold = 0.4f;

      public const int EncounterChanceLow = 24;
      public const int EncounterChanceMedium = 16;
      public const int EncounterChanceHigh = 8;

      public const int NpcMinActionSeconds = 1;
      public const int NpcMaxActionSeconds = 4;
      public const int NpcMinVelocity = 8;
      public const int NpcMaxVelocity = 40;

      public const int GenericEntityWidth = 12 * UnitsPerPixel;
      public const int GenericEntityHeight = 12 * UnitsPerPixel;

      public const int PlayerMoveHistorySize = 16;
      public const int PlayerSpriteXOffsetPixels = 2;
      public const int PlayerSpriteYOffsetPixels = 4;

      public const int ScreenWidth = 256;
      public const int ScreenHeight = 224;
      public const int ScreenPaletteSize = 256;
      public const int ScreenTextTileCount = 85;
      public const int ScreenTextTileSize = 8;
      public const int ScreenTransparentColor = 0xF81F;
      public const int ScreenMenuBorderCharTopLeft = 1;
      public const int ScreenMenuBorderCharTopRight = 2;
      public const int ScreenMenuBorderCharBottomLeft = 3;
      public const int ScreenMenuBorderCharBottomRight = 4;
      public const int ScreenMenuBorderCharLeft = 5;
      public const int ScreenMenuBorderCharTop = 6;
      public const int ScreenMenuBorderCharRight = 7;
      public const int ScreenMenuBorderCharBottom = 8;
      public const int ScreenDownwardCaratChar = 9;

      public const int TileSizePixels = 16;
      public const int TileMapMinTilesX = 3;
      public const int TileMapMinTilesY = 3;
      public const int TileMapMaxTilesX = 256;
      public const int TileMapMaxTilesY = 256;
      public const int TileMapDefaultTilesX = 24;
      public const int TileMapDefaultTilesY = 24;
      public const int TileMapMaxStaticSprites = 64;
      public const int TileMapMaxActiveSprites = 16;
      public const int TileMapMaxPortals = 32;
      public const int TileMapMaxEntities = 32;
      public const int TileMapMaxNpcs = 24;
      public const int TileTextureSetSize = 32;
      public const float TileMapSwapFadeSeconds = 0.3f;
      public const float TileMapSwapPauseSeconds = 0.2f;

      public const int TileWalkSpeedNormal = 100;
      public const int TileWalkSpeedSlow = 80;
      public const int TileWalkSpeedVerySlow = 60;
      public const int TileWalkSpeedCrawl = 40;
      public const int TileWalkSpeedDiagonalNormal = 80;
      public const int TileWalkSpeedDiagonalSlow = 60;
      public const int TileWalkSpeedDiagonalVerySlow = 40;
      public const int TileWalkSpeedDiagonalCrawl = 20;

      public const int StaticSpriteTextureSize = 16;
      public const int StaticSpriteTextureSetSize = 32;
      public const int ActiveSpriteTextureWidth = 32;
      public const int ActiveSpriteTextureHeight = 64;
      public const int ActiveSpriteFrames = 2;
      public const int ActiveSpriteFrameSize = 16;
      public const int ActiveSpriteTextureSetSize = 16;
      public const float ActiveSpriteFrameTotalSeconds = 0.35f;

      public const string GeneratedFileHeaderMessage = "// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!\n\n";

      public const int EditorTileSize = 32;
   }
}
