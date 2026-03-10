#include "screen.h"
#include "window.h"

internal i8 Screen_GetCharTileIndexFromChar( const char c );

void Screen_Init( Screen_t* screen, u16* buffer )
{
   screen->buffer = buffer;
   screen->paletteColorCount = 0;
   screen->dayFilterIntensity = 1.0f;

   Screen_LoadPalette( screen );
   Screen_LoadTextBitFields( screen );
}

void Screen_BackupPalette( Screen_t* screen )
{
   u32 i;

   for ( i = 0; i < screen->paletteColorCount; i++ )
   {
      screen->backupPalette[i] = screen->palette[i];
   }
}

void Screen_RestorePalette( Screen_t* screen )
{
   u32 i;

   for ( i = 0; i < screen->paletteColorCount; i++ )
   {
      screen->palette[i] = screen->backupPalette[i];
   }
}

void Screen_ClearPalette( Screen_t* screen, u16 color )
{
   u32 i;

   for ( i = 0; i < screen->paletteColorCount; i++ )
   {
      if ( screen->palette[i] != SCREEN_COLOR16_TRANSPARENT )
      {
         screen->palette[i] = color;
      }
   }
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
   r32 rgFactor, bFactor;
   
   // the lower these are, the lighter the colors
#if defined( VISUAL_STUDIO_DEV )
   rgFactor = 0.72f;
   bFactor = 0.45f;
#else
   rgFactor = 0.84f;
   bFactor = 0.56f;
#endif

   for ( bufferRow = 0, y = screenY; bufferRow < (i32)bufferHeight; bufferRow++, y++ )
   {
      if ( y >= topBound && y < bottomBound )
      {
         for ( bufferCol = 0, x = screenX; bufferCol < (i32)bufferWidth; bufferCol++, x++ )
         {
            color = screen->palette[*bufferPos];

            if ( color != SCREEN_COLOR16_TRANSPARENT && x >= leftBound && x < rightBound )
            {
               r = ( color >> 11 ) & 0x1F; // 5 bits (0-31)
               g = ( color >> 5 ) & 0x3F;  // 6 bits (0-63)
               b = color & 0x1F;           // 5 bits (0-31)

               // leave the blue colors a bit lighter, feels more like nighttime
               rgValue = 1.0f - ( rgFactor * ( 1.0f - screen->dayFilterIntensity ) );
               bValue = 1.0f - ( bFactor * ( 1.0f - screen->dayFilterIntensity ) );

               newR = (u32)( r * rgValue );
               newG = (u32)( g * rgValue );
               newB = (u32)( b * bValue );

               newColor565 = ( (u16)( newR & 0x1F ) << 11 ) | (u16)( ( newG & 0x3F ) << 5 ) | (u16)( newB & 0x1F );

               screen->buffer[( y * SCREEN_WIDTH ) + x] = newColor565;
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

void Screen_DrawChar( Screen_t* screen, char c, u32 x, u32 y, u16 color )
{
   i32 i;
   u32 j, row;
   u8* bitField;
   i8 charIndex = Screen_GetCharTileIndexFromChar( c );
   u16* bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;

   if ( charIndex < 0 )
   {
      for ( i = 0, j = 0; i < SCREEN_TEXT_TILE_SIZE * SCREEN_TEXT_TILE_SIZE; i++ )
      {
         *bufferPos = 0;
         bufferPos++;
         j++;

         if ( j == SCREEN_TEXT_TILE_SIZE )
         {
            bufferPos += ( SCREEN_WIDTH - SCREEN_TEXT_TILE_SIZE );
            j = 0;
         }
      }
   }
   else
   {
#pragma warning( disable: 4047 )
      bitField = &( screen->textBitFields[charIndex] );
#pragma warning( default: 4047 )

      for ( row = 0; row < SCREEN_TEXT_TILE_SIZE; row++ )
      {
         for ( i = ( SCREEN_TEXT_TILE_SIZE - 1 ); i >= 0; i-- )
         {
            *bufferPos = ( bitField[row] & ( 0x01 << i ) ) ? color : 0;
            bufferPos++;
         }

         bufferPos += ( SCREEN_WIDTH - SCREEN_TEXT_TILE_SIZE );
      }
   }
}

void Screen_DrawText( Screen_t* screen, const char* text, u32 x, u32 y, u16 color )
{
   u16 ch, j;
   i8 charIndex, i;
   u8 row;
   u8* bitField;
   u16* bufferPos;

   for ( ch = 0; ch < strlen( text ); ch++ )
   {
      bufferPos = screen->buffer + ( y * SCREEN_WIDTH ) + x;
      charIndex = Screen_GetCharTileIndexFromChar( text[ch] );

      if ( charIndex < 0 )
      {
         for ( i = 0, j = 0; i < SCREEN_TEXT_TILE_SIZE * SCREEN_TEXT_TILE_SIZE; i++ )
         {
            *bufferPos = 0;
            bufferPos++;
            j++;

            if ( j == SCREEN_TEXT_TILE_SIZE )
            {
               bufferPos += ( SCREEN_WIDTH - SCREEN_TEXT_TILE_SIZE );
               j = 0;
            }
         }
      }
      else
      {
#pragma warning( disable: 4047 )
         bitField = &( screen->textBitFields[charIndex] );
#pragma warning( default: 4047 )

         for ( row = 0; row < SCREEN_TEXT_TILE_SIZE; row++ )
         {
            for ( i = ( SCREEN_TEXT_TILE_SIZE - 1 ); i >= 0; i-- )
            {
               *bufferPos = ( bitField[row] & ( 0x01 << i ) ) ? color : 0;
               bufferPos++;
            }

            bufferPos += ( SCREEN_WIDTH - SCREEN_TEXT_TILE_SIZE );
         }
      }

      x += 8;
   }
}

void Screen_DrawWindow( Screen_t* screen, Window_t* window )
{
   u32 i;

   // corners
   Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_TOPLEFT, window->pos.x, window->pos.y, WINDOW_TEXT_COLOR );
   Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_TOPRIGHT, window->pos.x + ( ( window->pos.w - 1 ) * SCREEN_TEXT_TILE_SIZE ), window->pos.y, WINDOW_TEXT_COLOR );
   Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_BOTTOMLEFT, window->pos.x, window->pos.y + ( ( window->pos.h - 1 ) * SCREEN_TEXT_TILE_SIZE ), WINDOW_TEXT_COLOR );
   Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_BOTTOMRIGHT, window->pos.x + ( ( window->pos.w - 1 ) * SCREEN_TEXT_TILE_SIZE ), window->pos.y + ( ( window->pos.h - 1 ) * SCREEN_TEXT_TILE_SIZE ), WINDOW_TEXT_COLOR );

   // top/bottom border
   for ( i = 0; i < window->pos.w - 2; i++ )
   {
      Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_TOP, window->pos.x + ( ( 1 + i ) * SCREEN_TEXT_TILE_SIZE ), window->pos.y, WINDOW_TEXT_COLOR );
      Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_BOTTOM, window->pos.x + ( ( 1 + i ) * SCREEN_TEXT_TILE_SIZE ), window->pos.y + ( ( window->pos.h - 1 ) * SCREEN_TEXT_TILE_SIZE ), WINDOW_TEXT_COLOR );
   }

   // left/right border
   for ( i = 0; i < window->pos.h - 2; i++ )
   {
      Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_LEFT, window->pos.x, window->pos.y + ( ( 1 + i ) * SCREEN_TEXT_TILE_SIZE ), WINDOW_TEXT_COLOR );
      Screen_DrawChar( screen, SCREEN_MENU_BORDER_CHAR_RIGHT, window->pos.x + ( ( window->pos.w - 1 ) * SCREEN_TEXT_TILE_SIZE ), window->pos.y + ( ( 1 + i ) * SCREEN_TEXT_TILE_SIZE ), WINDOW_TEXT_COLOR );
   }

   // inner window
   Screen_DrawRect( screen,
                    (i32)window->pos.x + SCREEN_TEXT_TILE_SIZE, (i32)window->pos.y + SCREEN_TEXT_TILE_SIZE,
                    (i32)( ( window->pos.w - 2 ) * SCREEN_TEXT_TILE_SIZE ), (i32)( ( window->pos.h - 2 ) * SCREEN_TEXT_TILE_SIZE ),
                    WINDOW_BACKGROUND_COLOR );
}

