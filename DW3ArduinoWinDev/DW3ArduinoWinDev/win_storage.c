#include <stdio.h>
#include "cJSON/cJSON.h"

#include "win_common.h"
#include "game.h"

internal Bool_t Storage_WritePlayersJSON( Game_t* game, cJSON* node );
internal Bool_t Storage_LoadGameFromJSONString( Game_t* game, const char* jsonStr, const char* fileName );
internal Bool_t Storage_LoadPlayersFromJSON( Game_t* game, cJSON* node );
internal Bool_t Storage_FindJSONItem32i( cJSON* node, i32* val, const char* itemName );
internal Bool_t Storage_FindJSONArray( cJSON* node, cJSON** val, const char* itemName );
internal Bool_t Storage_FindJSONItemString( cJSON* node, char** val, const char* itemName );
internal Bool_t Storage_FindJSONItemObject( cJSON* node, cJSON** val, const char* itemName );

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
      sprintf( msg, JSON_ERROR_FILE_OPEN_WRITE, fileName );
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
   char fileName[256];
   char msg[256];
   FILE* saveFile;
   long fileSize;
   char* fileContents;
   size_t bytesRead = 0;
   Bool_t result;

   sprintf( fileName, JSON_SAVE_FILE_NAME, slot );
   saveFile = fopen( fileName, "rb" );

   if ( !saveFile )
   {
      sprintf( msg, JSON_ERROR_FILE_OPEN_READ, fileName );
      Program_Log( msg );
      return False;
   }

   fseek( saveFile, 0, SEEK_END );
   fileSize = ftell( saveFile );
   fseek( saveFile, 0, SEEK_SET );

   fileContents = (char*)malloc( sizeof( char ) * ( (size_t)fileSize + 1 ) );
   if ( fileContents == 0 )
   {
      fclose( saveFile );
      Program_Log( JSON_ERROR_MALLOC );
      return False;
   }
   else
   {
      bytesRead = fread( fileContents, 1, (size_t)fileSize, saveFile );
   }

   if ( bytesRead != fileSize )
   {
      free( fileContents );
      fclose( saveFile );
      sprintf( msg, JSON_ERROR_FILE_READ, fileName );
      Program_Log( msg );
      return False;
   }

   fileContents[fileSize] = '\0';
   fclose( saveFile );

   result = Storage_LoadGameFromJSONString( game, fileContents, fileName );

   free( fileContents );
   return result;
}

Bool_t Storage_DeleteSlot( u32 slot )
{
   char fileName[256];
   int result;

   sprintf( fileName, JSON_SAVE_FILE_NAME, slot );
   result = remove( fileName );

   if ( result != 0 )
   {
      Program_Log( JSON_ERROR_REMOVE_FILE );
   }

   return ( result == 0 ) ? True : False;
}

internal Bool_t Storage_WritePlayersJSON( Game_t* game, cJSON* node )
{
   u32 i;
   cJSON *playerCount, *players, *player, *name, *playerClass, *stats, *hp, *mp;

   // player count object
   playerCount = cJSON_CreateNumber( game->playerCount );
   if ( playerCount == 0 )
      return False;
   cJSON_AddItemToObject( node, JSON_PLAYER_COUNT, playerCount );

   // players array
   players = cJSON_CreateArray();
   if ( players == 0 )
      return False;
   cJSON_AddItemToObject( node, JSON_PLAYERS, players );

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

      // player stats object
      stats = cJSON_CreateObject();
      if ( stats == 0 )
         return False;
      cJSON_AddItemToObject( player, JSON_PLAYER_STATS, stats );

      // player stats - hit points
      hp = cJSON_CreateNumber( game->players[i].stats.hp );
      if ( hp == 0 )
         return False;
      cJSON_AddItemToObject( stats, JSON_PLAYER_STATS_HP, hp );

      // player stats - magic points
      mp = cJSON_CreateNumber( game->players[i].stats.mp );
      if ( mp == 0 )
         return False;
      cJSON_AddItemToObject( stats, JSON_PLAYER_STATS_MP, mp );
   }

   return True;
}

