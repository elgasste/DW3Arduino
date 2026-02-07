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

   // test_input.c
   RUN_TEST( Input_Init_Always_SetsAllStatesToFalse );
   RUN_TEST( Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse );
   RUN_TEST( Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyLeftIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyUpIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyRightIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_OnlyDownIsDown_VelocityIsCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAreDown_VelocitiesAreCorrect );
   RUN_TEST( Input_HandleOverworldGeneral_LeftIsDown_PlayerFacesLeft );
   RUN_TEST( Input_HandleOverworldGeneral_UpIsDown_PlayerFacesUp );
   RUN_TEST( Input_HandleOverworldGeneral_RightIsDown_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_DownIsDown_PlayerFacesDown );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAreDownAndPlayerIsNotFacingUp_PlayerFacesLeft );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAreDownAndPlayerIsNotFacingUp_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAreDownAndPlayerIsNotFacingDown_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAreDownAndPlayerIsNotFacingDown_PlayerFacesLeft );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndRightAreDown_PlayerDirectionDoesNotChange );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndDownAreDown_PlayerDirectionDoesNotChange );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingDown_PlayerFacesUp );
   RUN_TEST( Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingVertical_PlayerKeepsFacingVertical );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingLeft_PlayerFacesRight );
   RUN_TEST( Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingRight_PlayerKeepsFacingRight );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingUp_PlayerFacesDown );
   RUN_TEST( Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingUp_PlayerFacesDown );
   RUN_TEST( Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown );

   // test_player.c
   RUN_TEST( Player_ResetChaining_ResetsParameters );

   // test_sprite.c
   RUN_TEST( ActiveSprite_Init_Always_InitializesParameters );
   RUN_TEST( ActiveSprite_SetDirection_DirectionHasNotChanged_DoesNotResetFrame );
   RUN_TEST( ActiveSprite_SetDirection_DirectionHasChanged_ResetsFrameAndChangesDirection );
   RUN_TEST( ActiveSprite_Tic_FrameSecondsHasNotExceededTotal_FrameAndFrameSecondsDoNotChange );
   RUN_TEST( ActiveSprite_Tic_FrameSecondsHasExceededTotal_ResetsFrameSecondsAndIncrementsFrame );
   RUN_TEST( ActiveSprite_Tic_LastFrameAndFrameSecondsHasExceededTotal_SetsFrameToZero );

   // test_validate_save.c
   RUN_TEST( Validate_PlayerCount_TooFewPlayers_ReturnsFalse );
   RUN_TEST( Validate_PlayerCount_TooManyPlayers_ReturnsFalse );
   RUN_TEST( Validate_PlayerCount_EnoughPlayers_ReturnsTrue );
   RUN_TEST( Validate_PlayerName_TooShort_ReturnsFalse );
   RUN_TEST( Validate_PlayerName_TooLong_ReturnsFalse );
   RUN_TEST( Validate_PlayerName_GoodLength_ReturnsTrue );
   RUN_TEST( Validate_PlayerClass_TooLow_ReturnsFalse );
   RUN_TEST( Validate_PlayerClass_TooHigh_ReturnsFalse );
   RUN_TEST( Validate_PlayerClass_GoodClass_ReturnsTrue );
   RUN_TEST( Validate_SingleHero_NoHeroes_ReturnsFalse );
   RUN_TEST( Validate_SingleHero_MultipleHeroes_ReturnsFalse );
   RUN_TEST( Validate_SingleHero_OneHero_ReturnsTrue );

   return UNITY_END();
}
