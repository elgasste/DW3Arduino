using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor.SaveData
{
   public class TileMapSaveData
   {
      public uint Index { get; set; }
      public string Name { get; set; } = string.Empty;
      public uint TilesX { get; set; }
      public uint TilesY { get; set; }
      public bool Wraps { get; set; }
      public bool AffectsDaylight { get; set; }
      public uint TileTextureSetIndex { get; set; }
      public List<TileSaveData> Tiles { get; set; } = [];
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
         AffectsDaylight = viewModel.AffectsDaylight;
         TileTextureSetIndex = viewModel.TileTextureSetIndex;

         for ( uint i = 0; i < viewModel.Tiles.Count; i++ )
         {
            Tiles.Add( new( viewModel.Tiles[(int)i], i ) );
         }

         foreach ( var portal in viewModel.Portals )
         {
            Portals.Add( new( portal ) );
         }

         if ( viewModel.EdgePortal is not null )
         {
            EdgePortal = new( viewModel.EdgePortal );
         }
      }
   }
}
