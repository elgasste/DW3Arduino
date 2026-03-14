using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class PlayerClassExpTableSaveData
   {
      public PlayerClass PlayerClass { get; set; } = 0;
      public List<int> ExpTable { get; set; } = [];

      public PlayerClassExpTableSaveData()
      {
         ExpTable = new List<int>( Constants.PlayerMaxLevel - 1 );
      }

      public PlayerClassExpTableSaveData( PlayerClassExpTableViewModel viewModel )
      {
         PlayerClass = viewModel.PlayerClass;

         foreach ( var table in viewModel.ExpTable )
         {
            ExpTable.Add( table );
         }
      }
   }
}
