#if !defined( WINDOW_H )
#define WINDOW_H

#include "common.h"
#include "vector.h"

#define WINDOW_BACKGROUND_COLOR  SCREEN_COLOR16_BLACK
#define WINDOW_TEXT_COLOR_DAY    SCREEN_COLOR16_WHITE
#define WINDOW_TEXT_COLOR_NIGHT  SCREEN_COLOR16_NIGHTBLUE

typedef struct Window_t
{
   Vector4u32_t pos;    // width and height are in chars
   u16 textColor;
}
Window_t;

#endif // WINDOW_H