internal i8 Screen_GetCharTileIndexFromChar( const char c )
{
   if ( c >= 97 && c <= 122 )
   {
      // a - z (lower case letters start at 0 in our table)
      return c - 97;
   }
   else if ( c >= 65 && c <= 90 )
   {
      // A - Z (upper case letters start at 26 in our table)
      return c - 39;
   }
   else if ( c >= 48 && c <= 57 )
   {
      // 0 - 9 (numbers start at 52 in our table)
      return c + 4;
   }
   else
   {
      // special characters start at 62 in our table
      switch ( c )
      {
         case 44: return 62;     // comma
         case 33: return 63;     // exclamation point
         case 39: return 64;     // single quote
         case 38: return 65;     // ampersand
         case 46: return 66;     // period
         case 34: return 67;     // double quotes
         case 63: return 68;     // question mark
         case 45: return 69;     // dash
         case 62: return 70;     // greater-than
         case 58: return 71;     // colon
         case 47: return 72;     // forward slash
         case 40: return 73;     // left parenthesis
         case 41: return 74;     // right parenthesis
         case 9:  return 75;     // downward arrow
         case 42: return 76;     // asterisk

         case SCREEN_MENU_BORDER_CHAR_TOPLEFT: return 77;
         case SCREEN_MENU_BORDER_CHAR_TOPRIGHT: return 78;
         case SCREEN_MENU_BORDER_CHAR_BOTTOMLEFT: return 79;
         case SCREEN_MENU_BORDER_CHAR_BOTTOMRIGHT: return 80;
         case SCREEN_MENU_BORDER_CHAR_LEFT: return 81;
         case SCREEN_MENU_BORDER_CHAR_TOP: return 82;
         case SCREEN_MENU_BORDER_CHAR_RIGHT: return 83;
         case SCREEN_MENU_BORDER_CHAR_BOTTOM: return 84;

         default: return -1;
      }
   }
}
