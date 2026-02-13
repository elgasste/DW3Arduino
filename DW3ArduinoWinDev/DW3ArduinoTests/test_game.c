#include <unity.h>
#include <game.h>

#include "main.h"
#include "mock_clock.h"
#include "mock_random.h"

void Game_Init_Always_InitializesClock( void )
{
   u16* screenBuffer = (u16*)malloc( sizeof( u16 ) * ( SCREEN_WIDTH * SCREEN_HEIGHT ) );
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );

   TEST_ASSERT_NOT_NULL( screenBuffer );
   TEST_ASSERT_NOT_NULL( game );

   if ( screenBuffer && game )
   {
      g_mock_clock_frameStartMicro = 1000;
      g_mock_clock_lagFrameCount = 9;

      game->clock.frameStartMicro = 0;
      game->clock.lagFrameCount = 0;

      Game_Init( game, screenBuffer );

      TEST_ASSERT_EQUAL( 1000, game->clock.frameStartMicro );
      TEST_ASSERT_EQUAL( 9, game->clock.lagFrameCount );

      free( screenBuffer );
      free( game );
   }
}

void Game_Init_Always_InitializesScreen( void )
{
   u16* screenBuffer = (u16*)malloc( sizeof( u16 ) * ( SCREEN_WIDTH * SCREEN_HEIGHT ) );
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );

   TEST_ASSERT_NOT_NULL( screenBuffer );
   TEST_ASSERT_NOT_NULL( game );

   if ( screenBuffer && game )
   {
      Game_Init( game, screenBuffer );

      TEST_ASSERT_EQUAL( screenBuffer, game->screen.buffer );

      free( screenBuffer );
      free( game );
   }
}

void Game_Init_Always_InitializesInput( void )
{
   u32 i;
   u16* screenBuffer = (u16*)malloc( sizeof( u16 ) * ( SCREEN_WIDTH * SCREEN_HEIGHT ) );
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );

   TEST_ASSERT_NOT_NULL( screenBuffer );
   TEST_ASSERT_NOT_NULL( game );

   if ( screenBuffer && game )
   {
      for ( i = 0; i < InputButton_Count; i++ )
      {
         game->input.buttonStates[i].pressed = True;
         game->input.buttonStates[i].released = True;
         game->input.buttonStates[i].down = True;
      }

      Game_Init( game, screenBuffer );

      for ( i = 0; i < InputButton_Count; i++ )
      {
         TEST_ASSERT_EQUAL( False, game->input.buttonStates[i].pressed );
         TEST_ASSERT_EQUAL( False, game->input.buttonStates[i].released );
         TEST_ASSERT_EQUAL( False, game->input.buttonStates[i].down );
      }

      free( screenBuffer );
      free( game );
   }
}

void Game_Init_Always_InitializesAnimationChain( void )
{
   u16* screenBuffer = (u16*)malloc( sizeof( u16 ) * ( SCREEN_WIDTH * SCREEN_HEIGHT ) );
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );

   TEST_ASSERT_NOT_NULL( screenBuffer );
   TEST_ASSERT_NOT_NULL( game );

   if ( screenBuffer && game )
   {
      Game_Init( game, screenBuffer );

      TEST_ASSERT_EQUAL( &game->screen, game->animationChain.screen );

      free( screenBuffer );
      free( game );
   }
}

void Game_Init_Always_InitializesTileMap( void )
{
   u16* screenBuffer = (u16*)malloc( sizeof( u16 ) * ( SCREEN_WIDTH * SCREEN_HEIGHT ) );
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );

   TEST_ASSERT_NOT_NULL( screenBuffer );
   TEST_ASSERT_NOT_NULL( game );

   if ( screenBuffer && game )
   {
      Game_Init( game, screenBuffer );

      TEST_ASSERT_EQUAL( game->players, game->tileMap.players );
      TEST_ASSERT_EQUAL( game, game->tileMap.playerCountProvider );

      free( screenBuffer );
      free( game );
   }
}

void Game_Init_Always_InitializesViewports( void )
{
   u16* screenBuffer = (u16*)malloc( sizeof( u16 ) * ( SCREEN_WIDTH * SCREEN_HEIGHT ) );
   Game_t* game = (Game_t*)malloc( sizeof( Game_t ) );

   TEST_ASSERT_NOT_NULL( screenBuffer );
   TEST_ASSERT_NOT_NULL( game );

   if ( screenBuffer && game )
   {
      Game_Init( game, screenBuffer );

      TEST_ASSERT_EQUAL( SCREEN_WIDTH * UNITS_PER_PIXEL, game->tileMap.viewport.w );
      TEST_ASSERT_EQUAL( SCREEN_HEIGHT * UNITS_PER_PIXEL, game->tileMap.viewport.h );
      TEST_ASSERT_EQUAL( 0, game->tileMap.viewportScreenPos.x );
      TEST_ASSERT_EQUAL( 0, game->tileMap.viewportScreenPos.y );

      free( screenBuffer );
      free( game );
   }
}
