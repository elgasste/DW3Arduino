#include "game.h"
#include "utility.h"

internal void Physics_MoveEntities( Game_t* game );
internal Bool_t Physics_EntityCollidesWithTileMap( TileMap_t* tileMap, Entity_t* entity, i32 sign, Bool_t horizontal );
internal Bool_t Physics_EntityCollidesWithRigidBodies( TileMap_t* tileMap, Entity_t* entity, i32 sign, Bool_t horizontal );
internal Bool_t Physics_EntityCollidesWithRigidBody( TileMap_t* tileMap, Entity_t* entity, i32 rx, i32 ry, i32 rw, i32 rh, i32 sign, Bool_t horizontal );

void Physics_Tic( Game_t* game )
{
   Physics_MoveEntities( game );

   game->players->entity->velocity.x = 0;
   game->players->entity->velocity.y = 0;
}

internal void Physics_MoveEntities( Game_t* game )
{
   i32 i, pixelsRemaining, delta, deltaX, deltaY, deltaRemaining, sign, prev;
   Entity_t* entity = 0;
      
   for ( i = 0; i < (i32)game->tileMap.entityCount + 1; i++ )
   {
      // clip the player first (we only need to check the player that's being controlled,
      // the other player entities should be allowed to clip)
      if ( entity == 0 )
      {
         i = -1;
         entity = game->players->entity;
      }
      else if ( i == (i32)game->tileMap.entityCount )
      {
         break;
      }

      entity->prevPos = entity->pos;

      deltaX = entity->velocity.x;
      deltaY = entity->velocity.y;

#if defined( VISUAL_STUDIO_DEV )
      if ( g_winDebugFlags.noClip && entity == game->players->entity )
      {
         entity->pos.x += deltaX;
         entity->pos.y += deltaY;
      }
      else
      {
#endif

         // the idea for this came from Maddy Thorson's game, Celeste. instead of setting an entity's
         // new position and doing collision resolution, we just move a pixel at a time and check for collisions.
         // it may not be very performant though, we'll have to keep an eye on that.

         // horizontal pass
         if ( deltaX != 0 )
         {
            deltaRemaining = deltaX;
            sign = ( deltaX < 0 ) ? -1 : 1;
            delta = sign * UNITS_PER_PIXEL;
            pixelsRemaining = deltaX / UNITS_PER_PIXEL;

            // move full pixels first, one at a time
            while ( pixelsRemaining != 0 )
            {
               prev = entity->pos.x;
               entity->pos.x += delta;

               if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, True ) ||
                    Physics_EntityCollidesWithRigidBodies( &game->tileMap, entity, sign, True ) )
               {
                  entity->pos.x = prev;
                  break;
               }

               deltaRemaining -= delta;
               pixelsRemaining -= sign;
            }

            // move remaining sub-pixels
            if ( deltaRemaining != 0 )
            {
               prev = entity->pos.x;
               entity->pos.x += deltaRemaining;

               if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, True ) ||
                    Physics_EntityCollidesWithRigidBodies( &game->tileMap, entity, sign, True ) )
               {
                  entity->pos.x = prev;
               }
            }
         }

         // vertical pass
         if ( deltaY != 0 )
         {
            deltaRemaining = deltaY;
            sign = ( deltaY < 0 ) ? -1 : 1;
            delta = sign * UNITS_PER_PIXEL;
            pixelsRemaining = deltaY;

            // move full pixels first, one at a time
            while ( pixelsRemaining != 0 )
            {
               prev = entity->pos.y;
               entity->pos.y += delta;

               if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, False ) ||
                    Physics_EntityCollidesWithRigidBodies( &game->tileMap, entity, sign, False ) )
               {
                  entity->pos.y = prev;
                  break;
               }

               deltaRemaining -= delta;
               pixelsRemaining -= sign;
            }

            // move remaining sub-pixels
            if ( deltaRemaining != 0 )
            {
               prev = entity->pos.y;
               entity->pos.y += deltaRemaining;

               if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, False ) ||
                    Physics_EntityCollidesWithRigidBodies( &game->tileMap, entity, sign, False ) )
               {
                  entity->pos.y = prev;
               }
            }
         }

#if defined( VISUAL_STUDIO_DEV )
      }
