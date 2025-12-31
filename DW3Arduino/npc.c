#include "npc.h"
#include "entity.h"
#include "sprite.h"
#include "clock.h"
#include "random.h"

void Npc_Init( Npc_t* npc, Entity_t* entity, Bool_t wanders )
{
   npc->entity = entity;
   npc->wanders = wanders;
   
   if ( wanders )
   {
      npc->isWandering = ( Random_u32( 0, 1 ) == 0 ) ? True : False;
      npc->actionSeconds = Random_u32( NPC_MIN_ACTION_SECONDS * 1000, NPC_MAX_ACTION_SECONDS * 1000 ) / 1000.0f;
      npc->elapsedSeconds = 0.0f;
   }
}

void Npc_Tic( Npc_t* npc )
{
   r32 vx, vy;
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
         npc->entity->velocity.x = 0.0f;
         npc->entity->velocity.y = 0.0f;
      }
      else
      {
         vx = (r32)Random_u32( NPC_MIN_VELOCITY, NPC_MAX_VELOCITY );
         vy = (r32)Random_u32( NPC_MIN_VELOCITY, NPC_MAX_VELOCITY );

         movingUp = False;
         movingDown = False;
         movingLeft = False;
         movingRight = False;

         if ( Random_u32( 0, 1 ) == 0 ) // move horizontal
         {
            movingLeft = ( vx < 0.0f ) ? True : False;
            movingRight = ( vx > 0.0f ) ? True : False;
            npc->entity->velocity.x = ( Random_u32( 0, 1 ) == 0 ) ? -vx : vx;
         }
         
         if ( Random_u32( 0, 1 ) == 0 ) // move vertical
         {
            movingUp = ( vy < 0.0f ) ? True : False;
            movingDown = ( vy > 0.0f ) ? True : False;
            npc->entity->velocity.y = ( Random_u32( 0, 1 ) == 0 ) ? -vy : vy;
         }

         if ( movingLeft || movingUp || movingRight || movingDown )
         {
            if ( npc->entity->sprite->direction == Direction_Left && !movingLeft )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingUp ? Direction_Up : movingRight ? Direction_Right : Direction_Down );
            }
            else if ( npc->entity->sprite->direction == Direction_Right && !movingRight )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingUp ? Direction_Up : movingLeft ? Direction_Left : Direction_Down );
            }
            else if ( npc->entity->sprite->direction == Direction_Up && !movingUp )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingLeft ? Direction_Left : movingDown ? Direction_Down : Direction_Right );
            }
            else if ( npc->entity->sprite->direction == Direction_Down && !movingDown )
            {
               ActiveSprite_SetDirection( npc->entity->sprite, movingLeft ? Direction_Left : movingUp ? Direction_Right : Direction_Right );
            }
         }
      }

      TOGGLE_BOOL( npc->isWandering );
      npc->actionSeconds = Random_u32( NPC_MIN_ACTION_SECONDS * 1000, NPC_MAX_ACTION_SECONDS * 1000 ) / 1000.0f;
      npc->elapsedSeconds = 0.0f;
   }
}
