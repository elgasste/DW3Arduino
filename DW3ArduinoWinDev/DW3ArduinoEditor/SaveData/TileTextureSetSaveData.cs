using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TileTextureSetSaveData
   {
      public uint Index { get; set; }
      public List<int> TileTexturePoolIndexes { get; set; } = [];

      public TileTextureSetSaveData() { }

      public TileTextureSetSaveData( uint index )
      {
         Index = index;

         for ( int i = 0; i < Constants.TileTextureSetSize; i++ )
         {
            TileTexturePoolIndexes.Add( 0 );
         }
      }

      public TileTextureSetSaveData( TileTextureSetViewModel viewModel )
      {
         Index = viewModel.Index;

         foreach ( var index in viewModel.TileTexturePoolIndexes )
         {
            TileTexturePoolIndexes.Add( index );
         }
      }
   }
}
