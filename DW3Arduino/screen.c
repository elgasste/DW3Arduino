#include "screen.h"

void Screen_Init( Screen_t* screen, u16* buffer )
{
   screen->buffer = buffer;
   screen->dayFilterIntensity = 1.0f;
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

void Screen_DrawRect( Screen_t* screen, i32 x, i32 y, i32 w, i32 h, u16 color )
{
   Screen_DrawBoundedRect( screen, x, y, w, h, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color );
}

void Screen_DrawBoundedRect( Screen_t* screen, i32 x, i32 y, i32 w, i32 h,
                             i32 leftBound, i32 topBound, i32 rightBound, i32 bottomBound, u16 color )
{
   u32 row, col;
   i32 cx, cy;

   for ( row = 0, cy = y; row < (u32)h; row++, cy++ )
   {
      if ( cy >= topBound && cy < bottomBound )
      {
         for ( col = 0, cx = x; col < (u32)w; col++, cx++ )
         {
            if ( cx >= leftBound && cx < rightBound )
            {
               screen->buffer[( ( y + row ) * SCREEN_WIDTH ) + ( x + col )] = color;
            }
         }
      }
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
   u16 color, newColor565;
   u32 r, g, b, newR, newG, newB;
   r32 rgValue, bValue;
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
               color = screen->palette[*bufferPos];

               r = ( color >> 11 ) & 0x1F; // 5 bits (0-31)
               g = ( color >> 5 ) & 0x3F;  // 6 bits (0-63)
               b = color & 0x1F;           // 5 bits (0-31)

               rgValue = 1.0f - ( 0.75f * ( 1.0f - screen->dayFilterIntensity ) );
               bValue = 1.0f - ( 0.6f * ( 1.0f - screen->dayFilterIntensity ) );

               newR = (u32)( r * rgValue );
               newG = (u32)( g * rgValue );
               newB = (u32)( b * bValue );

               newColor565 = ( (u16)( newR & 0x1F ) << 11 ) | (u16)( ( newG & 0x3F ) << 5 ) | (u16)( newB & 0x1F );

               screen->buffer[ ( y * SCREEN_WIDTH ) + x ] = newColor565;
            }

            bufferPos++;
         }
      }
      else
      {
         bufferPos += bufferWidth;
      }
   }
}