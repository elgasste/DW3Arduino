using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TextureSetSaveData
   {
      public uint Index { get; set; }
      public string Name { get; set; } = string.Empty;
      public List<int> TexturePoolIndexes { get; set; } = [];

      public TextureSetSaveData() { }

      public TextureSetSaveData( uint index, string name )
      {
         Index = index;
         Name = name;
      }

      public TextureSetSaveData( TextureSetViewModel viewModel )
      {
         Index = viewModel.Index;
         Name = viewModel.Name;

         foreach ( var index in viewModel.TexturePoolIndexes )
         {
            TexturePoolIndexes.Add( index );
         }
      }
   }
}
