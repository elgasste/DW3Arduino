#if !defined( NPC_H )
#define NPC_H

#include "common.h"

#define NPC_MIN_ACTION_SECONDS      1
#define NPC_MAX_ACTION_SECONDS      4
#define NPC_MIN_VELOCITY            8    // units per frame
#define NPC_MAX_VELOCITY            40

typedef struct Entity_t Entity_t;

typedef struct Npc_t
{
   Entity_t* entity;
   Bool_t wanders;
   Bool_t isWandering;
   r32 actionSeconds;
   r32 elapsedSeconds;
}
Npc_t;

#if defined( __cplusplus )
extern "C" {
#endif

void Npc_Init( Npc_t* npc, Entity_t* entity, Bool_t wanders );
void Npc_Tic( Npc_t* npc );

#if defined( __cplusplus )
}
#endif

#endif // NPC_H
