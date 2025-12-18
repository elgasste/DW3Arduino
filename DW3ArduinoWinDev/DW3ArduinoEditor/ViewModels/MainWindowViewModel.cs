using DW3ArduinoEditor.Commands;
using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.Graphics;
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
      private Palette _palette = new();
      private TileTexturePool? _tileTexturePool;

      public ObservableCollection<TileMapViewModel> TileMaps { get; } = [];
      public ObservableCollection<TileTextureSetViewModel> TileTextureSets { get; } = [];

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

               // TODO: verify there are no more than 32 textures per set
               foreach ( var set in saveData.TileTextureSets )
               {
                  TileTextureSets.Add( new( set  ) );
               }
            }
         }
         catch
         {
            MessageBox.Show( "Something went wrong when loading save data, file is possibly corrupt! Starting from scratch.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
         }

         if ( TileTextureSets.Count == 0 )
         {
            TileTextureSets.Add( new( 0 ) );
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
      }

      // TODO: this is essentially the same as the temp Overworld loading code, just without the
      // shoreline detection. let's keep this around for now too, just in case.
      //private void LoadAliahanFromTempFile()
      //{
      //   uint w = 34;
      //   uint h = 44;
      //   string fileName = "najima_1f_main.txt";
      //   string mapName = "Najima 1F Main";
      //   uint mapIndex = 22;
      //   uint tileSetIndex = 2;
      //   int defaultTexture = 10;
      //   List<uint> impassableTextureIndexes = [0, 1, 2, 4, 5, 7, 8, 11, 12, 13, 14, 15, 16, 20, 21, 22, 23];

      //   List<List<int>> textureIndexes = [];
      //   int lineIndex = 0;

      //   var lines = File.ReadLines( Constants.DocsBasePath + fileName );

      //   foreach ( var line in lines )
      //   {
      //      if ( lineIndex == h )
      //      {
      //         MessageBox.Show( "Too many lines!" );
      //         Application.Current.Shutdown();
      //      }
      //      else if ( line.Length != w )
      //      {
      //         MessageBox.Show( "Line is too long!" );
      //         Application.Current.Shutdown();
      //      }

      //      textureIndexes.Add( [] );

      //      for ( int i = 0; i < line.Length; i++ )
      //      {
      //         switch ( line[i] )
      //         {
      //            case '0': textureIndexes[lineIndex].Add( 0 ); break;
      //            case '1': textureIndexes[lineIndex].Add( 1 ); break;
      //            case '2': textureIndexes[lineIndex].Add( 2 ); break;
      //            case '3': textureIndexes[lineIndex].Add( 3 ); break;
      //            case '4': textureIndexes[lineIndex].Add( 4 ); break;
      //            case '5': textureIndexes[lineIndex].Add( 5 ); break;
      //            case '6': textureIndexes[lineIndex].Add( 6 ); break;
      //            case '7': textureIndexes[lineIndex].Add( 7 ); break;
      //            case '8': textureIndexes[lineIndex].Add( 8 ); break;
      //            case '9': textureIndexes[lineIndex].Add( 9 ); break;
      //            case 'a': textureIndexes[lineIndex].Add( 10 ); break;
      //            case 'b': textureIndexes[lineIndex].Add( 11 ); break;
      //            case 'c': textureIndexes[lineIndex].Add( 12 ); break;
      //            case 'd': textureIndexes[lineIndex].Add( 13 ); break;
      //            case 'e': textureIndexes[lineIndex].Add( 14 ); break;
      //            case 'f': textureIndexes[lineIndex].Add( 15 ); break;
      //            case 'g': textureIndexes[lineIndex].Add( 16 ); break;
      //            case 'h': textureIndexes[lineIndex].Add( 17 ); break;
      //            case 'i': textureIndexes[lineIndex].Add( 18 ); break;
      //            case 'j': textureIndexes[lineIndex].Add( 19 ); break;
      //            case 'k': textureIndexes[lineIndex].Add( 20 ); break;
      //            case 'l': textureIndexes[lineIndex].Add( 21 ); break;
      //            case 'm': textureIndexes[lineIndex].Add( 22 ); break;
      //            case 'n': textureIndexes[lineIndex].Add( 23 ); break;
      //            case 'o': textureIndexes[lineIndex].Add( 24 ); break;
      //            case 'p': textureIndexes[lineIndex].Add( 25 ); break;
      //            case 'q': textureIndexes[lineIndex].Add( 26 ); break;
      //            case 'r': textureIndexes[lineIndex].Add( 27 ); break;
      //            case 's': textureIndexes[lineIndex].Add( 28 ); break;
      //            case 't': textureIndexes[lineIndex].Add( 29 ); break;
      //            case 'u': textureIndexes[lineIndex].Add( 30 ); break;
      //            case 'v': textureIndexes[lineIndex].Add( 31 ); break;

      //            default: textureIndexes[lineIndex].Add( defaultTexture ); break;
      //         }
      //      }

      //      lineIndex++;
      //   }

      //   if ( textureIndexes.Count != h )
      //   {
      //      MessageBox.Show( "Not enough lines!" );
      //      Application.Current.Shutdown();
      //   }

      //   var newTileMap = new TileMapViewModel( mapIndex, mapName, w, h, false, false, tileSetIndex );
      //   TileMaps.Add( newTileMap );
      //   SelectedTileMap = newTileMap;

      //   int tileIndex = 0;

      //   for ( int i = 0; i < textureIndexes.Count; i++ )
      //   {
      //      for ( int j = 0; j < textureIndexes[i].Count; j++, tileIndex++ )
      //      {
      //         newTileMap.Tiles[tileIndex].TextureIndex = (uint)textureIndexes[i][j];

      //         if ( impassableTextureIndexes.Contains( newTileMap.Tiles[tileIndex].TextureIndex ) )
      //         {
      //            newTileMap.Tiles[tileIndex].IsPassable = false;
      //         }
      //      }
      //   }
      //}

      // TODO: this is how the overworld was originally loaded into here, we'll keep it around
      // for now, in case we want to manually build any other maps
      //private void LoadOverworldFromTempFile()
      //{
      //   List<List<int>> textureIndexes = [];
      //   List<List<int>> shoreCheckTextureIndexes = [];
      //   int lineIndex = 0;

      //   var lines = File.ReadLines( Constants.AssetsBasePath + "map\\temp_overworld_codes.txt" );

      //   foreach ( var line in lines )
      //   {
      //      if ( lineIndex == 256 )
      //      {
      //         MessageBox.Show( "Too many lines!" );
      //         Application.Current.Shutdown();
      //      }
      //      else if ( line.Length != 256 )
      //      {
      //         MessageBox.Show( "Line is not 256 chars!" );
      //         Application.Current.Shutdown();
      //      }

      //      textureIndexes.Add( [] );
      //      shoreCheckTextureIndexes.Add( [] );

      //      for ( int i = 0; i < line.Length; i++ )
      //      {
      //         switch ( line[i] )
      //         {
      //            case '0': textureIndexes[lineIndex].Add( 0 ); shoreCheckTextureIndexes[lineIndex].Add( 0 ); break;
      //            case '1': textureIndexes[lineIndex].Add( 1 ); shoreCheckTextureIndexes[lineIndex].Add( 1 ); break;
      //            case '2': textureIndexes[lineIndex].Add( 2 ); shoreCheckTextureIndexes[lineIndex].Add( 2 ); break;
      //            case '3': textureIndexes[lineIndex].Add( 3 ); shoreCheckTextureIndexes[lineIndex].Add( 3 ); break;
      //            case '4': textureIndexes[lineIndex].Add( 4 ); shoreCheckTextureIndexes[lineIndex].Add( 4 ); break;
      //            case '5': textureIndexes[lineIndex].Add( 5 ); shoreCheckTextureIndexes[lineIndex].Add( 5 ); break;
      //            case '6': textureIndexes[lineIndex].Add( 6 ); shoreCheckTextureIndexes[lineIndex].Add( 6 ); break;
      //            case '7': textureIndexes[lineIndex].Add( 7 ); shoreCheckTextureIndexes[lineIndex].Add( 7 ); break;
      //            case '8': textureIndexes[lineIndex].Add( 8 ); shoreCheckTextureIndexes[lineIndex].Add( 8 ); break;
      //            case ' ': textureIndexes[lineIndex].Add( 9 ); shoreCheckTextureIndexes[lineIndex].Add( 9 ); break;
      //            case '9': textureIndexes[lineIndex].Add( 10 ); shoreCheckTextureIndexes[lineIndex].Add( 10 ); break;
      //            case 'a': textureIndexes[lineIndex].Add( 11 ); shoreCheckTextureIndexes[lineIndex].Add( 11 ); break;
      //            case 'b': textureIndexes[lineIndex].Add( 12 ); shoreCheckTextureIndexes[lineIndex].Add( 12 ); break;
      //            case 'c': textureIndexes[lineIndex].Add( 13 ); shoreCheckTextureIndexes[lineIndex].Add( 13 ); break;
      //            case 'd': textureIndexes[lineIndex].Add( 14 ); shoreCheckTextureIndexes[lineIndex].Add( 14 ); break;
      //            case 'e': textureIndexes[lineIndex].Add( 15 ); shoreCheckTextureIndexes[lineIndex].Add( 15 ); break;
      //            case 'f': textureIndexes[lineIndex].Add( 16 ); shoreCheckTextureIndexes[lineIndex].Add( 16 ); break;
      //            case 'g': textureIndexes[lineIndex].Add( 17 ); shoreCheckTextureIndexes[lineIndex].Add( 17 ); break;
      //            case 'h': textureIndexes[lineIndex].Add( 18 ); shoreCheckTextureIndexes[lineIndex].Add( 18 ); break;
      //            case 'i': textureIndexes[lineIndex].Add( 19 ); shoreCheckTextureIndexes[lineIndex].Add( 19 ); break;
      //            case 'j': textureIndexes[lineIndex].Add( 20 ); shoreCheckTextureIndexes[lineIndex].Add( 20 ); break;
      //            case 'k': textureIndexes[lineIndex].Add( 21 ); shoreCheckTextureIndexes[lineIndex].Add( 21 ); break;
      //            case 'l': textureIndexes[lineIndex].Add( 22 ); shoreCheckTextureIndexes[lineIndex].Add( 22 ); break;
      //            case 'm': textureIndexes[lineIndex].Add( 23 ); shoreCheckTextureIndexes[lineIndex].Add( 23 ); break;
      //            case 'n': textureIndexes[lineIndex].Add( 24 ); shoreCheckTextureIndexes[lineIndex].Add( 24 ); break;
      //            case 'o': textureIndexes[lineIndex].Add( 25 ); shoreCheckTextureIndexes[lineIndex].Add( 25 ); break;
      //            case 'p': textureIndexes[lineIndex].Add( 26 ); shoreCheckTextureIndexes[lineIndex].Add( 26 ); break;
      //            case 'q': textureIndexes[lineIndex].Add( 27 ); shoreCheckTextureIndexes[lineIndex].Add( 27 ); break;

      //            default: textureIndexes[lineIndex].Add( 9 ); shoreCheckTextureIndexes[lineIndex].Add( 9 ); break;
      //         }
      //      }

      //      lineIndex++;
      //   }

      //   if ( textureIndexes.Count != 256 )
      //   {
      //      MessageBox.Show( "Not enough lines!" );
      //      Application.Current.Shutdown();
      //   }

      //   List<int> waterTiles = [9, 10, 11, 12];

      //   for ( int i = 0; i < shoreCheckTextureIndexes.Count; i++ )
      //   {
      //      for ( int j = 0; j <  shoreCheckTextureIndexes[i].Count; j++ )
      //      {
      //         bool leftShore = false, topShore = false, rightShore = false, bottomShore = false;

      //         if ( shoreCheckTextureIndexes[i][j] == 9 ) // water tile
      //         {
      //            if ( ( j > 0 ) && !waterTiles.Contains( shoreCheckTextureIndexes[i][j - 1] ) )
      //            {
      //               leftShore = true;
      //            }
      //            if ( ( j < 255 ) && !waterTiles.Contains( shoreCheckTextureIndexes[i][j + 1] ) )
      //            {
      //               rightShore = true;
      //            }
      //            if ( ( i > 0 ) && !waterTiles.Contains( shoreCheckTextureIndexes[i - 1][j] ) )
      //            {
      //               topShore = true;
      //            }
      //            if ( ( i < 255 ) && !waterTiles.Contains( shoreCheckTextureIndexes[i + 1 ][j] ) )
      //            {
      //               bottomShore = true;
      //            }

      //            if ( leftShore && !topShore && !rightShore && !bottomShore )
      //            {
      //               textureIndexes[i][j] = 13;
      //            }
      //            else if ( topShore && !leftShore && !rightShore && !bottomShore )
      //            {
      //               textureIndexes[i][j] = 14;
      //            }
      //            else if ( rightShore && !leftShore && !topShore && !bottomShore )
      //            {
      //               textureIndexes[i][j] = 15;
      //            }
      //            else if ( bottomShore && !leftShore && !topShore && !rightShore )
      //            {
      //               textureIndexes[i][j] = 16;
      //            }
      //            else if ( leftShore && topShore && !rightShore && !bottomShore )
      //            {
      //               textureIndexes[i][j] = 17;
      //            }
      //            else if ( topShore && rightShore && !bottomShore && !leftShore )
      //            {
      //               textureIndexes[i][j] = 18;
      //            }
      //            else if ( rightShore && bottomShore && !leftShore && !topShore )
      //            {
      //               textureIndexes[i][j] = 19;
      //            }
      //            else if ( bottomShore && leftShore && !topShore && !rightShore )
      //            {
      //               textureIndexes[i][j] = 20;
      //            }
      //            else if ( leftShore && rightShore && !topShore && !bottomShore )
      //            {
      //               textureIndexes[i][j] = 21;
      //            }
      //            else if ( topShore && bottomShore && !leftShore && !rightShore )
      //            {
      //               textureIndexes[i][j] = 22;
      //            }
      //            else if ( bottomShore && leftShore && topShore && !rightShore )
      //            {
      //               textureIndexes[i][j] = 23;
      //            }
      //            else if ( leftShore && topShore && rightShore && !bottomShore )
      //            {
      //               textureIndexes[i][j] = 24;
      //            }
      //            else if ( topShore && rightShore && bottomShore && !leftShore )
      //            {
      //               textureIndexes[i][j] = 25;
      //            }
      //            else if ( rightShore && bottomShore && leftShore && !topShore )
      //            {
      //               textureIndexes[i][j] = 26;
      //            }
      //            else if ( leftShore && topShore && rightShore && bottomShore )
      //            {
      //               textureIndexes[i][j] = 27;
      //            }
      //         }
      //      }
      //   }

      //   var newTileMap = new TileMapViewModel( 0, "Overworld", 256, 256, true, true );
      //   TileMaps.Add( newTileMap );
      //   SelectedTileMap = newTileMap;

      //   for ( int i = 0; i < 28; i++ )
      //   {
      //      newTileMap.TileTexturePoolIndexes.Add( i );
      //   }

      //   int tileIndex = 0;
      //   List<uint> impassableTileIndexes = new() { 5, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

      //   for ( int i = 0; i < textureIndexes.Count; i++ )
      //   {
      //      for ( int j = 0; j < textureIndexes[i].Count; j++, tileIndex++ )
      //      {
      //         newTileMap.Tiles[tileIndex].TextureIndex = (uint)textureIndexes[i][j];

      //         if ( impassableTileIndexes.Contains( newTileMap.Tiles[tileIndex].TextureIndex ) )
      //         {
      //            newTileMap.Tiles[tileIndex].IsPassable = false;
      //         }
      //      }
      //   }
      //}

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
            var newTileMap = new TileMapViewModel( index, window.NewTileMapName, window.NewTilesX, window.NewTilesY, window.NewWraps, window.NewAffectsDaylight, TileTextureSets[0].Index );
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
         var saveData = new GameSaveData( TileMaps, TileTextureSets );
         File.WriteAllText( Constants.SaveDataFilePath, JsonSerializer.Serialize( saveData ) );
         MessageBox.Show( "Editor data has been saved." );
      }

      private void WriteGameDataSource()
      {
         var generator = new GameDataGenerator();
         generator.WriteGameDataSourceFile( new( TileMaps, TileTextureSets ), _palette, _tileTexturePool );
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
