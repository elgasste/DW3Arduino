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

void Input_HandleOverworldGeneral_OnlyLeftIsDown_VelocityIsCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_OnlyUpIsDown_VelocityIsCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_OnlyRightIsDown_VelocityIsCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_OnlyDownIsDown_VelocityIsCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_LeftAndUpAreDown_VelocitiesAreCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_UpAndRightAreDown_VelocitiesAreCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_RightAndDownAreDown_VelocitiesAreCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_DownAndLeftAreDown_VelocitiesAreCorrect( void )
{
   i32 velocityX, velocityY, expected;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

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

void Input_HandleOverworldGeneral_OnlyLeftIsDown_PlayerFacesLeft( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Left, dir );
   }
}

void Input_HandleOverworldGeneral_OnlyUpIsDown_PlayerFacesUp( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_OnlyRightIsDown_PlayerFacesRight( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_OnlyDownIsDown_PlayerFacesDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}

void Input_HandleOverworldGeneral_LeftAndRightAreDown_PlayerDirectionDoesNotChange( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_UpAndDownAreDown_PlayerDirectionDoesNotChange( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Right );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

// MUFFINS: multiple directional tests:
//
// - we're pressing left:
//    - the sprite is facing up and we're also pressing up
//       - check that it still faces up
//       - otherwise check that it faces left
//    - the sprite is facing down and we're also pressing down
//       - check that it still faces down
//       - otherwise check that it faces left
//
// (figure out the rest of these later)
