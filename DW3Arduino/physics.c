#include "game.h"
#include "utility.h"

#define COLLISION_THETA    0.001f

internal void Game_MoveAndClipEntity( Game_t* game, Entity_t* entity );
internal void Game_ClipEntities( Entity_t* entity, Entity_t* otherEntity, r32 prevX, r32 prevY, r32* newX, r32* newY );

void Game_TicPhysics( Game_t* game )
{
   u32 i;

   for ( i = 0; i < game->tileMap.entityCount; i++ )
   {
      Game_MoveAndClipEntity( game, &game->tileMap.entities[i] );
   }

   game->playerEntity->velocity.x = 0.0f;
   game->playerEntity->velocity.y = 0.0f;

   TileMap_ClampViewportToEntity( &game->tileMap, game->playerEntity );
}

internal void Game_MoveAndClipEntity( Game_t* game, Entity_t* entity )
{
   u32 tileRowStart, tileRowEnd, tileColStart, tileColEnd, row, col, tile, tileIndex, i;
   r32 newX, newY;
   Bool_t checkCollision;
   Entity_t* otherEntity;

   newX = entity->hitBox.x + ( entity->velocity.x * CLOCK_FRAME_SECONDS );
   newY = entity->hitBox.y + ( entity->velocity.y * CLOCK_FRAME_SECONDS );

   // clip to other entities
   otherEntity = game->tileMap.entities;
   checkCollision = False;

   for ( i = 0; i < game->tileMap.entityCount; i++ )
   {
      if ( !checkCollision )
      {
         // only check collisions with entities AFTER the current one in the array
         if ( otherEntity == entity )
         {
            checkCollision = True;
         }

         otherEntity++;
      }
      else
      {
         Game_ClipEntities( entity, otherEntity, entity->hitBox.x, entity->hitBox.y, &newX, &newY );
         otherEntity++;
      }
   }

   // clip to unpassable horizontal tiles
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
      // moving right (or standing still), check rightward tiles
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

   entity->hitBox.x = newX;

   // clip to unpassable vertical tiles
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
      // moving down (or standing still), check downward tiles
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

#define PHYSICS_CLIPENTITY_TOPLEFT() if ( *newX < prevX ) if ( *newY < prevY ) if ( ( clipHitBoxR - *newX ) > ( clipHitBoxB - *newY ) ) *newY = clipHitBoxB + COLLISION_THETA; else *newX = clipHitBoxR + COLLISION_THETA; else *newX = clipHitBoxR + COLLISION_THETA; else *newY = clipHitBoxB + COLLISION_THETA
#define PHYSICS_CLIPENTITY_TOPRIGHT() if ( *newX > prevX ) if ( *newY < prevY ) if ( ( mainHitBoxR - clipHitBoxX ) > ( clipHitBoxB - *newY ) ) *newY = clipHitBoxB + COLLISION_THETA; else *newX = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else *newX = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else *newY = clipHitBoxY + clipHitBoxH + COLLISION_THETA
#define PHYSICS_CLIPENTITY_BOTTOMLEFT() if ( *newX < prevX ) if ( *newY > prevY ) if ( ( clipHitBoxR - *newX ) > ( mainHitBoxB - clipHitBoxY ) ) *newY = clipHitBoxY - mainHitBoxH - COLLISION_THETA; else *newX = clipHitBoxR + COLLISION_THETA; else *newX = clipHitBoxR + COLLISION_THETA; else *newY = clipHitBoxY - mainHitBoxH - COLLISION_THETA
#define PHYSICS_CLIPENTITY_BOTTOMRIGHT() if ( *newX > prevX ) if ( *newY > prevY ) if ( ( mainHitBoxR - clipHitBoxX ) > ( mainHitBoxB - clipHitBoxY ) ) *newY = clipHitBoxY - mainHitBoxH - COLLISION_THETA; else *newX = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else *newX = clipHitBoxX - mainHitBoxW - COLLISION_THETA; else *newY = clipHitBoxY - mainHitBoxH - COLLISION_THETA

internal void Game_ClipEntities( Entity_t* entity, Entity_t* otherEntity, r32 prevX, r32 prevY, r32* newX, r32* newY )
{
   r32 clipHitBoxX = otherEntity->hitBox.x;
   r32 clipHitBoxY = otherEntity->hitBox.y;
   r32 clipHitBoxW = (r32)( otherEntity->hitBox.w );
   r32 clipHitBoxH = (r32)( otherEntity->hitBox.h );
   r32 clipHitBoxR = clipHitBoxX + clipHitBoxW;
   r32 clipHitBoxB = clipHitBoxY + clipHitBoxH;
   r32 mainHitBoxW = (r32)( entity->hitBox.w );
   r32 mainHitBoxH = (r32)( entity->hitBox.h );
   r32 mainHitBoxR = *newX + mainHitBoxW;
   r32 mainHitBoxB = *newY + mainHitBoxH;

   if ( *newX < clipHitBoxR && *newX > clipHitBoxX && *newY < clipHitBoxB && mainHitBoxB > clipHitBoxY )
   {
      // left side is colliding
      if ( Utility_PointInRectF( *newX, *newY, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_TOPLEFT();
      }
      else if ( Utility_PointInRectF( *newX, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_BOTTOMLEFT();
      }
      else
      {
         *newX = clipHitBoxR + COLLISION_THETA;
      }
   }
   else if ( *newY < clipHitBoxB && *newY > clipHitBoxY && *newX < clipHitBoxR && mainHitBoxR > clipHitBoxX )
   {
      // top side is colliding
      if ( Utility_PointInRectF( *newX, *newY, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_TOPLEFT();
      }
      else if ( Utility_PointInRectF( mainHitBoxR, *newY, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_TOPRIGHT();
      }
      else
      {
         *newY = clipHitBoxB + COLLISION_THETA;
      }
   }
   else if ( mainHitBoxR > clipHitBoxX && mainHitBoxR < clipHitBoxR && *newY < clipHitBoxB && mainHitBoxB > clipHitBoxY )
   {
      // right side is colliding
      if ( Utility_PointInRectF( mainHitBoxR, *newY, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_TOPRIGHT();
      }
      else if ( Utility_PointInRectF( mainHitBoxR, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_BOTTOMRIGHT();
      }
      else
      {
         *newX = clipHitBoxX - mainHitBoxW - COLLISION_THETA;
      }
   }
   else if ( mainHitBoxB > clipHitBoxY && mainHitBoxB < clipHitBoxB && *newX < clipHitBoxR && mainHitBoxR > clipHitBoxX )
   {
      // bottom side is colliding
      if ( Utility_PointInRectF( *newX, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_BOTTOMLEFT();
      }
      else if ( Utility_PointInRectF( mainHitBoxR, mainHitBoxB, clipHitBoxX, clipHitBoxY, clipHitBoxW, clipHitBoxH ) )
      {
         PHYSICS_CLIPENTITY_BOTTOMRIGHT();
      }
      else
      {
         *newY = clipHitBoxY - mainHitBoxH - COLLISION_THETA;
      }
   }
}
