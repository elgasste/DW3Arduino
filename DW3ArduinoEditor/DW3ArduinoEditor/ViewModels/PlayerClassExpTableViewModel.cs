using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class PlayerClassExpTableViewModel : ViewModelBase
   {
      private PlayerClass _playerClass = 0;
      public PlayerClass PlayerClass
      {
         get => _playerClass;
         set => SetProperty( ref _playerClass, value );
      }

      private List<int> _expTable = [];
      public List<int> ExpTable
      {
         get => _expTable;
         set => SetProperty( ref _expTable, value );
      }

      public PlayerClassExpTableViewModel() { }

      public PlayerClassExpTableViewModel( PlayerClassExpTableSaveData saveData )
      {
         PlayerClass = saveData.PlayerClass;
         ExpTable = saveData.ExpTable;
      }
   }
}
