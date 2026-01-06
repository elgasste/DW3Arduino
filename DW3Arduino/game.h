#if !defined( GAME_H )
#define GAME_H

#define DAY_FACTOR_TOTAL_SECONDS          60
#define DAY_FACTOR_LOW_CUTOFF             0.3f
#define DAY_FACTOR_HIGH_CUTOFF            0.7f
#define DAY_FACTOR_UNDERGROUND_THRESHOLD  0.4f

#define ENCOUNTER_CHANCE_LOW              24    // 1 out of 24 chance
#define ENCOUNTER_CHANCE_MEDIUM           16    // 1 out of 16 chance
#define ENCOUNTER_CHANCE_HIGH             8     // 1 out of 8 chance

#include "common.h"
#include "clock.h"
#include "screen.h"
#include "input.h"
#include "animation.h"
#include "tile_map.h"
#include "player.h"

typedef struct Game_t
{
   Clock_t clock;
   Screen_t screen;
   Input_t input;
   AnimationChain_t animationChain;

   TileMap_t tileMap;
   Player_t player;

   Bool_t isAM;
   r32 daylightFactor;

   void ( *playerMovedCallback )( void* );
}
Game_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Game_Init( Game_t* game, u16* screenBuffer );
void Game_Tic( Game_t* game );

// game_data.c
void Game_Reset( Game_t* game );

// physics.c
void Physics_Tic( Game_t* game );

// render.c
void Render_DrawGame( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
