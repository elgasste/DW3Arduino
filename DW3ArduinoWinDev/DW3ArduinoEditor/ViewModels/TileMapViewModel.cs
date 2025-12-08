using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;
using System.Windows;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileMapViewModel : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; private set; } = [];

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
         set
         {
            if ( value < _tilesX ) // reducing the horizontal size
            {
               ObservableCollection<TileViewModel> newTiles = [];

               for ( int i = 0; i < ( _tilesX * _tilesY ); i++ )
               {
                  for ( int j = 0; j < value; i++, j++ )
                  {
                     newTiles.Add( Tiles[i] );
                  }

                  i += ( ( _tilesX - value ) - 1 );
               }

               Tiles = newTiles;
            }
            else if ( value > _tilesX ) // expanding the horizontal size
            {
               ObservableCollection<TileViewModel> newTiles = [];

               for ( int i = 0; i < ( _tilesX * _tilesY ); i++ )
               {
                  for ( int j = 0; j < value; j++ )
                  {
                     if ( j < _tilesX )
                     {
                        newTiles.Add( Tiles[i] );
                        i++;
                     }
                     else
                     {
                        newTiles.Add( new() );
                     }
                  }

                  i--;
               }

               Tiles = newTiles;
            }

            SetProperty( ref _tilesX, value );
         }
      }

      private int _tilesY;
      public int TilesY
      {
         get => _tilesY;
         set
         {
            if ( value < _tilesY ) // reducing the vertical size
            {
               ObservableCollection<TileViewModel> newTiles = [];

               for ( int i = 0; i < ( _tilesX * value ); i++ )
               {
                  newTiles.Add( Tiles[i] );
               }

               Tiles = newTiles;
            }
            else if ( value > _tilesY ) // expanding the vertical size
            {
               ObservableCollection<TileViewModel> newTiles = [];
               int i = 0;

               for ( ; i < ( _tilesX * _tilesY ); i++ )
               {
                  newTiles.Add( Tiles[i] );
               }

               for ( ; i < ( _tilesX * value ); i++ )
               {
                  newTiles.Add( new() );
               }

               Tiles = newTiles;
            }

            SetProperty( ref _tilesY, value );
         }
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

         for ( int i = 0; i < _tilesX * _tilesY; i++ )
         {
            Tiles.Add( new() );
         }
      }

      public TileMapViewModel( TileMapSaveData saveData )
      {
         _index = saveData.Index;
         _name = saveData.Name;
         _tilesX = saveData.TilesX;
         _tilesY = saveData.TilesY;
         _wraps = saveData.Wraps;

         if ( string.IsNullOrEmpty( _name ) )
         {
            _name = "(unnamed)";
         }

         if ( _tilesX < Constants.TileMapMinTilesX || _tilesX > Constants.TileMapMaxTilesX ||
              _tilesY < Constants.TileMapMinTilesY || _tilesY > Constants.TileMapMaxTilesY )
         {
            MessageBox.Show( "The tile map \"" + _name + "\" contains and invalid number of horizontal and/or vertical tiles. " +
                             "The tiles for this map will be reset.", "Save Data Error", MessageBoxButton.OK, MessageBoxImage.Error );

            _tilesX = Constants.TileMapDefaultTilesX;
            _tilesY = Constants.TileMapDefaultTilesY;

            for ( int i = 0; i < _tilesX * _tilesY; i++ )
            {
               Tiles.Add( new() );
            }
         }
         else
         {
            if ( saveData.Tiles.Count != ( _tilesX * _tilesY ) )
            {
               MessageBox.Show( "The tile map \"" + _name + "\" contains an invalid number of tiles (expected " +
                                ( _tilesX * _tilesY ) + ", but found " + ( saveData.Tiles.Count ).ToString() + "). " +
                                "The tiles for this map will be reset.", "Save Data Error", MessageBoxButton.OK, MessageBoxImage.Error );

               for ( int i = 0; i < _tilesX * _tilesY; i++ )
               {
                  Tiles.Add( new() );
               }
            }
            else
            {
               foreach ( var tile in saveData.Tiles )
               {
                  Tiles.Add( new( tile ) );
               }
            }
         }
      }
   }
}
