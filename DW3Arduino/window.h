#if !defined( WINDOW_H )
#define WINDOW_H

#include "common.h"
#include "vector.h"

#define WINDOW_BACKGROUND_COLOR  SCREEN_COLOR16_BLACK
#define WINDOW_TEXT_COLOR        SCREEN_COLOR16_WHITE

typedef struct Window_t
{
   Vector4u32_t pos;    // width and height are in chars
}
Window_t;

#endif // WINDOW_H
