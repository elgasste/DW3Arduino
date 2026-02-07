#include <unity.h>
#include <sprite.h>

#include "main.h"

void ActiveSprite_Init_Always_InitializesParameters( void )
{
   ActiveSprite_t sprite;
   sprite.frame = 5;
   sprite.frameSeconds = 7.9f;

   ActiveSprite_Init( &sprite, 1, 2, 3, Direction_Right );

   TEST_ASSERT_EQUAL( 1, sprite.textureIndex );
   TEST_ASSERT_EQUAL( 2, sprite.offset.x );
   TEST_ASSERT_EQUAL( 3, sprite.offset.y );
   TEST_ASSERT_EQUAL( Direction_Right, sprite.direction );
   TEST_ASSERT_EQUAL( 0, sprite.frame );
   TEST_ASSERT_EQUAL( 0.0f, sprite.frameSeconds );
}

void ActiveSprite_SetDirection_DirectionHasNotChanged_DoesNotResetFrame( void )
{
   ActiveSprite_t sprite;
   ActiveSprite_Init( &sprite, 0, 0, 0, Direction_Down );
   sprite.frame = 3;

   ActiveSprite_SetDirection( &sprite, Direction_Down );

   TEST_ASSERT_EQUAL( 3, sprite.frame );
   TEST_ASSERT_EQUAL( Direction_Down, sprite.direction );
}

void ActiveSprite_SetDirection_DirectionHasChanged_ResetsFrameAndChangesDirection( void )
{
   ActiveSprite_t sprite;
   ActiveSprite_Init( &sprite, 0, 0, 0, Direction_Down );
   sprite.frame = 3;

   ActiveSprite_SetDirection( &sprite, Direction_Right );

   TEST_ASSERT_EQUAL( 0, sprite.frame );
   TEST_ASSERT_EQUAL( Direction_Right, sprite.direction );
}

void ActiveSprite_Tic_FrameSecondsHasNotExceededTotal_FrameAndFrameSecondsDoNotChange( void )
{
   ActiveSprite_t sprite;
   ActiveSprite_Init( &sprite, 0, 0, 0, Direction_Down );

   ActiveSprite_Tic( &sprite );

   TEST_ASSERT_EQUAL( 0, sprite.frame );
   TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, sprite.frameSeconds );
}

void ActiveSprite_Tic_FrameSecondsHasExceededTotal_ResetsFrameSecondsAndIncrementsFrame( void )
{
   ActiveSprite_t sprite;
   ActiveSprite_Init( &sprite, 0, 0, 0, Direction_Down );
   sprite.frameSeconds = ACTIVE_SPRITE_FRAME_TOTAL_SECONDS - ( CLOCK_FRAME_SECONDS / 2.0f );

   ActiveSprite_Tic( &sprite );

   TEST_ASSERT_EQUAL( 1, sprite.frame );
   TEST_ASSERT_EQUAL( ( CLOCK_FRAME_SECONDS / 2.0f ), sprite.frameSeconds );
}

void ActiveSprite_Tic_LastFrameAndFrameSecondsHasExceededTotal_SetsFrameToZero( void )
{
   ActiveSprite_t sprite;
   ActiveSprite_Init( &sprite, 0, 0, 0, Direction_Down );
   sprite.frame = ACTIVE_SPRITE_FRAMES - 1;
   sprite.frameSeconds = ACTIVE_SPRITE_FRAME_TOTAL_SECONDS - ( CLOCK_FRAME_SECONDS / 2.0f );

   ActiveSprite_Tic( &sprite );

   TEST_ASSERT_EQUAL( 0, sprite.frame );
}
