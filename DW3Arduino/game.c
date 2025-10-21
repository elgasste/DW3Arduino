#include "game.h"

internal void Game_HandleInput( Game_t* game );
internal void Game_TicPhysics( Game_t* game );
internal void Game_ClipPlayerEntity( Game_t* game );
internal void Game_UpdateTileMapViewport( Game_t* game );

void Game_Init( Game_t* game, u16* screenBuffer )
{
   Clock_Init( &game->clock );
   Screen_Init( &game->screen, screenBuffer );
   Input_Init( &game->input );
   TileMap_Init( &game->tileMap );

   Screen_LoadPaletteFromIndex( &game->screen, 0 );
   TileMap_LoadTileTextures( &game->tileMap );
   TileMap_LoadFromIndex( &game->tileMap, 0 );

   game->tileMapViewport.w = 380;
   game->tileMapViewport.h = 220;
   game->tileMapViewportScreenPos.x = 20;
   game->tileMapViewportScreenPos.y = 10;

   game->playerEntity.hitBox.x = 3000.0f;
   game->playerEntity.hitBox.y = 3000.0f;
   game->playerEntity.hitBox.w = 50.0f;
   game->playerEntity.hitBox.h = 50.0f;
   game->playerEntity.velocity.x = 0.0f;
   game->playerEntity.velocity.y = 0.0f;

   Game_UpdateTileMapViewport( game );
}

void Game_Tic( Game_t* game )
{
   Input_Read( &game->input );
   Game_HandleInput( game );
   Game_TicPhysics( game );
   Game_Draw( game );
}

internal void Game_HandleInput( Game_t* game )
{
   if ( game->input.buttonStates[InputButton_Left].down )
   {
      game->playerEntity.velocity.x = -PLAYER_MAX_VELOCITY;
   }
   if ( game->input.buttonStates[InputButton_Right].down )
   {
      game->playerEntity.velocity.x = PLAYER_MAX_VELOCITY;
   }
   if ( game->input.buttonStates[InputButton_Up].down )
   {
      game->playerEntity.velocity.y = -PLAYER_MAX_VELOCITY;
   }
   if ( game->input.buttonStates[InputButton_Down].down )
   {
      game->playerEntity.velocity.y = PLAYER_MAX_VELOCITY;
   }
}

internal void Game_TicPhysics( Game_t* game )
{
   game->playerEntity.hitBox.x += game->playerEntity.velocity.x * CLOCK_FRAME_SECONDS;
   game->playerEntity.hitBox.y += game->playerEntity.velocity.y * CLOCK_FRAME_SECONDS;
   game->playerEntity.velocity.x = 0.0f;
   game->playerEntity.velocity.y = 0.0f;

   Game_ClipPlayerEntity( game );
   Game_UpdateTileMapViewport( game );
}

internal void Game_ClipPlayerEntity( Game_t* game )
{
   if ( game->playerEntity.hitBox.x < 0.0f )
   {
      game->playerEntity.hitBox.x = 0.0f;
   }
   else if ( ( game->playerEntity.hitBox.x + game->playerEntity.hitBox.w ) >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      game->playerEntity.hitBox.x = (r32)( ( game->tileMap.tilesX * TILE_SIZE ) ) - game->playerEntity.hitBox.w;
   }

   if ( game->playerEntity.hitBox.y < 0.0f )
   {
      game->playerEntity.hitBox.y = 0.0f;
   }
   else if ( ( game->playerEntity.hitBox.y + game->playerEntity.hitBox.h ) >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      game->playerEntity.hitBox.y = (r32)( ( game->tileMap.tilesY * TILE_SIZE ) ) - game->playerEntity.hitBox.h;
   }
}

internal void Game_UpdateTileMapViewport( Game_t* game )
{
   game->tileMapViewport.x = (i32)( game->playerEntity.hitBox.x + ( game->playerEntity.hitBox.w / 2 ) ) - ( game->tileMapViewport.w / 2 );
   game->tileMapViewport.y = (i32)( game->playerEntity.hitBox.y + ( game->playerEntity.hitBox.h / 2 ) ) - ( game->tileMapViewport.h / 2 );
}
