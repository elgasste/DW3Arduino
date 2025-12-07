using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TileMapSaveData
   {
      public List<TileSaveData> Tiles { get; set; } = new();

      public int Index { get; set; }
      public string Name { get; set; } = string.Empty;
      public int TilesX { get; set; }
      public int TilesY { get; set; }
      public bool Wraps { get; set; }
      public List<PortalSaveData> Portals { get; set; } = [];
      public PortalSaveData? EdgePortal { get; set; }
      public List<EntitySaveData> Entities { get; set; } = [];
      public List<NpcSaveData> Npcs { get; set; } = [];

      public TileMapSaveData() { }

      public TileMapSaveData( TileMapViewModel viewModel )
      {
         Index = viewModel.Index;
         Name = viewModel.Name;
         TilesX = viewModel.TilesX;
         TilesY = viewModel.TilesY;
         Wraps = viewModel.Wraps;

         foreach ( var tile in viewModel.Tiles )
         {
            Tiles.Add( new( tile ) );
         }
      }
   }
}
