#include <unity.h>
#include <input.h>

void Input_Init_Always_SetsAllStatesToFalse( void )
{
   u32 i;
   Input_t input;

   Input_Init( &input );

   for ( i = 0; i < InputButton_Count; i++ )
   {
      TEST_ASSERT_EQUAL( False, input.buttonStates[i].pressed );
      TEST_ASSERT_EQUAL( False, input.buttonStates[i].released );
      TEST_ASSERT_EQUAL( False, input.buttonStates[i].down );
   }
}

void Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse( void )
{
   Input_t input;
   Bool_t result;

   Input_Init( &input );
   result = Input_AnyButtonPressed( &input );

   TEST_ASSERT_EQUAL( False, result );
}

void Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue( void )
{
   Input_t input;
   Bool_t result;

   Input_Init( &input );
   input.buttonStates[InputButton_A].pressed = True;
   result = Input_AnyButtonPressed( &input );

   TEST_ASSERT_EQUAL( True, result );
}
