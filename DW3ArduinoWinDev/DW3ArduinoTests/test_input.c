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

void Input_HandleInput_OverworldActiveState_OnlyLeftIsDown_HorizontalVelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocity = 0, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Left].down = True;

      game->players->entity->velocity.x = 0;
      Input_HandleInput( game );
      velocity = game->players->entity->velocity.x;
      expected = -( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocity );
   }
}

void Input_HandleInput_OverworldActiveState_OnlyUpIsDown_VerticalVelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocity = 0, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Up].down = True;

      game->players->entity->velocity.y = 0;
      Input_HandleInput( game );
      velocity = game->players->entity->velocity.y;
      expected = -( TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex ) );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocity );
   }
}

void Input_HandleInput_OverworldActiveState_OnlyRightIsDown_HorizontalVelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocity = 0, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Right].down = True;

      game->players->entity->velocity.x = 0;
      Input_HandleInput( game );
      velocity = game->players->entity->velocity.x;
      expected = TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocity );
   }
}

void Input_HandleInput_OverworldActiveState_OnlyDownIsDown_VerticalVelocityIsCorrect( void )
{
   Game_t* game = GameUtil_CreateSimpleGame();
   i32 velocity = 0, expected;

   if ( game )
   {
      game->state = GameState_Overworld_Active;
      game->input.buttonStates[InputButton_Down].down = True;

      game->players->entity->velocity.y = 0;
      Input_HandleInput( game );
      velocity = game->players->entity->velocity.y;
      expected = TileMap_GetTileVelocity( &game->tileMap, game->players->entity->tileIndex );
      GameUtil_DeleteGame( game );

      TEST_ASSERT_EQUAL( expected, velocity );
   }
}
