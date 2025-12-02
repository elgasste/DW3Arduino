#include "game.h"
#include "utility.h"

internal void Game_ClipEntityToEntity( Entity_t* entity, Entity_t* rigidEntity, Bool_t movingLeft, Bool_t movingRight, Bool_t movingUp, Bool_t movingDown );
internal void Game_ClipEntityToTileMap( Game_t* game, Entity_t* entity, Bool_t movingLeft, Bool_t movingRight, Bool_t movingUp, Bool_t movingDown );

void Game_TicPhysics( Game_t* game )
{
   u32 i, j;
   Entity_t *entity, *rigidEntity;
   Bool_t movingLeft, movingRight, movingUp, movingDown;

   // move entities and do clipping
   for ( i = 0, entity = game->tileMap.entities; i < game->tileMap.entityCount; i++, entity++ )
   {
      entity->prevHitBox.x = entity->hitBox.x;
      entity->prevHitBox.y = entity->hitBox.y;
      entity->hitBox.x += ( entity->velocity.x * CLOCK_FRAME_SECONDS );
      entity->hitBox.y += ( entity->velocity.y * CLOCK_FRAME_SECONDS );

      movingLeft = entity->hitBox.x < entity->prevHitBox.x;
      movingRight = entity->hitBox.x > entity->prevHitBox.x;
      movingUp = entity->hitBox.y < entity->prevHitBox.y;
      movingDown = entity->hitBox.y > entity->prevHitBox.y;

      for ( j = 0, rigidEntity = game->tileMap.entities; j < game->tileMap.entityCount; j++, rigidEntity++ )
      {
         if ( entity != rigidEntity )
         {
            Game_ClipEntityToEntity( entity, rigidEntity, movingLeft, movingRight, movingUp, movingDown );
         }
      }

      Game_ClipEntityToTileMap( game, entity, movingLeft, movingRight, movingUp, movingDown );
   }

   // reset player velocity and clamp the viewport to the player
   game->playerEntity->velocity.x = 0.0f;
   game->playerEntity->velocity.y = 0.0f;

   TileMap_ClampViewportToEntity( &game->tileMap, game->playerEntity );
}

