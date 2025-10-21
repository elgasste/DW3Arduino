#include "game.h"

void Screen_LoadPaletteFromIndex( Screen_t* screen, u32 index )
{
   u32 i;

   UNUSED_PARAM( index );

   for ( i = 0; i < SCREEN_PALETTE_SIZE; i++ )
   {
      screen->palette[i] = COLOR16_BLACK;
   }

   screen->palette[1]  = COLOR16_WHITE;
   screen->palette[2]  = COLOR16_RED;
   screen->palette[3]  = COLOR16_BLUE;
   screen->palette[4]  = COLOR16_YELLOW;
   screen->palette[5]  = COLOR16_MAGENTA;
   screen->palette[6]  = COLOR16_CYAN;
}
