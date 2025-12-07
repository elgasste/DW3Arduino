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
         // TODO
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
