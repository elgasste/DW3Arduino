#include <game.h>

void Screen_LoadPalette( Screen_t* screen )
{
   UNUSED_PARAM( screen );
}

void Screen_LoadTextBitFields( Screen_t* screen )
{
   UNUSED_PARAM( screen );
}

void TileMap_LoadPlayerSprites( TileMap_t* tileMap )
{
   UNUSED_PARAM( tileMap );
}

void TileMap_LoadFromIndex( TileMap_t* tileMap, u32 index )
{
   UNUSED_PARAM( tileMap );
   UNUSED_PARAM( index );
}

Bool_t TileMap_AllowsShip( u32 index )
{
   UNUSED_PARAM( index );

   return False;
}

Bool_t TileMap_AllowsRamia( u32 index )
{
   UNUSED_PARAM( index );
   
   return False;
}

void Game_Reset( Game_t* game )
{
   UNUSED_PARAM( game );
}
