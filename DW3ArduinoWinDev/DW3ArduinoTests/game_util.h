#if !defined( GAME_UTIL_H )
#define GAME_UTIL_H

#include "game.h"

#define GAMEUTIL_DEFAULT_TILEMAP_TILES_X        32
#define GAMEUTIL_DEFAULT_TILEMAP_TILES_Y        32

Game_t* GameUtil_CreateSimpleGame();

void GameUtil_DeleteGame( Game_t* game );

#endif // GAME_UTIL_H
