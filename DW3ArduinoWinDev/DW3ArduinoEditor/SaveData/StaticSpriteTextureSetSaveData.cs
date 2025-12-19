using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class StaticSpriteTextureSetSaveData
   {
      public uint Index { get; set; }
      public List<int> StaticSpriteTexturePoolIndexes { get; set; } = [];

      public StaticSpriteTextureSetSaveData() { }

      public StaticSpriteTextureSetSaveData( uint index )
      {
         Index = index;

         for ( int i = 0; i < Constants.StaticSpriteTextureSetSize; i++ )
         {
            StaticSpriteTexturePoolIndexes.Add( 0 );
         }
      }

      public StaticSpriteTextureSetSaveData( StaticSpriteTextureSetViewModel viewModel )
      {
         Index = viewModel.Index;

         foreach ( var index in viewModel.StaticSpriteTexturePoolIndexes )
         {
            StaticSpriteTexturePoolIndexes.Add( index );
         }
      }
   }
}
