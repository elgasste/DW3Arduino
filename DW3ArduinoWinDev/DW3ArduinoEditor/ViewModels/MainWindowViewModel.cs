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
            var contents = File.ReadAllText( Constants.AssetsBasePath + Constants.SaveDataFileName );
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

      private void WriteSaveData() => File.WriteAllText( Constants.AssetsBasePath + Constants.SaveDataFileName, JsonSerializer.Serialize( _gameSaveData ) );

      private void SaveGameData()
      {
         WriteSaveData();
         MessageBox.Show( "Game data has been saved.", "Hooray!", MessageBoxButton.OK, MessageBoxImage.Information );
      }

      private ICommand? _saveGameDataCommand;
      public ICommand? SaveGameDataCommand => _saveGameDataCommand ??= new RelayCommand( SaveGameData, () => true );
   }
}
