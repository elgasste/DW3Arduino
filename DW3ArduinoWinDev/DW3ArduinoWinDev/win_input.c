#include "input.h"

void Input_ResetState( Input_t* input )
{
   u32 i;
   InputButtonState_t* state = input->buttonStates;

   for ( i = 0; i < InputButton_Count; i++ )
   {
      state->pressed = False;
      state->released = False;
      state++;
   }
}

void Input_ButtonPressed( Input_t* input, u32 button )
{
   InputButtonState_t* state = &( input->buttonStates[button] );

   if ( !state->down )
   {
      state->down = True;
      state->pressed = True;
   }
}

void Input_ButtonReleased( Input_t* input, u32 button )
{
   InputButtonState_t* state = &( input->buttonStates[button] );

   if ( state->down )
   {
      state->down = False;
      state->released = True;
   }
}
