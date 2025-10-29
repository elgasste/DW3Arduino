#include "game.h"
#include "utility.h"

internal void Game_ClipEntityToTileMap( Game_t* game, Entity_t* entity );
internal void Game_ResolveEntityCollision( Entity_t* entity, Vector4r32_t collidingRect );

void Game_TicPhysics( Game_t* game )
{
   u32 i;
   Entity_t* entity = game->tileMap.entities;

   for ( i = 0; i < game->tileMap.entityCount; i++ )
   {
      entity->prevHitBox.x = entity->hitBox.x;
      entity->prevHitBox.y = entity->hitBox.y;
      entity->hitBox.x += ( entity->velocity.x * CLOCK_FRAME_SECONDS );
      entity->hitBox.y += ( entity->velocity.y * CLOCK_FRAME_SECONDS );

      // MUFFINS: let's just clip the player for now
      if ( i == PLAYER_ENTITY_INDEX )
      {
         Game_ClipEntityToTileMap( game, entity );
      }

      entity++;
   }

   game->playerEntity->velocity.x = 0.0f;
   game->playerEntity->velocity.y = 0.0f;

   TileMap_ClampViewportToEntity( &game->tileMap, game->playerEntity );
}

internal void Game_ClipEntityToTileMap( Game_t* game, Entity_t* entity )
{
   u32 tileRowStart, tileRowEnd, tileColStart, tileColEnd, row, col;

   tileRowStart = (u32)( entity->hitBox.y / TILE_SIZE );
   tileRowEnd = (u32)( ( entity->hitBox.y + entity->hitBox.h ) / TILE_SIZE );
   tileColStart = (u32)( entity->hitBox.x / TILE_SIZE );
   tileColEnd = (u32)( ( entity->hitBox.x + entity->hitBox.w ) / TILE_SIZE );

   // MUFFINS: we need to go over all tiles the entity could possibly be overlapping after the move.
   // after we've clipped to those, we should clip to the viewport bounds, then we're done.

   for ( row = tileRowStart; row <= tileRowEnd; row++ )
   {
      for ( col = tileColStart; col <= tileColEnd; col++ )
      {
         u32 tileIndex = col + ( row * game->tileMap.tilesX );
         u16 tile = game->tileMap.tiles[tileIndex];

         if ( !TILE_GET_IS_PASSABLE( tile ) )
         {
            Vector4r32_t tileRect =
            {
               (r32)( col * TILE_SIZE ),
               (r32)( row * TILE_SIZE ),
               (r32)TILE_SIZE,
               (r32)TILE_SIZE
            };

            Game_ResolveEntityCollision( entity, tileRect );
         }
      }
   }
}

