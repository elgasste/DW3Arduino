#include "tile_map.h"
#include "entity.h"

void TileMap_Init( TileMap_t* tileMap, Player_t* players, u32( *getPlayerCountFunc )( void* ), void* playerCountProvider )
{
   tileMap->players = players;
   tileMap->getPlayerCountFunc = getPlayerCountFunc;
   tileMap->playerCountProvider = playerCountProvider;

   tileMap->staticSpriteCount = 0;
   tileMap->activeSpriteCount = 0;
   tileMap->entityCount = 0;
   tileMap->npcCount = 0;
}

void TileMap_Tic( TileMap_t* tileMap )
{
   u32 i;

   for ( i = 0; i < tileMap->npcCount; i++ )
   {
      Npc_Tic( tileMap->npcs + i );
   }

   for ( i = 0; i < tileMap->activeSpriteCount; i++ )
   {
      ActiveSprite_Tic( tileMap->activeSprites + i );
   }

   for ( i = 0; i < tileMap->getPlayerCountFunc( tileMap->playerCountProvider ); i++ )
   {
      ActiveSprite_Tic( tileMap->playerSprites + i );
   }
}

void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity )
{
   i32 tileMapW, tileMapH;

   if ( tileMap->wraps )
   {
      // wrapping maps should always center on the focal entity
      tileMap->viewport.x = ( entity->pos.x + ( entity->pos.w / 2 ) ) - ( tileMap->viewport.w / 2 );
      tileMap->viewport.y = ( entity->pos.y + ( entity->pos.h / 2 ) ) - ( tileMap->viewport.h / 2 );
   }
   else
   {
      tileMapW = ( tileMap->tilesX * TILEMAP_TILE_SIZE_UNITS );

      if ( tileMap->viewport.w > tileMapW )
      {
         // map is thinner than the viewport, center it horizontally
         tileMap->viewport.x = -( ( tileMap->viewport.w - tileMapW ) / 2 );
      }
      else
      {
         tileMap->viewport.x = ( entity->pos.x + ( entity->pos.w / 2 ) ) - ( tileMap->viewport.w / 2 );

         // clamp to left or right edge if necessary
         if ( tileMap->viewport.x < 0 )
         {
            tileMap->viewport.x = 0;
         }
         else if ( ( tileMap->viewport.x + tileMap->viewport.w ) >= tileMapW )
         {
            tileMap->viewport.x = tileMapW - tileMap->viewport.w;
         }
      }

      tileMapH = ( tileMap->tilesY * TILEMAP_TILE_SIZE_UNITS );

      if ( tileMap->viewport.h > tileMapH )
      {
         // map is shorter than the viewport, center it vertically
         tileMap->viewport.y = -( ( tileMap->viewport.h - tileMapH ) / 2 );
      }
      else
      {
         tileMap->viewport.y = ( entity->pos.y + ( entity->pos.h / 2 ) ) - ( tileMap->viewport.h / 2 );

         // clamp to top or bottom edge if necessary
         if ( tileMap->viewport.y < 0 )
         {
            tileMap->viewport.y = 0;
         }
         else if ( ( tileMap->viewport.y + tileMap->viewport.h ) >= tileMapH )
         {
            tileMap->viewport.y = tileMapH - tileMap->viewport.h;
         }
      }
   }
}

void TileMap_CenterEntityOnTile( TileMap_t* tileMap, Entity_t* entity, u32 tileIndex )
{
   i32 tileX = ( tileIndex % tileMap->tilesX ) * TILEMAP_TILE_SIZE_UNITS;
   i32 tileY = ( tileIndex / tileMap->tilesX ) * TILEMAP_TILE_SIZE_UNITS;

   entity->pos.x = tileX + ( ( TILEMAP_TILE_SIZE_UNITS / 2 ) - ( entity->pos.w / 2 ) );
   entity->pos.y = tileY + ( ( TILEMAP_TILE_SIZE_UNITS / 2 ) - ( entity->pos.h / 2 ) );
   entity->tileIndex = tileIndex;
}

u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, i32 x, i32 y )
{
   return ( (i32)( y / ( TILEMAP_TILE_SIZE_UNITS ) ) * tileMap->tilesX ) + ( x / ( TILEMAP_TILE_SIZE_UNITS ) );
}

void TileMap_GetPositionOfTileIndex( TileMap_t* tileMap, u32 tileIndex, i32* x, i32* y )
{
   *y = (i32)( ( tileIndex / tileMap->tilesX ) * TILEMAP_TILE_SIZE_UNITS );
   *x = (i32)( ( tileIndex % tileMap->tilesX ) * TILEMAP_TILE_SIZE_UNITS );
}

Bool_t TileMap_TileIndexIsEdgeTile( TileMap_t* tileMap, u32 tileIndex )
{
   if ( tileMap->wraps )
   {
      return False;
   }

   return ( tileIndex < tileMap->tilesX ) ||                                     // top edge
          ( ( tileIndex % tileMap->tilesX ) == 0 ) ||                            // left edge
          ( ( tileIndex % tileMap->tilesX ) == ( tileMap->tilesX - 1 ) ) ||      // right edge
          ( tileIndex >= ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ) );        // bottom edge
}

i32 TileMap_GetTileVelocity( TileMap_t* tileMap, u32 tileIndex )
{
   u16 tile = tileMap->tiles[tileIndex];
   u32 speedFactor = TILE_GET_WALKING_SPEED( tile );

   switch ( speedFactor )
   {
      case 0: return TILE_WALK_SPEED_NORMAL;
      case 1: return TILE_WALK_SPEED_SLOW;
      case 2: return TILE_WALK_SPEED_VERY_SLOW;
      case 3: return TILE_WALK_SPEED_CRAWL;

      default: return TILE_WALK_SPEED_NORMAL;
   }
}

i32 TileMap_GetTileDiagonalVelocity( i32 regularVelocity )
{
   switch ( regularVelocity )
   {
      case TILE_WALK_SPEED_NORMAL: return TILE_WALK_SPEED_DIAGONAL_NORMAL;
      case TILE_WALK_SPEED_SLOW: return TILE_WALK_SPEED_DIAGONAL_SLOW;
      case TILE_WALK_SPEED_VERY_SLOW: return TILE_WALK_SPEED_DIAGONAL_VERY_SLOW;
      case TILE_WALK_SPEED_CRAWL: return TILE_WALK_SPEED_DIAGONAL_CRAWL;

      default: return (i32)( regularVelocity * 0.707f );
   }
}
