#include <unity.h>

#include "game_util.h"

void Input_Init_Always_SetsAllStatesToFalse( void )
{
   u32 i;
   Input_t input;

   Input_Init( &input );

   for ( i = 0; i < InputButton_Count; i++ )
   {
      TEST_ASSERT_EQUAL( False, input.buttonStates[i].pressed );
      TEST_ASSERT_EQUAL( False, input.buttonStates[i].released );
      TEST_ASSERT_EQUAL( False, input.buttonStates[i].down );
   }
}

void Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse( void )
{
   Input_t input;
   Bool_t result;

   Input_Init( &input );
   result = Input_AnyButtonPressed( &input );

   TEST_ASSERT_EQUAL( False, result );
}

void Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue( void )
{
   Input_t input;
   Bool_t result;

   Input_Init( &input );
   input.buttonStates[InputButton_A].pressed = True;
   result = Input_AnyButtonPressed( &input );

   TEST_ASSERT_EQUAL( True, result );
}

void Input_HandleInput_OverworldActiveState_OnlyLeftIsDown_VelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = -( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocityX );
      TEST_ASSERT_EQUAL( 0, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_OnlyUpIsDown_VelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = -( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( 0, velocityX );
      TEST_ASSERT_EQUAL( expected, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_OnlyRightIsDown_VelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocityX );
      TEST_ASSERT_EQUAL( 0, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_OnlyDownIsDown_VelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( 0, velocityX );
      TEST_ASSERT_EQUAL( expected, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_LeftAndUpAreDown_VelocitiesAreCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Up].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = TileMap_GetTileDiagonalVelocity( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( -expected, velocityX );
      TEST_ASSERT_EQUAL( -expected, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_UpAndRightAreDown_VelocitiesAreCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = TileMap_GetTileDiagonalVelocity( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocityX );
      TEST_ASSERT_EQUAL( -expected, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_RightAndDownAreDown_VelocitiesAreCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = TileMap_GetTileDiagonalVelocity( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocityX );
      TEST_ASSERT_EQUAL( expected, velocityY );
   }
}

void Input_HandleInput_OverworldActiveState_DownAndLeftAreDown_VelocitiesAreCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocityX, velocityY, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;

      Input_HandleInput( game );
      velocityX = game->players->entity->velocity.x;
      velocityY = game->players->entity->velocity.y;
      expected = TileMap_GetTileDiagonalVelocity( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( -expected, velocityX );
      TEST_ASSERT_EQUAL( expected, velocityY );
   }
}

//void Input_HandleInput_OverworldActiveState_LeftAndUpAreDown_

// MUFFINS: test sprite directions
