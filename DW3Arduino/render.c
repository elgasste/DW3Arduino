#include "game.h"
#include "utility.h"

internal void Render_UpdateDayFilterIntensity( Game_t* game );
internal void Render_DrawTileMapLayer( Game_t* game, void ( *layerFunc )( Game_t*, i32, i32, i32, i32, i32, i32 ) );
internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_DrawStaticSpritesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_DrawEntitiesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_SortEntities( Entity_t* entities, u32 entityCount, Entity_t** sortedEntities, u32* sortCount );
internal void Render_DrawSortedEntities( Game_t* game, Entity_t** sortedEntities, u32 entityCount, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );

void Render_DrawGame( Game_t* game )
{
   Screen_WipeColor( &game->screen, SCREEN_COLOR16_BLACK );
   Render_UpdateDayFilterIntensity( game );

   Render_DrawTileMapLayer( game, Render_DrawTileMapSection );
   Render_DrawTileMapLayer( game, Render_DrawStaticSpritesInSection );
   Render_DrawTileMapLayer( game, Render_DrawEntitiesInSection );

   Screen_Blit( &game->screen );
}

internal void Render_UpdateDayFilterIntensity( Game_t* game )
{
   // use cutoffs to determine when the sun rises and sets
   if ( game->daylightFactor < DAY_FACTOR_LOW_CUTOFF )
   {
      game->screen.dayFilterIntensity = 0.0f;
   }
   else if ( game->daylightFactor > DAY_FACTOR_HIGH_CUTOFF )
   {
      game->screen.dayFilterIntensity = 1.0f;
   }
   else
   {
      game->screen.dayFilterIntensity = ( game->daylightFactor - DAY_FACTOR_LOW_CUTOFF ) / ( DAY_FACTOR_HIGH_CUTOFF - DAY_FACTOR_LOW_CUTOFF );
   }

   // if we're underground, don't go full-nighttime
   if ( game->tileMap.isUnderground && game->screen.dayFilterIntensity < DAY_FACTOR_UNDERGROUND_THRESHOLD )
   {
      game->screen.dayFilterIntensity = DAY_FACTOR_UNDERGROUND_THRESHOLD;
   }
}

internal void Render_DrawTileMapLayer( Game_t* game, void ( *layerFunc )( Game_t*, i32, i32, i32, i32, i32, i32 ) )
{
   i32 mapW, mapH;
   i32 x, y, w, h;

   layerFunc( game, game->tileMap.viewport.x, game->tileMap.viewport.y, game->tileMap.viewport.w, game->tileMap.viewport.h, 0, 0 );

   if ( game->tileMap.wraps )
   {
      mapW = game->tileMap.tilesX * TILEMAP_TILE_SIZE;
      mapH = game->tileMap.tilesY * TILEMAP_TILE_SIZE;

      if ( game->tileMap.viewport.x < 0 ) // draw "left" map
      {
         x = mapW + game->tileMap.viewport.x;
         y = game->tileMap.viewport.y;
         h = game->tileMap.viewport.h;
         w = -game->tileMap.viewport.x;
         layerFunc( game, x, y, w, h, 0, 0 );

         if ( game->tileMap.viewport.y < 0 ) // draw "top left" map
         {
            y = mapH + game->tileMap.viewport.y;
            h = -game->tileMap.viewport.y;
            layerFunc( game, x, y, w, h, 0, 0 );
         }

         if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom left" map
         {
            y = 0;
            h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
            layerFunc( game, x, y, w, h, 0, mapH - game->tileMap.viewport.y );
         }
      }

      if ( ( game->tileMap.viewport.x + game->tileMap.viewport.w ) >= mapW ) // draw "right" map
      {
         x = 0;
         y = game->tileMap.viewport.y;
         w = ( game->tileMap.viewport.x + game->tileMap.viewport.w ) - mapW;
         h = game->tileMap.viewport.h;
         layerFunc( game, x, y, w, h, mapW - game->tileMap.viewport.x, 0 );

         if ( game->tileMap.viewport.y < 0 ) // draw "top right" map
         {
            y = mapH + game->tileMap.viewport.y;
            h = -game->tileMap.viewport.y;
            layerFunc( game, x, y, w, h, mapW - game->tileMap.viewport.x, 0 );
         }

         if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom right" map
         {
            y = 0;
            h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
            layerFunc( game, x, y, w, h, mapW - game->tileMap.viewport.x, mapH - game->tileMap.viewport.y );
         }
      }

      x = game->tileMap.viewport.x;
      w = game->tileMap.viewport.w;

      if ( game->tileMap.viewport.y < 0 ) // draw "top" map
      {
         y = mapH + game->tileMap.viewport.y;
         h = -game->tileMap.viewport.y;
         layerFunc( game, x, y, w, h, 0, 0 );
      }

      if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom" map
      {
         y = 0;
         h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
         layerFunc( game, x, y, w, h, 0, mapH - game->tileMap.viewport.y );
      }
   }
}

internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   i32 tileOffsetX, tileOffsetY;
   i32 startTileX, endTileX, startTileY, endTileY;
   u32 row, col, x, y;
   u16* tile;

   // X values
   if ( vx < 0 )
   {
      startTileX = 0;
      tileOffsetX = -vx;
   }
   else
   {
      startTileX = vx / TILEMAP_TILE_SIZE;
      tileOffsetX = -( vx - ( startTileX * TILEMAP_TILE_SIZE ) );
   }

   endTileX = ( ( vx + vw ) / TILEMAP_TILE_SIZE );

   if ( endTileX >= (i32)( game->tileMap.tilesX ) )
   {
      endTileX = (i32)( game->tileMap.tilesX - 1 );
   }

   // Y values
   if ( vy < 0 )
   {
      startTileY = 0;
      tileOffsetY = -vy;
   }
   else
   {
      startTileY = vy / TILEMAP_TILE_SIZE;
      tileOffsetY = -( vy - ( startTileY * TILEMAP_TILE_SIZE ) );
   }

   endTileY = ( ( vy + vh ) / TILEMAP_TILE_SIZE );

   if ( endTileY >= (i32)( game->tileMap.tilesY ) )
   {
      endTileY = (i32)( game->tileMap.tilesY - 1 );
   }

   // actual drawing
   tile = game->tileMap.tiles + ( startTileY * game->tileMap.tilesX ) + startTileX;

   for ( row = 0, y = game->tileMap.viewportScreenPos.y + yOffset; row <= (u32)( endTileY - startTileY ); row++, y += TILEMAP_TILE_SIZE )
   {
      for ( col = 0, x = game->tileMap.viewportScreenPos.x + xOffset; col <= (u32)( endTileX - startTileX ); col++, x += TILEMAP_TILE_SIZE )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.tileTextures[TILE_GET_TEXTURE_INDEX( *tile )].paletteIndexes,
                                    TILEMAP_TILE_SIZE, TILEMAP_TILE_SIZE,
                                    x + tileOffsetX, y + tileOffsetY,
                                    game->tileMap.viewportScreenPos.x + xOffset, game->tileMap.viewportScreenPos.y + yOffset,
                                    game->tileMap.viewportScreenPos.x + vw + xOffset,
                                    game->tileMap.viewportScreenPos.y + vh + yOffset );

         tile++;
      }

      tile += ( game->tileMap.tilesX - ( endTileX - startTileX + 1 ) );
   }
}

internal void Render_DrawStaticSpritesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   u32 i;
   u32 spriteX, spriteY;
   StaticSprite_t* sprite = game->tileMap.staticSprites;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   for ( i = 0; i < game->tileMap.staticSpriteCount; i++ )
   {
      TileMap_GetPositionOfTileIndex( &game->tileMap, sprite->tileIndex, &spriteX, &spriteY );

      if ( Utility_RectsIntersect32i( (i32)spriteX, (i32)spriteY, (i32)( spriteX + STATIC_SPRITE_SIZE ), (i32)( spriteY + STATIC_SPRITE_SIZE ), vx, vy, vw, vh) )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.staticSpriteTextures[sprite->textureIndex].paletteIndexes,
                                    STATIC_SPRITE_SIZE, STATIC_SPRITE_SIZE,
                                    ( (i32)( spriteX ) - vx ) + viewportScreenPos->x + xOffset,
                                    ( (i32)( spriteY ) - vy ) + viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + xOffset, viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + vw + xOffset,
                                    viewportScreenPos->y + vh + yOffset );
      }

      sprite++;
   }
}

