using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileMapViewModel : ViewModelBase
   {
      private int _index;
      public int Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      private string _name = string.Empty;
      public string Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      private int _tilesX;
      public int TilesX
      {
         get => _tilesX;
         set => SetProperty( ref _tilesX, value );
      }

      private int _tilesY;
      public int TilesY
      {
         get => _tilesY;
         set => SetProperty( ref _tilesY, value );
      }

      private bool _wraps;
      public bool Wraps
      {
         get => _wraps;
         set => SetProperty( ref _wraps, value );
      }

      public TileMapViewModel( int index, string name, int tilesX, int tilesY, bool wraps )
      {
         _index = index;
         _name = name;
         _tilesX = tilesX;
         _tilesY = tilesY;
         _wraps = wraps;
      }
   }
}
