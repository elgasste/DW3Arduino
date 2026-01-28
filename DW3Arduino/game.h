#if !defined( GAME_H )
#define GAME_H

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

   u32 saveSlot;

   Player_t players[MAX_PLAYERS];
   u32 playerCount;

   GameState_t state;
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

// input.c
void Input_HandleInput( Game_t* game );

// physics.c
void Physics_Tic( Game_t* game );

// render.c
void Render_DrawGame( Game_t* game );

// platform-dependent storage
Bool_t Storage_SaveGame( Game_t* game );
Bool_t Storage_LoadGame( Game_t* game, u32 slot );
Bool_t Storage_DeleteSlot( u32 slot );

// validate_save.c
Bool_t Validate_PlayerCount( i32 count );
Bool_t Validate_PlayerName( const char* name );
Bool_t Validate_PlayerClass( i32 playerClass );
Bool_t Validate_SingleHero( Game_t* game );

#if defined( __cplusplus )
}
#endif

#endif // GAME_H
