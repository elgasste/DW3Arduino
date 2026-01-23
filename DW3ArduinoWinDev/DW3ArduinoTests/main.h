#if !defined( MAIN_H )
#define MAIN_H

// test_input.c
void Input_Init_Always_SetsAllStatesToFalse( void );
void Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse( void );
void Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue( void );

#endif // MAIN_H
