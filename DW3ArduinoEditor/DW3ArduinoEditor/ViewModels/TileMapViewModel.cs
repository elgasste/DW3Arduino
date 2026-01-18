using DW3ArduinoEditor.SaveData;
using System.Collections.ObjectModel;
using System.Windows;

namespace DW3ArduinoEditor.ViewModels
{
   public class TileMapViewModel : ViewModelBase
   {
      public ObservableCollection<TileViewModel> Tiles { get; private set; } = [];
      public ObservableCollection<StaticSpriteViewModel> StaticSprites { get; private set; } = [];
      public ObservableCollection<ActiveSpriteViewModel> ActiveSprites { get; private set; } = [];
      public ObservableCollection<ActiveSpriteViewModel> PlayerSprites { get; private set; } = [];
      public ObservableCollection<PortalViewModel> Portals { get; private set; } = [];
      public ObservableCollection<EntityViewModel> Entities { get; private set; } = [];
      public ObservableCollection<NpcViewModel> Npcs { get; private set; } = [];

      private uint _index;
      public uint Index
      {
         get => _index;
         set => SetProperty( ref _index, value );
      }

      private string _name = string.Empty;
      public string Name
      {
         get => _name;
         set => SetProperty( ref _name, value );
      }

      private uint _tileTextureSetIndex;
      public uint TileTextureSetIndex
      {
         get => _tileTextureSetIndex;
         set => SetProperty( ref _tileTextureSetIndex, value );
      }

      private uint _staticSpriteTextureSetIndex;
      public uint StaticSpriteTextureSetIndex
      {
         get => _staticSpriteTextureSetIndex;
         set => SetProperty( ref _staticSpriteTextureSetIndex, value );
      }

      private uint _activeSpriteTextureSetIndex;
      public uint ActiveSpriteTextureSetIndex
      {
         get => _activeSpriteTextureSetIndex;
         set => SetProperty( ref _activeSpriteTextureSetIndex, value );
      }

      private uint _tilesX;
      public uint TilesX
      {
         get => _tilesX;
         set
         {
            uint oldTilesX = _tilesX;

            if ( value < _tilesX ) // reducing the horizontal size
            {
               ObservableCollection<TileViewModel> newTiles = [];

               for ( int i = 0; i < ( _tilesX * _tilesY ); i++ )
               {
                  for ( int j = 0; j < value; i++, j++ )
                  {
                     newTiles.Add( Tiles[i] );
                  }

                  i += (int)( ( _tilesX - value ) - 1 );
               }

               Tiles = newTiles;
            }
            else if ( value > _tilesX ) // expanding the horizontal size
            {
               ObservableCollection<TileViewModel> newTiles = [];

               for ( int i = 0; i < ( _tilesX * _tilesY ); i++ )
               {
                  for ( int j = 0; j < value; j++ )
                  {
                     if ( j < _tilesX )
                     {
                        newTiles.Add( Tiles[i] );
                        i++;
                     }
                     else
                     {
                        newTiles.Add( new() );
                     }
                  }

                  i--;
               }

               Tiles = newTiles;
            }

            SetProperty( ref _tilesX, value );
            UpdateObjectsAfterResize( oldTilesX, _tilesY );
         }
      }

      private uint _tilesY;
      public uint TilesY
      {
         get => _tilesY;
         set
         {
            uint oldTilesY = _tilesY;

            if ( value < _tilesY ) // reducing the vertical size
            {
               ObservableCollection<TileViewModel> newTiles = [];

               for ( int i = 0; i < ( _tilesX * value ); i++ )
               {
                  newTiles.Add( Tiles[i] );
               }

               Tiles = newTiles;
            }
            else if ( value > _tilesY ) // expanding the vertical size
            {
               ObservableCollection<TileViewModel> newTiles = [];
               int i = 0;

               for ( ; i < ( _tilesX * _tilesY ); i++ )
               {
                  newTiles.Add( Tiles[i] );
               }

               for ( ; i < ( _tilesX * value ); i++ )
               {
                  newTiles.Add( new() );
               }

               Tiles = newTiles;
            }

            SetProperty( ref _tilesY, value );
            UpdateObjectsAfterResize( _tilesX, oldTilesY );
         }
      }

      private bool _wraps;
      public bool Wraps
      {
         get => _wraps;
         set => SetProperty( ref _wraps, value );
      }

      private bool _affectsDaylight;
      public bool AffectsDaylight
      {
         get => _affectsDaylight;
         set => SetProperty( ref _affectsDaylight, value );
      }

      private bool _isUnderground;
      public bool IsUnderground
      {
         get => _isUnderground;
         set => SetProperty( ref _isUnderground, value );
      }

      private bool _hasEncounters;
      public bool HasEncounters
      {
         get => _hasEncounters;
         set => SetProperty( ref _hasEncounters, value );
      }

