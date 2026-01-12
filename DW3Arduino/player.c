#include "player.h"

void Player_ResetChaining( Player_t* player )
{
   player->chainNextPlayer = False;
   player->moveChainIndex = 0;
   player->moveHistoryIndex = 0;
}
