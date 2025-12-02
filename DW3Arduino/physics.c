#include "game.h"
#include "utility.h"

internal void Physics_MoveEntities( Game_t* game );
internal Bool_t Physics_EntityCollidesWithTileMap( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal );
internal Bool_t Physics_EntityCollidesWithEntities( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal );

void Physics_Tic( Game_t* game )
{
   Physics_MoveEntities( game );

   // reset player velocity and clamp the viewport to the player
   game->playerEntity->velocity.x = 0.0f;
   game->playerEntity->velocity.y = 0.0f;

   TileMap_ClampViewportToEntity( &game->tileMap, game->playerEntity );
}

internal void Physics_MoveEntities( Game_t* game )
{
   u32 i;
   i32 pixelsRemaining;
   r32 deltaX, deltaY, deltaRemaining, sign;
   Entity_t* entity;

   // the idea for this came from Maddy Thorson's game, Celeste. instead of setting an entity's
   // new position and doing collision resolution, we just move a pixel at a time and check for collisions.
   // it may not be very performant though, we'll have to keep an eye on that.
   for ( i = 0, entity = game->tileMap.entities; i < game->tileMap.entityCount; i++, entity++ )
   {
      deltaX = entity->velocity.x * CLOCK_FRAME_SECONDS;
      deltaY = entity->velocity.y * CLOCK_FRAME_SECONDS;

      // horizontal pass
      if ( deltaX != 0.0f )
      {
         deltaRemaining = deltaX;
         sign = ( deltaX < 0.0f ) ? -1.0f : 1.0f;
         pixelsRemaining = (i32)deltaX;

         // move full pixels first, one at a time
         while ( pixelsRemaining != 0 )
         {
            entity->pos.x += sign;

            if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, True ) ||
                 Physics_EntityCollidesWithEntities( &game->tileMap, entity, sign, True ) )
            {
               entity->pos.x = entity->prevPos.x;
               break;
            }
            else
            {
               entity->prevPos.x = entity->pos.x;
            }

            deltaRemaining -= sign;
            pixelsRemaining -= (i32)sign;
         }

         // move remaining sub-pixels
         if ( deltaRemaining != 0.0f )
         {
            entity->pos.x += deltaRemaining;

            if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, True ) ||
                 Physics_EntityCollidesWithEntities( &game->tileMap, entity, sign, True ) )
            {
               entity->pos.x = entity->prevPos.x;
            }
            else
            {
               entity->prevPos.x = entity->pos.x;
            }
         }
      }

      // vertical pass
      if ( deltaY != 0.0f )
      {
         deltaRemaining = deltaY;
         sign = ( deltaY < 0.0f ) ? -1.0f : 1.0f;
         pixelsRemaining = (i32)deltaY;

         // move full pixels first, one at a time
         while ( pixelsRemaining != 0 )
         {
            entity->pos.y += sign;

            if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, False ) ||
                 Physics_EntityCollidesWithEntities( &game->tileMap, entity, sign, False ) )
            {
               entity->pos.y = entity->prevPos.y;
               break;
            }
            else
            {
               entity->prevPos.y = entity->pos.y;
            }

            deltaRemaining -= sign;
            pixelsRemaining -= (i32)sign;
         }

         // move remaining sub-pixels
         if ( deltaRemaining != 0.0f )
         {
            entity->pos.y += deltaRemaining;

            if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, False ) ||
                 Physics_EntityCollidesWithEntities( &game->tileMap, entity, sign, False ) )
            {
               entity->pos.y = entity->prevPos.y;
            }
            else
            {
               entity->prevPos.y = entity->pos.y;
            }
         }
      }
   }
}

internal Bool_t Physics_EntityCollidesWithTileMap( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal )
{
   u32 i;
   u16 tile;
   u32 start, end, side;

   if ( horizontal )
   {
      start = (u32)( entity->pos.y / TILE_SIZE );                    // top row
      end = (u32)( ( entity->pos.y + entity->pos.h ) / TILE_SIZE );  // bottom row
      side = ( sign < 0.0f )
         ? (u32)( entity->pos.x / TILE_SIZE )                        // left side
         : (u32)( ( entity->pos.x + entity->pos.w  ) / TILE_SIZE );  // right side

      for ( i = start; i <= end; i++ )
      {
         tile = tileMap->tiles[side + ( i * tileMap->tilesX )];

         if ( !TILE_GET_IS_PASSABLE( tile ) )
         {
            return True;
         }
      }
   }
   else
   {
      start = (u32)( entity->pos.x / TILE_SIZE );                    // left col
      end = (u32)( ( entity->pos.x + entity->pos.w ) / TILE_SIZE );  // right col
      side = ( sign < 0.0f )
         ? (u32)( entity->pos.y / TILE_SIZE )                        // top side
         : (u32)( ( entity->pos.y + entity->pos.h  ) / TILE_SIZE );  // bottom side

      for ( i = start; i <= end; i++ )
      {
         tile = tileMap->tiles[i + ( side * tileMap->tilesX )];

         if ( !TILE_GET_IS_PASSABLE( tile ) )
         {
            return True;
         }
      }
   }

   return False;
}

internal Bool_t Physics_EntityCollidesWithEntities( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal )
{
   u32 i;
   Entity_t* rigidEntity;

   for ( i = 0, rigidEntity = tileMap->entities; i < tileMap->entityCount; i++, rigidEntity++ )
   {
      if ( entity != rigidEntity )
      {
         if ( horizontal )
         {
            if ( sign < 0.0f ) // moving left
            {
               if ( Utility_VerticalLineIntersectsRect32r( entity->pos.x, entity->pos.y, entity->pos.y + entity->pos.h,
                                                           rigidEntity->pos.x, rigidEntity->pos.y, rigidEntity->pos.w, rigidEntity->pos.h ) )
               {
                  return True;
               }
            }
            else // moving right
            {
               if ( Utility_VerticalLineIntersectsRect32r( entity->pos.x + entity->pos.w, entity->pos.y, entity->pos.y + entity->pos.h,
                                                           rigidEntity->pos.x, rigidEntity->pos.y, rigidEntity->pos.w, rigidEntity->pos.h ) )
               {
                  return True;
               }
            }
         }
         else
         {
            if ( sign < 0.0f ) // moving up
            {
               if ( Utility_HorizontalLineIntersectsRect32r( entity->pos.x, entity->pos.x + entity->pos.w, entity->pos.y,
                                                             rigidEntity->pos.x, rigidEntity->pos.y, rigidEntity->pos.w, rigidEntity->pos.h ) )
               {
                  return True;
               }
            }
            else // moving down
            {
               if ( Utility_HorizontalLineIntersectsRect32r( entity->pos.x, entity->pos.x + entity->pos.w, entity->pos.y + entity->pos.h,
                                                             rigidEntity->pos.x, rigidEntity->pos.y, rigidEntity->pos.w, rigidEntity->pos.h ) )
               {
                  return True;
               }
            }
         }
      }
   }

   return False;
}
