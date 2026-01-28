#if !defined( MAIN_H )
#define MAIN_H

// test_input.c
void Input_Init_Always_SetsAllStatesToFalse( void );
void Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse( void );
void Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue( void );

// test_validate_save.c
void Validate_PlayerCount_TooFewPlayers_ReturnsFalse( void );
void Validate_PlayerCount_TooManyPlayers_ReturnsFalse( void );
void Validate_PlayerCount_EnoughPlayers_ReturnsTrue( void );

#endif // MAIN_H
