#if !defined( NPC_H )
#define NPC_H

#include "common.h"

typedef struct Entity_t Entity_t;

typedef struct Npc_t
{
   Entity_t* entity;
   Bool_t wanders;
   Bool_t isWandering;
   r32 actionSeconds;
   r32 secondsElapsed;
}
Npc_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Npc_Tic( Npc_t* npc );

#if defined( __cplusplus )
}
#endif

#endif // NPC_H
