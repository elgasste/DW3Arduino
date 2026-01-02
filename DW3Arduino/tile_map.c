#include "tile_map.h"
#include "entity.h"

void TileMap_Init( TileMap_t* tileMap )
{
   tileMap->staticSpriteCount = 0;
   tileMap->activeSpriteCount = 0;
   tileMap->entityCount = 0;
   tileMap->playerCount = 0;
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

   for ( i = 0; i < tileMap->playerCount; i++ )
   {
      ActiveSprite_Tic( tileMap->playerSprites + i );
   }
}

void TileMap_ClampViewportToEntity( TileMap_t* tileMap, Entity_t* entity )
{
   if ( tileMap->wraps )
   {
      // wrapping maps should always center on the focal entity
      tileMap->viewport.x = (i32)( entity->pos.x + ( entity->pos.w / 2 ) ) - ( tileMap->viewport.w / 2 );
      tileMap->viewport.y = (i32)( entity->pos.y + ( entity->pos.h / 2 ) ) - ( tileMap->viewport.h / 2 );
   }
   else
   {
      if ( tileMap->viewport.w > (i32)( tileMap->tilesX * TILEMAP_TILE_SIZE ) )
      {
         // map is thinner than the viewport, center it horizontally
         tileMap->viewport.x = -(i32)( ( tileMap->viewport.w - ( tileMap->tilesX * TILEMAP_TILE_SIZE ) ) / 2 );
      }
      else
      {
         tileMap->viewport.x = (i32)( entity->pos.x + ( entity->pos.w / 2 ) ) - ( tileMap->viewport.w / 2 );

         // clamp to left or right edge if necessary
         if ( tileMap->viewport.x < 0 )
         {
            tileMap->viewport.x = 0;
         }
         else if ( ( tileMap->viewport.x + tileMap->viewport.w ) >= (i32)( tileMap->tilesX * TILEMAP_TILE_SIZE ) )
         {
            tileMap->viewport.x = (i32)( ( tileMap->tilesX * TILEMAP_TILE_SIZE ) - tileMap->viewport.w );
         }
      }

      if ( tileMap->viewport.h > ( i32 )( tileMap->tilesY * TILEMAP_TILE_SIZE ) )
      {
         // map is shorter than the viewport, center it vertically
         tileMap->viewport.y = -(i32)( ( tileMap->viewport.h - ( tileMap->tilesY * TILEMAP_TILE_SIZE ) ) / 2 );
      }
      else
      {
         tileMap->viewport.y = (i32)( entity->pos.y + ( entity->pos.h / 2 ) ) - ( tileMap->viewport.h / 2 );

         // clamp to top or bottom edge if necessary
         if ( tileMap->viewport.y < 0 )
         {
            tileMap->viewport.y = 0;
         }
         else if ( ( tileMap->viewport.y + tileMap->viewport.h ) >= (i32)( tileMap->tilesY * TILEMAP_TILE_SIZE ) )
         {
            tileMap->viewport.y = (i32)( ( tileMap->tilesY * TILEMAP_TILE_SIZE ) - tileMap->viewport.h );
         }
      }
   }
}

void TileMap_CenterEntityOnTile( TileMap_t* tileMap, Entity_t* entity, u32 tileIndex )
{
   u32 tileX = ( tileIndex % tileMap->tilesX ) * TILEMAP_TILE_SIZE;
   u32 tileY = ( tileIndex / tileMap->tilesX ) * TILEMAP_TILE_SIZE;

   entity->pos.x = (r32)( tileX + ( ( TILEMAP_TILE_SIZE / 2 ) - ( entity->pos.w / 2 ) ) );
   entity->pos.y = (r32)( tileY + ( ( TILEMAP_TILE_SIZE / 2 ) - ( entity->pos.h / 2 ) ) );
}

u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, u32 x, u32 y )
{
   return ( ( y / TILEMAP_TILE_SIZE ) * tileMap->tilesX ) + ( x / TILEMAP_TILE_SIZE );
}

void TileMap_GetPositionOfTileIndex( TileMap_t* tileMap, u32 tileIndex, u32* x, u32* y )
{
   *y = ( tileIndex / tileMap->tilesX ) * TILEMAP_TILE_SIZE;
   *x = ( tileIndex % tileMap->tilesX ) * TILEMAP_TILE_SIZE;
}

Bool_t TileMap_TileIndexIsEdgeTile( TileMap_t* tileMap, u32 tileIndex )
{
   return ( tileIndex < tileMap->tilesX ) ||                                     // top edge
          ( ( tileIndex % tileMap->tilesX ) == 0 ) ||                            // left edge
          ( ( tileIndex % tileMap->tilesX ) == ( tileMap->tilesX - 1 ) ) ||      // right edge
          ( tileIndex >= ( tileMap->tilesX * ( tileMap->tilesY - 1 ) ) );        // bottom edge
}

r32 TileMap_GetTileVelocity( TileMap_t* tileMap, u32 tileIndex )
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
