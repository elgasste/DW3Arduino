#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enemy.h"

#define BATTLE_MAX_ENEMIES    8

typedef struct Battle_t
{
   Enemy_t enemies[BATTLE_MAX_ENEMIES];
}
Battle_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Battle_Generate( Battle_t* battle );

#if defined( __cplusplus )
}
#endif

#endif // BATTLE_H
