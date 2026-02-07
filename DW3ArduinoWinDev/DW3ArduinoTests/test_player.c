#include <unity.h>
#include <player.h>

#include "main.h"

void Player_ResetChaining_ResetsParameters( void )
{
   Player_t player;
   player.chainNextPlayer = True;
   player.moveHistoryIndex = 5;

   Player_ResetChaining( &player );

   TEST_ASSERT_EQUAL( False, player.chainNextPlayer );
   TEST_ASSERT_EQUAL( 0, player.moveHistoryIndex );
}
