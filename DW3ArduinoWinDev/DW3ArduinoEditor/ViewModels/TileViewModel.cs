using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.SaveData;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileViewModel : ViewModelBase
   {
      private uint _textureIndex;
      public uint TextureIndex
      {
         get => _textureIndex;
         set => SetProperty( ref _textureIndex, value );
      }

      private bool _isPassable;
      public bool IsPassable
      {
         get => _isPassable;
         set => SetProperty( ref _isPassable, value );
      }

      private TileWalkSpeed _walkSpeed;
      public TileWalkSpeed WalkSpeed
      {
         get => _walkSpeed;
         set => SetProperty( ref _walkSpeed, value );
      }

      private EncounterRate _encounterRate;
      public EncounterRate EncounterRate
      {
         get => _encounterRate;
         set => SetProperty( ref _encounterRate, value );
      }

      public TileViewModel()
      {
         _textureIndex = 0;
         _isPassable = true;
         _walkSpeed = TileWalkSpeed.Normal;
         _encounterRate = EncounterRate.None;
      }

      public TileViewModel( TileSaveData saveData )
      {
         _textureIndex = saveData.TextureIndex;
         _isPassable = saveData.IsPassable;
         _walkSpeed = saveData.WalkSpeed;
         _encounterRate = saveData.EncounterRate;
      }
   }
}
