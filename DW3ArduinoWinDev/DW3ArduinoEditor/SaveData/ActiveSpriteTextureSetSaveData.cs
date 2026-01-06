using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class ActiveSpriteTextureSetSaveData
   {
      public uint Index { get; set; }
      public string Name { get; set; } = string.Empty;
      public List<int> ActiveSpriteTexturePoolIndexes { get; set; } = [];

      public ActiveSpriteTextureSetSaveData() { }

      public ActiveSpriteTextureSetSaveData( uint index, string name )
      {
         Index = index;
         Name = name;

         for ( int i = 0; i < Constants.ActiveSpriteTextureSetSize; i++ )
         {
            ActiveSpriteTexturePoolIndexes.Add( 0 );
         }
      }

      public ActiveSpriteTextureSetSaveData( ActiveSpriteTextureSetViewModel viewModel )
      {
         Index = viewModel.Index;
         Name = viewModel.Name;

         foreach ( var index in viewModel.ActiveSpriteTexturePoolIndexes )
         {
            ActiveSpriteTexturePoolIndexes.Add( index );
         }
      }
   }
}
