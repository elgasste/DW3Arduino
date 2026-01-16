using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class StaticSpriteSaveData
   {
      public uint TextureIndex { get; set; }
      public uint TileIndex { get; set; }
      public bool IsPassable { get; set; }

      public StaticSpriteSaveData() { }

      public StaticSpriteSaveData( uint textureIndex, uint tileIndex, bool isPassable )
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