#endif

      // clamp to tile map boundaries, or wrap if possible
      if ( entity->pos.x < 0 )
      {
         if ( game->tileMap.wraps )
         {
            entity->pos.x = (i32)( game->tileMap.tilesX * TILEMAP_TILE_SIZE_UNITS ) + entity->pos.x;
         }
         else
         {
            entity->pos.x = 0;
         }
      }
      else
      {
         if ( game->tileMap.wraps )
         {
            if ( entity->pos.x >= (i32)( game->tileMap.tilesX * TILEMAP_TILE_SIZE_UNITS ) )
            {
               entity->pos.x -= ( game->tileMap.tilesX * TILEMAP_TILE_SIZE_UNITS );
            }
         }
         else
         {
            if ( ( entity->pos.x + entity->pos.w ) >= (i32)( game->tileMap.tilesX * TILEMAP_TILE_SIZE_UNITS ) )
            {
               entity->pos.x = ( game->tileMap.tilesX * TILEMAP_TILE_SIZE_UNITS ) - entity->pos.w - 1;
            }
         }
      }

      if ( entity->pos.y < 0 )
      {
         if ( game->tileMap.wraps )
         {
            entity->pos.y = (i32)( game->tileMap.tilesY * TILEMAP_TILE_SIZE_UNITS ) + entity->pos.y;
         }
         else
         {
            entity->pos.y = 0;
         }
      }
      else
      {
         if ( game->tileMap.wraps )
         {
            if ( entity->pos.y >= (i32)( game->tileMap.tilesY * TILEMAP_TILE_SIZE_UNITS ) )
            {
               entity->pos.y -= (i32)( game->tileMap.tilesY * TILEMAP_TILE_SIZE_UNITS );
            }
         }
         else
         {
            if ( ( entity->pos.y + entity->pos.h ) >= (i32)( game->tileMap.tilesY * TILEMAP_TILE_SIZE_UNITS ) )
            {
               entity->pos.y = (i32)( game->tileMap.tilesY * TILEMAP_TILE_SIZE_UNITS ) - entity->pos.h - 1;
            }
         }
      }

      if ( entity == game->players->entity && ( entity->pos.x != entity->prevPos.x || entity->pos.y != entity->prevPos.y ) )
      {
         game->playerMovedCallback( game );
      }

      if ( i == -1 )
      {
         entity = game->tileMap.entities;
      }
      else
      {
         entity++;
      }
   }
}

