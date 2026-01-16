#if !defined( SCREEN_H )
#define SCREEN_H

#include "common.h"

#define SCREEN_COLOR16_BLACK                    0x0000
#define SCREEN_COLOR16_BLUE                     0x001F
#define SCREEN_COLOR16_GREEN                    0x07E0
#define SCREEN_COLOR16_CYAN                     0x07FF
#define SCREEN_COLOR16_RED                      0xF800
#define SCREEN_COLOR16_MAGENTA                  0xF81F
#define SCREEN_COLOR16_YELLOW                   0xFFE0
#define SCREEN_COLOR16_WHITE                    0xFFFF

typedef struct Screen_t
{
   u16* buffer;
   u16 palette[SCREEN_PALETTE_SIZE];
   u16 backupPalette[SCREEN_PALETTE_SIZE];
   u8 textBitFields[SCREEN_TEXT_TILE_COUNT][SCREEN_TEXT_TILE_SIZE];
   u32 paletteColorCount;
   r32 dayFilterIntensity;
}
Screen_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Screen_Init( Screen_t* screen, u16* buffer );
void Screen_BackupPalette( Screen_t* screen );
void Screen_RestorePalette( Screen_t* screen );
void Screen_ClearPalette( Screen_t* screen, u16 color );
void Screen_WipeColor( Screen_t* screen, u16 color );
void Screen_DrawRect( Screen_t* screen, i32 x, i32 y, i32 w, i32 h, u16 color );
void Screen_DrawBoundedRect( Screen_t* screen, i32 x, i32 y, i32 w, i32 h,
                             i32 leftBound, i32 topBound, i32 rightBound, i32 bottomBound, u16 color );
void Screen_DrawBuffer8( Screen_t* screen, u8* buffer, u32 bufferWidth, u32 bufferHeight, i32 screenX, i32 screenY );
void Screen_DrawBoundedBuffer8( Screen_t* screen, u8* buffer,
                                u32 bufferWidth, u32 bufferHeight,
                                i32 screenX, i32 screenY,
                                i32 leftBound, i32 topBound, i32 rightBound, i32 bottomBound );
void Screen_DrawChar( Screen_t* screen, char c, u32 x, u32 y, u16 color );
void Screen_DrawText( Screen_t* screen, const char* text, u32 x, u32 y, u16 color );

// game_data.c
void Screen_LoadPalette( Screen_t* screen );
void Screen_LoadTextBitFields( Screen_t* screen );

// platform-specific
void Screen_Blit( Screen_t* screen );

#if defined( __cplusplus )
}
#endif

#endif // SCREEN_H
