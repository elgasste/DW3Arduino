#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_WIDTH                   420
#define SCREEN_HEIGHT                  240
#define SCREEN_PIXELS                  ( SCREEN_WIDTH * SCREEN_HEIGHT )

#define COLOR16_BLACK                  0x0000
#define COLOR16_BLUE                   0x001F
#define COLOR16_GREEN                  0x07E0
#define COLOR16_CYAN                   0x07FF
#define COLOR16_RED                    0xF800
#define COLOR16_MAGENTA                0xF81F
#define COLOR16_YELLOW                 0xFFE0
#define COLOR16_WHITE                  0xFFFF

typedef struct Screen_t
{
   u16* buffer;
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

   void Screen_Init( Screen_t* screen, u16* buffer );
   void Screen_WipeColor( Screen_t* screen, u16 color );

   // platform-specific
   void Screen_BlitBuffer( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
