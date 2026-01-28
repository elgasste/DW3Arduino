#include "game.h"

Bool_t Validate_PlayerCount( i32 count )
{
   return ( count > 0 && count <= MAX_PLAYERS ) ? True : False;
}

Bool_t Validate_PlayerName( const char* name )
{
   // TODO: also check that the characters are valid
   size_t length = strlen( name );
   return ( length > 0 || length < PLAYER_MAX_NAME_LENGTH ) ? True : False;
}

Bool_t Validate_PlayerClass( i32 playerClass )
{
   return ( playerClass >= 0 && playerClass < PlayerClass_Count ) ? True : False;
}

Bool_t Validate_SingleHero( Game_t* game )
{
   u32 i;
   Bool_t heroFound;

   for ( i = 0, heroFound = False; i < game->playerCount; i++ )
   {
      if ( game->players[i].playerClass == PlayerClass_Hero )
      {
         if ( heroFound ) // only one hero allowed
         {
            return False;
         }

         heroFound = True;
      }
   }

   return heroFound;
}
