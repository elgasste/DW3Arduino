#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_WIDTH                   420
#define SCREEN_HEIGHT                  240
#define SCREEN_PIXELS                  ( SCREEN_WIDTH * SCREEN_HEIGHT )

#define SCREEN_PALETTE_SIZE            256

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
   u16 palette[SCREEN_PALETTE_SIZE];
   r32 dayFilterIntensity;
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen, u16* buffer );
void Screen_WipeColor( Screen_t* screen, u16 color );
void Screen_DrawRect( Screen_t* screen, i32 x, i32 y, i32 w, i32 h, u16 color );
void Screen_DrawBoundedRect( Screen_t* screen, i32 x, i32 y, i32 w, i32 h,
                             i32 leftBound, i32 topBound, i32 rightBound, i32 bottomBound, u16 color );
void Screen_DrawBuffer8( Screen_t* screen, u8* buffer, u32 bufferWidth, u32 bufferHeight, i32 screenX, i32 screenY );
void Screen_DrawBoundedBuffer8( Screen_t* screen, u8* buffer,
                                u32 bufferWidth, u32 bufferHeight,
                                i32 screenX, i32 screenY,
                                i32 leftBound, i32 topBound, i32 rightBound, i32 bottomBound );

// game_data.c
void Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index );

// platform-specific
void Screen_Blit( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
