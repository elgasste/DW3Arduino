#include "game.h"

internal void Game_ClipPlayerEntity( Game_t* game );

void Game_TicPhysics( Game_t* game )
{
   game->playerEntity.hitBox.x += game->playerEntity.velocity.x * CLOCK_FRAME_SECONDS;
   game->playerEntity.hitBox.y += game->playerEntity.velocity.y * CLOCK_FRAME_SECONDS;
   game->playerEntity.velocity.x = 0.0f;
   game->playerEntity.velocity.y = 0.0f;

   Game_ClipPlayerEntity( game );
   TileMap_UpdateViewport( &game->tileMap, &game->playerEntity );
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
