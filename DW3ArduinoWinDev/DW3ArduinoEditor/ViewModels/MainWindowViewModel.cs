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
      private Palette _palette = new();
      private TileTexturePool? _tileTexturePool;
      private StaticSpriteTexturePool? _staticSpriteTexturePool;
      private ActiveSpriteTexturePool? _activeSpriteTexturePool;

      public ObservableCollection<TileMapViewModel> TileMaps { get; } = [];
      public ObservableCollection<TileTextureSetViewModel> TileTextureSets { get; } = [];
      public ObservableCollection<StaticSpriteTextureSetViewModel> StaticSpriteTextureSets { get; } = [];
      public ObservableCollection<ActiveSpriteTextureSetViewModel> ActiveSpriteTextureSets { get; } = [];
      public ActiveSpriteViewModel PlayerSprite { get; } = new();

      private TileMapViewModel? _selectedTileMap;
      public TileMapViewModel? SelectedTileMap
      {
         get => _selectedTileMap;
         set => SetProperty( ref _selectedTileMap, value );
      }

      public MainWindowViewModel()
      {
         // TODO: move all this stuff to some other function to do all the sanity checks
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

               // TODO: verify there are no more than 32 textures per set
               foreach ( var set in saveData.TileTextureSets )
               {
                  TileTextureSets.Add( new( set  ) );
               }

               // TODO: verify there are no more than 32 textures per set
               foreach ( var set in saveData.StaticSpriteTextureSets )
               {
                  StaticSpriteTextureSets.Add( new( set ) );
               }

               // TODO: verify there are no more than 16 textures per set
               foreach ( var set in saveData.ActiveSpriteTextureSets )
               {
                  ActiveSpriteTextureSets.Add( new( set ) );
               }
            }
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Something went wrong when loading save data, file is possibly corrupt: \"{0}\". Starting from scratch.", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
         }

         // TODO: check static/active sprite sets as well? or should this not be here?
         if ( TileTextureSets.Count == 0 )
         {
            TileTextureSets.Add( new( 0, "Default" ) );
         }

         try
         {
            _tileTexturePool = new( Constants.TileTexturePoolImagePath, _palette );

            // TODO: verify our tile texture sets contain valid pool indexes, and our
            // tile maps contain valid tile texture set indexes.
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load tile map texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

         try
         {
            _staticSpriteTexturePool = new( Constants.StaticSpriteTexturePoolImagePath, _palette );

            // TODO: verify our static sprite texture sets contain valid pool indexes, and our
            // tile maps contain valid static sprite texture set indexes.
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load static sprite texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

         try
         {
            _activeSpriteTexturePool = new( Constants.ActiveSpriteTexturePoolImagePath, _palette );

            // TODO: verify our active sprite texture sets contain valid pool indexes, and our
            // tile maps contain valid active sprite texture set indexes.

            // TODO: we'll need to load a player sprite pool in here as well, when we have more than one player
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load active sprite texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }
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
                                                   ActiveSpriteTextureSets[0].Index,
                                                   PlayerSprite );
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
         var saveData = new GameSaveData( TileMaps, TileTextureSets, StaticSpriteTextureSets, ActiveSpriteTextureSets );
         File.WriteAllText( Constants.SaveDataFilePath, JsonSerializer.Serialize( saveData ) );
         MessageBox.Show( "Editor data has been saved." );
      }

      private void WriteGameDataSource()
      {
         var generator = new GameDataGenerator();
         generator.WriteGameDataSourceFile( new( TileMaps, TileTextureSets, StaticSpriteTextureSets, ActiveSpriteTextureSets ), _palette, _tileTexturePool, _staticSpriteTexturePool, _activeSpriteTexturePool );
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
