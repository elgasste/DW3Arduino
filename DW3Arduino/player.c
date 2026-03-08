#include "player.h"

void Player_Init( Player_t* player )
{
   player->name[0] = '\0';
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
