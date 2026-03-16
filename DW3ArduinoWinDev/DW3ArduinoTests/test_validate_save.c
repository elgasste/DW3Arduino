#include <unity.h>

#include "game_util.h"

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

void Validate_PlayerClass_TooLow_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerClass( -1 );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerClass_TooHigh_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerClass( (i32)PlayerClass_Count );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerClass_GoodClass_ReturnsTrue( void )
{
   Bool_t result = Validate_PlayerClass( (i32)PlayerClass_Count - 1 );

   TEST_ASSERT_EQUAL( True, result );
}

void Validate_PlayerGender_TooLow_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerGender( -1 );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerGender_TooHigh_ReturnsFalse( void )
{
   Bool_t result = Validate_PlayerGender( (i32)Gender_Count );

   TEST_ASSERT_EQUAL( False, result );
}

void Validate_PlayerGender_GoodGender_ReturnsTrue( void )
{
   Bool_t result = Validate_PlayerGender( (i32)Gender_Count - 1 );

   TEST_ASSERT_EQUAL( True, result );
}

void Validate_SingleHero_NoHeroes_ReturnsFalse( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   Bool_t result = True;

   if ( game )
   {
      game->playerCount = 3;
      game->players[0].playerClass = PlayerClass_Fighter;
      game->players[1].playerClass = PlayerClass_GoofOff;
      game->players[2].playerClass = PlayerClass_Soldier;
      result = Validate_SingleHero( game );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( False, result );
   }
}

void Validate_SingleHero_MultipleHeroes_ReturnsFalse( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   Bool_t result = True;

   if ( game )
   {
      game->playerCount = 3;
      game->players[0].playerClass = PlayerClass_Hero;
      game->players[1].playerClass = PlayerClass_Merchant;
      game->players[2].playerClass = PlayerClass_Hero;
      result = Validate_SingleHero( game );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( False, result );
   }
}

void Validate_SingleHero_OneHero_ReturnsTrue( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   Bool_t result = False;

   if ( game )
   {
      game->playerCount = 3;
      game->players[0].playerClass = PlayerClass_Pilgrim;
      game->players[1].playerClass = PlayerClass_Hero;
      game->players[2].playerClass = PlayerClass_Wizard;
      result = Validate_SingleHero( game );
      free( game );

      TEST_ASSERT_EQUAL( True, result );
   }
}
