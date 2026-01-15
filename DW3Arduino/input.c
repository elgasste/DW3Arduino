#include "game.h"

#define DIAGONAL_SCALAR 0.707f

internal void Input_UpdateButtonState( InputButtonState_t* buttonState, Bool_t down );
internal void Input_HandleOverworldGeneral( Game_t* game );

void Input_Init( Input_t* input )
{
   u32 i;

   for ( i = 0; i < InputButton_Count; i++ )
   {
      input->buttonStates[i].pressed = False;
      input->buttonStates[i].released = False;
      input->buttonStates[i].down = False;
   }
}

void Input_Read( Input_t* input )
{
#if defined( VISUAL_STUDIO_DEV )
   UNUSED_PARAM( input );
#else
   u8 nesInput = Input_ReadNesController();

   Input_UpdateButtonState( &( input->buttonStates[ InputButton_A ] ), ( nesInput & INPUT_A_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_B ] ), ( nesInput & INPUT_B_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_Left ] ), ( nesInput & INPUT_LEFT_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_Up ] ), ( nesInput & INPUT_UP_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_Right ] ), ( nesInput & INPUT_RIGHT_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_Down ] ), ( nesInput & INPUT_DOWN_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_Start ] ), ( nesInput & INPUT_START_FLAG ) ? True : False );
   Input_UpdateButtonState( &( input->buttonStates[ InputButton_Select ] ), ( nesInput & INPUT_SELECT_FLAG ) ? True: False );
#endif
}

Bool_t Input_AnyButtonPressed( Input_t* input )
{
   u32 i;

   for ( i = 0; i < InputButton_Count; i++ )
   {
      if ( input->buttonStates[i].pressed )
      {
         return True;
      }
   }

   return False;
}

void Input_HandleInput( Game_t* game )
{
   if ( game->state < GameState_Intro_Count )
   {
      // TODO: handle intro input
   }
   else if ( game->state <= GameState_Overworld_Inactive )
   {
      Input_HandleOverworldGeneral( game );
   }
}

internal void Input_UpdateButtonState( InputButtonState_t* buttonState, Bool_t down )
{
   if ( down )
   {
      buttonState->released = False;
      buttonState->pressed = buttonState->down ? False : True;
   }
   else
   {
      buttonState->pressed = False;
      buttonState->released = buttonState->down ? True : False;
   }

   buttonState->down = down;
}

internal void Input_HandleOverworldGeneral( Game_t* game )
{
   Entity_t* entity = game->players->entity;
   ActiveSprite_t* sprite = game->players->entity->sprite;
   i32 velocity = TileMap_GetTileVelocity( &game->tileMap, game->players->tileIndex );

#if defined( VISUAL_STUDIO_DEV )
   if ( g_winDebugFlags.fastWalk )
   {
      velocity = 256;
   }
#endif

   Bool_t leftIsDown = game->input.buttonStates[InputButton_Left].down;
   Bool_t upIsDown = game->input.buttonStates[InputButton_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[InputButton_Right].down;
   Bool_t downIsDown = game->input.buttonStates[InputButton_Down].down;

   if ( leftIsDown && !rightIsDown )
   {
      entity->velocity.x = -velocity;

      if ( !( upIsDown && sprite->direction == Direction_Up ) &&
           !( downIsDown && sprite->direction == Direction_Down ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Left );
      }

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x = (i32)( entity->velocity.x * DIAGONAL_SCALAR );
      }
   }
   else if ( rightIsDown && !leftIsDown )
   {
      entity->velocity.x = velocity;

      if ( !( upIsDown && sprite->direction == Direction_Up ) &&
           !( downIsDown && sprite->direction == Direction_Down ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Right );
      }

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x = (i32)( entity->velocity.x * DIAGONAL_SCALAR );
      }
   }
   if ( upIsDown && !downIsDown )
   {
      entity->velocity.y = -velocity;

      if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
           !( rightIsDown && sprite->direction == Direction_Right ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Up );
      }

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y = (i32)( entity->velocity.y * DIAGONAL_SCALAR );
      }
   }
   else if ( downIsDown && !upIsDown )
   {
      entity->velocity.y = velocity;

      if ( !( leftIsDown && sprite->direction == Direction_Left ) &&
           !( rightIsDown && sprite->direction == Direction_Right ) )
      {
         ActiveSprite_SetDirection( sprite, Direction_Down );
      }

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y = (i32)( entity->velocity.y * DIAGONAL_SCALAR );
      }
   }
}
