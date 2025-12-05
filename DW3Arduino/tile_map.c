#include "tile_map.h"
#include "entity.h"

void TileMap_Init( TileMap_t* tileMap )
{
   u32 i, j;

   for ( i = 0; i < TILEMAP_MAX_TEXTURES; i++ )
   {
      for ( j = 0; j < TILE_SIZE * TILE_SIZE; j++ )
      {
         tileMap->tileTextures[i].paletteIndexes[j] = 0;
      }
   }

   for ( i = 0; i < TILEMAP_MAX_TILES_X * TILEMAP_MAX_TILES_Y; i++ )
   {
      tileMap->tiles[i] = 0;
   }

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
      if ( tileMap->viewport.w > (i32)( tileMap->tilesX * TILE_SIZE ) )
      {
         // map is thinner than the viewport, center it horizontally
         tileMap->viewport.x = -(i32)( ( tileMap->viewport.w - ( tileMap->tilesX * TILE_SIZE ) ) / 2 );
      }
      else
      {
         tileMap->viewport.x = (i32)( entity->pos.x + ( entity->pos.w / 2 ) ) - ( tileMap->viewport.w / 2 );

         // clamp to left or right edge if necessary
         if ( tileMap->viewport.x < 0 )
         {
            tileMap->viewport.x = 0;
         }
         else if ( ( tileMap->viewport.x + tileMap->viewport.w ) >= (i32)( tileMap->tilesX * TILE_SIZE ) )
         {
            tileMap->viewport.x = (i32)( ( tileMap->tilesX * TILE_SIZE ) - tileMap->viewport.w );
         }
      }

      if ( tileMap->viewport.h > ( i32 )( tileMap->tilesY * TILE_SIZE ) )
      {
         // map is shorter than the viewport, center it vertically
         tileMap->viewport.y = -(i32)( ( tileMap->viewport.h - ( tileMap->tilesY * TILE_SIZE ) ) / 2 );
      }
      else
      {
         tileMap->viewport.y = (i32)( entity->pos.y + ( entity->pos.h / 2 ) ) - ( tileMap->viewport.h / 2 );

         // clamp to top or bottom edge if necessary
         if ( tileMap->viewport.y < 0 )
         {
            tileMap->viewport.y = 0;
         }
         else if ( ( tileMap->viewport.y + tileMap->viewport.h ) >= (i32)( tileMap->tilesY * TILE_SIZE ) )
         {
            tileMap->viewport.y = (i32)( ( tileMap->tilesY * TILE_SIZE ) - tileMap->viewport.h );
         }
      }
   }
}

u32 TileMap_GetTileIndexAtPosition( TileMap_t* tileMap, u32 x, u32 y )
{
   return ( ( y / TILE_SIZE ) * tileMap->tilesX ) + ( x / TILE_SIZE );
}
