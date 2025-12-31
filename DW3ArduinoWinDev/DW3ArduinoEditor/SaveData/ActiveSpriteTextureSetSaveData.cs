using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class ActiveSpriteTextureSetSaveData
   {
      public uint Index { get; set; }
      public List<int> ActiveSpriteTexturePoolIndexes { get; set; } = [];

      public ActiveSpriteTextureSetSaveData() { }

      public ActiveSpriteTextureSetSaveData( uint index )
      {
         Index = index;

         for ( int i = 0; i < Constants.ActiveSpriteTextureSetSize; i++ )
         {
            ActiveSpriteTexturePoolIndexes.Add( 0 );
         }
      }

      public ActiveSpriteTextureSetSaveData( ActiveSpriteTextureSetViewModel viewModel )
      {
         Index = viewModel.Index;

         foreach ( var index in viewModel.ActiveSpriteTexturePoolIndexes )
         {
            ActiveSpriteTexturePoolIndexes.Add( index );
         }
      }
   }
}