internal Bool_t Physics_EntityCollidesWithTileMap( TileMap_t* tileMap, Entity_t* entity, i32 sign, Bool_t horizontal )
{
   u16 tile;
   i32 i, start, end, side;

   // Ramia only ever runs into the edge of the screen
   if ( entity == tileMap->playerEntities && tileMap->isOnRamia )
   {
      return False;
   }

   if ( horizontal )
   {
      start = entity->pos.y / TILEMAP_TILE_SIZE_UNITS;                    // top row
      end = ( entity->pos.y + entity->pos.h ) / TILEMAP_TILE_SIZE_UNITS;  // bottom row
      side = ( sign < 0 )
         ? entity->pos.x / TILEMAP_TILE_SIZE_UNITS                        // left side
         : ( entity->pos.x + entity->pos.w ) / TILEMAP_TILE_SIZE_UNITS;   // right side

      for ( i = start; i <= end; i++ ) // start and end rows
      {
         if ( i < 0 ) // wrap to the bottom
            if ( side < 0 ) // wrap to the right
               tile = tileMap->tiles[( tileMap->tilesX + side ) + ( ( tileMap->tilesY + i ) * tileMap->tilesX )];
            else if ( side >= (i32)tileMap->tilesX ) // wrap to the left
               tile = tileMap->tiles[( side - tileMap->tilesX ) + ( ( tileMap->tilesY + i ) * tileMap->tilesX )];
            else // no horizontal wrapping
               tile = tileMap->tiles[side + ( ( tileMap->tilesY + i ) * tileMap->tilesX )];
         else if ( i >= (i32)tileMap->tilesY ) // wrap to the top
            if ( side < 0 ) // wrap to the right
               tile = tileMap->tiles[( tileMap->tilesX + side ) + ( ( i - tileMap->tilesY ) * tileMap->tilesX )];
            else if ( side >= (i32)tileMap->tilesX ) // wrap to the left
               tile = tileMap->tiles[( side - tileMap->tilesX ) + ( ( i - tileMap->tilesY ) * tileMap->tilesX )];
            else // no horizontal wrapping
               tile = tileMap->tiles[side + ( ( i - tileMap->tilesY ) * tileMap->tilesX )];
         else // no vertical wrapping
            if ( side < 0 ) // wrap to the right
               tile = tileMap->tiles[( tileMap->tilesX + side ) + ( i * tileMap->tilesX )];
            else if ( side >= (i32)tileMap->tilesX ) // wrap to the left
               tile = tileMap->tiles[( side - tileMap->tilesX ) + ( i * tileMap->tilesX )];
            else // no horizontal wrapping
               tile = tileMap->tiles[side + ( i * tileMap->tilesX )];

         if ( !TILE_GET_IS_PASSABLE( tile ) )
         {
            return True;
         }
         else if ( entity == tileMap->playerEntities && tileMap->isOnShip == TILE_GET_IS_LAND( tile ) )
         {
            // the player can collide with either land or water, depending on whether they're on the ship
            return True;
         }
      }
   }
   else
   {
      start = entity->pos.x / TILEMAP_TILE_SIZE_UNITS;                    // left col
      end = ( entity->pos.x + entity->pos.w ) / TILEMAP_TILE_SIZE_UNITS;  // right col
      side = ( sign < 0 )
         ? entity->pos.y / TILEMAP_TILE_SIZE_UNITS                        // top side
         : ( entity->pos.y + entity->pos.h  ) / TILEMAP_TILE_SIZE_UNITS;  // bottom side

      for ( i = start; i <= end; i++ ) // start and end cols
      {
         if ( i < 0 ) // wrap to the right
            if ( side < 0 ) // wrap to the bottom
               tile = tileMap->tiles[( tileMap->tilesY + i ) + ( ( tileMap->tilesX + side ) * tileMap->tilesX )];
            else if ( side >= (i32)tileMap->tilesY ) // wrap to the top
               tile = tileMap->tiles[( tileMap->tilesY + i ) + ( ( side - tileMap->tilesX ) * tileMap->tilesX )];
            else // no vertical wrapping
               tile = tileMap->tiles[( tileMap->tilesY + i ) + ( side * tileMap->tilesX )];
         else if ( i >= (i32)tileMap->tilesX ) // wrap to the left
            if ( side < 0 ) // wrap to the bottom
               tile = tileMap->tiles[( i - tileMap->tilesY ) + ( ( tileMap->tilesX + side ) * tileMap->tilesX )];
            else if ( side >= (i32)tileMap->tilesY ) // wrap to the top
               tile = tileMap->tiles[( i - tileMap->tilesY ) + ( ( side - tileMap->tilesX ) * tileMap->tilesX )];
            else // no vertical wrapping
               tile = tileMap->tiles[( i - tileMap->tilesY ) + ( side * tileMap->tilesX )];
         else // no horizontal wrapping
            if ( side < 0 ) // wrap to the bottom
               tile = tileMap->tiles[i + ( ( tileMap->tilesY + side ) * tileMap->tilesX )];
            else if ( side >= (i32)tileMap->tilesY ) // wrap to the top
               tile = tileMap->tiles[i + ( ( side - tileMap->tilesY ) * tileMap->tilesX )];
            else // no vertical wrapping
               tile = tileMap->tiles[i + ( side * tileMap->tilesX )];

         if ( !TILE_GET_IS_PASSABLE( tile ) )
         {
            return True;
         }
         else if ( entity == tileMap->playerEntities && tileMap->isOnShip == TILE_GET_IS_LAND( tile ) )
         {
            // the player can collide with either land or water, depending on whether they're on the ship
            return True;
         }
      }
   }

   return False;
}

internal Bool_t Physics_EntityCollidesWithRigidBodies( TileMap_t* tileMap, Entity_t* entity, i32 sign, Bool_t horizontal )
{
   i32 sx, sy;
   u32 i;
   Entity_t* rigidEntity;

   // Ramia only ever runs into the edge of the screen
   if ( entity == tileMap->playerEntities && tileMap->isOnRamia )
   {
      return False;
   }

   // check player entities
   for ( i = 0, rigidEntity = tileMap->playerEntities; i < tileMap->getPlayerCountFunc( tileMap->playerCountProvider ); i++, rigidEntity++ )
   {
      if ( entity != rigidEntity &&
           entity != tileMap->playerEntities &&
           Physics_EntityCollidesWithRigidBody( tileMap, entity,
                                                rigidEntity->pos.x, rigidEntity->pos.y, rigidEntity->pos.w, rigidEntity->pos.h,
                                                sign, horizontal ) )
      {
         return True;
      }
   }

   // check other entities
   for ( i = 0, rigidEntity = tileMap->entities; i < tileMap->entityCount; i++, rigidEntity++ )
   {
      if ( entity != rigidEntity &&
           Physics_EntityCollidesWithRigidBody( tileMap, entity,
                                                rigidEntity->pos.x, rigidEntity->pos.y, rigidEntity->pos.w, rigidEntity->pos.h,
                                                sign, horizontal ) )
      {
         return True;
      }
   }

   // check unpassable static sprites
   for ( i = 0; i < tileMap->staticSpriteCount; i++ )
   {
      if ( !tileMap->staticSprites[i].isPassable )
      {
         TileMap_GetPositionOfTileIndex( tileMap, tileMap->staticSprites[i].tileIndex, &sx, &sy );

         if ( Physics_EntityCollidesWithRigidBody( tileMap, entity, (i32)sx, (i32)sy, STATIC_SPRITE_SIZE_UNITS, STATIC_SPRITE_SIZE_UNITS, sign, horizontal ) )
         {
            return True;
         }
      }
   }

   return False;
}

