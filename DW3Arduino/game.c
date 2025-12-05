#include "game.h"

#define DIAGONAL_SCALAR 0.707f

internal void Game_HandleInput( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   TileMap_Init( &game->tileMap );

   Screen_LoadPaletteFromIndex( &game->screen, 0 );
   TileMap_LoadTileTextures( &game->tileMap );
   TileMap_LoadFromIndex( &game->tileMap, 0 );

   game->tileMap.viewport.w = 380;
   game->tileMap.viewport.h = 220;
   game->tileMap.viewportScreenPos.x = 20;
   game->tileMap.viewportScreenPos.y = 10;

   game->player.entity = &game->tileMap.entities[0];
   game->player.entity->pos.x = 24.0f;
   game->player.entity->pos.y = 24.0f;
   game->player.entity->pos.w = 12.0f;
   game->player.entity->pos.h = 12.0f;
   game->player.entity->velocity.x = 0.0f;
   game->player.entity->velocity.y = 0.0f;

   TileMap_ClampViewportToEntity( &game->tileMap, game->player.entity );
}

void Game_Tic( Game_t* game )
{
   Input_Read( &game->input );
   Game_HandleInput( game );
   TileMap_Tic( &game->tileMap );
   Physics_Tic( game );
   Render_DrawGame( game );
}

internal void Game_HandleInput( Game_t* game )
{
   Entity_t* entity = game->player.entity;

   Bool_t leftIsDown = game->input.buttonStates[InputButton_Left].down;
   Bool_t upIsDown = game->input.buttonStates[InputButton_Up].down;
   Bool_t rightIsDown = game->input.buttonStates[InputButton_Right].down;
   Bool_t downIsDown = game->input.buttonStates[InputButton_Down].down;

   if ( leftIsDown && !rightIsDown )
   {
      entity->velocity.x = -PLAYER_MAX_VELOCITY;

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= DIAGONAL_SCALAR;
      }
   }
   else if ( rightIsDown && !leftIsDown )
   {
      entity->velocity.x = PLAYER_MAX_VELOCITY;

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= DIAGONAL_SCALAR;
      }
   }
   if ( upIsDown && !downIsDown )
   {
      entity->velocity.y = -PLAYER_MAX_VELOCITY;

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= DIAGONAL_SCALAR;
      }
   }
   else if ( downIsDown && !upIsDown )
   {
      entity->velocity.y = PLAYER_MAX_VELOCITY;

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= DIAGONAL_SCALAR;
      }
   }
}
