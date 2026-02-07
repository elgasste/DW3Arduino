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

void Input_HandleOverworldGeneral_LeftIsDown_PlayerFacesLeft( void )
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

void Input_HandleOverworldGeneral_UpIsDown_PlayerFacesUp( void )
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

void Input_HandleOverworldGeneral_RightIsDown_PlayerFacesRight( void )
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

void Input_HandleOverworldGeneral_DownIsDown_PlayerFacesDown( void )
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

void Input_HandleOverworldGeneral_LeftAndUpAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Up].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_LeftAndUpAreDownAndPlayerIsNotFacingUp_PlayerFacesLeft( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Up].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Right );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Left, dir );
   }
}

void Input_HandleOverworldGeneral_UpAndRightAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_UpAndRightAreDownAndPlayerIsNotFacingUp_PlayerFacesRight( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Left );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_RightAndDownAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}

void Input_HandleOverworldGeneral_RightAndDownAreDownAndPlayerIsNotFacingDown_PlayerFacesRight( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_DownAndLeftAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}

void Input_HandleOverworldGeneral_DownAndLeftAreDownAndPlayerIsNotFacingDown_PlayerFacesLeft( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Left, dir );
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

void Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Right );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir);
   }
}

void Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingDown_PlayerFacesUp( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_LeftAndUpAndRightAreDownAndPlayerIsFacingUp_PlayerKeepsFacingUp( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingVertical_PlayerKeepsFacingVertical( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Up, dir );
   }
}

void Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingLeft_PlayerFacesRight( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Left );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_UpAndRightAndDownAreDownAndPlayerIsFacingRight_PlayerKeepsFacingRight( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Right );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Right );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingUp_PlayerFacesDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}

void Input_HandleOverworldGeneral_RightAndDownAndLeftAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}

void Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingHorizontal_PlayerKeepsFacingHorizontal( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Right );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Right, dir );
   }
}

void Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingUp_PlayerFacesDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Up );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}

void Input_HandleOverworldGeneral_DownAndLeftAndRightAreDownAndPlayerIsFacingDown_PlayerKeepsFacingDown( void )
{
   Direction_t dir;
   Game_t* game = GameUtil_CreateSimpleGame();
   TEST_ASSERT_NOT_NULL( game );

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;
      game->input.buttonStates[InputButton_Left].down = True;
      game->input.buttonStates[InputButton_Right].down = True;
      ActiveSprite_SetDirection( game->players->entity->sprite, Direction_Down );

      Input_HandleInput( game );
      dir = game->players->entity->sprite->direction;

      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( Direction_Down, dir );
   }
}