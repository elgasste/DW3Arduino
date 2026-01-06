using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileTextureSetViewModel : ViewModelBase
   {
      private uint _index;
      public uint Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      private string _name;
      public string Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      public ObservableCollection<int> TileTexturePoolIndexes { get; private set; } = [];

      public TileTextureSetViewModel( uint index, string name )
      {
         _index = index;
         _name = name;

         for ( int i = 0; i < Constants.TileTextureSetSize; i++ )
         {
            TileTexturePoolIndexes.Add( 0 );
         }
      }

      public TileTextureSetViewModel( TileTextureSetSaveData saveData )
      {
         _index = saveData.Index;
         _name = saveData.Name;

         if ( saveData.TileTexturePoolIndexes.Count > Constants.TileTextureSetSize )
         {
            throw new Exception( "Saved tile set is too large" );
         }

         foreach ( var index in saveData.TileTexturePoolIndexes )
         {
            TileTexturePoolIndexes.Add( index );
         }
      }
   }
}
