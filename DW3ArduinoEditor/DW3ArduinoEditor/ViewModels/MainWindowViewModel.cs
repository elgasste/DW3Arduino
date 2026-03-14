using DW3ArduinoEditor.Commands;
using DW3ArduinoEditor.Enums;
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
      private readonly GameStartupViewModel _gameStartup = new();
      private readonly HeaderGuidsViewModel _headerGuids = new();
      private readonly Palette _palette = new();
      public TileTexturePool TileTexturePool { get; private set; } = new();
      public StaticSpriteTexturePool StaticSpriteTexturePool { get; private set; } = new();
      private readonly ActiveSpriteTexturePool _activeSpriteTexturePool = new();
      private readonly ActiveSpriteTexturePool _playerSpriteTexturePool = new();

      public ObservableCollection<TileMapViewModel> TileMaps { get; } = [];
      public ObservableCollection<TextureSetViewModel> TileTextureSets { get; } = [];
      public ObservableCollection<TextureSetViewModel> StaticSpriteTextureSets { get; } = [];
      public ObservableCollection<TextureSetViewModel> ActiveSpriteTextureSets { get; } = [];

      public ObservableCollection<PlayerClassExpTableViewModel> PlayerClassExpTables { get; } = [];

      private TileMapViewModel? _selectedTileMap;
      public TileMapViewModel? SelectedTileMap
      {
         get => _selectedTileMap;
         set
         {
            if ( SetProperty( ref _selectedTileMap, value ) )
            {
               if ( _selectedTileMap is not null )
               {
                  SelectedTileTextureSet = TileTextureSets[(int)_selectedTileMap.TileTextureSetIndex];
               }
            }
         }
      }

      private TextureSetViewModel? _selectedTileTextureSet;
      public TextureSetViewModel? SelectedTileTextureSet
      {
         get => _selectedTileTextureSet;
         set
         {
            if ( SetProperty( ref _selectedTileTextureSet, value ) )
            {
               bool invalidTextureIndexFound = false;

               if ( SelectedTileMap is not null && SelectedTileTextureSet is not null )
               {
                  SelectedTileMap.TileTextureSetIndex = SelectedTileTextureSet.Index;

                  foreach ( var tile in SelectedTileMap.Tiles )
                  {
                     if ( tile.TextureIndex >= SelectedTileTextureSet.TexturePoolIndexes.Count )
                     {
                        tile.TextureIndex = 0;
                        invalidTextureIndexFound = true;
                     }
                  }
               }

               if ( invalidTextureIndexFound )
               {
                  MessageBox.Show( "The selected tile map contains tile texture indexes that are out of range, they will be reset to zero.", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning );
               }
            }
         }
      }

      private TextureSetViewModel? _selectedStaticSpriteTextureSet;
      public TextureSetViewModel? SelectedStaticSpriteTextureSet
      {
         get => _selectedStaticSpriteTextureSet;
         set
         {
            if ( SetProperty( ref _selectedStaticSpriteTextureSet, value ) )
            {
               bool invalidTextureIndexFound = false;

               if ( SelectedTileMap is not null && SelectedStaticSpriteTextureSet is not null )
               {
                  SelectedTileMap.StaticSpriteTextureSetIndex = SelectedStaticSpriteTextureSet.Index;

                  foreach ( var sprite in SelectedTileMap.StaticSprites )
                  {
                     if ( sprite.TextureIndex >= SelectedStaticSpriteTextureSet.TexturePoolIndexes.Count )
                     {
                        sprite.TextureIndex = 0;
                        invalidTextureIndexFound = true;
                     }
                  }
               }

               if ( invalidTextureIndexFound )
               {
                  MessageBox.Show( "The selected tile map contains static sprite texture indexes that are out of range, they will be reset to zero.", "Warning", MessageBoxButton.OK, MessageBoxImage.Warning );
               }
            }
         }
      }

      public IEnumerable<TileEditTool> TileEditToolValues
      {
         get
         {
            return Enum.GetValues( typeof( TileEditTool ) ).Cast<TileEditTool>();
         }
      }

      private TileEditTool _selectedTileEditTool = TileEditTool.ChangeTexture;
      public TileEditTool SelectedTileEditTool
      {
         get => _selectedTileEditTool;
         set => SetProperty( ref _selectedTileEditTool, value );
      }

      public MainWindowViewModel()
      {
         try
         {
            TileTexturePool = new( Constants.TileTexturePoolImagePath, _palette );
         }
         catch ( Exception ex )
         {
            MessageBox.Show( string.Format( "Failed to load tile map texture pool: {0}", ex.Message ), "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            Application.Current.Shutdown();
         }

         try
         {
            StaticSpriteTexturePool = new( Constants.StaticSpriteTexturePoolImagePath, _palette );
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
                                                      TileTexturePool.TilePaletteIndexes.Count,
                                                      StaticSpriteTexturePool.StaticSpritePaletteIndexes.Count,
                                                      _activeSpriteTexturePool.ActiveSpritePaletteIndexes.Count,
                                                      _playerSpriteTexturePool.ActiveSpritePaletteIndexes.Count );

               _gameStartup = new( saveData.GameStartup );
               _headerGuids = new( saveData.HeaderGuids );

               foreach ( var savedTileMap in saveData.TileMaps )
               {
                  TileMaps.Add( new( savedTileMap ) );
               }

               foreach ( var set in saveData.TileTextureSets )
               {
                  TileTextureSets.Add( new( TileTexturePool, set ) );
               }

               foreach ( var set in saveData.StaticSpriteTextureSets )
               {
                  StaticSpriteTextureSets.Add( new( StaticSpriteTexturePool, set ) );
               }

               foreach ( var set in saveData.ActiveSpriteTextureSets )
               {
                  ActiveSpriteTextureSets.Add( new( _activeSpriteTexturePool, set ) );
               }

               foreach ( var table in saveData.PlayerClassExpTables )
               {
                  PlayerClassExpTables.Add( new( table ) );
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

      // TODO: move this to a utility or something
      private void PadLeftOfTileMap( TileMapViewModel tileMap, uint count, uint newTextureIndex )
      {
         int rows = (int)( tileMap.TilesY );
         int cols = (int)( tileMap.TilesX );
         List<TileViewModel> tiles = [];

         for ( int i = 0, j = 0; i < ( rows * cols ); i++, j++ )
         {
            if ( i % cols == 0 )
            {
               for ( int k = 0; k < count; k++, j++ )
               {
                  tiles.Add( new() );
                  tiles[j].EncounterRate = 0;
                  tiles[j].DamageRate = 0;
                  tiles[j].IsPassable = true;
                  tiles[j].TextureIndex = newTextureIndex;
                  tiles[j].WalkSpeed = TileWalkSpeed.Normal;
               }
            }

            tiles.Add( new() );
            tiles[j].EncounterRate = tileMap.Tiles[i].EncounterRate;
            tiles[j].DamageRate = tileMap.Tiles[i].DamageRate;
            tiles[j].IsPassable = tileMap.Tiles[i].IsPassable;
            tiles[j].TextureIndex = tileMap.Tiles[i].TextureIndex;
            tiles[j].WalkSpeed = tileMap.Tiles[i].WalkSpeed;
         }

         // move portal destinations from other tile maps
         for ( int k = 0; k < TileMaps.Count; k++ )
         {
            foreach ( var portal in TileMaps[k].Portals )
            {
               if ( portal.DestTileMapIndex == tileMap.Index )
               {
                  uint row = portal.DestTileIndex / tileMap.TilesX;
                  uint prevCol = portal.DestTileIndex % tileMap.TilesX;
                  portal.DestTileIndex = ( row * ( tileMap.TilesX + count ) ) + ( prevCol + count );
               }
            }
         }

         tileMap.TilesX += count;

         // portals should all be shifted right (setting TilesX assumes we padded on the right)
         for ( int k = 0; k < tileMap.Portals.Count; k++ )
         {
            tileMap.Portals[k].SourceTileIndex += count;
         }

         // move static sprites as well (same rules apply)
         for ( int k = 0; k < tileMap.StaticSprites.Count; k++ )
         {
            tileMap.StaticSprites[k].TileIndex += count;
         }

         // copy new tiles over
         tileMap.Tiles.Clear();
         foreach ( var tile in tiles )
         {
            tileMap.Tiles.Add( tile );
         }
      }

      // TODO: move this to a utility or something
      private void PadRightOfTileMap( TileMapViewModel tileMap, uint count, uint newTextureIndex )
      {
         int rows = (int)( tileMap.TilesY );
         int cols = (int)( tileMap.TilesX );
         List<TileViewModel> tiles = [];

         for ( int i = 0, j = 0; i < ( rows * cols ); i++ )
         {
            tiles.Add( new() );
            tiles[j].EncounterRate = tileMap.Tiles[i].EncounterRate;
            tiles[j].DamageRate = tileMap.Tiles[i].DamageRate;
            tiles[j].IsPassable = tileMap.Tiles[i].IsPassable;
            tiles[j].TextureIndex = tileMap.Tiles[i].TextureIndex;
            tiles[j].WalkSpeed = tileMap.Tiles[i].WalkSpeed;
            j++;

            if ( ( i > 0 ) && ( ( i + 1 ) % cols == 0 ) )
            {
               for ( int k = 0; k < count; k++, j++ )
               {
                  tiles.Add( new() );
                  tiles[j].EncounterRate = 0;
                  tiles[j].DamageRate = 0;
                  tiles[j].IsPassable = true;
                  tiles[j].TextureIndex = newTextureIndex;
                  tiles[j].WalkSpeed = TileWalkSpeed.Normal;
               }
            }
         }

         // move portal destinations from other tile maps
         for ( int k = 0; k < TileMaps.Count; k++ )
         {
            foreach ( var portal in TileMaps[k].Portals )
            {
               if ( portal.DestTileMapIndex == tileMap.Index )
               {
                  uint row = portal.DestTileIndex / tileMap.TilesX;
                  uint prevCol = portal.DestTileIndex % tileMap.TilesX;
                  portal.DestTileIndex = ( row * ( tileMap.TilesX + count ) ) + prevCol;
               }
            }
         }

         // this will move portals and static sprites
         tileMap.TilesX += count;

         // copy new tiles over
         tileMap.Tiles.Clear();
         foreach ( var tile in tiles )
         {
            tileMap.Tiles.Add( tile );
         }
      }

      // TODO: move this to a utility or something
      private void PadTopOfTileMap( TileMapViewModel tileMap, uint count, uint newTextureIndex )
      {
         int rows = (int)( tileMap.TilesY );
         int cols = (int)( tileMap.TilesX );
         int tilesToPad = (int)( cols * count );
         List<TileViewModel> tiles = [];

         for ( int i = 0; i < ( cols * rows ) + tilesToPad; i++ )
         {
            tiles.Add( new() );

            if ( i < tilesToPad )
            {
               tiles[i].EncounterRate = 0;
               tiles[i].DamageRate = 0;
               tiles[i].IsPassable = true;
               tiles[i].TextureIndex = newTextureIndex;
               tiles[i].WalkSpeed = TileWalkSpeed.Normal;
            }
            else
            {
               tiles[i].EncounterRate = tileMap.Tiles[i - tilesToPad].EncounterRate;
               tiles[i].DamageRate = tileMap.Tiles[i - tilesToPad].DamageRate;
               tiles[i].IsPassable = tileMap.Tiles[i - tilesToPad].IsPassable;
               tiles[i].TextureIndex = tileMap.Tiles[i - tilesToPad].TextureIndex;
               tiles[i].WalkSpeed = tileMap.Tiles[i - tilesToPad].WalkSpeed;
            }
         }

         tileMap.TilesY += count;

         // move portals
         for ( int k = 0; k < tileMap.Portals.Count; k++ )
         {
            tileMap.Portals[k].SourceTileIndex += (uint)tilesToPad;
         }

         // move static sprites
         for ( int k = 0; k < tileMap.StaticSprites.Count; k++ )
         {
            tileMap.StaticSprites[k].TileIndex += (uint)tilesToPad;
         }

         // move portal destinations from other tile maps
         for ( int k = 0; k < TileMaps.Count; k++ )
         {
            foreach ( var portal in TileMaps[k].Portals )
            {
               if ( portal.DestTileMapIndex == tileMap.Index )
               {
                  portal.DestTileIndex += (uint)tilesToPad;
               }
            }
         }

         // copy new tiles over
         tileMap.Tiles.Clear();
         foreach ( var tile in tiles )
         {
            tileMap.Tiles.Add( tile );
         }
      }

      // TODO: move this to a utility or something
      private void PadBottomOfTileMap( TileMapViewModel tileMap, uint count, uint newTextureIndex )
      {
         int rows = (int)( tileMap.TilesY );
         int cols = (int)( tileMap.TilesX );
         int tilesToPad = (int)( cols * count );
         List<TileViewModel> tiles = [];

         for ( int i = 0; i < ( cols * rows ) + tilesToPad; i++ )
         {
            tiles.Add( new() );

            if ( i >= ( cols * rows ) )
            {
               tiles[i].EncounterRate = 0;
               tiles[i].DamageRate = 0;
               tiles[i].IsPassable = true;
               tiles[i].TextureIndex = newTextureIndex;
               tiles[i].WalkSpeed = TileWalkSpeed.Normal;
            }
            else
            {
               tiles[i].EncounterRate = tileMap.Tiles[i].EncounterRate;
               tiles[i].DamageRate = tileMap.Tiles[i].DamageRate;
               tiles[i].IsPassable = tileMap.Tiles[i].IsPassable;
               tiles[i].TextureIndex = tileMap.Tiles[i].TextureIndex;
               tiles[i].WalkSpeed = tileMap.Tiles[i].WalkSpeed;
            }
         }

         tileMap.TilesY += count;

         // copy new tiles over
         tileMap.Tiles.Clear();
         foreach ( var tile in tiles )
         {
            tileMap.Tiles.Add( tile );
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

            // TODO: if this is the tile map the player starts on, we need to change that data.
            // we also need to explore what happens if there are no tile maps after this.
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
         var saveData = new GameSaveData( _gameStartup, _headerGuids, TileMaps, TileTextureSets, StaticSpriteTextureSets, ActiveSpriteTextureSets, PlayerClassExpTables );
         File.WriteAllText( Constants.SaveDataFilePath, JsonSerializer.Serialize( saveData ) );
         MessageBox.Show( "Editor data has been saved." );
      }

      private void WriteGameDataSource()
      {
         var generator = new GameDataGenerator();
         generator.WriteGameDataSourceFile( new( _gameStartup, _headerGuids, TileMaps, TileTextureSets, StaticSpriteTextureSets, ActiveSpriteTextureSets, PlayerClassExpTables ), _palette, TileTexturePool, StaticSpriteTexturePool, _activeSpriteTexturePool, _playerSpriteTexturePool );
         MessageBox.Show( "Game data source file has been written." );
      }

      private void ChangeTileTextureSet()
      {
         // TODO: open a window to select
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

      private ICommand? _changeTileTextureSetCommand;
      public ICommand? ChangeTileTextureSetCommand => _changeTileTextureSetCommand ??= new RelayCommand( ChangeTileTextureSet, () => true );
   }
}
