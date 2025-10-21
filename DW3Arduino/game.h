#if !defined( GAME_H )
#define GAME_H

#define CAM_VELOCITY    64.0f    // pixels per second

#include "common.h"
#include "clock.h"
#include "screen.h"
#include "input.h"
#include "tile_map.h"
#include "vector.h"

typedef struct Game_t
{
   Clock_t clock;
   Screen_t screen;
   Input_t input;
   TileMap_t tileMap;
   Vector4i32_t tileMapViewport;
   Vector2u32_t tileMapViewportScreenPos;
   Vector2r32_t camPosition;
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Game_Init( Game_t* game, u16* screenBuffer );
void Game_Tic( Game_t* game );

// render.c
void Game_Draw( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
