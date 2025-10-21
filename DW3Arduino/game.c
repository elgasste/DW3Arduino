#include "game.h"

internal void Game_HandleInput( Game_t* game );
internal void Game_ClipCamPosition( Game_t* game );
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
   game->camPosition.x = 500.0f;
   game->camPosition.y = 500.0f;


}

void Game_Tic( Game_t* game )
{
   Input_Read( &game->input );
   Game_HandleInput( game );
   Game_Draw( game );
}

internal void Game_HandleInput( Game_t* game )
{
   if ( game->input.buttonStates[InputButton_Left].down )
   {
      game->camPosition.x -= ( CAM_VELOCITY * CLOCK_FRAME_SECONDS );
   }
   if ( game->input.buttonStates[InputButton_Right].down )
   {
      game->camPosition.x += ( CAM_VELOCITY * CLOCK_FRAME_SECONDS );
   }
   if ( game->input.buttonStates[InputButton_Up].down )
   {
      game->camPosition.y -= ( CAM_VELOCITY * CLOCK_FRAME_SECONDS );
   }
   if ( game->input.buttonStates[InputButton_Down].down )
   {
      game->camPosition.y += ( CAM_VELOCITY * CLOCK_FRAME_SECONDS );
   }

   Game_ClipCamPosition( game );
   Game_UpdateTileMapViewport( game );
}

internal void Game_ClipCamPosition( Game_t* game )
{
   if ( game->camPosition.x < 0.0f )
   {
      game->camPosition.x = 0.0f;
   }
   else if ( game->camPosition.x >= ( game->tileMap.tilesX * TILE_SIZE ) )
   {
      game->camPosition.x = (r32)( ( game->tileMap.tilesX * TILE_SIZE ) - 1 );
   }

   if ( game->camPosition.y < 0.0f )
   {
      game->camPosition.y = 0.0f;
   }
   else if ( game->camPosition.y >= ( game->tileMap.tilesY * TILE_SIZE ) )
   {
      game->camPosition.y = (r32)( ( game->tileMap.tilesY * TILE_SIZE ) - 1 );
   }
}

internal void Game_UpdateTileMapViewport( Game_t* game )
{
   game->tileMapViewport.x = (i32)( game->camPosition.x ) - ( game->tileMapViewport.w / 2 );
   game->tileMapViewport.y = (i32)( game->camPosition.y ) - ( game->tileMapViewport.h / 2 );
}
