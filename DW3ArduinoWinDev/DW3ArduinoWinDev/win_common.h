#if !defined( WIN_COMMON_H )
#define WIN_COMMON_H

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#define STRING_SIZE_DEFAULT               1024
#define GRAPHICS_SCALE                    3.0f

#define VK_NOCLIP                         49    // 1
#define VK_FASTWALK                       50    // 2
#define VK_SHOWHITBOXES                   51    // 3

#define JSON_SAVE_FILE_NAME               "save_%d.json"
#define JSON_PLAYER_COUNT                 "playerCount"
#define JSON_PLAYERS                      "players"
#define JSON_PLAYER_NAME                  "name"
#define JSON_PLAYER_CLASS                 "class"

#define JSON_ERROR_ROOT                   "error creating JSON root object"
#define JSON_ERROR_PLAYERS                "error creating JSON player objects"
#define JSON_ERROR_MALLOC                 "error allocating memory for reading save file contents"
#define JSON_ERROR_FILE_OPEN_WRITE        "error opening JSON save file for writing: %s"
#define JSON_ERROR_FILE_OPEN_READ         "error opening JSON save file for reading: %s"
#define JSON_ERROR_FILE_WRITE             "error writing JSON save file: %s"
#define JSON_ERROR_FILE_READ              "error reading JSON save file: %s"
#define JSON_ERROR_PARSE_SAVE_FILE        "error parsing JSON from save file: %s"

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
