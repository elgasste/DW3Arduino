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
   r32 newX, newY;

   newX = entity->hitBox.x + ( entity->velocity.x * CLOCK_FRAME_SECONDS );
   newY = entity->hitBox.y + ( entity->velocity.y * CLOCK_FRAME_SECONDS );

   // clip to unpassable horizontal tiles
   if ( newX != entity->hitBox.x )
   {
      tileRowStart = (u32)( entity->hitBox.y / TILE_SIZE );
      tileRowEnd = (u32)( ( entity->hitBox.y + entity->hitBox.h ) / TILE_SIZE );

      if ( newX < entity->hitBox.x )
      {
         // moving left, check leftward tiles
         col = (u32)( newX / TILE_SIZE );

         for ( row = tileRowStart; row <= tileRowEnd; row++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newX = (r32)( ( ( col + 1 ) * TILE_SIZE ) );
               break;
            }
         }
      }
      else
      {
         // moving right, check rightward tiles
         col = (u32)( ( newX + entity->hitBox.w ) / TILE_SIZE );

         for ( row = tileRowStart; row <= tileRowEnd; row++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newX = ( col * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
               break;
            }
         }
      }
   }

   // clip to unpassable vertical tiles
   if ( newY != entity->hitBox.y )
   {
      tileColStart = (u32)( entity->hitBox.x / TILE_SIZE );
      tileColEnd = (u32)( ( entity->hitBox.x + entity->hitBox.w ) / TILE_SIZE );

      if ( newY < entity->hitBox.y )
      {
         // moving up, check upward tiles
         row = (u32)( newY / TILE_SIZE );

         for ( col = tileColStart; col <= tileColEnd; col++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newY = (r32)( ( ( row + 1 ) * TILE_SIZE ) );
               break;
            }
         }
      }
      else
      {
         // moving down, check downward tiles
         row = (u32)( ( newY + entity->hitBox.h ) / TILE_SIZE );

         for ( col = tileColStart; col <= tileColEnd; col++ )
         {
            tileIndex = col + ( row * game->tileMap.tilesX );
            tile = game->tileMap.tiles[tileIndex];

            if ( !TILE_GET_IS_PASSABLE( tile ) )
            {
               newY = ( row * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
               break;
            }
         }
      }
   }

   entity->hitBox.x = newX;
   entity->hitBox.y = newY;

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