internal void Game_ResolveEntityCollision( Entity_t* entity, Vector4r32_t collider )
{
   Vector4r32_t resolvedPos = entity->hitBox;
   Bool_t movingLeft, movingRight, movingUp, movingDown;
   Bool_t upperLeftCollision, lowerLeftCollision, upperRightCollision, lowerRightCollision;
   Bool_t leftSideCollision, rightSideCollision, topSideCollision, bottomSideCollision;
   r32 colliderR, colliderB, resolvedPosR, resolvedPosB;

   if ( !Utility_RectsIntersect32r( entity->hitBox.x, entity->hitBox.y, entity->hitBox.w, entity->hitBox.h,
                                    collider.x, collider.y, collider.w, collider.h ) )
   {
      return;
   }

   movingLeft = entity->hitBox.x < entity->prevHitBox.x;
   movingRight = entity->hitBox.x > entity->prevHitBox.x;
   movingUp = entity->hitBox.y < entity->prevHitBox.y;
   movingDown = entity->hitBox.y > entity->prevHitBox.y;

   // MUFFINS: make sure to account for the case where the entity entirely eclipses the rect
   //
   // - maybe we don't actually need to account for that? I should never happen, and even if it does,
   //   when the entity moves it'll eventually intersect the collision rect.

   // MUFFINS: TESTING NOTES
   //
   // - in this current iteration, moving down and to the left on a wall works fine, but moving up and left
   //   has issues. Similar things happen on the top side, but not on the right or on the bottom.

   if ( !movingLeft && !movingRight && !movingUp && !movingDown )
   {
      // MUFFINS: we're not moving, resolve based on center positions and depth
   }
   else
   {
      colliderR = collider.x + collider.w;
      colliderB = collider.y + collider.h;
      resolvedPosR = entity->hitBox.x + entity->hitBox.w;
      resolvedPosB = entity->hitBox.y + entity->hitBox.h;

      if ( movingLeft || movingRight )
      {
         upperLeftCollision = Utility_PointInRect32r( entity->hitBox.x, entity->hitBox.y, collider.x, collider.y, collider.w, collider.h );
         lowerLeftCollision = Utility_PointInRect32r( entity->hitBox.x, resolvedPosB, collider.x, collider.y, collider.w, collider.h );
         upperRightCollision = Utility_PointInRect32r( resolvedPosR, entity->hitBox.y, collider.x, collider.y, collider.w, collider.h );
         lowerRightCollision = Utility_PointInRect32r( resolvedPosR, resolvedPosB, collider.x, collider.y, collider.w, collider.h );

         leftSideCollision = Utility_VerticalLineIntersectsRect32r( entity->hitBox.x, entity->hitBox.y, resolvedPosB, collider.x, collider.y, collider.w, collider.h);
         rightSideCollision = Utility_VerticalLineIntersectsRect32r( resolvedPosR, entity->hitBox.y, resolvedPosB, collider.x, collider.y, collider.w, collider.h);

         // horizontal pass
         if ( leftSideCollision )
         {
            if ( ( upperLeftCollision && lowerLeftCollision ) || ( !upperLeftCollision && !lowerLeftCollision ) )
            {
               // entire left side is colliding, resolve based on direction
               resolvedPos.x = movingLeft ? colliderR : ( collider.x - resolvedPos.w );
            }
            else if ( upperLeftCollision )
            {
               if ( !upperRightCollision ) // the upperRightCollision case will be handled in the vertical pass
               {
                  // only upper-left corner is colliding. if we're moving up, resolve based on collision depth
                  if ( !movingUp || ( ( colliderR - resolvedPos.x ) < ( colliderB - resolvedPos.y ) ) )
                  {
                     resolvedPos.x = colliderR;
                  }
               }
            }
            else if ( lowerLeftCollision )
            {
               if ( !lowerRightCollision ) // the lowerRightCollision case will be handled in the vertical pass
               {
                  // only lower-left corner is colliding. if we're moving down, resolve based on collision depth
                  if ( !movingDown || ( ( colliderR - resolvedPos.x ) < ( resolvedPosB - collider.y ) ) )
                  {
                     resolvedPos.x = colliderR;
                  }
               }
            }
         }
         else if ( rightSideCollision )
         {
            if ( ( upperRightCollision && lowerRightCollision ) || ( !upperRightCollision && !lowerRightCollision ) )
            {
               // entire right side is colliding, resolve based on direction
               resolvedPos.x = movingLeft ? colliderR : ( collider.x - resolvedPos.w );
            }
            else if ( upperRightCollision )
            {
               if ( !upperLeftCollision ) // the upperLeftCollision case will be handled in the vertical pass
               {
                  // only upper-right corner is colliding. if we're moving up, resolve based on collision depth
                  if ( !movingUp || ( ( resolvedPosR - collider.x ) < ( colliderB - resolvedPos.y ) ) )
                  {
                     resolvedPos.x = ( collider.x - resolvedPos.w );
                  }
               }
            }
            else if ( lowerRightCollision )
            {
               if ( !lowerLeftCollision ) // the lowerLeftCollision case will be handled in the vertical pass
               {
                  // only lower-right corner is colliding. if we're moving down, resolve based on collision depth
                  if ( !movingDown || ( ( resolvedPosR - collider.x ) < ( resolvedPosB - collider.y ) ) )
                  {
                     resolvedPos.x = ( collider.x - resolvedPos.w );
                  }
               }
            }
         }

         entity->hitBox.x = resolvedPos.x;
         resolvedPosR = entity->hitBox.x + entity->hitBox.w;

         if ( !Utility_RectsIntersect32r( entity->hitBox.x, entity->hitBox.y, entity->hitBox.w, entity->hitBox.h,
                                          collider.x, collider.y, collider.w, collider.h ) )
         {
            return;
         }
      }

      if ( movingUp || movingDown )
      {
         upperLeftCollision = Utility_PointInRect32r( entity->hitBox.x, entity->hitBox.y, collider.x, collider.y, collider.w, collider.h );
         lowerLeftCollision = Utility_PointInRect32r( entity->hitBox.x, resolvedPosB, collider.x, collider.y, collider.w, collider.h );
         upperRightCollision = Utility_PointInRect32r( resolvedPosR, entity->hitBox.y, collider.x, collider.y, collider.w, collider.h );
         lowerRightCollision = Utility_PointInRect32r( resolvedPosR, resolvedPosB, collider.x, collider.y, collider.w, collider.h );

         topSideCollision = Utility_HorizontalLineIntersectsRect32r( entity->hitBox.x, resolvedPosR, entity->hitBox.y, collider.x, collider.y, collider.w, collider.h );
         bottomSideCollision = Utility_HorizontalLineIntersectsRect32r( entity->hitBox.x, resolvedPosR, resolvedPosB, collider.x, collider.y, collider.w, collider.h );

         // vertical pass
         if ( topSideCollision )
         {
            if ( ( upperLeftCollision && upperRightCollision ) || ( !upperLeftCollision && !upperRightCollision ) )
            {
               // entire top side is colliding, resolve based on direction
               resolvedPos.y = movingUp ? colliderB : ( collider.y - resolvedPos.h );
            }
            else if ( upperLeftCollision )
            {
               // only upper-left corner is colliding. if we're moving left, resolve based on collision depth
               if ( !movingLeft || ( ( colliderR - resolvedPos.x ) > ( colliderB - resolvedPos.y ) ) )
               {
                  resolvedPos.y = colliderB;
               }
            }
            else if ( upperRightCollision )
            {
               // only upper-right corner is colliding. if we're moving right, resolve based on collision depth
               if ( !movingRight || ( ( resolvedPosR - collider.x ) > ( colliderB - resolvedPos.y ) ) )
               {
                  resolvedPos.y = colliderB;
               }
            }
         }
         else if ( bottomSideCollision )
         {
            if ( ( lowerLeftCollision && lowerRightCollision ) || ( !lowerLeftCollision && !lowerRightCollision ) )
            {
               // entire bottom side is colliding, resolve based on direction
               resolvedPos.y = movingUp ? colliderB : ( collider.y - resolvedPos.h );
            }
            else if ( lowerLeftCollision )
            {
               // only lower-left corner is colliding. if we're moving left, resolve based on collision depth
               if ( !movingLeft || ( ( colliderR - resolvedPos.x ) > ( resolvedPosB - collider.y ) ) )
               {
                  resolvedPos.y = ( collider.y - resolvedPos.h );
               }
            }
            else if ( lowerRightCollision )
            {
               // only lower-right corner is colliding. if we're moving right, resolve based on collision depth
               if ( !movingRight || ( ( resolvedPosR - collider.x ) > ( resolvedPosB - collider.y ) ) )
               {
                  resolvedPos.y = ( collider.y - resolvedPos.h );
               }
            }
         }

         entity->hitBox.y = resolvedPos.y;
      }
   }
}
