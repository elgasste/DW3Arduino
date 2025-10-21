#include "game.h"

internal void Game_HandleInput( Game_t* game );
internal void Game_Draw( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );

   Screen_LoadPaletteFromIndex( &game->screen, 0 );
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

internal void Game_Draw( Game_t* game )
{
   Screen_WipeColor( &game->screen, COLOR16_BLACK );
   Screen_Blit( &game->screen );
}
