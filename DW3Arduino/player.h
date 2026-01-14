#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "tile_map.h"

#define PLAYER_MOVE_HISTORY_SIZE    16

typedef struct Entity_t Entity_t;

typedef struct PlayerMovement_t
{
   Vector2r32_t newPos;
   Direction_t newDir;
   TileDamageRate_t tileDamageRate;
}
PlayerMovement_t;

typedef struct Player_t
{
   PlayerClass_t playerClass;
   Entity_t* entity;
   u32 tileIndex;

   PlayerMovement_t moveHistory[PLAYER_MOVE_HISTORY_SIZE];
   u32 moveHistoryIndex;
   u32 moveChainIndex;
   Bool_t chainNextPlayer;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_ResetChaining( Player_t* player );
void Player_ApplyTileDamage( Player_t* player );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
