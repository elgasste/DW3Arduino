using DW3ArduinoEditor.Commands;
using DW3ArduinoEditor.Graphics;
using DW3ArduinoEditor.SaveData;
using DW3ArduinoEditor.Views;
using System.Collections.ObjectModel;
using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Input;

namespace DW3ArduinoEditor.ViewModels
{
   public class MainWindowViewModel : ViewModelBase
   {
      private GameStartupViewModel _gameStartup = new();
      private HeaderGuidsViewModel _headerGuids = new();
      private Palette _palette = new();
      private TileTexturePool? _tileTexturePool;
      private StaticSpriteTexturePool? _staticSpriteTexturePool;
      private ActiveSpriteTexturePool? _activeSpriteTexturePool;
      private ActiveSpriteTexturePool? _playerSpriteTexturePool;

      public ObservableCollection<TileMapViewModel> TileMaps { get; } = [];
      public ObservableCollection<TextureSetViewModel> TileTextureSets { get; } = [];
      public ObservableCollection<TextureSetViewModel> StaticSpriteTextureSets { get; } = [];
      public ObservableCollection<TextureSetViewModel> ActiveSpriteTextureSets { get; } = [];

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
            _tileTexturePool = new( Constants.TileTexturePoolImagePath, _palette );
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load tile map texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

         try
         {
            _staticSpriteTexturePool = new( Constants.StaticSpriteTexturePoolImagePath, _palette );
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load static sprite texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

         try
         {
            _activeSpriteTexturePool = new( Constants.ActiveSpriteTexturePoolImagePath, _palette );
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load active sprite texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

         try
         {
            _playerSpriteTexturePool = new( Constants.PlayerSpriteTexturePoolImagePath, _palette );
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load player sprite texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

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
               GameSaveDataSanityChecker.CheckSanity( saveData,
                                                      _tileTexturePool is null ? 0 : _tileTexturePool.TilePaletteIndexes.Count,
                                                      _staticSpriteTexturePool is null ? 0 : _staticSpriteTexturePool.StaticSpritePaletteIndexes.Count,
                                                      _activeSpriteTexturePool is null ? 0 : _activeSpriteTexturePool.ActiveSpritePaletteIndexes.Count,
                                                      _playerSpriteTexturePool is null ? 0 : _playerSpriteTexturePool.ActiveSpritePaletteIndexes.Count );

               _gameStartup = new( saveData.GameStartup );
               _headerGuids = new( saveData.HeaderGuids );

               foreach ( var savedTileMap in saveData.TileMaps )
               {
                  TileMaps.Add( new( savedTileMap ) );
               }

               foreach ( var set in saveData.TileTextureSets )
               {
                  TileTextureSets.Add( new( set  ) );
               }

               foreach ( var set in saveData.StaticSpriteTextureSets )
               {
                  StaticSpriteTextureSets.Add( new( set ) );
               }

               foreach ( var set in saveData.ActiveSpriteTextureSets )
               {
                  ActiveSpriteTextureSets.Add( new( set ) );
               }

               if ( TileMaps.Count > 0 )
               {
                  SelectedTileMap = TileMaps[0];
               }
            }
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Something went wrong when loading save data, file is possibly corrupt: \"{0}\". Starting from scratch.", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );

            _headerGuids = new();
            TileMaps = [];
            TileTextureSets = [];
            StaticSpriteTextureSets = [];
            ActiveSpriteTextureSets = [];
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
            var newTileMap = new TileMapViewModel( index,
                                                   window.NewTileMapName,
                                                   window.NewTilesX,
                                                   window.NewTilesY,
                                                   window.NewWraps,
                                                   window.NewAffectsDaylight,
                                                   window.NewIsUnderground,
                                                   window.NewHasEncounters,
                                                   TileTextureSets[0].Index,
                                                   StaticSpriteTextureSets[0].Index,
                                                   ActiveSpriteTextureSets[0].Index );
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
               uint oldTilesX = SelectedTileMap.TilesX;
               uint oldTilesY = SelectedTileMap.TilesY;

               SelectedTileMap.TilesX = window.NewTilesX;
               SelectedTileMap.TilesY = window.NewTilesY;

               // update any portals with indexes matching the selected tile map
               foreach ( var tileMap in TileMaps )
               {
                  if ( tileMap != SelectedTileMap )
                  {
                     for ( int i = 0; i < tileMap.Portals.Count; i++ )
                     {
                        if ( tileMap.Portals[i].DestTileMapIndex == SelectedTileMap.Index )
                        {
                           uint row = tileMap.Portals[i].DestTileIndex / oldTilesX;
                           uint col = tileMap.Portals[i].DestTileIndex % oldTilesX;

                           if ( row >= SelectedTileMap.TilesY || col >= SelectedTileMap.TilesX )
                           {
                              // the section of the map with this portal's destination is now gone
                              tileMap.Portals.Remove( tileMap.Portals[i] );
                              i--;
                           }
                           else if ( oldTilesX != SelectedTileMap.TilesX )
                           {
                              tileMap.Portals[i].DestTileIndex = ( row * SelectedTileMap.TilesX ) + col;
                           }
                        }
                     }
                  }
               }

               // update the player's starting tile index if necessary
               if ( SelectedTileMap.Index == _gameStartup.PlayerStartTileMapIndex )
               {
                  uint row = _gameStartup.PlayerStartTileIndex / oldTilesX;
                  uint col = _gameStartup.PlayerStartTileIndex % oldTilesX;

                  if ( row >= SelectedTileMap.TilesY || col >= SelectedTileMap.TilesX )
                  {
                     _gameStartup.PlayerStartTileIndex = 0;
                     MessageBox.Show( "The player's starting tile index no longer exists, resetting to zero.", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning );
                  }
                  else if ( oldTilesX != SelectedTileMap.TilesX )
                  {
                     _gameStartup.PlayerStartTileIndex = ( row * SelectedTileMap.TilesX ) + col;
                  }
               }
            }
         }
      }

      private void WriteSaveData()
      {
         var saveData = new GameSaveData( _gameStartup, _headerGuids, TileMaps, TileTextureSets, StaticSpriteTextureSets, ActiveSpriteTextureSets );
         File.WriteAllText( Constants.SaveDataFilePath, JsonSerializer.Serialize( saveData ) );
         MessageBox.Show( "Editor data has been saved." );
      }

      private void WriteGameDataSource()
      {
         var generator = new GameDataGenerator();
         generator.WriteGameDataSourceFile( new( _gameStartup, _headerGuids, TileMaps, TileTextureSets, StaticSpriteTextureSets, ActiveSpriteTextureSets ), _palette, _tileTexturePool, _staticSpriteTexturePool, _activeSpriteTexturePool, _playerSpriteTexturePool );
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