internal Bool_t Storage_LoadGameFromJSONString( Game_t* game, const char* jsonStr, const char* fileName )
{
   char msg[256];
   cJSON* root;
   Bool_t result;

   root = cJSON_Parse( jsonStr );
   if ( root == 0 || root->child == 0 )
   {
      sprintf( msg, JSON_ERROR_PARSE_SAVE_FILE, fileName );
      Program_Log( msg );
      return False;
   }

   result = Storage_LoadPlayersFromJSON( game, root->child );

   cJSON_Delete( root );
   return result;
}

internal Bool_t Storage_LoadPlayersFromJSON( Game_t* game, cJSON* node )
{
   i32 i, playerCount, playerClass, playerExp, hp, mp;
   cJSON *players, *player, *stats;
   char* playerName;

   // player count
   if ( !Storage_FindJSONItem32i( node, &playerCount, JSON_PLAYER_COUNT ) )
      return False;
   if ( !Validate_PlayerCount( playerCount ) )
      return False;
   game->playerCount = (u32)playerCount;

   // players
   if ( !Storage_FindJSONArray( node, &players, JSON_PLAYERS ) )
      return False;

   for ( i = 0, player = players->child; i < playerCount; i++ )
   {
      if ( player == 0 || player->child == 0 )
         return False;

      // player name
      if ( !Storage_FindJSONItemString( player->child, &playerName, JSON_PLAYER_NAME ) )
         return False;
      if ( !Validate_PlayerName( playerName ) )
         return False;

      strcpy( game->players[i].name, playerName );

      // player class
      if ( !Storage_FindJSONItem32i( player->child, &playerClass, JSON_PLAYER_CLASS ) )
         return False;
      if ( !Validate_PlayerClass( playerClass ) )
         return False;
      game->players[i].playerClass = (PlayerClass_t)playerClass;

      // player experience
      if ( !Storage_FindJSONItem32i( player->child, &playerExp, JSON_PLAYER_EXP ) )
         return False;
      if ( !Validate_PlayerExp( playerExp ) )
         return False;
      game->players[i].exp = (u32)playerExp;

      // player stats
      if ( !Storage_FindJSONItemObject( player->child, &stats, JSON_PLAYER_STATS ) || !stats || !stats->child )
         return False;
      if ( !Storage_FindJSONItem32i( stats->child, &hp, JSON_PLAYER_STATS_HP ) )
         return False;
      if ( !Storage_FindJSONItem32i( stats->child, &mp, JSON_PLAYER_STATS_MP ) )
         return False;
      if ( !Validate_PlayerStats( (u32)hp, (u32)mp ) )
         return False;
      game->players[i].stats.hp = (u32)hp;
      game->players[i].stats.mp = (u32)mp;

      // next player
      player = player->next;
   }

   if ( !Validate_SingleHero( game ) )
      return False;

   return True;
}

internal Bool_t Storage_FindJSONItem32i( cJSON* node, i32* val, const char* itemName )
{
   cJSON* item = node;

   while ( item != 0 )
   {
      if ( cJSON_IsNumber( item ) && strcmp( item->string, itemName ) == 0 )
      {
         *val = item->valueint;
         return True;
      }

      item = item->next;
   }

   return False;
}

internal Bool_t Storage_FindJSONArray( cJSON* node, cJSON** val, const char* itemName )
{
   cJSON* item = node;

   while ( item != 0 )
   {
      if ( cJSON_IsArray( item ) && strcmp( item->string, itemName ) == 0 )
      {
         *val = item;
         return True;
      }

      item = item->next;
   }

   return False;
}

internal Bool_t Storage_FindJSONItemString( cJSON* node, char** val, const char* itemName )
{
   cJSON* item = node;

   while ( item != 0 )
   {
      if ( cJSON_IsString( item ) && strcmp( item->string, itemName ) == 0 )
      {
         *val = item->valuestring;
         return True;
      }

      item = item->next;
   }

   return False;
}

internal Bool_t Storage_FindJSONItemObject( cJSON* node, cJSON** val, const char* itemName )
{
   cJSON* item = node;

   while ( item != 0 )
   {
      if ( cJSON_IsObject( item ) && strcmp( item->string, itemName ) == 0 )
      {
         *val = item;
         return True;
      }

      item = item->next;
   }

   return False;
}
