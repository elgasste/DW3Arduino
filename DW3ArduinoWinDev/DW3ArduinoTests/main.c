#include "main.h"

#include <unity.h>

void setUp()
{
}

void tearDown()
{
}

int main( int argc, char** argv )
{
   UNITY_BEGIN();

   // input tests
   RUN_TEST( Input_Init_Always_SetsAllStatesToFalse );
   RUN_TEST( Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse );
   RUN_TEST( Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue );

   // validate_save tests
   RUN_TEST( Validate_PlayerCount_TooFewPlayers_ReturnsFalse );
   RUN_TEST( Validate_PlayerCount_TooManyPlayers_ReturnsFalse );
   RUN_TEST( Validate_PlayerCount_EnoughPlayers_ReturnsTrue );
   RUN_TEST( Validate_PlayerName_TooShort_ReturnsFalse );
   RUN_TEST( Validate_PlayerName_TooLong_ReturnsFalse );
   RUN_TEST( Validate_PlayerName_GoodLength_ReturnsTrue );

   return UNITY_END();
}
