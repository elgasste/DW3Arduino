using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TextureSetSaveData
   {
      public uint Index { get; set; }
      public string Name { get; set; } = string.Empty;
      public List<uint> TexturePoolIndexes { get; set; } = [];
      public int WaterTextureIndex { get; set; }
      public int HorizontalBridgeTextureIndex { get; set; }
      public int VerticalBridgeTextureIndex { get; set; }
      public int ShoalTextureIndex { get; set; }
      public uint ShoreTextureStartIndex { get; set; }

      public TextureSetSaveData() { }

      public TextureSetSaveData( uint index, string name )
      {
         Index = index;
         Name = name;
         WaterTextureIndex = -1;
         HorizontalBridgeTextureIndex = -1;
         VerticalBridgeTextureIndex = -1;
         ShoalTextureIndex = -1;
      }

      public TextureSetSaveData( TextureSetViewModel viewModel )
      {
         Index = viewModel.Index;
         Name = viewModel.Name;
         WaterTextureIndex = viewModel.WaterTextureIndex;
         HorizontalBridgeTextureIndex = viewModel.HorizontalBridgeTextureIndex;
         VerticalBridgeTextureIndex = viewModel.VerticalBridgeTextureIndex;
         ShoalTextureIndex = viewModel.ShoalTextureIndex;
         ShoreTextureStartIndex = viewModel.ShoreTextureStartIndex;

         foreach ( var index in viewModel.TexturePoolIndexes )
         {
            TexturePoolIndexes.Add( index );
         }
      }
   }
}
