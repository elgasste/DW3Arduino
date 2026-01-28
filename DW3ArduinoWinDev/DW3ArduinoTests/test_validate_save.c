#include <unity.h>
#include <game.h>

void Validate_PlayerCount_TooFewPlayers_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerCount( 0 );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerCount_TooManyPlayers_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerCount( MAX_PLAYERS + 1 );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerCount_EnoughPlayers_ReturnsTrue( void )
{
   Bool_t result = Validate_PlayerCount( MAX_PLAYERS );

   TEST_ASSERT_EQUAL( True, result );
}
