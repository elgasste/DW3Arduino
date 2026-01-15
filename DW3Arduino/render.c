#include "game.h"
#include "utility.h"

internal void Render_DrawTileMapLayer( Game_t* game, void ( *layerFunc )( Game_t*, i32, i32, i32, i32, i32, i32 ) );
internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels );
internal void Render_DrawStaticSpritesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels );
internal void Render_DrawEntitiesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels );
internal void Render_SortEntities( Entity_t* entities, u32 entityCount, Entity_t** sortedEntities, u32* sortCount, Bool_t reverseOrder );
internal void Render_SortEntity( Entity_t* entity, Entity_t** sortedEntities, u32* sortCount );
internal void Render_DrawSortedEntities( Game_t* game, Entity_t** sortedEntities, u32 entityCount, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels );

void Render_DrawGame( Game_t* game )
{
   Screen_WipeColor( &game->screen, SCREEN_COLOR16_BLACK );

   if ( game->state < GameState_Intro_Count )
   {
      // TODO: render intro stuff
   }
   else if ( game->state < GameState_Overworld_Count )
   {
      Render_DrawTileMapLayer( game, Render_DrawTileMapSection );
      Render_DrawTileMapLayer( game, Render_DrawStaticSpritesInSection );
      Render_DrawTileMapLayer( game, Render_DrawEntitiesInSection );
   }

   Screen_Blit( &game->screen );
}

internal void Render_DrawTileMapLayer( Game_t* game, void ( *layerFunc )( Game_t*, i32, i32, i32, i32, i32, i32 ) )
{
   i32 mapW, mapH;
   i32 x, y, w, h;

   layerFunc( game, game->tileMap.viewport.x, game->tileMap.viewport.y, game->tileMap.viewport.w, game->tileMap.viewport.h, 0, 0 );

   if ( game->tileMap.wraps )
   {
      mapW = game->tileMap.tilesX * TILEMAP_TILE_SIZE_UNITS;
      mapH = game->tileMap.tilesY * TILEMAP_TILE_SIZE_UNITS;

      if ( game->tileMap.viewport.x < 0 ) // draw "left" map
      {
         x = mapW + game->tileMap.viewport.x;
         y = game->tileMap.viewport.y;
         h = game->tileMap.viewport.h;
         w = -game->tileMap.viewport.x;
         layerFunc( game, x, y, w, h, 0, 0 );

         if ( game->tileMap.viewport.y ) // draw "top left" map
         {
            y = mapH + game->tileMap.viewport.y;
            h = -game->tileMap.viewport.y;
            layerFunc( game, x, y, w, h, 0, 0 );
         }

         if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom left" map
         {
            y = 0;
            h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
            layerFunc( game, x, y, w, h, 0, ( mapH - game->tileMap.viewport.y ) / UNITS_PER_PIXEL );
         }
      }

      if ( ( game->tileMap.viewport.x + game->tileMap.viewport.w ) >= mapW ) // draw "right" map
      {
         x = 0;
         y = game->tileMap.viewport.y;
         w = ( game->tileMap.viewport.x + game->tileMap.viewport.w ) - mapW;
         h = game->tileMap.viewport.h;
         layerFunc( game, x, y, w, h, ( mapW - game->tileMap.viewport.x ) / UNITS_PER_PIXEL, 0 );

         if ( game->tileMap.viewport.y < 0 ) // draw "top right" map
         {
            y = mapH + game->tileMap.viewport.y;
            h = -game->tileMap.viewport.y;
            layerFunc( game, x, y, w, h, ( mapW - game->tileMap.viewport.x ) / UNITS_PER_PIXEL, 0 );
         }

         if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom right" map
         {
            y = 0;
            h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
            layerFunc( game, x, y, w, h, ( mapW - game->tileMap.viewport.x ) / UNITS_PER_PIXEL, ( mapH - game->tileMap.viewport.y ) / UNITS_PER_PIXEL );
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
         layerFunc( game, x, y, w, h, 0, ( mapH - game->tileMap.viewport.y ) / UNITS_PER_PIXEL );
      }
   }
}

internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels )
{
   i32 tileOffsetPixelsX, tileOffsetPixelsY, startTileX, endTileX, startTileY, endTileY;
   u32 row, col, x, y;
   u16* tile;

   // X values
   if ( vx < 0 )
   {
      startTileX = 0;
      tileOffsetPixelsX = -( vx / UNITS_PER_PIXEL );
   }
   else
   {
      startTileX = vx / TILEMAP_TILE_SIZE_UNITS;
      tileOffsetPixelsX = -( ( vx - ( startTileX * TILEMAP_TILE_SIZE_UNITS ) ) / UNITS_PER_PIXEL );
   }

   endTileX = ( ( vx + vw ) / TILEMAP_TILE_SIZE_UNITS );

   if ( endTileX >= (i32)( game->tileMap.tilesX ) )
   {
      endTileX = (i32)( game->tileMap.tilesX - 1 );
   }

   // Y values
   if ( vy < 0 )
   {
      startTileY = 0;
      tileOffsetPixelsY = -( vy / UNITS_PER_PIXEL );
   }
   else
   {
      startTileY = vy / TILEMAP_TILE_SIZE_UNITS;
      tileOffsetPixelsY = -( ( vy - ( startTileY * TILEMAP_TILE_SIZE_UNITS ) ) / UNITS_PER_PIXEL );
   }

   endTileY = ( ( vy + vh ) / TILEMAP_TILE_SIZE_UNITS );

   if ( endTileY >= (i32)( game->tileMap.tilesY ) )
   {
      endTileY = (i32)( game->tileMap.tilesY - 1 );
   }

   // actual drawing
   tile = game->tileMap.tiles + ( startTileY * game->tileMap.tilesX ) + startTileX;

   for ( row = 0, y = game->tileMap.viewportScreenPos.y + yOffsetPixels; row <= (u32)( endTileY - startTileY ); row++, y += TILEMAP_TILE_SIZE_PIXELS )
   {
      for ( col = 0, x = game->tileMap.viewportScreenPos.x + xOffsetPixels; col <= (u32)( endTileX - startTileX ); col++, x += TILEMAP_TILE_SIZE_PIXELS )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.tileTextures[TILE_GET_TEXTURE_INDEX( *tile )].paletteIndexes,
                                    TILEMAP_TILE_SIZE_PIXELS, TILEMAP_TILE_SIZE_PIXELS,
                                    x + tileOffsetPixelsX, y + tileOffsetPixelsY,
                                    game->tileMap.viewportScreenPos.x + xOffsetPixels, game->tileMap.viewportScreenPos.y + yOffsetPixels,
                                    game->tileMap.viewportScreenPos.x + ( vw / UNITS_PER_PIXEL ) + xOffsetPixels,
                                    game->tileMap.viewportScreenPos.y + ( vh / UNITS_PER_PIXEL ) + yOffsetPixels );

         tile++;
      }

      tile += ( game->tileMap.tilesX - ( endTileX - startTileX + 1 ) );
   }
}

