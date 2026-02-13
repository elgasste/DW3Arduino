#include <unity.h>
#include <game.h>

#include "main.h"

void Npc_Init_Always_InitializesBasicParameters( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Left;
   entity.sprite = &sprite;

   Npc_Init( &npc, &entity, Direction_Right, False );

   TEST_ASSERT_EQUAL( &entity, npc.entity );
   TEST_ASSERT_EQUAL( False, npc.wanders );
   TEST_ASSERT_EQUAL( Direction_Right, sprite.direction );
}

void Npc_Init_NpcWanders_InitializesWanderParameters( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   entity.sprite = &sprite;
   g_random32uResult = 1;
   npc.elapsedSeconds = 100.0f;

   Npc_Init( &npc, &entity, Direction_Right, True );

   TEST_ASSERT_EQUAL( False, npc.isWandering );
   TEST_ASSERT_EQUAL( 1 / 1000.0f, npc.actionSeconds );
   TEST_ASSERT_EQUAL( 0.0f, npc.elapsedSeconds );
}

void Npc_Init_NpcDoesNotWander_DoesNotInitializeWanderParameters( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   entity.sprite = &sprite;
   npc.isWandering = True;
   npc.actionSeconds = 2.0f;
   npc.elapsedSeconds = 100.0f;
   g_random32uResult = 1;

   Npc_Init( &npc, &entity, Direction_Right, False );

   TEST_ASSERT_EQUAL( True, npc.isWandering );
   TEST_ASSERT_EQUAL( 2.0f, npc.actionSeconds );
   TEST_ASSERT_EQUAL( 100.0f, npc.elapsedSeconds );
}
