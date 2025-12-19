using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class StaticSpriteSaveData
   {
      public int TextureIndex { get; set; }
      public int TileIndex { get; set; }
      public bool IsPassable { get; set; }

      public StaticSpriteSaveData() { }

      public StaticSpriteSaveData( int textureIndex, int tileIndex, bool isPassable )
      {
         TextureIndex = textureIndex;
         TileIndex = tileIndex;
         IsPassable = isPassable;
      }

      public StaticSpriteSaveData( StaticSpriteViewModel vm )
      {
         TextureIndex = vm.TextureIndex;
         TileIndex = vm.TileIndex;
         IsPassable = vm.IsPassable;
      }
   }
}