internal void Render_DrawStaticSpritesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels )
{
   i32 vxPixels, vyPixels, vwPixels, vhPixels, spriteX, spriteY;
   u32 i;
   StaticSprite_t* sprite = game->tileMap.staticSprites;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   vxPixels = vx / UNITS_PER_PIXEL;
   vyPixels = vy / UNITS_PER_PIXEL;
   vwPixels = vw / UNITS_PER_PIXEL;
   vhPixels = vh / UNITS_PER_PIXEL;

   for ( i = 0; i < game->tileMap.staticSpriteCount; i++ )
   {
      TileMap_GetPositionOfTileIndex( &game->tileMap, sprite->tileIndex, &spriteX, &spriteY );
      spriteX /= UNITS_PER_PIXEL;
      spriteY /= UNITS_PER_PIXEL;

      if ( Utility_RectsIntersect32i( spriteX, spriteY, spriteX + STATIC_SPRITE_SIZE_PIXELS, spriteY + STATIC_SPRITE_SIZE_PIXELS, vxPixels, vyPixels, vwPixels, vhPixels ) )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.staticSpriteTextures[sprite->textureIndex].paletteIndexes,
                                    STATIC_SPRITE_SIZE_PIXELS, STATIC_SPRITE_SIZE_PIXELS,
                                    ( spriteX - vxPixels ) + viewportScreenPos->x + xOffsetPixels,
                                    ( spriteY - vyPixels ) + viewportScreenPos->y + yOffsetPixels,
                                    viewportScreenPos->x + xOffsetPixels, viewportScreenPos->y + yOffsetPixels,
                                    viewportScreenPos->x + vwPixels + xOffsetPixels,
                                    viewportScreenPos->y + vhPixels + yOffsetPixels );
      }

      sprite++;
   }
}

internal void Render_DrawEntitiesInSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels )
{
   Entity_t* sortedEntities[TILEMAP_MAX_ENTITIES + MAX_PLAYERS];
   u32 sortCount = 0;

   Render_SortEntities( game->tileMap.entities, game->tileMap.entityCount, sortedEntities, &sortCount, False );
   Render_SortEntities( game->tileMap.playerEntities, game->playerCount, sortedEntities, &sortCount, True );

   Render_DrawSortedEntities( game, sortedEntities, sortCount, vx, vy, vw, vh, xOffsetPixels, yOffsetPixels );
}

