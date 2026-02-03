#if !defined( MAIN_H )
#define MAIN_H

// test_input.c
void Input_Init_Always_SetsAllStatesToFalse( void );
void Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse( void );
void Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue( void );
void Input_HandleInput_OverworldActiveState_OnlyLeftIsDown_VelocityIsCorrect( void );
void Input_HandleInput_OverworldActiveState_OnlyUpIsDown_VelocityIsCorrect( void );
void Input_HandleInput_OverworldActiveState_OnlyRightIsDown_VelocityIsCorrect( void );
void Input_HandleInput_OverworldActiveState_OnlyDownIsDown_VelocityIsCorrect( void );
void Input_HandleInput_OverworldActiveState_LeftAndUpAreDown_VelocitiesAreCorrect( void );
void Input_HandleInput_OverworldActiveState_UpAndRightAreDown_VelocitiesAreCorrect( void );
void Input_HandleInput_OverworldActiveState_RightAndDownAreDown_VelocitiesAreCorrect( void );
void Input_HandleInput_OverworldActiveState_DownAndLeftAreDown_VelocitiesAreCorrect( void );

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
