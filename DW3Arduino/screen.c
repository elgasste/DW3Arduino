#include "screen.h"

void Screen_Init( Screen_t* screen, u16* buffer )
{
   screen->buffer = buffer;
}

void Screen_WipeColor( Screen_t* screen, u16 color )
{
   u32 i;
   u16* bufferPos = screen->buffer;

   for ( i = 0; i < SCREEN_PIXELS; i++ )
   {
      *bufferPos = color;
      bufferPos++;
   }
}
