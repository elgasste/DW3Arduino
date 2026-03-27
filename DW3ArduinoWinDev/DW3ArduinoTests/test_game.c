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

void Game_Tic_AnimationChainPausesAction_DoesNotPerformActions( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->daylightFactor = DAY_FACTOR_HIGH_CUTOFF + 1.0f;
      game->screen.dayFilterIntensity = 0.0f;
      AnimationChain_Push( &game->animationChain, AnimationType_Pause, 1.0f );
      AnimationChain_Start( &game->animationChain );

      TEST_ASSERT_EQUAL( True, game->animationChain.isRunning );
      TEST_ASSERT_EQUAL( True, AnimationChain_PausesAction( &game->animationChain ) );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( 0.0f, game->screen.dayFilterIntensity );

      GameUtil_DeleteGame( game );
   }
}

void Game_TicByState_GameStateIsOverworldActive_UpdatesDayFilterIntensity( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->daylightFactor = 1.0f;
      game->screen.dayFilterIntensity = 0.0f;

      Game_Tic( game );

      TEST_ASSERT_EQUAL( 1.0f, game->screen.dayFilterIntensity );

      GameUtil_DeleteGame( game );
   }
}

void Game_TicByState_GameStateIsOverworldAndNotActive_DoesNotUpdateDayFilterIntensity( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Inactive;
      game->daylightFactor = 1.0f;
      game->screen.dayFilterIntensity = 0.0f;

      Game_Tic( game );

      TEST_ASSERT_EQUAL( 0.0f, game->screen.dayFilterIntensity );

      GameUtil_DeleteGame( game );
   }
}

void Game_TicByState_GameStateIsOverworld_TicsTileMap( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->players[0].entity->sprite->frameSeconds = 0.0f;

      Game_Tic( game );

      TEST_ASSERT_EQUAL( CLOCK_FRAME_SECONDS, game->players[0].entity->sprite->frameSeconds );

      GameUtil_DeleteGame( game );
   }
}

void Game_TicByState_GameStateIsOverworld_TicsPhysics( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->players[0].entity->pos.x = UNITS_PER_PIXEL;
      game->input.buttonStates[InputButton_Left].down = True;
      expected = UNITS_PER_PIXEL - TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( expected, game->players[0].entity->pos.x );

      GameUtil_DeleteGame( game );
   }
}

void Game_TicByState_GameStateIsOverworld_ClampsViewportToEntity( void )
{
   i32 xPos, yPos;
   Vector4i32_t oldV;
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      oldV = game->tileMap.viewport;
      xPos = ( game->tileMap.tilesX / 2 ) * TILEMAP_TILE_SIZE_UNITS;
      yPos = ( game->tileMap.tilesY / 2 ) * TILEMAP_TILE_SIZE_UNITS;
      game->players[0].entity->pos.x = xPos;
      game->players[0].entity->pos.y = yPos;

      Game_Tic( game );

      TEST_ASSERT_NOT_EQUAL( oldV.x, game->tileMap.viewport.x );
      TEST_ASSERT_NOT_EQUAL( oldV.y, game->tileMap.viewport.y );
      TEST_ASSERT_EQUAL( xPos + ( ( game->players[0].entity->pos.w / 2 ) ) - ( game->tileMap.viewport.w / 2 ), game->tileMap.viewport.x );
      TEST_ASSERT_EQUAL( yPos + ( ( game->players[0].entity->pos.h / 2 ) ) - ( game->tileMap.viewport.h / 2 ), game->tileMap.viewport.y );

      GameUtil_DeleteGame( game );
   }
}

void Game_HandlePlayerMoved_Always_UpdatesFrontPlayerMoveHistory( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->players[0].entity->pos.x = 500;
      game->players[0].entity->pos.y = 400;
      game->players[0].entity->velocity.x = 100;
      game->players[0].entity->velocity.y = 400;
      game->players[0].entity->sprite->direction = Direction_Right;

      Game_Tic( game );

      TEST_ASSERT_EQUAL( 600, game->players[0].moveHistory[0].newPos.x );
      TEST_ASSERT_EQUAL( 800, game->players[0].moveHistory[0].newPos.y );
      TEST_ASSERT_EQUAL( Direction_Right, game->players[0].moveHistory[0].newDir );
      TEST_ASSERT_EQUAL( 1, game->players[0].moveHistoryIndex );

      GameUtil_DeleteGame( game );
   }
}

void Game_HandlePlayerMoved_FrontPlayerMoveHistoryIndexOverflow_ChainsNextPlayer( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->players[0].moveHistoryIndex = PLAYER_MOVE_HISTORY_SIZE - 1;
      game->players[0].entity->velocity.x = 100;
      game->players[0].entity->velocity.y = 400;

      TEST_ASSERT_EQUAL( False, game->players[0].chainNextPlayer );

      Game_Tic( game );

      TEST_ASSERT_EQUAL( True, game->players[0].chainNextPlayer );
      TEST_ASSERT_EQUAL( 0, game->players[0].moveHistoryIndex );

      GameUtil_DeleteGame( game );
   }
}

// TODO: this is just here for reference

//internal void Game_HandlePlayerMoved( Game_t* game )
//{
//   u32 tileIndex;
//   Player_t* frontPlayer = game->players;
//
//   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].newPos.x = frontPlayer->entity->pos.x;
//   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].newPos.y = frontPlayer->entity->pos.y;
//   frontPlayer->moveHistory[frontPlayer->moveHistoryIndex].newDir = frontPlayer->entity->sprite->direction;
//   frontPlayer->moveHistoryIndex++;
//
//   if ( frontPlayer->moveHistoryIndex >= PLAYER_MOVE_HISTORY_SIZE )
//   {
//      frontPlayer->chainNextPlayer = True;
//      frontPlayer->moveHistoryIndex = 0;
//   }
//
//   Game_AnchorRearPlayers( game );
//
//   if ( game->tileMap.affectsDaylight )
//   {
//      Game_IncrementDaylightFactor( game );
//   }
//
//   tileIndex = TileMap_GetTileIndexAtPosition( &game->tileMap,
//                                               ( game->players->entity->pos.x + ( game->players->entity->pos.w / 2 ) ),
//                                               ( game->players->entity->pos.y + ( game->players->entity->pos.h / 2 ) ) );
//
//   if ( tileIndex != game->players->entity->tileIndex )
//   {
//      Game_SteppedOnTile( game, tileIndex );
//   }
//}

void Game_HandlePlayerMoved_Always_AnchorsRearPlayers( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->players[0].moveHistoryIndex = PLAYER_MOVE_HISTORY_SIZE - 1;
      game->players[0].entity->velocity.x = 100;
      game->players[0].entity->velocity.y = 400;

      Game_Tic( game );

      // TODO: woof, figure this one out

      GameUtil_DeleteGame( game );
   }
}

void Game_HandlePlayerMoved_TileMapAffectsDaylight_IncrementsDaylightFactor( void )
{
   // TODO
}

void Game_HandlePlayerMoved_SteppedOnDifferentTile_StepsOnTile( void )
{
   // TODO
}