internal Bool_t Physics_EntityCollidesWithRigidBody( TileMap_t* tileMap, Entity_t* entity, i32 rx, i32 ry, i32 rw, i32 rh, i32 sign, Bool_t horizontal )
{
   i32 mapW = ( tileMap->tilesX * TILEMAP_TILE_SIZE_PIXELS * UNITS_PER_PIXEL );
   i32 mapH = ( tileMap->tilesY * TILEMAP_TILE_SIZE_PIXELS * UNITS_PER_PIXEL );
   Vector4i32_t entityPos = entity->pos;

   if ( horizontal )
   {
      if ( sign < 0 ) // moving left
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.x < 0 ) // entity is wrapping leftward
               entityPos.x = mapW + entityPos.x;
            if ( ( rx + rw ) >= mapW ) // rigid entity is wrapping rightward
               rx = -( mapW - rx );
            if ( Utility_VerticalLineIntersectsRect32i( entityPos.x, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.y < 0 ) { // entity is wrapping upward
               if ( Utility_VerticalLineIntersectsRect32i( entityPos.x, mapH + entityPos.y, mapH + entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.y + entityPos.h ) >= mapH ) { // entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32i( entityPos.x, -( mapH - entityPos.y ), -( mapH - entityPos.y ) + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( ry + rh ) >= mapH ) { // rigid entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32i( entityPos.x, entityPos.y, entityPos.y + entityPos.h, rx, -( mapH - ry ), rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_VerticalLineIntersectsRect32i( entityPos.x, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
            return True;
      }
      else // moving right
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.x + entityPos.w >= mapW ) // entity is wrapping rightward
               entityPos.x = -( mapW - entityPos.x );
            if ( rx < 0 ) // rigid entity is wrapping leftward
               rx = mapW + rx;
            if ( Utility_VerticalLineIntersectsRect32i( entityPos.x + entityPos.w, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.y < 0 ) { // entity is wrapping upward
               if ( Utility_VerticalLineIntersectsRect32i( entityPos.x + entityPos.w, mapH + entityPos.y, mapH + entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.y + entityPos.h ) >= mapH ) { // entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32i( entityPos.x + entityPos.w, -( mapH - entityPos.y ), -( mapH - entityPos.y ) + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( ry + rh ) >= mapH ) { // rigid entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32i( entityPos.x + entityPos.w, entityPos.y, entityPos.y + entityPos.h, rx, -( mapH - ry ), rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_VerticalLineIntersectsRect32i( entityPos.x + entityPos.w, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
            return True;
      }
   }
   else
   {
      if ( sign < 0 ) // moving up
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.y < 0 ) // entity is wrapping upward
               entityPos.y = mapH + entityPos.y;
            if ( ( ry + rh ) >= mapH ) // rigid entity is wrapping downward
               ry = -( mapH - ry );
            if ( Utility_HorizontalLineIntersectsRect32i( entityPos.x, entityPos.x + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.x < 0 ) { // entity is wrapping leftward
               if ( Utility_HorizontalLineIntersectsRect32i( mapW + entityPos.x, mapW + entityPos.x + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.x + entityPos.w ) >= mapW ) { // entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32i( -( mapW - entityPos.x ), -( mapW - entityPos.x ) + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( rx + rw ) >= mapW ) { // rigid entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32i( entityPos.x, entityPos.x + entityPos.w, entityPos.y, -( mapW - rx ), ry, rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_HorizontalLineIntersectsRect32i( entityPos.x, entityPos.x + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
            return True;
      }
      else // moving down
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.y + entityPos.h >= mapH ) // entity is wrapping downward
               entityPos.y = -( mapH - entityPos.y );
            if ( ry < 0 ) // rigid entity is wrapping upward
               ry = mapH + ry;
            if ( Utility_HorizontalLineIntersectsRect32i( entityPos.x, entityPos.x + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.x < 0 ) { // entity is wrapping leftward
               if ( Utility_HorizontalLineIntersectsRect32i( mapW + entityPos.x, mapW + entityPos.x + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.x + entityPos.w ) >= mapW ) { // entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32i( -( mapW - entityPos.x ), -( mapW - entityPos.x ) + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( rx + rw ) >= mapW ) { // rigid entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32i( entityPos.x, entityPos.x + entityPos.w, entityPos.y + entityPos.h, -( mapW - rx ), ry, rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_HorizontalLineIntersectsRect32i( entityPos.x, entityPos.x + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
            return True;
      }
   }

   return False;
}
