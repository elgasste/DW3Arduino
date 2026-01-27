#include "KVStore.h"
#include "kvstore_global_api.h"
#include "mbed_error.h"

#include "giga_common.h"
#include "game.h"

internal Bool_t Storage_WritePlayers( Game_t* game );
internal Bool_t Storage_ReadPlayers( Game_t* game, u32 slot );

// NOTE:
//
// The format of these keys should be a prefix, then the current game's save slot,
// and lastly the name of the key, all separated by underscores. For example, if the
// current game is on save slot 1, and you want to store the number of players,
// the key would look like this:
//
// dw3_1_playerCount

Bool_t Storage_SaveGame( Game_t* game )
{
   if ( !Storage_WritePlayers( game ) )
   {
      Program_Log( KVSTORE_ERROR_PLAYER_WRITE );
      return False;
   }

   return True;
}

Bool_t Storage_LoadGame( Game_t* game, u32 slot )
{
   if ( !Storage_ReadPlayers( game, slot ) )
   {
      Program_Log( KVSTORE_ERROR_PLAYER_READ );
      return False;
   }

   return True;
}

internal Bool_t Storage_WritePlayers( Game_t* game )
{
   u32 i;
   char key[64];
   char subkey[64];

   // player count
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, KVSTORE_PLAYER_COUNT_KEY );
   if ( kv_set( key, &( game->playerCount ), sizeof( u32 ), 0 ) != MBED_SUCCESS )
      return False;

   for ( i = 0; i < game->playerCount; i++ )
   {
      // player name
      sprintf( subkey, KVSTORE_PLAYER_NAME_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, subkey );
      if ( kv_set( key, &( game->players[i].name ), strlen( game->players[i].name ), 0 ) != MBED_SUCCESS )
         return False;

      // player class
      sprintf( subkey, KVSTORE_PLAYER_CLASS_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, subkey );
      if ( kv_set( key, &( game->players[i].playerClass ), sizeof( PlayerClass_t ), 0 ) != MBED_SUCCESS )
         return False;
   }

   return True;
}

internal Bool_t Storage_ReadPlayers( Game_t* game, u32 slot )
{
   u32 i, playerCount;
   PlayerClass_t playerClass;
   int result;
   char key[64];
   char subkey[64];
   char playerName[PLAYER_MAX_NAME_LENGTH + 1];
   kv_info_t info;

   // player count
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, KVSTORE_PLAYER_COUNT_KEY );
   if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &playerCount, sizeof( u32 ), 0 ) != MBED_SUCCESS )
      return False;
   if ( !Validate_PlayerCount( (i32)playerCount ) )
      return False;
   game->playerCount = playerCount;

   // players
   for ( i = 0; i < playerCount; i++ )
   {
      // player name
      sprintf( subkey, KVSTORE_PLAYER_NAME_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey );
      if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, playerName, PLAYER_MAX_NAME_LENGTH, 0 ) != MBED_SUCCESS )
         return False;
      playerName[PLAYER_MAX_NAME_LENGTH] = '\0';
      if ( !Validate_PlayerName( playerName ) )
         return False;
      strcpy( game->players[i].name, playerName );

      // player class
      sprintf( subkey, KVSTORE_PLAYER_CLASS_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey );
      if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &playerClass, sizeof( PlayerClass_t ), 0 ) != MBED_SUCCESS )
         return False;
      if ( !Validate_PlayerClass( (i32)playerClass ) )
         return False;
      game->players[i].playerClass = playerClass;
   }

   if ( !Validate_SingleHero( game ) )
      return False;

   return True;
}
