#if !defined( PLAYER_H )
#define PLAYER_H

#define PLAYER_MAX_VELOCITY      64.0f    // pixels per second

#include "common.h"

typedef struct Entity_t Entity_t;

typedef struct Player_t
{
   Entity_t* entity;
}
Player_t;

#endif // PLAYER_H
