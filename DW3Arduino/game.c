#include "game.h"

internal void Game_HandleInput( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   TileMap_Init( &game->tileMap );

   Screen_LoadPaletteFromIndex( &game->screen, 0 );
   TileMap_LoadTileTextures( &game->tileMap );
   TileMap_LoadFromIndex( &game->tileMap, 0 );

   game->tileMapViewport.x = 4;
   game->tileMapViewport.y = 4;
   game->tileMapViewport.w = 380;
   game->tileMapViewport.h = 220;
   game->tileMapViewportScreenPos.x = 20;
   game->tileMapViewportScreenPos.y = 10;
}

void Game_Tic( Game_t* game )
{
   Input_Read( &game->input );
   Game_HandleInput( game );
   Game_Draw( game );
}

internal void Game_HandleInput( Game_t* game )
{
   UNUSED_PARAM( game );
}
