#include <unity.h>
#include <player.h>

#include "main.h"

void Player_ResetChaining_Always_ResetsParameters( void )
{
   Player_t player;
   player.chainNextPlayer = True;
   player.moveHistoryIndex = 5;

   Player_ResetChaining( &player );

   TEST_ASSERT_EQUAL( False, player.chainNextPlayer );
   TEST_ASSERT_EQUAL( 0, player.moveHistoryIndex );
}

void Player_UpdateLevel_NoExperience_SetsCorrectLevel( void )
{
   u32 i;
   Player_t players[PlayerClass_Count];

   for ( i = 0; i < PlayerClass_Count; i++ )
   {
      players[i].playerClass = i;
      players[i].exp = 0;

      Player_UpdateLevel( players + i );

      TEST_ASSERT_EQUAL( 1, players[i].level );
   }
}

void Player_UpdateLevel_MaxExperience_SetsCorrectLevel( void )
{
   u32 i;
   Player_t players[PlayerClass_Count];

   for ( i = 0; i < PlayerClass_Count; i++ )
   {
      players[i].playerClass = i;
      players[i].exp = PLAYER_MAX_EXPERIENCE;

      Player_UpdateLevel( players + i );

      TEST_ASSERT_EQUAL( PLAYER_MAX_LEVEL, players[i].level );
   }
}

void Player_UpdateLevel_MidExperienceAlmostNextLevel_SetsCorrectLevel( void )
{
   u32 i;
   u32 tableIndex = TABLE_CLASS_EXP_TABLE_SIZE / 2;
   Player_t players[PlayerClass_Count];

   for ( i = 0; i < PlayerClass_Count; i++ )
   {
      players[i].playerClass = i;
      players[i].exp = g_playerClassExpTables[i][tableIndex] - 1;

      Player_UpdateLevel( players + i );

      TEST_ASSERT_EQUAL( tableIndex + 1, players[i].level );
   }
}

void Player_UpdateLevel_MidExperienceExactlyNextLevel_SetsCorrectLevel( void )
{
   u32 i;
   u32 tableIndex = TABLE_CLASS_EXP_TABLE_SIZE / 2;
   Player_t players[PlayerClass_Count];

   for ( i = 0; i < PlayerClass_Count; i++ )
   {
      players[i].playerClass = i;
      players[i].exp = g_playerClassExpTables[i][tableIndex];

      Player_UpdateLevel( players + i );

      TEST_ASSERT_EQUAL( tableIndex + 2, players[i].level );
   }
}