      private PortalViewModel? _edgePortal;
      public PortalViewModel? EdgePortal
      {
         get => _edgePortal;
         set => SetProperty( ref _edgePortal, value );
      }

      public TileMapViewModel( uint index,
                               string name,
                               uint tilesX,
                               uint tilesY,
                               bool wraps,
                               bool affectsDaylight,
                               bool isUnderground,
                               bool hasEncounters,
                               uint tileTextureSetIndex,
                               uint staticSpriteTextureSetIndex,
                               uint activeSpriteTextureSetIndex )
      {
         _index = index;
         _name = name;
         _tilesX = tilesX;
         _tilesY = tilesY;
         _wraps = wraps;
         _affectsDaylight = affectsDaylight;
         _isUnderground = isUnderground;
         _hasEncounters = hasEncounters;
         _tileTextureSetIndex = tileTextureSetIndex;
         _staticSpriteTextureSetIndex = staticSpriteTextureSetIndex;
         _activeSpriteTextureSetIndex = activeSpriteTextureSetIndex;

         for ( int i = 0; i < _tilesX * _tilesY; i++ )
         {
            Tiles.Add( new() );
         }
      }

      public TileMapViewModel( TileMapSaveData saveData )
      {
         _index = saveData.Index;
         _name = saveData.Name;
         _tilesX = saveData.TilesX;
         _tilesY = saveData.TilesY;
         _wraps = saveData.Wraps;
         _affectsDaylight = saveData.AffectsDaylight;
         _isUnderground = saveData.IsUnderground;
         _hasEncounters = saveData.HasEncounters;
         _tileTextureSetIndex = saveData.TileTextureSetIndex;
         _staticSpriteTextureSetIndex = saveData.StaticSpriteTextureSetIndex;
         _activeSpriteTextureSetIndex = saveData.ActiveSpriteTextureSetIndex;

         if ( string.IsNullOrEmpty( _name ) )
         {
            _name = "(unnamed)";
         }

         foreach ( var tile in saveData.Tiles )
         {
            Tiles.Add( new( tile ) );
         }

         foreach ( var staticSprite in saveData.StaticSprites )
         {
            StaticSprites.Add( new( staticSprite ) );
         }

         foreach ( var activeSprite in saveData.ActiveSprites )
         {
            ActiveSprites.Add( new( activeSprite ) );
         }

         foreach ( var playerSprite in saveData.PlayerSprites )
         {
            PlayerSprites.Add( new( playerSprite ) );
         }

         foreach ( var portal in saveData.Portals )
         {
            Portals.Add( new( portal ) );
         }

         if ( saveData.EdgePortal is not null )
         {
            _edgePortal = new( saveData.EdgePortal );
         }

         foreach ( var entity in saveData.Entities )
         {
            Entities.Add( new( entity ) );
         }

         foreach ( var npc in saveData.Npcs )
         {
            Npcs.Add( new( npc ) );
         }
      }

      private void UpdateObjectsAfterResize( uint oldTilesX, uint oldTilesY )
      {
         // update portals
         for ( int i = 0; i < Portals.Count; i++ )
         {
            uint row = Portals[i].SourceTileIndex / oldTilesX;
            uint col = Portals[i].SourceTileIndex % oldTilesX;

            if ( row >= TilesY || col >= TilesX )
            {
               // the section of the map containing this portal is now gone
               Portals.Remove( Portals[i] );
               i--;
            }
            else if ( oldTilesX != TilesX )
            {
               Portals[i].SourceTileIndex = ( row * TilesX ) + col;
            }
         }

         // update entities and NPCs
         for ( int i = 0; i < Entities.Count; i++ )
         {
            uint row = (uint)( Entities[i].Pos.X / Constants.UnitsPerPixel ) / oldTilesX;
            uint col = (uint)( Entities[i].Pos.Y / Constants.UnitsPerPixel ) % oldTilesX;

            if ( row >= TilesY || col >= TilesX )
            {
               // the section of the map containing this entity is now gone
               foreach ( var npc in Npcs )
               {
                  if ( npc.EntityIndex == i )
                  {
                     Npcs.Remove( npc );
                  }
               }

               Entities.Remove( Entities[i] );
               i--;
            }
         }

         // update static sprites
         for ( int i = 0; i < StaticSprites.Count; i++ )
         {
            uint row = StaticSprites[i].TileIndex / oldTilesX;
            uint col = StaticSprites[i].TileIndex % oldTilesX;

            if ( row >= TilesY || col >= TilesX )
            {
               // the section of the map containing this static sprite is now gone
               StaticSprites.Remove( StaticSprites[i] );
               i--;
            }
            else if ( oldTilesX != TilesX )
            {
               StaticSprites[i].TileIndex = ( row * TilesX ) + col;
            }
         }
      }
   }
}
