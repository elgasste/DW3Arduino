#if !defined( MAIN_H )
#define MAIN_H

// test_input.c
void Input_Init_Always_SetsAllStatesToFalse( void );
void Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse( void );
void Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue( void );
void Input_HandleOverworldGeneral_OnlyLeftIsDown_VelocityIsCorrect( void );
void Input_HandleOverworldGeneral_OnlyUpIsDown_VelocityIsCorrect( void );
void Input_HandleOverworldGeneral_OnlyRightIsDown_VelocityIsCorrect( void );
void Input_HandleOverworldGeneral_OnlyDownIsDown_VelocityIsCorrect( void );
void Input_HandleOverworldGeneral_LeftAndUpAreDown_VelocitiesAreCorrect( void );
void Input_HandleOverworldGeneral_UpAndRightAreDown_VelocitiesAreCorrect( void );
void Input_HandleOverworldGeneral_RightAndDownAreDown_VelocitiesAreCorrect( void );
void Input_HandleOverworldGeneral_DownAndLeftAreDown_VelocitiesAreCorrect( void );
void Input_HandleOverworldGeneral_OnlyLeftIsDown_PlayerFacesLeft( void );
void Input_HandleOverworldGeneral_OnlyUpIsDown_PlayerFacesUp( void );
void Input_HandleOverworldGeneral_OnlyRightIsDown_PlayerFacesRight( void );
void Input_HandleOverworldGeneral_OnlyDownIsDown_PlayerFacesDown( void );

// MUFFINS: these are new
void Input_HandleOverworldGeneral_LeftAndRightAreDown_PlayerDirectionDoesNotChange( void );
void Input_HandleOverworldGeneral_UpAndDownAreDown_PlayerDirectionDoesNotChange( void );

// test_sprite.c
void ActiveSprite_Init_Always_InitializesParameters( void );
void ActiveSprite_SetDirection_DirectionHasNotChanged_DoesNotResetFrame( void );
void ActiveSprite_SetDirection_DirectionHasChanged_ResetsFrameAndChangesDirection( void );
void ActiveSprite_Tic_FrameSecondsHasNotExceededTotal_FrameAndFrameSecondsDoNotChange( void );
void ActiveSprite_Tic_FrameSecondsHasExceededTotal_ResetsFrameSecondsAndIncrementsFrame( void );
void ActiveSprite_Tic_LastFrameAndFrameSecondsHasExceededTotal_SetsFrameToZero( void );

// test_validate_save.c
void Validate_PlayerCount_TooFewPlayers_ReturnsFalse( void );
void Validate_PlayerCount_TooManyPlayers_ReturnsFalse( void );
void Validate_PlayerCount_EnoughPlayers_ReturnsTrue( void );
void Validate_PlayerName_TooShort_ReturnsFalse( void );
void Validate_PlayerName_TooLong_ReturnsFalse( void );
void Validate_PlayerName_GoodLength_ReturnsTrue( void );
void Validate_PlayerClass_TooLow_ReturnsFalse( void );
void Validate_PlayerClass_TooHigh_ReturnsFalse( void );
void Validate_PlayerClass_GoodClass_ReturnsTrue( void );
void Validate_SingleHero_NoHeroes_ReturnsFalse( void );
void Validate_SingleHero_MultipleHeroes_ReturnsFalse( void );
void Validate_SingleHero_OneHero_ReturnsTrue( void );

#endif // MAIN_H
