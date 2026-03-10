#if !defined( PLAYER_H )
#define PLAYER_H

#include "common.h"
#include "vector.h"
#include "tile_map.h"
#include "battle_stats.h"

#define PLAYER_STATS_DEFAULT_HP        10
#define PLAYER_STATS_DEFAULT_MP        0

#define PLAYER_MAX_EXPERIENCE          9999999

typedef struct Entity_t Entity_t;

typedef struct PlayerMovement_t
{
   Vector2i32_t newPos;
   Direction_t newDir;
   TileDamageRate_t tileDamageRate;
}
PlayerMovement_t;

typedef struct Player_t
{
   PlayerClass_t playerClass;
   Entity_t* entity;
   BattleStats_t stats;

   char name[PLAYER_MAX_NAME_LENGTH + 1];
   u32 exp;

   PlayerMovement_t moveHistory[PLAYER_MOVE_HISTORY_SIZE];
   u32 moveHistoryIndex;
   Bool_t chainNextPlayer;
}
Player_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Player_Init( Player_t* player );
void Player_ResetChaining( Player_t* player );
void Player_ApplyTileDamage( Player_t* player );
void Player_GetClassAbbrStr( Player_t* player, char* str );
u32 Player_GetLevel( Player_t* player );

#if defined( __cplusplus )
}
#endif

#endif // PLAYER_H
