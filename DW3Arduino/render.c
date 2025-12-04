#include "game.h"
#include "utility.h"

internal void Render_DrawTileMap( Game_t* game );
internal void Render_DrawTileMapSection( Game_t* game, i32 vx, i32 vy, i32 vw, i32 vh, i32 xOffset, i32 yOffset );
internal void Render_DrawEntities( Game_t* game );

void Render_DrawGame( Game_t* game )
{
   Screen_WipeColor( &game->screen, COLOR16_BLACK );
   Render_DrawTileMap( game );
   Render_DrawEntities( game );
   Screen_Blit( &game->screen );
}

internal void Render_DrawTileMap( Game_t* game )
{
   i32 mapW, mapH;
   i32 x, y, w, h;

   Render_DrawTileMapSection( game, game->tileMap.viewport.x, game->tileMap.viewport.y, game->tileMap.viewport.w, game->tileMap.viewport.h, 0, 0 );

   if ( game->tileMap.wraps )
   {
      mapW = game->tileMap.tilesX * TILE_SIZE;
      mapH = game->tileMap.tilesY * TILE_SIZE;

      if ( game->tileMap.viewport.x < 0 ) // draw "left" map
      {
         x = mapW + game->tileMap.viewport.x;
         y = game->tileMap.viewport.y;
         h = game->tileMap.viewport.h;
         w = -game->tileMap.viewport.x;
         Render_DrawTileMapSection( game, x, y, w, h, 0, 0 );

         if ( game->tileMap.viewport.y < 0 ) // draw "top left" map
         {
            y = mapH + game->tileMap.viewport.y;
            h = -game->tileMap.viewport.y;
            Render_DrawTileMapSection( game, x, y, w, h, 0, 0 );
         }

         if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom left" map
         {
            y = 0;
            h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
            Render_DrawTileMapSection( game, x, y, w, h, 0, mapH - game->tileMap.viewport.y );
         }
      }

      if ( ( game->tileMap.viewport.x + game->tileMap.viewport.w ) >= mapW ) // draw "right" map
      {
         x = 0;
         y = game->tileMap.viewport.y;
         w = ( game->tileMap.viewport.x + game->tileMap.viewport.w ) - mapW;
         h = game->tileMap.viewport.h;
         Render_DrawTileMapSection( game, x, y, w, h, mapW - game->tileMap.viewport.x, 0 );

         if ( game->tileMap.viewport.y < 0 ) // draw "top right" map
         {
            y = mapH + game->tileMap.viewport.y;
            h = -game->tileMap.viewport.y;
            Render_DrawTileMapSection( game, x, y, w, h, mapW - game->tileMap.viewport.x, 0 );
         }

         if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom right" map
         {
            y = 0;
            h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
            Render_DrawTileMapSection( game, x, y, w, h, mapW - game->tileMap.viewport.x, mapH - game->tileMap.viewport.y );
         }
      }

      x = game->tileMap.viewport.x;
      w = game->tileMap.viewport.w;

      if ( game->tileMap.viewport.y < 0 ) // draw "top" map
      {
         y = mapH + game->tileMap.viewport.y;
         h = -game->tileMap.viewport.y;
         Render_DrawTileMapSection( game, x, y, w, h, 0, 0 );
      }

      if ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) > mapH ) // draw "bottom" map
      {
         y = 0;
         h = ( game->tileMap.viewport.y + game->tileMap.viewport.h ) - mapH;
         Render_DrawTileMapSection( game, x, y, w, h, 0, mapH - game->tileMap.viewport.y );
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
      startTileX = vx / TILE_SIZE;
      tileOffsetX = -( vx - ( startTileX * TILE_SIZE ) );
   }

   endTileX = ( ( vx + vw ) / TILE_SIZE );

   if ( ( ( vx + vw ) % TILE_SIZE ) != 0 )
   {
      endTileX++;
   }

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
      startTileY = vy / TILE_SIZE;
      tileOffsetY = -( vy - ( startTileY * TILE_SIZE ) );
   }

   endTileY = ( ( vy + vh ) / TILE_SIZE );

   if ( ( ( vy + vw ) % TILE_SIZE ) != 0 )
   {
      endTileY++;
   }

   if ( endTileY >= (i32)( game->tileMap.tilesY ) )
   {
      endTileY = (i32)( game->tileMap.tilesY - 1 );
   }

   // drawing
   tile = game->tileMap.tiles + ( startTileY * game->tileMap.tilesX ) + startTileX;

   for ( row = 0, y = game->tileMap.viewportScreenPos.y + yOffset; row <= (u32)( endTileY - startTileY ); row++, y += TILE_SIZE )
   {
      for ( col = 0, x = game->tileMap.viewportScreenPos.x + xOffset; col <= (u32)( endTileX - startTileX ); col++, x += TILE_SIZE )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.tileTextures[TILE_GET_TEXTURE_INDEX( *tile )].paletteIndexes,
                                    TILE_SIZE, TILE_SIZE,
                                    x + tileOffsetX, y + tileOffsetY,
                                    game->tileMap.viewportScreenPos.x + xOffset, game->tileMap.viewportScreenPos.y + yOffset,
                                    game->tileMap.viewportScreenPos.x + vw + xOffset,
                                    game->tileMap.viewportScreenPos.y + vh + yOffset );

         tile++;
      }

      tile += ( game->tileMap.tilesX - ( endTileX - startTileX + 1 ) );
   }
}

// MUFFINS: there's more to do here for wrapping tile maps
internal void Render_DrawEntities( Game_t* game )
{
   u32 i;
   Entity_t* entity = game->tileMap.entities;
   Vector4i32_t* viewport = &game->tileMap.viewport;
   Vector2u32_t* viewportScreenPos = &game->tileMap.viewportScreenPos;

   for ( i = 0; i < game->tileMap.entityCount; i++ )
   {
      if ( Utility_RectsIntersect32i( (i32)entity->pos.x, (i32)entity->pos.y, (i32)entity->pos.w, (i32)entity->pos.h,
                                      viewport->x, viewport->y, viewport->w, viewport->h ) )
      {
         Screen_DrawBoundedRect( &game->screen,
                                 ( (i32)( entity->pos.x ) - viewport->x ) + viewportScreenPos->x,
                                 ( (i32)( entity->pos.y ) - viewport->y ) + viewportScreenPos->y,
                                 (i32)( entity->pos.w ), (i32)( entity->pos.h ),
                                 viewportScreenPos->x, viewportScreenPos->y,
                                 viewportScreenPos->x + viewport->w,
                                 viewportScreenPos->y + viewport->h,
                                 ( entity == game->playerEntity ) ? COLOR16_MAGENTA : COLOR16_YELLOW );
      }
      entity++;
   }
}
