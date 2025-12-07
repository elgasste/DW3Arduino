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
      // MUFFINS: next let's set up the list of existing tile maps
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
               foreach ( var savedTileMap in saveData.TileMaps )
               {
                  TileMaps.Add( new( savedTileMap ) );
               }
            }
         }
         catch
         {
            MessageBox.Show( "Something went wrong when loading save data, file is possibly corrupt! Starting from scratch.", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
         }
      }

      private void AddNewTileMap()
      {
         var window = new AddNewTileMapWindow();
         var result = window.ShowDialog();

         if ( result.HasValue && result.Value )
         {
            int index = TileMaps.Count > 0 ? TileMaps[^1].Index + 1 : 0;
            var newTileMap = new TileMapViewModel( index, window.NewTileMapName, window.NewTilesX, window.NewTilesY, window.NewWraps );
            TileMaps.Add( newTileMap );
            SelectedTileMap = newTileMap;
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
         GameDataGenerator.WriteGameDataSourceFile();
         MessageBox.Show( "Game data source file has been written." );
      }

      private ICommand? _addNewTileMapCommand;
      public ICommand? AddNewTileMapCommand => _addNewTileMapCommand ??= new RelayCommand( AddNewTileMap, () => true );

      private ICommand? _saveGameDataCommand;
      public ICommand? SaveGameDataCommand => _saveGameDataCommand ??= new RelayCommand( WriteSaveData, () => true );

      private ICommand? _writeGameDataSourceCommand;
      public ICommand? WriteGameDataSourceCommand => _writeGameDataSourceCommand ??= new RelayCommand( WriteGameDataSource, () => true );
   }
}
