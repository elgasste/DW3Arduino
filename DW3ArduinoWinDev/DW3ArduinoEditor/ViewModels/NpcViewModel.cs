using DW3ArduinoEditor.Enums;
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

      private Direction _initialDirection;
      public Direction InitialDirection
      {
         get => _initialDirection;
         set => SetProperty( ref _initialDirection, value );
      }

      public NpcViewModel( uint entityIndex, bool wanders, Direction initialDirection )
      {
         _entityIndex = entityIndex;
         _wanders = wanders;
         _initialDirection = initialDirection;
      }

      public NpcViewModel( NpcSaveData saveData )
      {
         _entityIndex = saveData.EntityIndex;
         _wanders = saveData.Wanders;
         _initialDirection = saveData.InitialDirection;
      }
   }
}
