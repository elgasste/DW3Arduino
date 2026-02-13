#include <unity.h>
#include <game.h>

#include "main.h"
#include "mock_random.h"

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
   Mock_Random_ResetAndPush32u( 0 );
   Mock_Random_Push32u( 1000 );
   npc.elapsedSeconds = 100.0f;

   Npc_Init( &npc, &entity, Direction_Right, True );

   TEST_ASSERT_EQUAL( True, npc.isWandering );
   TEST_ASSERT_EQUAL( 1.0f, npc.actionSeconds );
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
   Mock_Random_ResetAndPush32u( 1 );
   Mock_Random_Push32u( 1000 );

   Npc_Init( &npc, &entity, Direction_Right, False );

   TEST_ASSERT_EQUAL( True, npc.isWandering );
   TEST_ASSERT_EQUAL( 2.0f, npc.actionSeconds );
   TEST_ASSERT_EQUAL( 100.0f, npc.elapsedSeconds );
}

void Npc_Tic_DoesNotWander_DoesNothing( void )
{
   Npc_t npc;

   npc.wanders = False;
   npc.elapsedSeconds = 0.0f;

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( 0.0f, npc.elapsedSeconds );
}

void Npc_Tic_Wanders_IncrementsElapsedSeconds( void )
{
   Npc_t npc;

   npc.wanders = True;
   npc.actionSeconds = CLOCK_FRAME_SECONDS * 2;
   npc.elapsedSeconds = 0.0f;

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, npc.elapsedSeconds );
}

void Npc_Tic_WandersAndIsWanderingAndActionHasElapsed_StopsMoving( void )
{
   Npc_t npc;
   Entity_t entity;

   entity.velocity.x = 20;
   entity.velocity.y = 20;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = True;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32u( 1000 );

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( False, npc.isWandering );
   TEST_ASSERT_EQUAL( 0, npc.entity->velocity.x );
   TEST_ASSERT_EQUAL( 0, npc.entity->velocity.y );
   TEST_ASSERT_EQUAL( 0.0f, npc.elapsedSeconds );
   TEST_ASSERT_EQUAL( 1.0f, npc.actionSeconds );
}

void Npc_Tic_WandersAndIsNotWanderingAndActionHasElapsed_StartsMoving( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   entity.sprite = &sprite;
   entity.velocity.x = 0;
   entity.velocity.y = 0;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 0 );           // leftward
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 1 );           // downward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( True, npc.isWandering );
   TEST_ASSERT_EQUAL( -20, npc.entity->velocity.x );
   TEST_ASSERT_EQUAL( 20, npc.entity->velocity.y );
   TEST_ASSERT_EQUAL( 0.0f, npc.elapsedSeconds );
   TEST_ASSERT_EQUAL( 1.0f, npc.actionSeconds );
}

void Npc_Tic_FacingLeftAndNotMovingLeftAndMovingUp_SetsDirectionToUp( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Left;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 1 );   // don't move horizontal
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 0 );           // downward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Up, sprite.direction );
}

void Npc_Tic_FacingLeftAndNotMovingLeftAndNotMovingUpAndMovingRight_SetsDirectionToRight( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Left;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 1 );           // rightward
   Mock_Random_Push32u( 1 );           // don't move vertical
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Right, sprite.direction );
}

void Npc_Tic_FacingLeftAndNotMovingLeftAndNotMovingUpAndNotMovingRight_SetsDirectionToDown( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Left;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 1 );   // don't move horizontal
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 1 );           // downward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Down, sprite.direction );
}

void Npc_Tic_FacingUpAndNotMovingUpAndMovingRight_SetsDirectionToRight( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Up;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 1 );           // rightward
   Mock_Random_Push32u( 1 );           // don't move vertical
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Right, sprite.direction );
}

void Npc_Tic_FacingUpAndNotMovingUpAndNotMovingRightAndMovingDown_SetsDirectionToDown( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Up;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 1 );   // don't move horizontal
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 1 );           // downward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Down, sprite.direction );
}

void Npc_Tic_FacingUpAndNotMovingUpAndNotMovingRightAndNotMovingDown_SetsDirectionToLeft( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Up;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 0 );           // leftward
   Mock_Random_Push32u( 1 );           // don't move vertical
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Left, sprite.direction );
}

void Npc_Tic_FacingRightAndNotMovingRightAndMovingDown_SetsDirectionToDown( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Right;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 1 );   // don't move horizontal
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 1 );           // downward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Down, sprite.direction );
}

void Npc_Tic_FacingRightAndNotMovingRightAndNotMovingDownAndMovingLeft_SetsDirectionToLeft( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Right;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 0 );           // leftward
   Mock_Random_Push32u( 1 );           // don't move vertical
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Left, sprite.direction );
}

void Npc_Tic_FacingRightAndNotMovingRightAndNotMovingDownAndNotMovingLeft_SetsDirectionToUp( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Right;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 1 );   // don't move horizontal
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 0 );           // upward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Up, sprite.direction );
}

void Npc_Tic_FacingDownAndNotMovingDownAndMovingLeft_SetsDirectionToLeft( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Down;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 0 );           // leftward
   Mock_Random_Push32u( 1 );           // don't move vertical
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Left, sprite.direction );
}

void Npc_Tic_FacingDownAndNotMovingDownAndNotMovingLeftAndMovingUp_SetsDirectionToUp( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Down;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 1 );   // don't move horizontal
   Mock_Random_Push32u( 0 );           // move vertical
   Mock_Random_Push32u( 0 );           // upward
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Up, sprite.direction );
}

void Npc_Tic_FacingDownAndNotMovingDownAndNotMovingLeftAndNotMovingUp_SetsDirectionToRight( void )
{
   Npc_t npc;
   Entity_t entity;
   ActiveSprite_t sprite;

   sprite.direction = Direction_Down;
   entity.sprite = &sprite;
   npc.entity = &entity;
   npc.wanders = True;
   npc.isWandering = False;
   npc.actionSeconds = CLOCK_FRAME_SECONDS / 2;
   npc.elapsedSeconds = 0.0f;
   Mock_Random_ResetAndPush32i( 20 );  // X velocity
   Mock_Random_Push32i( 20 );          // Y velocity
   Mock_Random_ResetAndPush32u( 0 );   // move horizontal
   Mock_Random_Push32u( 1 );           // rightward
   Mock_Random_Push32u( 1 );           // don't move vertical
   Mock_Random_Push32u( 1000 );        // action seconds * 1000

   Npc_Tic( &npc );

   TEST_ASSERT_EQUAL( Direction_Right, sprite.direction );
}
