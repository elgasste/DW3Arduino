#include "npc.h"
#include "entity.h"
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
         if ( Random_u32( 0, 1 ) == 0 ) // move horizontal
         {
            npc->entity->velocity.x = ( Random_u32( 0, 1 ) == 0 ) ? -NPC_VELOCITY : NPC_VELOCITY;
         }
         
         if ( Random_u32( 0, 1 ) == 0 ) // move vertical
         {
            npc->entity->velocity.y = ( Random_u32( 0, 1 ) == 0 ) ? -NPC_VELOCITY : NPC_VELOCITY;
         }
      }

      TOGGLE_BOOL( npc->isWandering );
      npc->actionSeconds = Random_u32( NPC_MIN_ACTION_SECONDS * 1000, NPC_MAX_ACTION_SECONDS * 1000 ) / 1000.0f;
      npc->elapsedSeconds = 0.0f;
   }
}
