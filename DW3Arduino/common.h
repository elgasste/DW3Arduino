#if !defined( COMMON_H )
#define COMMON_H

// ARDUINO DEBUG FLAGS
// un-comment to get serial logging
//#define DEBUG_SERIAL

#if !defined( VISUAL_STUDIO_DEV )

#include <Arduino.h>

#if defined( DEBUG_SERIAL )
#define SERIAL_BAUD 115200
#endif

#endif // !VISUAL_STUDIO_DEV

#include <stdint.h>
#include <memory.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "enums.h"

#define internal static
#define global static
#define local_persist static

#define Bool_t int32_t
#define True 1
#define False 0
#define TOGGLE_BOOL( b )               b = b ? False : True;

#define UNUSED_PARAM( x )              (void)x

#define UNITS_PER_PIXEL                100
#define MAX_PLAYERS                    4

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float r32;
typedef double r64;

#if defined( VISUAL_STUDIO_DEV )
#define DELAY_MS( x )                  Sleep( x )
// this will try to make us use sprintf_s
#pragma warning( disable: 4996 )
#else
#define DELAY_MS( x )                  delay( x )
#endif

#if defined( VISUAL_STUDIO_DEV )

typedef struct WinDebugFlags_t
{
   Bool_t showDiagnostics;
   Bool_t noClip;
   Bool_t fastWalk;
   Bool_t showHitBoxes;
}
WinDebugFlags_t;

WinDebugFlags_t g_winDebugFlags;

#endif // VISUAL_STUDIO_DEV

#if defined( __cplusplus )
extern "C"
{
#endif

void Program_Log( const char* msg );

#if defined( __cplusplus )
}
#endif

#endif // COMMON_H