internal void Render_SortEntities( Entity_t* entities, u32 entityCount, Entity_t** sortedEntities, u32* sortCount, Bool_t reverseOrder )
{
   i32 i;

   if ( reverseOrder )
   {
      for ( i = ( (i32)entityCount - 1 ); i >= 0; i--, ( *sortCount )++ )
      {
         Render_SortEntity( entities + i, sortedEntities, sortCount );
      }
   }
   else
   {
      for ( i = 0; i < (i32)entityCount; i++, ( *sortCount )++ )
      {
         Render_SortEntity( entities + i, sortedEntities, sortCount );
      }
   }
}

internal void Render_SortEntity( Entity_t* entity, Entity_t** sortedEntities, u32* sortCount )
{
   u32 i, j;
   Bool_t inserted = False;

   for ( i = 0; i < *sortCount; i++ )
   {
      if ( entity->pos.y < sortedEntities[i]->pos.y )
      {
         for ( j = *sortCount; j > i; j-- )
         {
            sortedEntities[j] = sortedEntities[j - 1];
         }

         sortedEntities[i] = entity;
         inserted = True;
         break;
      }
   }

   if ( !inserted )
   {
      sortedEntities[*sortCount] = entity;
   }
}

internal void Render_DrawSortedEntities( Game_t* game, Entity_t** sortedEntities, u32 entityCount, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffsetPixels, i32 yOffsetPixels )
{
   i32 xPixels, yPixels, vxPixels, vyPixels, vwPixels, vhPixels;
   u32 i, j, startPos;
   ActiveSpriteTexture_t* textures;
   Entity_t* entity;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   vxPixels = vx / UNITS_PER_PIXEL;
   vyPixels = vy / UNITS_PER_PIXEL;
   vwPixels = vw / UNITS_PER_PIXEL;
   vhPixels = vh / UNITS_PER_PIXEL;

   for ( i = 0; i < entityCount; i++ )
   {
      entity = sortedEntities[i];

      if ( Utility_RectsIntersect32i( entity->pos.x, entity->pos.y, entity->pos.w, entity->pos.h, vx, vy, vw, vh ) )
      {
         if ( entity->sprite->frame > 0 )
         {
            entity->sprite->frame = entity->sprite->frame;
         }

         startPos = ( ACTIVE_SPRITE_FRAME_PIXELS * ACTIVE_SPRITE_FRAMES ) * entity->sprite->direction + ( ACTIVE_SPRITE_FRAME_PIXELS * entity->sprite->frame );
         textures = 0;

         for ( j = 0; j < game->playerCount; j++ )
         {
            if ( entity == game->players[j].entity )
            {
               textures = game->tileMap.playerSpriteTextures;
               break;
            }
         }

         if ( !textures )
         {
            textures = game->tileMap.activeSpriteTextures;
         }

         xPixels = entity->pos.x / UNITS_PER_PIXEL;
         yPixels = entity->pos.y / UNITS_PER_PIXEL;

         Screen_DrawBoundedBuffer8( &game->screen,
                                    textures[entity->sprite->textureIndex].paletteIndexes + startPos,
                                    ACTIVE_SPRITE_FRAME_SIZE, ACTIVE_SPRITE_FRAME_SIZE,
                                    ( xPixels - vxPixels - entity->sprite->offset.x ) + viewportScreenPos->x + xOffsetPixels,
                                    ( yPixels - vyPixels - entity->sprite->offset.y ) + viewportScreenPos->y + yOffsetPixels,
                                    viewportScreenPos->x + xOffsetPixels, viewportScreenPos->y + yOffsetPixels,
                                    viewportScreenPos->x + vwPixels + xOffsetPixels,
                                    viewportScreenPos->y + vhPixels + yOffsetPixels );

#if defined( VISUAL_STUDIO_DEV )
         if ( g_winDebugFlags.showHitBoxes )
         {
            Screen_DrawBoundedRect( &game->screen,
                                    ( xPixels - vxPixels ) + viewportScreenPos->x + xOffsetPixels,
                                    ( yPixels - vyPixels ) + viewportScreenPos->y + yOffsetPixels,
                                    entity->pos.w / UNITS_PER_PIXEL, entity->pos.h / UNITS_PER_PIXEL,
                                    viewportScreenPos->x + xOffsetPixels, viewportScreenPos->y + yOffsetPixels,
                                    viewportScreenPos->x + vwPixels + xOffsetPixels,
                                    viewportScreenPos->y + vhPixels + yOffsetPixels,
                                    SCREEN_COLOR16_MAGENTA );
         }
#endif
      }
   }
}