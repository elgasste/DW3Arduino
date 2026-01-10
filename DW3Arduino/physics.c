#include "game.h"
#include "utility.h"

internal void Physics_MoveEntities( Game_t* game );
internal Bool_t Physics_EntityCollidesWithTileMap( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal );
internal Bool_t Physics_EntityCollidesWithRigidBodies( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal );
internal Bool_t Physics_EntityCollidesWithRigidBody( TileMap_t* tileMap, Entity_t* entity, r32 rx, r32 ry, r32 rw, r32 rh, r32 sign, Bool_t horizontal );

void Physics_Tic( Game_t* game )
{
   Physics_MoveEntities( game );

   game->frontPlayer->entity->velocity.x = 0.0f;
   game->frontPlayer->entity->velocity.y = 0.0f;
}

internal void Physics_MoveEntities( Game_t* game )
{
   i32 i;
   i32 pixelsRemaining;
   r32 deltaX, deltaY, deltaRemaining, sign, prev;
   Entity_t* entity = 0;
      
   for ( i = 0; i < (i32)game->tileMap.entityCount + 1; i++ )
   {
      // clip the player first (we only need to check the player that's being controlled,
      // the other player entities should be allowed to clip)
      if ( entity == 0 )
      {
         i = -1;
         entity = game->frontPlayer->entity;
      }
      else if ( i == (i32)game->tileMap.entityCount )
      {
         break;
      }

      entity->prevPos = entity->pos;

      deltaX = entity->velocity.x * CLOCK_FRAME_SECONDS;
      deltaY = entity->velocity.y * CLOCK_FRAME_SECONDS;

#if defined( VISUAL_STUDIO_DEV )
      if ( g_winDebugFlags.noClip && entity == game->frontPlayer->entity )
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
         if ( deltaX != 0.0f )
         {
            deltaRemaining = deltaX;
            sign = ( deltaX < 0.0f ) ? -1.0f : 1.0f;
            pixelsRemaining = (i32)deltaX;

            // move full pixels first, one at a time
            while ( pixelsRemaining != 0 )
            {
               prev = entity->pos.x;
               entity->pos.x += sign;

               if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, True ) ||
                    Physics_EntityCollidesWithRigidBodies( &game->tileMap, entity, sign, True ) )
               {
                  entity->pos.x = prev;
                  break;
               }

               deltaRemaining -= sign;
               pixelsRemaining -= (i32)sign;
            }

            // move remaining sub-pixels
            if ( deltaRemaining != 0.0f )
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
         if ( deltaY != 0.0f )
         {
            deltaRemaining = deltaY;
            sign = ( deltaY < 0.0f ) ? -1.0f : 1.0f;
            pixelsRemaining = (i32)deltaY;

            // move full pixels first, one at a time
            while ( pixelsRemaining != 0 )
            {
               prev = entity->pos.y;
               entity->pos.y += sign;

               if ( Physics_EntityCollidesWithTileMap( &game->tileMap, entity, sign, False ) ||
                    Physics_EntityCollidesWithRigidBodies( &game->tileMap, entity, sign, False ) )
               {
                  entity->pos.y = prev;
                  break;
               }

               deltaRemaining -= sign;
               pixelsRemaining -= (i32)sign;
            }

            // move remaining sub-pixels
            if ( deltaRemaining != 0.0f )
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
      if ( entity->pos.x < 0.0f )
      {
         if ( game->tileMap.wraps )
         {
            entity->pos.x = ( game->tileMap.tilesX * TILEMAP_TILE_SIZE ) + entity->pos.x;
         }
         else
         {
            entity->pos.x = 0.0f;
         }
      }
      else
      {
         if ( game->tileMap.wraps )
         {
            if ( entity->pos.x >= ( game->tileMap.tilesX * TILEMAP_TILE_SIZE ) )
            {
               entity->pos.x -= ( game->tileMap.tilesX * TILEMAP_TILE_SIZE );
            }
         }
         else
         {
            if ( ( entity->pos.x + entity->pos.w ) >= ( game->tileMap.tilesX * TILEMAP_TILE_SIZE ) )
            {
               entity->pos.x = ( game->tileMap.tilesX * TILEMAP_TILE_SIZE ) - entity->pos.w - 0.01f;
            }
         }
      }

      if ( entity->pos.y < 0.0f )
      {
         if ( game->tileMap.wraps )
         {
            entity->pos.y = ( game->tileMap.tilesY * TILEMAP_TILE_SIZE ) + entity->pos.y;
         }
         else
         {
            entity->pos.y = 0.0f;
         }
      }
      else
      {
         if ( game->tileMap.wraps )
         {
            if ( entity->pos.y >= ( game->tileMap.tilesY * TILEMAP_TILE_SIZE ) )
            {
               entity->pos.y -= ( game->tileMap.tilesY * TILEMAP_TILE_SIZE );
            }
         }
         else
         {
            if ( ( entity->pos.y + entity->pos.h ) >= ( game->tileMap.tilesY * TILEMAP_TILE_SIZE ) )
            {
               entity->pos.y = ( game->tileMap.tilesY * TILEMAP_TILE_SIZE ) - entity->pos.h - 0.01f;
            }
         }
      }

      if ( entity == game->frontPlayer->entity && ( entity->pos.x != entity->prevPos.x || entity->pos.y != entity->prevPos.y ) )
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

internal Bool_t Physics_EntityCollidesWithTileMap( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal )
{
   u16 tile;
   i32 i, start, end, side;

   if ( horizontal )
   {
      start = (i32)( entity->pos.y / TILEMAP_TILE_SIZE );                    // top row
      end = (i32)( ( entity->pos.y + entity->pos.h ) / TILEMAP_TILE_SIZE );  // bottom row
      side = ( sign < 0.0f )
         ? (i32)( entity->pos.x / TILEMAP_TILE_SIZE )                        // left side
         : (i32)( ( entity->pos.x + entity->pos.w ) / TILEMAP_TILE_SIZE );   // right side

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
      }
   }
   else
   {
      start = (i32)( entity->pos.x / TILEMAP_TILE_SIZE );                    // left col
      end = (i32)( ( entity->pos.x + entity->pos.w ) / TILEMAP_TILE_SIZE );  // right col
      side = ( sign < 0.0f )
         ? (i32)( entity->pos.y / TILEMAP_TILE_SIZE )                        // top side
         : (i32)( ( entity->pos.y + entity->pos.h  ) / TILEMAP_TILE_SIZE );  // bottom side

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
      }
   }

   return False;
}