internal void Render_DrawEntitiesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   Entity_t* sortedEntities[TILEMAP_MAX_ENTITIES + TILEMAP_MAX_PLAYER_OBJECTS];
   u32 sortCount = 0;

   Render_SortEntities( game->tileMap.entities, game->tileMap.entityCount, sortedEntities, &sortCount );
   Render_SortEntities( game->tileMap.playerEntities, game->tileMap.playerCount, sortedEntities, &sortCount );
   Render_DrawSortedEntities( game, sortedEntities, sortCount, vx, vy, vw, vh, xOffset, yOffset );
}

internal void Render_SortEntities( Entity_t* entities, u32 entityCount, Entity_t** sortedEntities, u32* sortCount )
{
   u32 i, j, k;
   Bool_t inserted;
   Entity_t* entity;

   for ( i = 0; i < entityCount; i++, ( *sortCount )++ )
   {
      entity = entities + i;
      inserted = False;

      for ( j = 0; j < *sortCount; j++ )
      {
         if ( entity->pos.y < sortedEntities[j]->pos.y )
         {
            for ( k = *sortCount; k > j; k-- )
            {
               sortedEntities[k] = sortedEntities[k - 1];
            }

            sortedEntities[j] = entity;
            inserted = True;
            break;
         }
      }

      if ( !inserted )
      {
         sortedEntities[*sortCount] = entity;
      }
   }
}

internal void Render_DrawSortedEntities( Game_t* game, Entity_t** sortedEntities, u32 entityCount, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset )
{
   u32 i, startPos;
   ActiveSpriteTexture_t* textures;
   Entity_t* entity;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   for ( i = 0; i < entityCount; i++ )
   {
      entity = sortedEntities[i];

      if ( Utility_RectsIntersect32i( (i32)entity->pos.x, (i32)entity->pos.y, (i32)entity->pos.w, (i32)entity->pos.h, vx, vy, vw, vh ) )
      {
         if ( entity->sprite->frame > 0 )
         {
            entity->sprite->frame = entity->sprite->frame;
         }

         startPos = ( ACTIVE_SPRITE_FRAME_PIXELS * ACTIVE_SPRITE_FRAMES ) * entity->sprite->direction + ( ACTIVE_SPRITE_FRAME_PIXELS * entity->sprite->frame );
         textures = ( entity == game->player.entity ) ? game->tileMap.playerSpriteTextures : game->tileMap.activeSpriteTextures;

         Screen_DrawBoundedBuffer8( &game->screen,
                                    textures[entity->sprite->textureIndex].paletteIndexes + startPos,
                                    ACTIVE_SPRITE_FRAME_SIZE, ACTIVE_SPRITE_FRAME_SIZE,
                                    ( (i32)( entity->pos.x ) - vx - entity->sprite->offset.x ) + viewportScreenPos->x + xOffset,
                                    ( (i32)( entity->pos.y ) - vy - entity->sprite->offset.y ) + viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + xOffset, viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + vw + xOffset,
                                    viewportScreenPos->y + vh + yOffset );

#if defined( VISUAL_STUDIO_DEV )
         if ( g_winDebugFlags.showHitBoxes )
         {
            Screen_DrawBoundedRect( &game->screen,
                                    ( (i32)( entity->pos.x ) - vx ) + viewportScreenPos->x + xOffset,
                                    ( (i32)( entity->pos.y ) - vy ) + viewportScreenPos->y + yOffset,
                                    (i32)( entity->pos.w ), (i32)( entity->pos.h ),
                                    viewportScreenPos->x + xOffset, viewportScreenPos->y + yOffset,
                                    viewportScreenPos->x + vw + xOffset,
                                    viewportScreenPos->y + vh + yOffset,
                                    SCREEN_COLOR16_MAGENTA );
         }
#endif
      }
   }
}