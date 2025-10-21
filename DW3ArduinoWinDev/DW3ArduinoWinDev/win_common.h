#if !defined( WIN_COMMON_H )
#define WIN_COMMON_H

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#define STRING_SIZE_DEFAULT         1024
#define GRAPHICS_SCALE              3.0f

#include <Windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "win_pixel_buffer.h"
#include "game.h"

typedef struct WinGlobalObjects_t
{
   HWND hWndMain;
   HFONT hFont;
   LARGE_INTEGER performanceFrequency;
   BITMAPINFO bmpInfo;
   WinPixelBuffer_t screenBuffer;
   u32 buttonMap[InputButton_Count];
   Game_t game;
   Bool_t shutdown;
}
WinGlobalObjects_t;

WinGlobalObjects_t g_winGlobals;

#endif // WIN_COMMON_H