internal Bool_t Physics_EntityCollidesWithRigidBodies( TileMap_t* tileMap, Entity_t* entity, r32 sign, Bool_t horizontal )
{
   u32 i, sx, sy;
   Entity_t* rigidEntity;

   // check player entities
   for ( i = 0, rigidEntity = tileMap->playerEntities; i < tileMap->getPlayerCountFunc( tileMap->playerCountProvider ); i++, rigidEntity++ )
   {
      if ( entity != rigidEntity &&
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

         if ( Physics_EntityCollidesWithRigidBody( tileMap, entity, (r32)sx, (r32)sy, STATIC_SPRITE_SIZE, STATIC_SPRITE_SIZE, sign, horizontal ) )
         {
            return True;
         }
      }
   }

   return False;
}

internal Bool_t Physics_EntityCollidesWithRigidBody( TileMap_t* tileMap, Entity_t* entity, r32 rx, r32 ry, r32 rw, r32 rh, r32 sign, Bool_t horizontal )
{
   r32 mapW = (r32)( tileMap->tilesX * TILEMAP_TILE_SIZE );
   r32 mapH = (r32)( tileMap->tilesY * TILEMAP_TILE_SIZE );
   Vector4r32_t entityPos = entity->pos;

   if ( horizontal )
   {
      if ( sign < 0.0f ) // moving left
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.x < 0.0f ) // entity is wrapping leftward
               entityPos.x = mapW + entityPos.x;
            if ( ( rx + rw ) >= mapW ) // rigid entity is wrapping rightward
               rx = -( mapW - rx );
            if ( Utility_VerticalLineIntersectsRect32r( entityPos.x, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.y < 0.0f ) { // entity is wrapping upward
               if ( Utility_VerticalLineIntersectsRect32r( entityPos.x, mapH + entityPos.y, mapH + entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.y + entityPos.h ) >= mapH ) { // entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32r( entityPos.x, -( mapH - entityPos.y ), -( mapH - entityPos.y ) + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( ry + rh ) >= mapH ) { // rigid entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32r( entityPos.x, entityPos.y, entityPos.y + entityPos.h, rx, -( mapH - ry ), rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_VerticalLineIntersectsRect32r( entityPos.x, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
            return True;
      }
      else // moving right
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.x + entityPos.w >= mapW ) // entity is wrapping rightward
               entityPos.x = -( mapW - entityPos.x );
            if ( rx < 0.0f ) // rigid entity is wrapping leftward
               rx = mapW + rx;
            if ( Utility_VerticalLineIntersectsRect32r( entityPos.x + entityPos.w, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.y < 0.0f ) { // entity is wrapping upward
               if ( Utility_VerticalLineIntersectsRect32r( entityPos.x + entityPos.w, mapH + entityPos.y, mapH + entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.y + entityPos.h ) >= mapH ) { // entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32r( entityPos.x + entityPos.w, -( mapH - entityPos.y ), -( mapH - entityPos.y ) + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( ry + rh ) >= mapH ) { // rigid entity is wrapping downward
               if ( Utility_VerticalLineIntersectsRect32r( entityPos.x + entityPos.w, entityPos.y, entityPos.y + entityPos.h, rx, -( mapH - ry ), rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_VerticalLineIntersectsRect32r( entityPos.x + entityPos.w, entityPos.y, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
            return True;
      }
   }
   else
   {
      if ( sign < 0.0f ) // moving up
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.y < 0.0f ) // entity is wrapping upward
               entityPos.y = mapH + entityPos.y;
            if ( ( ry + rh ) >= mapH ) // rigid entity is wrapping downward
               ry = -( mapH - ry );
            if ( Utility_HorizontalLineIntersectsRect32r( entityPos.x, entityPos.x + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.x < 0.0f ) { // entity is wrapping leftward
               if ( Utility_HorizontalLineIntersectsRect32r( mapW + entityPos.x, mapW + entityPos.x + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.x + entityPos.w ) >= mapW ) { // entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32r( -( mapW - entityPos.x ), -( mapW - entityPos.x ) + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( rx + rw ) >= mapW ) { // rigid entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32r( entityPos.x, entityPos.x + entityPos.w, entityPos.y, -( mapW - rx ), ry, rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_HorizontalLineIntersectsRect32r( entityPos.x, entityPos.x + entityPos.w, entityPos.y, rx, ry, rw, rh ) )
            return True;
      }
      else // moving down
      {
         if ( tileMap->wraps )
         {
            if ( entityPos.y + entityPos.h >= mapH ) // entity is wrapping downward
               entityPos.y = -( mapH - entityPos.y );
            if ( ry < 0.0f ) // rigid entity is wrapping upward
               ry = mapH + ry;
            if ( Utility_HorizontalLineIntersectsRect32r( entityPos.x, entityPos.x + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
               return True;
            if ( entityPos.x < 0.0f ) { // entity is wrapping leftward
               if ( Utility_HorizontalLineIntersectsRect32r( mapW + entityPos.x, mapW + entityPos.x + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( entityPos.x + entityPos.w ) >= mapW ) { // entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32r( -( mapW - entityPos.x ), -( mapW - entityPos.x ) + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
                  return True;
            }
            else if ( ( rx + rw ) >= mapW ) { // rigid entity is wrapping rightward
               if ( Utility_HorizontalLineIntersectsRect32r( entityPos.x, entityPos.x + entityPos.w, entityPos.y + entityPos.h, -( mapW - rx ), ry, rw, rh ) )
                  return True;
            }
         }
         else if ( Utility_HorizontalLineIntersectsRect32r( entityPos.x, entityPos.x + entityPos.w, entityPos.y + entityPos.h, rx, ry, rw, rh ) )
            return True;
      }
   }

   return False;
}
