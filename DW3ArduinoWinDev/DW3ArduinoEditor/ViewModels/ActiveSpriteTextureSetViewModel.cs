using System.Collections.ObjectModel;
using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class ActiveSpriteTextureSetViewModel : ViewModelBase
   {
      private uint _index;
      public uint Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      public ObservableCollection<int> ActiveSpriteTexturePoolIndexes { get; private set; } = [];

      public ActiveSpriteTextureSetViewModel( uint index )
      {
         _index = index;

         for ( int i = 0; i < Constants.ActiveSpriteTextureSetSize; i++ )
         {
            ActiveSpriteTexturePoolIndexes.Add( 0 );
         }
      }

      public ActiveSpriteTextureSetViewModel( ActiveSpriteTextureSetSaveData saveData )
      {
         _index = saveData.Index;

         if ( saveData.ActiveSpriteTexturePoolIndexes.Count > Constants.ActiveSpriteTextureSetSize )
         {
            throw new Exception( "Saved active sprite set is too large" );
         }

         foreach ( var index in saveData.ActiveSpriteTexturePoolIndexes )
         {
            ActiveSpriteTexturePoolIndexes.Add( index );
         }
      }
   }
}
