#include "game.h"

#define COLLISION_THETA    0.001f

internal void Game_MoveEntity( Game_t* game, Entity_t* entity );

void Game_TicPhysics( Game_t* game )
{
   Game_MoveEntity( game, &game->playerEntity );

   game->playerEntity.velocity.x = 0.0f;
   game->playerEntity.velocity.y = 0.0f;

   TileMap_UpdateViewport( &game->tileMap, &game->playerEntity );
}

internal void Game_MoveEntity( Game_t* game, Entity_t* entity )
{
   u32 tileRowStart, tileRowEnd, tileColStart, tileColEnd, row, col, tile, tileIndex;
   Vector2r32_t newPos;

   newPos.x = entity->hitBox.x + ( entity->velocity.x * CLOCK_FRAME_SECONDS );
   newPos.y = entity->hitBox.y + ( entity->velocity.y * CLOCK_FRAME_SECONDS );

   // clip to unpassable horizontal tiles
   if ( newPos.x != entity->hitBox.x )
   {
      tileRowStart = (u32)( entity->hitBox.y / TILE_SIZE );
      tileRowEnd = (u32)( ( entity->hitBox.y + entity->hitBox.h ) / TILE_SIZE );

      if ( newPos.x < entity->hitBox.x )
      {
         // moving left, check leftward tiles
         col = (u32)( newPos.x / TILE_SIZE );

         for ( row = tileRowStart; row <= tileRowEnd; row++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newPos.x = (r32)( ( ( col + 1 ) * TILE_SIZE ) );
               break;
            }
         }
      }
      else
      {
         // moving right, check rightward tiles
         col = (u32)( ( newPos.x + entity->hitBox.w ) / TILE_SIZE );

         for ( row = tileRowStart; row <= tileRowEnd; row++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newPos.x = ( col * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
               break;
            }
         }
      }
   }

   // clip to unpassable vertical tiles
   if ( newPos.y != entity->hitBox.y )
   {
      tileColStart = (u32)( entity->hitBox.x / TILE_SIZE );
      tileColEnd = (u32)( ( entity->hitBox.x + entity->hitBox.w ) / TILE_SIZE );

      if ( newPos.y < entity->hitBox.y )
      {
         // moving up, check upward tiles
         row = (u32)( newPos.y / TILE_SIZE );

         for ( col = tileColStart; col <= tileColEnd; col++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newPos.y = (r32)( ( ( row + 1 ) * TILE_SIZE ) );
               break;
            }
         }
      }
      else
      {
         // moving down, check downward tiles
         row = (u32)( ( newPos.y + entity->hitBox.h ) / TILE_SIZE );

         for ( col = tileColStart; col <= tileColEnd; col++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newPos.y = ( row * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
               break;
            }
         }
      }
   }

   entity->hitBox.x = newPos.x;
   entity->hitBox.y = newPos.y;

   // clip to map bounds
   if ( entity->hitBox.x < 0.0f )
   {
      entity->hitBox.x = 0.0f;
   }
   else if ( ( entity->hitBox.x + entity->hitBox.w ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      entity->hitBox.x = (r32)( ( game->tileMap.tilesX * TILE_SIZE ) ) - entity->hitBox.w;
   }

   if ( entity->hitBox.y < 0.0f )
   {
      entity->hitBox.y = 0.0f;
   }
   else if ( ( entity->hitBox.y + entity->hitBox.h ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      entity->hitBox.y = (r32)( ( game->tileMap.tilesY * TILE_SIZE ) ) - entity->hitBox.h;
   }
}
