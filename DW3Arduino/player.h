#if !defined( PLAYER_H )
#define PLAYER_H

#define PLAYER_MAX_VELOCITY      56.0f    // pixels per second

#include "common.h"

typedef struct Entity_t Entity_t;

typedef struct Player_t
{
   Entity_t* entity;
   u32 tileIndex;
}
Player_t;

#endif // PLAYER_H
