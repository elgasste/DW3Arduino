using DW3ArduinoEditor.Commands;
using DW3ArduinoEditor.SaveData;
using System.IO;
using System.Text.Json;
using System.Windows;
using System.Windows.Input;

namespace DW3ArduinoEditor.ViewModels
{
   public class MainWindowViewModel : ViewModelBase
   {
      private readonly GameSaveData? _gameSaveData;

      public MainWindowViewModel()
      {
         try
         {
            var contents = File.ReadAllText( Constants.SaveDataFilePath );
            _gameSaveData = JsonSerializer.Deserialize<GameSaveData>( contents );
         }
         catch
         {
            _gameSaveData = null;
         }

         if ( _gameSaveData is null )
         {
            MessageBox.Show( "Failed to load save data from file! Creating new save data...", "Error", MessageBoxButton.OK, MessageBoxImage.Error );
            _gameSaveData = new();
         }
      }

      private void WriteSaveData()
      {
         File.WriteAllText( Constants.SaveDataFilePath, JsonSerializer.Serialize( _gameSaveData ) );
         MessageBox.Show( "Editor data has been saved." );
      }

      private void WriteGameDataSource()
      {
         GameDataGenerator.WriteGameDataSourceFile();
         MessageBox.Show( "Game data source file has been written." );
      }

      private ICommand? _saveGameDataCommand;
      public ICommand? SaveGameDataCommand => _saveGameDataCommand ??= new RelayCommand( WriteSaveData, () => true );

      private ICommand? _writeGameDataSourceCommand;
      public ICommand? WriteGameDataSourceCommand => _writeGameDataSourceCommand ??= new RelayCommand( WriteGameDataSource, () => true );
   }
}
