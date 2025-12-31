#include "sprite.h"
#include "clock.h"

void ActiveSprite_Reset( ActiveSprite_t* sprite )
{
   sprite->frame = 0;
   sprite->frameSeconds = 0.0f;
   sprite->direction = Direction_Down;
}

void ActiveSprite_SetDirection( ActiveSprite_t* sprite, Direction_t direction )
{
   if ( sprite->direction != direction )
   {
      sprite->frame = 0;
      sprite->direction = direction;
   }
}

void ActiveSprite_Tic( ActiveSprite_t* sprite )
{
   sprite->frameSeconds += CLOCK_FRAME_SECONDS;

   while ( sprite->frameSeconds > ACTIVE_SPRITE_FRAME_TOTAL_SECONDS )
   {
      sprite->frameSeconds -= ACTIVE_SPRITE_FRAME_TOTAL_SECONDS;
      sprite->frame++;

      if ( sprite->frame >= ACTIVE_SPRITE_FRAMES )
      {
         sprite->frame = 0;
      }
   }
}
