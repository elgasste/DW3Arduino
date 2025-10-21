#include "game.h"

internal void Game_DrawTileMap( Game_t* game );

void Game_Draw( Game_t* game )
{
   Screen_WipeColor( &game->screen, COLOR16_BLACK );
   Game_DrawTileMap( game );
   Screen_Blit( &game->screen );
}

internal void Game_DrawTileMap( Game_t* game )
{
   i32 tileOffsetX, tileOffsetY;
   i32 startTileX, stopTileX, startTileY, stopTileY;
   u32 row, col, x, y;
   u16* tile;

   // X values
   if ( game->tileMapViewport.x < 0 )
   {
      startTileX = 0;
      tileOffsetX = -game->tileMapViewport.x;
   }
   else
   {
      startTileX = game->tileMapViewport.x / TILE_SIZE;
      tileOffsetX = -( game->tileMapViewport.x - ( startTileX * TILE_SIZE ) );
   }

   stopTileX = ( ( game->tileMapViewport.x + game->tileMapViewport.w ) / TILE_SIZE );
   
   if ( ( ( game->tileMapViewport.x + game->tileMapViewport.w ) % TILE_SIZE ) != 0 )
   {
      stopTileX++;
   }

   if ( stopTileX >= (i32)( game->tileMap.tilesX ) )
   {
      stopTileX = (i32)( game->tileMap.tilesX - 1 );
   }

   // Y values
   if ( game->tileMapViewport.y < 0 )
   {
      startTileY = 0;
      tileOffsetY = -game->tileMapViewport.y;
   }
   else
   {
      startTileY = game->tileMapViewport.y / TILE_SIZE;
      tileOffsetY = -( game->tileMapViewport.y - ( startTileY * TILE_SIZE ) );
   }

   stopTileY = ( ( game->tileMapViewport.y + game->tileMapViewport.h ) / TILE_SIZE );

   if ( ( ( game->tileMapViewport.y + game->tileMapViewport.w ) % TILE_SIZE ) != 0 )
   {
      stopTileY++;
   }

   if ( stopTileY >= (i32)( game->tileMap.tilesY ) )
   {
      stopTileY = (i32)( game->tileMap.tilesY - 1 );
   }

   // drawing
   tile = game->tileMap.tiles + ( startTileY * game->tileMap.tilesX ) + startTileX;

   for ( row = 0, y = game->tileMapViewportScreenPos.y; row <= (u32)( stopTileY - startTileY ); row++, y += TILE_SIZE )
   {
      for ( col = 0, x = game->tileMapViewportScreenPos.x; col <= (u32)( stopTileX - startTileX ); col++, x += TILE_SIZE )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.tileTextures[*tile].paletteIndexes,
                                    TILE_SIZE, TILE_SIZE,
                                    x + tileOffsetX, y + tileOffsetY,
                                    game->tileMapViewportScreenPos.x, game->tileMapViewportScreenPos.y,
                                    game->tileMapViewportScreenPos.x + game->tileMapViewport.w,
                                    game->tileMapViewportScreenPos.y + game->tileMapViewport.h );
         tile++;
      }

      tile += ( game->tileMap.tilesX - ( stopTileX - startTileX + 1 ) );
   }

   // TODO: this can be deleted after testing movement
   /*u16* screenPos = game->screen.buffer + ( SCREEN_WIDTH * game->tileMapViewportScreenPos.y ) + game->tileMapViewportScreenPos.x;
   for ( col = 0; col < (u32)( game->tileMapViewport.w ); col++ )
   {
      *screenPos = COLOR16_MAGENTA;
      *(screenPos + ( SCREEN_WIDTH * ( game->tileMapViewport.h - 1 ) ) ) = COLOR16_MAGENTA;

      screenPos++;
   }

   screenPos = game->screen.buffer + ( SCREEN_WIDTH * game->tileMapViewportScreenPos.y ) + game->tileMapViewportScreenPos.x;
   for ( row = 0; row < (u32)( game->tileMapViewport.h ); row++ )
   {
      *screenPos = COLOR16_MAGENTA;
      *(screenPos + ( game->tileMapViewport.w - 1 ) ) = COLOR16_MAGENTA;
      screenPos += SCREEN_WIDTH;
   }*/
}
