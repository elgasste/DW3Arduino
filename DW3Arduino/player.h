#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "direction.h"

#define PLAYER_MOVE_HISTORY_SIZE    16

typedef struct Entity_t Entity_t;

typedef struct PlayerMovement_t
{
   Vector2r32_t newPos;
   Direction_t newDir;
}
PlayerMovement_t;

typedef enum PlayerClass_t
{
   PlayerClass_Hero = 0,
   PlayerClass_Soldier,
   PlayerClass_Pilgrim,
   PlayerClass_Wizard,
   PlayerClass_Fighter,
   PlayerClass_Merchant,
   PlayerClass_GoofOff,

   PlayerClass_Count
}
PlayerClass_t;

typedef struct Player_t
{
   PlayerClass_t class;
   Entity_t* entity;
   u32 tileIndex;

   PlayerMovement_t moveHistory[PLAYER_MOVE_HISTORY_SIZE];
   u32 moveHistoryIndex;
   u32 moveChainIndex;
   Bool_t chainNextPlayer;
}
Player_t;

#endif // PLAYER_H
