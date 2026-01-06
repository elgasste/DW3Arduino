using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TileTextureSetSaveData
   {
      public uint Index { get; set; }
      public string Name { get; set; } = string.Empty;
      public List<int> TileTexturePoolIndexes { get; set; } = [];

      public TileTextureSetSaveData() { }

      public TileTextureSetSaveData( uint index, string name )
      {
         Index = index;
         Name = name;

         for ( int i = 0; i < Constants.TileTextureSetSize; i++ )
         {
            TileTexturePoolIndexes.Add( 0 );
         }
      }

      public TileTextureSetSaveData( TileTextureSetViewModel viewModel )
      {
         Index = viewModel.Index;
         Name = viewModel.Name;

         foreach ( var index in viewModel.TileTexturePoolIndexes )
         {
            TileTexturePoolIndexes.Add( index );
         }
      }
   }
}
