#include "npc.h"
#include "entity.h"
#include "sprite.h"
#include "clock.h"
#include "random.h"

void Npc_Init( Npc_t* npc, Entity_t* entity, Direction_t direction, Bool_t wanders )
{
   npc->entity = entity;
   npc->wanders = wanders;
   ActiveSprite_SetDirection( entity->sprite, direction );
   
   if ( wanders )
   {
      npc->isWandering = ( Random_u32( 0, 1 ) == 0 ) ? True : False;
      npc->actionSeconds = Random_u32( NPC_MIN_ACTION_SECONDS * 1000, NPC_MAX_ACTION_SECONDS * 1000 ) / 1000.0f;
      npc->elapsedSeconds = 0.0f;
   }
}

void Npc_Tic( Npc_t* npc )
{
   i32 vx, vy;
   Bool_t movingLeft, movingUp, movingRight, movingDown;

   if ( !npc->wanders )
   {
      return;
   }

   npc->elapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( npc->elapsedSeconds > npc->actionSeconds )
   {
      if ( npc->isWandering )
      {
         npc->entity->velocity.x = 0;
         npc->entity->velocity.y = 0;
      }
      else
      {
         vx = Random_i32( NPC_MIN_VELOCITY, NPC_MAX_VELOCITY );
         vy = Random_i32( NPC_MIN_VELOCITY, NPC_MAX_VELOCITY );

         movingUp = False;
         movingDown = False;
         movingLeft = False;
         movingRight = False;

         if ( Random_u32( 0, 1 ) == 0 ) // move horizontal
         {
            npc->entity->velocity.x = ( Random_u32( 0, 1 ) == 0 ) ? -vx : vx;
            movingLeft = ( npc->entity->velocity.x < 0 ) ? True : False;
            movingRight = ( npc->entity->velocity.x > 0 ) ? True : False;
         }
         
         if ( Random_u32( 0, 1 ) == 0 ) // move vertical
         {
            npc->entity->velocity.y = ( Random_u32( 0, 1 ) == 0 ) ? -vy : vy;
            movingUp = ( npc->entity->velocity.y < 0 ) ? True : False;
            movingDown = ( npc->entity->velocity.y > 0 ) ? True : False;
         }

         if ( movingLeft || movingUp || movingRight || movingDown )
         {
            if ( npc->entity->sprite->direction == Direction_Left && !movingLeft )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingUp ? Direction_Up : movingRight ? Direction_Right : Direction_Down );
            }
            else if ( npc->entity->sprite->direction == Direction_Up && !movingUp )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingRight ? Direction_Right : movingDown ? Direction_Down : Direction_Left );
            }
            else if ( npc->entity->sprite->direction == Direction_Right && !movingRight )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingDown ? Direction_Down : movingLeft ? Direction_Left : Direction_Up );
            }
            else if ( npc->entity->sprite->direction == Direction_Down && !movingDown )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingLeft ? Direction_Left : movingUp ? Direction_Up : Direction_Right );
            }
         }
      }

      TOGGLE_BOOL( npc->isWandering );
      npc->actionSeconds = Random_u32( NPC_MIN_ACTION_SECONDS * 1000, NPC_MAX_ACTION_SECONDS * 1000 ) / 1000.0f;
      npc->elapsedSeconds = 0.0f;
   }
}
