using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;

namespace DW3ArduinoEditor.ViewModels
{
   public class TextureSetViewModel : ViewModelBase
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

      public ObservableCollection<int> TexturePoolIndexes { get; private set; } = [];

      public TextureSetViewModel( uint index, string name )
      {
         _index = index;
         _name = name;
      }

      public TextureSetViewModel( TextureSetSaveData saveData )
      {
         _index = saveData.Index;
         _name = saveData.Name;

         foreach ( var index in saveData.TexturePoolIndexes )
         {
            TexturePoolIndexes.Add( index );
         }
      }
   }
}
