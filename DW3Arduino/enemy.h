#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "battle_stats.h"

#define ENEMY_MAX_NAME_LENGTH    31

typedef struct Enemy_t
{
   char name[ENEMY_MAX_NAME_LENGTH + 1];
   BattleStats_t stats;
}
Enemy_t;

#if defined( __cplusplus )
extern "C" {
#endif

// game_data.c
void Enemy_LoadFromId( Enemy_t* enemy, u32 id );

#if defined( __cplusplus )
}
#endif

#endif // ENEMY_H
