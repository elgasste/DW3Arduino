#include "input.h"

u8 Input_ReadNesController()
{
   u32 i;
   u8 result = 0;

   digitalWrite( INPUT_NES_LATCH_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_LATCH_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_A_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_B_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_SELECT_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_START_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_UP_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_DOWN_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_LEFT_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );
   
   if ( digitalRead( INPUT_NES_DATA_PIN ) == LOW )
   {
      result |= INPUT_RIGHT_FLAG;
   }

   digitalWrite( INPUT_NES_CLOCK_PIN, HIGH );
   delayMicroseconds( 12 );
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );

   return result;
}

void Input_ButtonPressed( Input_t* input, u32 button )
{
   InputButtonState_t* state = &input->buttonStates[button];

   if ( !state->down )
   {
      state->down = True;
      state->pressed = True;
   }
}

void Input_ButtonReleased( Input_t* input, u32 button )
{
   InputButtonState_t* state = &input->buttonStates[button];

   if ( state->down )
   {
      state->down = False;
      state->released = True;
   }
}
