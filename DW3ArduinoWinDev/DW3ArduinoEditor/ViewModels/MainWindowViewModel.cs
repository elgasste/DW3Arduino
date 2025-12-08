using DW3ArduinoEditor.Commands;
using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;
using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Input;

namespace DW3ArduinoEditor.ViewModels
{
   public class MainWindowViewModel : ViewModelBase
   {
      public ObservableCollection<TileMapViewModel> TileMaps { get; } = [];

      private TileMapViewModel? _selectedTileMap;
      public TileMapViewModel? SelectedTileMap
      {
         get => _selectedTileMap;
         set => SetProperty( ref _selectedTileMap, value );
      }

      public MainWindowViewModel()
      {
         try
         {
            var contents = File.ReadAllText( Constants.SaveDataFilePath );
            GameSaveData? saveData = JsonSerializer.Deserialize<GameSaveData>( contents );

            if ( saveData is null )
            {
               MessageBox.Show( "Failed to load save data from file! Starting from scratch.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            }
            else
            {
               bool hasShownError = false;

               foreach ( var savedTileMap in saveData.TileMaps )
               {
                  if ( !hasShownError )
                  {
                     foreach ( var tileMap in TileMaps )
                     {
                        if ( tileMap.Index == savedTileMap.Index )
                        {
                           MessageBox.Show( "The tile map \"" + tileMap.Name + "\" has the same index (" + tileMap.Index.ToString() + ") as the tile map \"" + savedTileMap.Name + "\"! " +
                                            "This would indicate corrupted Editor save data, it is recommended to either start from scratch or close Editor and manually fix the save data.",
                                            "Error", MessageBoxButton.OK, MessageBoxImage.Error );
                           hasShownError = true;
                        }
                     }
                  }

                  TileMaps.Add( new( savedTileMap ) );
               }

               foreach ( var srcTileMap in TileMaps )
               {
                  foreach ( var portal in srcTileMap.Portals )
                  {
                     VerifyOrDeletePortal( portal, srcTileMap );
                  }

                  if ( srcTileMap.EdgePortal is not null )
                  {
                     VerifyOrDeletePortal( srcTileMap.EdgePortal, srcTileMap );
                  }
               }

               if ( TileMaps.Count > 0 )
               {
                  SelectedTileMap = TileMaps[0];
               }
            }
         }
         catch
         {
            MessageBox.Show( "Something went wrong when loading save data, file is possibly corrupt! Starting from scratch.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
         }

         // MUFFINS: I think we should keep this around for a while, it might come in handy for "resetting" the save data.
         //var random = new Random();

         //// "overworld"
         //TileMaps.Add( new( 0, "Overworld", 32, 32, true ) );
         //for ( int i = 0; i < TileMaps[0].Tiles.Count; i++ )
         //{
         //   if ( random.Next( 5 ) == 0 )
         //   {
         //      TileMaps[0].Tiles[i].TextureIndex = 2u;
         //      TileMaps[0].Tiles[i].IsPassable = false;
         //   }
         //   else
         //   {
         //      TileMaps[0].Tiles[i].TextureIndex = 1u;
         //      TileMaps[0].Tiles[i].IsPassable = true;
         //   }
         //}
         //TileMaps[0].Tiles[201].TextureIndex = 6u;
         //TileMaps[0].Portals.Add( new( 201, 1, 285 ) );
         //TileMaps[0].EdgePortal = null;

         //// "big room"
         //TileMaps.Add( new( 1, "Big Room", 26, 20, false ) );
         //for ( int i = 0; i < TileMaps[1].Tiles.Count; i++ ) // overall tiles
         //{
         //   TileMaps[1].Tiles[i].TextureIndex = 2u;
         //   TileMaps[1].Tiles[i].IsPassable = true;
         //}
         //for ( int i = 0; i < TileMaps[1].TilesX; i++ ) // top wall
         //{
         //   TileMaps[1].Tiles[i].TextureIndex = 3u;
         //   TileMaps[1].Tiles[i].IsPassable = false;
         //}
         //for ( int i = (int)( ( TileMaps[1].TilesX * TileMaps[1].TilesY ) - TileMaps[1].TilesX ); i < ( TileMaps[1].TilesX * TileMaps[1].TilesY ); i++ ) // bottom wall
         //{
         //   TileMaps[1].Tiles[i].TextureIndex = 3u;
         //   TileMaps[1].Tiles[i].IsPassable = false;
         //}
         //for ( int i = 1; i < TileMaps[1].TilesY - 1; i++ ) // left and right walls
         //{
         //   TileMaps[1].Tiles[i * (int)TileMaps[1].TilesX].TextureIndex = 3u;
         //   TileMaps[1].Tiles[i * (int)TileMaps[1].TilesX].IsPassable = false;
         //}
         //TileMaps[1].Tiles[260].TextureIndex = 2u; // portal to hallway
         //TileMaps[1].Tiles[260].IsPassable = true;
         //TileMaps[1].Portals.Add( new( 260, 3, 39 ) );
         //TileMaps[1].Tiles[377].TextureIndex = 1u; // portal to small room
         //TileMaps[1].Portals.Add( new( 377, 2, 24 ) );
         //TileMaps[1].Tiles[285].TextureIndex = 2u; // right edge opening
         //TileMaps[1].Tiles[285].IsPassable = true;
         //TileMaps[1].EdgePortal = new( 0, 0, 201 );

         //// "small room"
         //TileMaps.Add( new( 2, "Small Room", 10, 10, false ) );
         //for ( int i = 0; i < TileMaps[2].Tiles.Count; i++ ) // overall tiles
         //{
         //   TileMaps[2].Tiles[i].TextureIndex = 2u;
         //   TileMaps[2].Tiles[i].IsPassable = true;
         //}
         //for ( int i = 0; i < TileMaps[2].TilesX; i++ ) // top wall
         //{
         //   TileMaps[2].Tiles[i].TextureIndex = 3u;
         //   TileMaps[2].Tiles[i].IsPassable = false;
         //}
         //for ( int i = (int)( ( TileMaps[2].TilesX * TileMaps[2].TilesY ) - TileMaps[2].TilesX ); i < ( TileMaps[2].TilesX * TileMaps[2].TilesY ); i++ ) // bottom wall
         //{
         //   TileMaps[2].Tiles[i].TextureIndex = 3u;
         //   TileMaps[2].Tiles[i].IsPassable = false;
         //}
         //for ( int i = 1; i < TileMaps[2].TilesY - 1; i++ ) // left and right walls
         //{
         //   TileMaps[2].Tiles[i * (int)TileMaps[2].TilesX].TextureIndex = 3u;
         //   TileMaps[2].Tiles[i * (int)TileMaps[2].TilesX].IsPassable = false;
         //   TileMaps[2].Tiles[( i * (int)TileMaps[2].TilesX ) + (int)( TileMaps[2].TilesX - 1 )].TextureIndex = 3u;
         //   TileMaps[2].Tiles[( i * (int)TileMaps[2].TilesX ) + (int)( TileMaps[2].TilesX - 1 )].IsPassable = false;
         //}
         //TileMaps[2].Tiles[69].TextureIndex = 2u; // portal to hallway
         //TileMaps[2].Tiles[69].IsPassable = true;
         //TileMaps[2].Portals.Add( new( 69, 3, 20 ) );
         //TileMaps[2].Tiles[24].TextureIndex = 1u; // portal to big room
         //TileMaps[2].Tiles[24].IsPassable = true;
         //TileMaps[2].Portals.Add( new( 24, 1, 377 ) );

         //// "hallway"
         //TileMaps.Add( new( 3, "Hallway", 20, 3, false ) );
         //for ( int i = 0; i < TileMaps[3].TilesX; i++ )
         //{
         //   TileMaps[3].Tiles[i].TextureIndex = 3u;
         //   TileMaps[3].Tiles[i].IsPassable = false;

         //   TileMaps[3].Tiles[i + (int)TileMaps[3].TilesX].TextureIndex = 2u;
         //   TileMaps[3].Tiles[i + (int)TileMaps[3].TilesX].IsPassable = true;

         //   TileMaps[3].Tiles[i + ( (int)TileMaps[3].TilesX * 2 )].TextureIndex = 3u;
         //   TileMaps[3].Tiles[i + ( (int)TileMaps[3].TilesX * 2 )].IsPassable = false;
         //}
         //TileMaps[3].Portals.Add( new( 20, 2, 69 ) ); // portal to small room
         //TileMaps[3].Portals.Add( new( 39, 1, 260 ) ); // portal to big room
      }

      private void VerifyOrDeletePortal( PortalViewModel portal, TileMapViewModel srcTileMap )
      {
         if ( portal.SourceTileIndex >= ( srcTileMap.TilesX * srcTileMap.TilesY ) )
         {
            MessageBox.Show( "The tile map \"" + srcTileMap.Name + "\" contains an unreachable portal, it will be removed.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            srcTileMap.Portals.Remove( portal );
         }
         else
         {
            bool foundDest = false;

            foreach ( var destTileMap in TileMaps )
            {
               if ( destTileMap.Index == portal.DestTileMapIndex )
               {
                  foundDest = true;

                  if ( portal.DestTileIndex >= ( destTileMap.TilesX * destTileMap.TilesY ) )
                  {
                     MessageBox.Show( "The tile map \"" + srcTileMap.Name + "\" contains an unreachable portal destination tile, it will be removed.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
                     srcTileMap.Portals.Remove( portal );
                  }
               }
            }

            if ( !foundDest )
            {
               MessageBox.Show( "The tile map \"" + srcTileMap.Name + "\" contains an unreachable portal destination, it will be removed.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
               srcTileMap.Portals.Remove( portal );
            }
         }
      }

      private void RenameSelectedTileMap()
      {
         if ( SelectedTileMap is not null )
         {
            var window = new RenameTileMapWindow( SelectedTileMap.Name );
            var result = window.ShowDialog();

            if ( result.HasValue && result.Value )
            {
               SelectedTileMap.Name = window.NewTileMapName;
            }
         }
      }

      private void AddNewTileMap()
      {
         var window = new AddNewTileMapWindow();
         var result = window.ShowDialog();

         if ( result.HasValue && result.Value )
         {
            uint index = ( TileMaps.Count > 0 ) ? TileMaps[^1].Index + 1 : 0;
            var newTileMap = new TileMapViewModel( index, window.NewTileMapName, window.NewTilesX, window.NewTilesY, window.NewWraps );
            TileMaps.Add( newTileMap );
            SelectedTileMap = newTileMap;
         }
      }

      private void DeleteSelectedTileMap()
      {
         if ( MessageBox.Show( "Are you sure you want to delete this tile map?", "Please Confirm", MessageBoxButton.YesNo ) == MessageBoxResult.Yes )
         {
            int index;

            for ( index = 0; index < TileMaps.Count; index++ )
            {
               if ( TileMaps[index] == SelectedTileMap )
               {
                  break;
               }
            }

            if ( TileMaps.Count == 1 )
            {
               SelectedTileMap = null;
            }
            else
            {
               SelectedTileMap = ( index > 0 ) ? TileMaps[index - 1] : TileMaps[0];
            }

            foreach ( var tileMap in TileMaps )
            {
               foreach ( var portal in tileMap.Portals )
               {
                  if ( portal.DestTileMapIndex == tileMap.Index )
                  {
                     tileMap.Portals.Remove( portal );
                  }
               }
            }

            TileMaps.RemoveAt( index );
         }
      }

      private void ResizeSelectedTileMap()
      {
         if ( SelectedTileMap is not null )
         {
            var window = new ResizeTileMapWindow( SelectedTileMap.TilesX, SelectedTileMap.TilesY );
            var result = window.ShowDialog();

            if ( result.HasValue && result.Value )
            {
               SelectedTileMap.TilesX = window.NewTilesX;
               SelectedTileMap.TilesY = window.NewTilesY;
            }
         }
      }

      private void WriteSaveData()
      {
         var saveData = new GameSaveData( TileMaps );
         File.WriteAllText( Constants.SaveDataFilePath, JsonSerializer.Serialize( saveData ) );
         MessageBox.Show( "Editor data has been saved." );
      }

      private void WriteGameDataSource()
      {
         var generator = new GameDataGenerator();
         generator.WriteGameDataSourceFile( new( TileMaps ) );
         MessageBox.Show( "Game data source file has been written." );
      }

      private ICommand? _renameSelectedTileMapCommand;
      public ICommand? RenameSelectedTileMapCommand => _renameSelectedTileMapCommand ??= new RelayCommand( RenameSelectedTileMap, () => true );

      private ICommand? _addNewTileMapCommand;
      public ICommand? AddNewTileMapCommand => _addNewTileMapCommand ??= new RelayCommand( AddNewTileMap, () => true );

      private ICommand? _deleteSelectedTileMapCommand;
      public ICommand? DeleteSelectedTileMapCommand => _deleteSelectedTileMapCommand ??= new RelayCommand( DeleteSelectedTileMap, () => TileMaps.Count > 0 );

      private ICommand? _resizeSelectedTileMapCommand;
      public ICommand? ResizeSelectedTileMapCommand => _resizeSelectedTileMapCommand ??= new RelayCommand( ResizeSelectedTileMap, () => TileMaps.Count > 0 );

      private ICommand? _saveGameDataCommand;
      public ICommand? SaveGameDataCommand => _saveGameDataCommand ??= new RelayCommand( WriteSaveData, () => true );

      private ICommand? _writeGameDataSourceCommand;
      public ICommand? WriteGameDataSourceCommand => _writeGameDataSourceCommand ??= new RelayCommand( WriteGameDataSource, () => true );
   }
}
