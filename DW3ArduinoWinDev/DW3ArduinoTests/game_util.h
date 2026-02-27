#if !defined( GAME_UTIL_H )
#define GAME_UTIL_H

#include "game.h"

#define GAMEUTIL_DEFAULT_TILEMAP_TILES_X        32
#define GAMEUTIL_DEFAULT_TILEMAP_TILES_Y        32
#define GAMEUTIL_DEFAULT_TILEMAP_VIEWPORT_W     ( SCREEN_WIDTH * UNITS_PER_PIXEL )
#define GAMEUTIL_DEFAULT_TILEMAP_VIEWPORT_H     ( SCREEN_HEIGHT * UNITS_PER_PIXEL )

Game_t* GameUtil_CreateSimpleGame();

void GameUtil_DeleteGame( Game_t* game );

#endif // GAME_UTIL_H