internal void Game_ClipEntityToEntity( Entity_t* entity, Entity_t* obstacle, Bool_t movingLeft, Bool_t movingRight, Bool_t movingUp, Bool_t movingDown )
{
   Vector4r32_t collider = obstacle->hitBox;
   Vector4r32_t resolvedPos = entity->hitBox;
   Bool_t upperLeftCollision, lowerLeftCollision, upperRightCollision, lowerRightCollision;
   Bool_t leftSideCollision, rightSideCollision, topSideCollision, bottomSideCollision;
   r32 colliderR, colliderB, resolvedPosR, resolvedPosB;

   if ( !Utility_RectsIntersect32r( entity->hitBox.x, entity->hitBox.y, entity->hitBox.w, entity->hitBox.h,
                                    collider.x, collider.y, collider.w, collider.h ) )
   {
      return;
   }

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
         if ( ( upperLeftCollision && lowerLeftCollision ) || ( !upperLeftCollision && !lowerLeftCollision ) ) // entire left side is colliding
         {
            resolvedPos.x = colliderR;
         }
         else if ( upperLeftCollision )
         {
            if ( !upperRightCollision ) // the lowerLeftCollision case will be handled in the vertical pass
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
         if ( ( upperRightCollision && lowerRightCollision ) || ( !upperRightCollision && !lowerRightCollision ) ) // entire right side is colliding
         {
            resolvedPos.x = collider.x - resolvedPos.w - COLLISION_THETA;
         }
         else if ( upperRightCollision )
         {
            if ( !upperLeftCollision ) // the upperLeftCollision case will be handled in the vertical pass
            {
               // only upper-right corner is colliding. if we're moving up, resolve based on collision depth
               if ( !movingUp || ( ( resolvedPosR - collider.x ) < ( colliderB - resolvedPos.y ) ) )
               {
                  resolvedPos.x = ( collider.x - resolvedPos.w - COLLISION_THETA );
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
                  resolvedPos.x = ( collider.x - resolvedPos.w - COLLISION_THETA );
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
         if ( ( upperLeftCollision && upperRightCollision ) || ( !upperLeftCollision && !upperRightCollision ) ) // entire top side is colliding
         {
            resolvedPos.y = colliderB;
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
         if ( ( lowerLeftCollision && lowerRightCollision ) || ( !lowerLeftCollision && !lowerRightCollision ) ) // entire bottom side is colliding
         {
            resolvedPos.y = collider.y - resolvedPos.h - COLLISION_THETA;
         }
         else if ( lowerLeftCollision )
         {
            // only lower-left corner is colliding. if we're moving left, resolve based on collision depth
            if ( !movingLeft || ( ( colliderR - resolvedPos.x ) > ( resolvedPosB - collider.y ) ) )
            {
               resolvedPos.y = ( collider.y - resolvedPos.h - COLLISION_THETA );
            }
         }
         else if ( lowerRightCollision )
         {
            // only lower-right corner is colliding. if we're moving right, resolve based on collision depth
            if ( !movingRight || ( ( resolvedPosR - collider.x ) > ( resolvedPosB - collider.y ) ) )
            {
               resolvedPos.y = ( collider.y - resolvedPos.h - COLLISION_THETA );
            }
         }
      }

      entity->hitBox.y = resolvedPos.y;
   }
}

// IMPORTANT NOTE:
//
// This type of clipping only works if the entity's hit box is smaller than the tile size,
// so it's very important not to make any entities that are larger than a map tile!
internal void Game_ClipEntityToTileMap( Game_t* game, Entity_t* entity, Bool_t movingLeft, Bool_t movingRight, Bool_t movingUp, Bool_t movingDown )
{
   r32 xDepth, yDepth;

   u32 topTileRow = (u32)( entity->hitBox.y / TILE_SIZE );
   u32 bottomTileRow = (u32)( ( entity->hitBox.y + entity->hitBox.h ) / TILE_SIZE );
   u32 leftTileCol = (u32)( entity->hitBox.x / TILE_SIZE );
   u32 rightTileCol = (u32)( ( entity->hitBox.x + entity->hitBox.w  ) / TILE_SIZE );

   u16 upperLeftTile = game->tileMap.tiles[leftTileCol + ( topTileRow * game->tileMap.tilesX )];
   u16 upperRightTile = game->tileMap.tiles[rightTileCol + ( topTileRow * game->tileMap.tilesX )];
   u16 lowerLeftTile = game->tileMap.tiles[leftTileCol + ( bottomTileRow * game->tileMap.tilesX )];
   u16 lowerRightTile = game->tileMap.tiles[rightTileCol + ( bottomTileRow * game->tileMap.tilesX )];

   Bool_t upperLeftPassable = TILE_GET_IS_PASSABLE( upperLeftTile );
   Bool_t upperRightPassable = TILE_GET_IS_PASSABLE( upperRightTile );
   Bool_t lowerLeftPassable = TILE_GET_IS_PASSABLE( lowerLeftTile );
   Bool_t lowerRightPassable = TILE_GET_IS_PASSABLE( lowerRightTile );

   if ( upperLeftPassable && upperRightPassable && lowerLeftPassable && lowerRightPassable )
   {
      return;
   }

   movingLeft = entity->hitBox.x < entity->prevHitBox.x;
   movingRight = entity->hitBox.x > entity->prevHitBox.x;
   movingUp = entity->hitBox.y < entity->prevHitBox.y;
   movingDown = entity->hitBox.y > entity->prevHitBox.y;

   if ( !upperLeftPassable && !upperRightPassable && !lowerLeftPassable && !lowerRightPassable ) // fully surrounded by un-passable tiles
   {
      // this shouldn't be possible, but let's not clip just in case, or the entity could get stuck.
      return;
   }
   else if ( !upperRightPassable && !lowerLeftPassable )
   {
      if ( !lowerRightPassable || ( movingRight || movingDown ) ) // colliding with a lower-right corner (presumably)
      {
         entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
         entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
      }
      else if ( !upperLeftPassable || ( movingUp || movingLeft ) ) // colliding with an upper-left corner (presumably)
      {
         entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
         entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
      }
   }
   else if ( !upperLeftPassable && !lowerRightPassable )
   {
      if ( !lowerLeftPassable || ( movingLeft || movingDown ) ) // colliding with a lower-left corner (presumably)
      {
         entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
         entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
      }
      else if ( !upperRightPassable || ( movingRight || movingUp ) ) // colliding with an upper-right corner (presumably)
      {
         entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
         entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
      }
   }
   else if ( !upperLeftPassable )
   {
      if ( !lowerLeftPassable ) // colliding with left wall
      {
         entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
      }
      else if ( !upperRightPassable ) // colliding with upper wall
      {
         entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
      }
      else // colliding with upper-left tile only
      {
         if ( movingLeft && movingUp )
         {
            xDepth = ( ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE ) - entity->hitBox.x;
            yDepth = ( ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE ) - entity->hitBox.y;

            if ( xDepth > yDepth )
            {
               entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
            }
            else
            {
               entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
            }
         }
         else if ( movingLeft )
         {
            entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
         }
         else
         {
            entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
         }
      }
   }
   else if ( !lowerLeftPassable )
   {
      if ( !lowerRightPassable ) // colliding with lower wall
      {
         entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
      }
      else if ( !upperRightPassable ) // colliding with right wall
      {
         entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
      }
      else // colliding with lower-left tile only
      {
         if ( movingLeft && movingDown )
         {
            xDepth = ( ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE ) - entity->hitBox.x;
            yDepth = ( entity->hitBox.y + entity->hitBox.h ) - ( ( (r32)bottomTileRow * TILE_SIZE ) );

            if ( xDepth > yDepth )
            {
               entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
            }
            else
            {
               entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
            }
         }
         else if ( movingLeft )
         {
            entity->hitBox.x = ( (r32)leftTileCol * TILE_SIZE ) + TILE_SIZE;
         }
         else
         {
            entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
         }
      }
   }
   else if ( !upperRightPassable )
   {
      if ( !lowerRightPassable ) // colliding with right wall
      {
         entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
      }
      else // colliding with upper-right tile only
      {
         if ( movingRight && movingUp )
         {
            xDepth = ( entity->hitBox.x + entity->hitBox.w ) - ( ( (r32)rightTileCol * TILE_SIZE ) );
            yDepth = ( ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE ) - entity->hitBox.y;

            if ( xDepth > yDepth )
            {
               entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
            }
            else
            {
               entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
            }
         }
         else if ( movingRight )
         {
            entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
         }
         else
         {
            entity->hitBox.y = ( (r32)topTileRow * TILE_SIZE ) + TILE_SIZE;
         }
      }
   }
   else if ( !lowerRightPassable )
   {
      if ( !lowerLeftPassable ) // colliding with lower wall
      {
         entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
      }
      else // colliding with lower-right tile only
      {
         if ( movingRight && movingDown )
         {
            xDepth = ( entity->hitBox.x + entity->hitBox.w ) - ( ( (r32)rightTileCol * TILE_SIZE ) );
            yDepth = ( entity->hitBox.y + entity->hitBox.h ) - ( ( (r32)bottomTileRow * TILE_SIZE ) );

            if ( xDepth > yDepth )
            {
               entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
            }
            else
            {
               entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
            }
         }
         else if ( movingRight )
         {
            entity->hitBox.x = ( (r32)rightTileCol * TILE_SIZE ) - entity->hitBox.w - COLLISION_THETA;
         }
         else
         {
            entity->hitBox.y = ( (r32)bottomTileRow * TILE_SIZE ) - entity->hitBox.h - COLLISION_THETA;
         }
      }
   }

   // clip to tile map boundaries
   if ( entity->hitBox.x < 0.0f )
   {
      entity->hitBox.x = 0.0f;
   }
   else if ( ( entity->hitBox.x + entity->hitBox.w ) >= (r32)( game->tileMap.tilesX * TILE_SIZE ) )
   {
      entity->hitBox.x = ( r32 )( ( game->tileMap.tilesX * TILE_SIZE ) - entity->hitBox.w ) - COLLISION_THETA;
   }

   if ( entity->hitBox.y < 0.0f )
   {
      entity->hitBox.y = 0.0f;
   }
   else if ( ( entity->hitBox.y + entity->hitBox.h ) >= (r32)( game->tileMap.tilesY * TILE_SIZE ) )
   {
      entity->hitBox.y = ( r32 )( ( game->tileMap.tilesY * TILE_SIZE ) - entity->hitBox.h ) - COLLISION_THETA;
   }
}
