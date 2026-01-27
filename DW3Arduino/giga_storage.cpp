#include "KVStore.h"
#include "kvstore_global_api.h"
#include "mbed_error.h"

#include "giga_common.h"
#include "game.h"

internal Bool_t Storage_WritePlayers( Game_t* game );

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
      Program_Log( "could not write player data to KV Store" );
      return False;
   }

   /*
      NOTES

      // this is how you get a value from a key ("actualSize" is the amount of data actually retrieved)
      // size_t actualSize;
      // kv_get( "kvstore_blahBlah", testOutValue, strlen( testInValue ), &actualSize );

      // this is how you check if a key exists
      kv_info_t info;
      int result = kv_get_info( "kvstore_testKey", &info );
      delay( 100 ); // for some reason Program_Log doesn't work without this

      if ( result == MBED_SUCCESS )
      {
         Program_Log( "The key exists" );
      }
      else // error code is probably MBED_ERROR_ITEM_NOT_FOUND
      {
         char msg[100];
         sprintf( msg, "Error reading the key, code is %d", result );
         Program_Log( msg );
      }

      // uncomment for a full clean-up of the entire store
      //kv_reset( "/kv/" );
   */

   return True;
}

Bool_t Storage_LoadGame( Game_t* game, u32 slot )
{
   // TODO
   UNUSED_PARAM( game );
   UNUSED_PARAM( slot );

   return False;
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
