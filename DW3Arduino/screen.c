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

void Screen_DrawBuffer8( Screen_t* screen, u8* buffer, u32 bufferWidth, u32 bufferHeight, i32 screenX, i32 screenY )
{
   Screen_DrawBoundedBuffer8( screen, buffer, bufferWidth, bufferHeight,
                              screenX, screenY,
                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
}

void Screen_DrawBoundedBuffer8( Screen_t* screen, u8* buffer,
                                u32 bufferWidth, u32 bufferHeight,
                                i32 screenX, i32 screenY,
                                i32 leftBound, i32 topBound, i32 rightBound, i32 bottomBound )
{
   i32 bufferRow, bufferCol, x, y;
   u8* bufferPos = buffer;

   for ( bufferRow = 0, y = screenY; bufferRow < (i32)bufferHeight; bufferRow++, y++ )
   {
      if ( y >= topBound && y < bottomBound )
      {
         for ( bufferCol = 0, x = screenX; bufferCol < (i32)bufferWidth; bufferCol++, x++ )
         {
            if ( x >= leftBound && x < rightBound )
            {
               screen->buffer[ ( y * SCREEN_WIDTH ) + x ] = screen->palette[*bufferPos];
            }

            bufferPos++;
         }
      }
   }
}