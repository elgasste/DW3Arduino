#include <unity.h>
#include <game.h>

#include "game_util.h"
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

void Game_Tic_Always_RendersGame( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->animationChain.isRunning = False;
      game->state = GameState_Count;
      Screen_WipeColor( &game->screen, SCREEN_COLOR16_BLUE );

      TEST_ASSERT_EQUAL( SCREEN_COLOR16_BLUE, game->screen.buffer[0] );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( SCREEN_COLOR16_BLACK, game->screen.buffer[0] );

      GameUtil_DeleteGame( game );
   }
}

void Game_Tic_AnimationChainIsRunning_TicsAnimationChain( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      AnimationChain_Push( &game->animationChain, AnimationType_Count, 0.0f );
      AnimationChain_Start( &game->animationChain );

      TEST_ASSERT_EQUAL( True, game->animationChain.isRunning );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( False, game->animationChain.isRunning );

      GameUtil_DeleteGame( game );
   }
}

void Game_Tic_AnimationChainIsNotRunning_DoesNotTicAnimationChain( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->animationChain.curAnimation = 0;
      game->animationChain.animations[0].elapsedSeconds = 1.0f;

      TEST_ASSERT_EQUAL( False, game->animationChain.isRunning );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( 1.0f, game->animationChain.animations[0].elapsedSeconds );

      GameUtil_DeleteGame( game );
   }
}

void Game_Tic_AnimationChainBlocksInput_DoesNotHandleInput( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->tileMap.playerSprites[0].direction = Direction_Down;
      AnimationChain_Push( &game->animationChain, AnimationType_Pause, 1.0f );
      AnimationChain_Start( &game->animationChain );

      TEST_ASSERT_EQUAL( True, game->animationChain.isRunning );
      TEST_ASSERT_EQUAL( True, AnimationChain_BlocksInput( &game->animationChain ) );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( Direction_Down, game->tileMap.playerSprites[0].direction );

      GameUtil_DeleteGame( game );
   }
}

void Game_Tic_AnimationChainIsNotBlockingInput_HandlesInput( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->tileMap.playerSprites[0].direction = Direction_Down;

      TEST_ASSERT_EQUAL( False, game->animationChain.isRunning );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( Direction_Left, game->tileMap.playerSprites[0].direction );

      GameUtil_DeleteGame( game );
   }
}

// TODO: this is where the different game state actions will go
//void Game_Tic_AnimationChainDoesNotPauseAction_
