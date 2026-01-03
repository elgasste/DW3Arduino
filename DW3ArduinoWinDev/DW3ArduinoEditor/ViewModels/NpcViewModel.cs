using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class NpcViewModel : ViewModelBase
   {
      private uint _entityIndex;
      public uint EntityIndex
      {
         get => _entityIndex;
         set => SetProperty( ref _entityIndex, value );
      }

      private bool _wanders;
      public bool Wanders
      {
         get => _wanders;
         set => SetProperty( ref _wanders, value );
      }

      public NpcViewModel( uint entityIndex, bool wanders )
      {
         _entityIndex = entityIndex;
         _wanders = wanders;
      }

      public NpcViewModel( NpcSaveData saveData )
      {
         _entityIndex = saveData.EntityIndex;
         _wanders = saveData.Wanders;
      }
   }
}
