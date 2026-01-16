using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class NpcSaveData
   {
      public uint EntityIndex { get; set; }
      public bool Wanders {  get; set; }
      public Direction InitialDirection { get; set; }

      public NpcSaveData() { }

      public NpcSaveData( NpcViewModel viewModel )
      {
         EntityIndex = viewModel.EntityIndex;
         Wanders = viewModel.Wanders;
         InitialDirection = viewModel.InitialDirection;
      }
   }
}
