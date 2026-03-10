#include "player.h"

void Player_Init( Player_t* player )
{
   player->name[0] = '\0';
   player->stats.hp = PLAYER_STATS_DEFAULT_HP;
   player->stats.mp = PLAYER_STATS_DEFAULT_MP;
   Player_ResetChaining( player );
}

void Player_ResetChaining( Player_t* player )
{
   player->chainNextPlayer = False;
   player->moveHistoryIndex = 0;
}

void Player_ApplyTileDamage( Player_t* player )
{
   switch ( player->moveHistory[player->moveHistoryIndex].tileDamageRate )
   {
      // TODO: actually apply damage to the player based on the rate
      case TileDamageRate_Low: return;
      case TileDamageRate_Medium: return;
      case TileDamageRate_High: return;
      default: return;
   }
}

void Player_GetClassAbbrStr( Player_t* player, char* str )
{
   switch ( player->playerClass )
   {
      case PlayerClass_Hero: strcpy( str, STR_PLAYER_CLASS_ABBR_HERO ); break;
      case PlayerClass_Soldier: strcpy( str, STR_PLAYER_CLASS_ABBR_SOLDIER ); break;
      case PlayerClass_Pilgrim: strcpy( str, STR_PLAYER_CLASS_ABBR_PILGRIM ); break;
      case PlayerClass_Wizard: strcpy( str, STR_PLAYER_CLASS_ABBR_WIZARD ); break;
      case PlayerClass_Fighter: strcpy( str, STR_PLAYER_CLASS_ABBR_FIGHTER ); break;
      case PlayerClass_Merchant: strcpy( str, STR_PLAYER_CLASS_ABBR_MERCHANT ); break;
      case PlayerClass_GoofOff: strcpy( str, STR_PLAYER_CLASS_ABBR_GOOFOFF ); break;
      case PlayerClass_Sage: strcpy( str, STR_PLAYER_CLASS_ABBR_SAGE ); break;

      default: strcpy( str, STR_PLAYER_CLASS_ABBR_UNKNOWN ); break;
   }
}

u32 Player_GetLevel( Player_t* player )
{
   // TODO: put the experience tables in the Editor and auto-generate them
   UNUSED_PARAM( player );
   return 1;
}
