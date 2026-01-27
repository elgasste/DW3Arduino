#include "KVStore.h"
#include "kvstore_global_api.h"

#include "game.h"

/*
   MUFFINS: I think we only care about max key length here, but we probably won't need to use it

   #define KV_WRITE_ONCE_FLAG                      (1 << 0)
   #define KV_REQUIRE_CONFIDENTIALITY_FLAG         (1 << 1)
   #define KV_RESERVED_FLAG                        (1 << 2)
   #define KV_REQUIRE_REPLAY_PROTECTION_FLAG       (1 << 3)

   #define KV_MAX_KEY_LENGTH 128
*/

Bool_t Storage_SaveGame( Game_t* game )
{
   UNUSED_PARAM( game );

   /*
      NOTES

      Program_Log( "Testing KV store" );

      const char* testKey = "kvstore_testKey";
      const char* testInValue = "I put my keys right here, I swear";

      // returns MBED_SUCCESS on success or error code on failure.
      //kv_set( testKey, testInValue, strlen( testInValue ), 0 );

      char testOutValue[strlen( testInValue ) + 1];
      memset( testOutValue, '\0', strlen( testInValue ) + 1);

      // the fourth parameter is the actual size of the data that was read
      //kv_get( testKey, testOutValue, strlen( testInValue ), 0 );

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

      // uncomment for clean-up
      //kv_reset( "/kv/" );
   */

   return False;
}

Bool_t Storage_LoadGame( Game_t* game, u32 slot )
{
   UNUSED_PARAM( game );
   UNUSED_PARAM( slot );

   return False;
}
