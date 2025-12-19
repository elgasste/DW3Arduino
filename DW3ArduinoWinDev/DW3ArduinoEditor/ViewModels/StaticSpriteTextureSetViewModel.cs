using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.ViewModels
{
   public class StaticSpriteTextureSetViewModel : ViewModelBase
   {
      private uint _index;
      public uint Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      public ObservableCollection<int> StaticSpriteTexturePoolIndexes { get; private set; } = [];

      public StaticSpriteTextureSetViewModel( uint index )
      {
         _index = index;

         for ( int i = 0; i < Constants.StaticSpriteTextureSetSize; i++ )
         {
            StaticSpriteTexturePoolIndexes.Add( 0 );
         }
      }

      public StaticSpriteTextureSetViewModel( StaticSpriteTextureSetSaveData saveData )
      {
         _index = saveData.Index;

         if ( saveData.StaticSpriteTexturePoolIndexes.Count > Constants.StaticSpriteTextureSetSize )
         {
            throw new Exception( "Saved static sprite set is too large" );
         }

         foreach ( var index in saveData.StaticSpriteTexturePoolIndexes )
         {
            StaticSpriteTexturePoolIndexes.Add( index );
         }
      }
   }
}
