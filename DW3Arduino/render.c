#include "game.h"

internal void Game_DrawTileMap( Game_t* game );
internal void Game_DrawPlayer( Game_t* game );

void Game_Draw( Game_t* game )
{
   Screen_WipeColor( &game->screen, COLOR16_BLACK );
   Game_DrawTileMap( game );
   Game_DrawPlayer( game );
   Screen_Blit( &game->screen );
}

internal void Game_DrawTileMap( Game_t* game )
{
   i32 tileOffsetX, tileOffsetY;
   i32 startTileX, endTileX, startTileY, endTileY;
   u32 row, col, x, y;
   u16* tile;

   // X values
   if ( game->tileMap.viewport.x < 0 )
   {
      startTileX = 0;
      tileOffsetX = -game->tileMap.viewport.x;
   }
   else
   {
      startTileX = game->tileMap.viewport.x / TILE_SIZE;
      tileOffsetX = -( game->tileMap.viewport.x - ( startTileX * TILE_SIZE ) );
   }

   endTileX = ( ( game->tileMap.viewport.x + game->tileMap.viewport.w ) / TILE_SIZE );
   
   if ( ( ( game->tileMap.viewport.x + game->tileMap.viewport.w ) % TILE_SIZE ) != 0 )
   {
      endTileX++;
   }

   if ( endTileX >= (i32)( game->tileMap.tilesX ) )
   {
      endTileX = (i32)( game->tileMap.tilesX - 1 );
   }

   // Y values
   if ( game->tileMap.viewport.y < 0 )
   {
      startTileY = 0;
      tileOffsetY = -game->tileMap.viewport.y;
   }
   else
   {
      startTileY = game->tileMap.viewport.y / TILE_SIZE;
      tileOffsetY = -( game->tileMap.viewport.y - ( startTileY * TILE_SIZE ) );
   }

   endTileY = ( ( game->tileMap.viewport.y + game->tileMap.viewport.h ) / TILE_SIZE );

   if ( ( ( game->tileMap.viewport.y + game->tileMap.viewport.w ) % TILE_SIZE ) != 0 )
   {
      endTileY++;
   }

   if ( endTileY >= (i32)( game->tileMap.tilesY ) )
   {
      endTileY = (i32)( game->tileMap.tilesY - 1 );
   }

   // drawing
   tile = game->tileMap.tiles + ( startTileY * game->tileMap.tilesX ) + startTileX;

   for ( row = 0, y = game->tileMap.viewportScreenPos.y; row <= (u32)( endTileY - startTileY ); row++, y += TILE_SIZE )
   {
      for ( col = 0, x = game->tileMap.viewportScreenPos.x; col <= (u32)( endTileX - startTileX ); col++, x += TILE_SIZE )
      {
         Screen_DrawBoundedBuffer8( &game->screen,
                                    game->tileMap.tileTextures[TILE_GET_TEXTURE_INDEX( *tile )].paletteIndexes,
                                    TILE_SIZE, TILE_SIZE,
                                    x + tileOffsetX, y + tileOffsetY,
                                    game->tileMap.viewportScreenPos.x, game->tileMap.viewportScreenPos.y,
                                    game->tileMap.viewportScreenPos.x + game->tileMap.viewport.w,
                                    game->tileMap.viewportScreenPos.y + game->tileMap.viewport.h );
         tile++;
      }

      tile += ( game->tileMap.tilesX - ( endTileX - startTileX + 1 ) );
   }
}

internal void Game_DrawPlayer( Game_t* game )
{
   Screen_DrawRect( &game->screen,
                    ( (i32)( game->playerEntity.hitBox.x ) - game->tileMap.viewport.x ) + game->tileMap.viewportScreenPos.x,
                    ( (i32)( game->playerEntity.hitBox.y ) - game->tileMap.viewport.y ) + game->tileMap.viewportScreenPos.y,
                    (i32)( game->playerEntity.hitBox.w ),
                    (i32)( game->playerEntity.hitBox.h ),
                    COLOR16_YELLOW );
}
