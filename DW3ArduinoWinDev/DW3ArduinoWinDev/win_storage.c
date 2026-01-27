#include <stdio.h>
#include "cJSON/cJSON.h"

#include "win_common.h"
#include "game.h"

internal Bool_t Storage_WritePlayersJSON( Game_t* game, cJSON* parent );

Bool_t Storage_SaveGame( Game_t* game )
{
   char fileName[256];
   char msg[256];
   char* jsonString;
   Bool_t result;
   FILE* saveFile;
   cJSON* root = cJSON_CreateObject();

   if ( root == 0 )
   {
      Program_Log( JSON_ERROR_ROOT );
      return False;
   }

   if ( !Storage_WritePlayersJSON( game, root ) )
   {
      Program_Log( JSON_ERROR_PLAYERS );
      cJSON_Delete( root );
      return False;
   }

   jsonString = cJSON_Print( root );

   sprintf( fileName, JSON_SAVE_FILE_NAME, game->saveSlot );
   saveFile = fopen( fileName, "w" );

   if ( !saveFile )
   {
      sprintf( msg, JSON_ERROR_FILE_OPEN, fileName );
      Program_Log( msg );
      cJSON_Delete( root );
      return False;
   }

   result = True;

   if ( fprintf( saveFile, "%s", jsonString ) < 0 )
   {
      sprintf( msg, JSON_ERROR_FILE_WRITE, fileName );
      Program_Log( msg );
      result = False;
      fclose( saveFile );
   }

   fclose( saveFile );
   cJSON_Delete( root );
   return result;
}

Bool_t Storage_LoadGame( Game_t* game, u32 slot )
{
   // TODO: read in JSON data from corresponding file (if it exists), then validate
   UNUSED_PARAM( game );
   UNUSED_PARAM( slot );

   return False;
}

internal Bool_t Storage_WritePlayersJSON( Game_t* game, cJSON* parent )
{
   u32 i;
   cJSON *playerCount, *players, *player, *name, *playerClass;

   // player count object
   playerCount = cJSON_CreateNumber( game->playerCount );
   if ( playerCount == 0 )
      return False;
   cJSON_AddItemToObject( parent, JSON_PLAYER_COUNT, playerCount );

   // players array
   players = cJSON_CreateArray();
   if ( players == 0 )
      return False;
   cJSON_AddItemToObject( parent, JSON_PLAYERS, players );

   for ( i = 0; i < game->playerCount; i++ )
   {
      // player object
      player = cJSON_CreateObject();
      if ( player == 0 )
         return False;
      cJSON_AddItemToArray( players, player );

      // player name
      name = cJSON_CreateString( game->players[i].name );
      if ( name == 0 )
         return False;
      cJSON_AddItemToObject( player, JSON_PLAYER_NAME, name );

      // player class
      playerClass = cJSON_CreateNumber( game->players[i].playerClass );
      if ( playerClass == 0 )
         return False;
      cJSON_AddItemToObject( player, JSON_PLAYER_CLASS, playerClass );
   }

   return True;
}
