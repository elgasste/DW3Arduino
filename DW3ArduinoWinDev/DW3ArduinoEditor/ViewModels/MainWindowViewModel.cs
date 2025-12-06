using DW3ArduinoEditor.SaveData;
using System.IO;
using System.Text.Json;
using System.Windows;

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
   }
}
