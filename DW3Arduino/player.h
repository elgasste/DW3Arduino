#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"

typedef struct Entity_t Entity_t;

typedef enum PlayerClass_t
{
   PlayerClass_Hero = 0,
   PlayerClass_Soldier,
   PlayerClass_Pilgrim,
   PlayerClass_Wizard,
   PlayerClass_Merchant,
   PlayerClass_Fighter,
   PlayerClass_Thief,
   PlayerClass_GoofOff,
   PlayerClass_Sage,

   PlayerClass_Count
}
PlayerClass_t;

typedef struct Player_t
{
   PlayerClass_t class;
   Entity_t* entity;
   u32 tileIndex;
}
Player_t;

#endif // PLAYER_H
