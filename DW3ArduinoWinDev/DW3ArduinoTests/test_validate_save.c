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

void Validate_PlayerName_TooShort_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerName( "" );
   
   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerName_TooLong_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerName( "NineChars" );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerName_GoodLength_ReturnsTrue( void )
{
   Bool_t result = Validate_PlayerName( "12345678" );

   TEST_ASSERT_EQUAL( True, result );
}
