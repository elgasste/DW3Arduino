#if !defined( GAME_H )
#define GAME_H

#define PLAYER_MAX_VELOCITY      64.0f    // pixels per second

#include "common.h"
#include "clock.h"
#include "screen.h"
#include "input.h"
#include "tile_map.h"

typedef struct Game_t
{
   Clock_t clock;
   Screen_t screen;
   Input_t input;
   TileMap_t tileMap;
   Entity_t* playerEntity;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Game_Init( Game_t* game, u16* screenBuffer );
void Game_Tic( Game_t* game );

// physics.c
void Physics_Tic( Game_t* game );

// render.c
void Render_DrawGame( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
