namespace DW3ArduinoEditor.SaveData
{
   public class TileMapSaveData
   {
      public int Index { get; set; }
      public int TilesX { get; set; }
      public int TilesY { get; set; }
      public bool Wraps { get; set; }
      public List<PortalSaveData> Portals { get; set; } = [];
      public PortalSaveData? EdgePortal { get; set; }
      public List<EntitySaveData> Entities { get; set; } = [];
      public List<NpcSaveData> Npcs { get; set; } = [];

      public TileMapSaveData() { }
   }
}
