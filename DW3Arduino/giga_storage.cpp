#include "KVStore.h"
#include "kvstore_global_api.h"
#include "mbed_error.h"

#include "giga_common.h"
#include "game.h"

#define TRY_DELETING_GENERAL_KEY( n ) \
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, n ); \
   if ( kv_remove( key ) == MBED_SUCCESS ) \
      result = True

#define TRY_DELETING_PLAYER_KEY( n, p ) \
   sprintf( subkey, n, p ); \
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey ); \
   if ( kv_remove( key ) == MBED_SUCCESS ) \
      result = True

internal Bool_t Storage_WriteGameBase( Game_t* game );
internal Bool_t Storage_WritePlayers( Game_t* game );
internal Bool_t Storage_ReadGameBase( Game_t* game, u32 slot );
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
   if ( !Storage_WriteGameBase( game ) )
   {
      Program_Log( KVSTORE_ERROR_GAME_BASE_WRITE );
      return False;
   }

   if ( !Storage_WritePlayers( game ) )
   {
      Program_Log( KVSTORE_ERROR_PLAYER_WRITE );
      return False;
   }

   return True;
}

Bool_t Storage_LoadGame( Game_t* game, u32 slot )
{
   if ( !Storage_ReadGameBase( game, slot ) )
   {
      Program_Log( KVSTORE_ERROR_GAME_BASE_READ );
      return False;
   }

   if ( !Storage_ReadPlayers( game, slot ) )
   {
      Program_Log( KVSTORE_ERROR_PLAYER_READ );
      return False;
   }

   return True;
}

Bool_t Storage_DeleteSlot( u32 slot )
{
   u32 i;
   char key[64];
   char subkey[64];
   char msg[128];
   Bool_t result = False;

   TRY_DELETING_GENERAL_KEY( KVSTORE_HAS_SHIP_KEY );
   TRY_DELETING_GENERAL_KEY( KVSTORE_HAS_RAMIA_KEY );
   TRY_DELETING_GENERAL_KEY( KVSTORE_PLAYER_COUNT_KEY );

   for ( i = 0; i < MAX_PLAYERS; i++ )
   {
      TRY_DELETING_PLAYER_KEY( KVSTORE_PLAYER_NAME_KEY, i );
      TRY_DELETING_PLAYER_KEY( KVSTORE_PLAYER_CLASS_KEY, i );
   }

   if ( !result )
   {
      sprintf( msg, KVSTORE_ERROR_DELETE_SLOT, slot );
      Program_Log( msg );
   }

   return result;
}

internal Bool_t Storage_WriteGameBase( Game_t* game )
{
   char key[64];

   // has-ship
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, KVSTORE_HAS_SHIP_KEY );
   if ( kv_set( key, &( game->hasShip ), sizeof( Bool_t ), 0 ) != MBED_SUCCESS )
      return False;

   // has-ramia
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, KVSTORE_HAS_RAMIA_KEY );
   if ( kv_set( key, &( game->hasRamia ), sizeof( Bool_t ), 0 ) != MBED_SUCCESS )
      return False;

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

      // player gender
      sprintf( subkey, KVSTORE_PLAYER_GENDER_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, subkey );
      if ( kv_set( key, &( game->players[i].gender ), sizeof( Gender_t ), 0 ) != MBED_SUCCESS )
         return False;

      // player experience
      sprintf( subkey, KVSTORE_PLAYER_EXP_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, subkey );
      if ( kv_set( key, &( game->players[i].exp ), sizeof( PlayerClass_t ), 0 ) != MBED_SUCCESS )
         return False;

      // player stats
      sprintf( subkey, KVSTORE_PLAYER_STATS_HP_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, subkey );
      if ( kv_set( key, &( game->players[i].stats.hp ), sizeof( u32 ), 0 ) != MBED_SUCCESS )
         return False;

      sprintf( subkey, KVSTORE_PLAYER_STATS_MP_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, game->saveSlot, subkey );
      if ( kv_set( key, &( game->players[i].stats.mp ), sizeof( u32 ), 0 ) != MBED_SUCCESS )
         return False;
   }

   return True;
}

internal Bool_t Storage_ReadGameBase( Game_t* game, u32 slot )
{
   Bool_t hasShip, hasRamia;
   char key[64];
   kv_info_t info;

   // has-ship
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, KVSTORE_HAS_SHIP_KEY );
   if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &hasShip, sizeof( Bool_t ), 0 ) != MBED_SUCCESS )
      return False;
   game->hasShip = ( hasShip > 0 ) ? True : False;

   // has-ramia
   sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, KVSTORE_HAS_RAMIA_KEY );
   if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &hasRamia, sizeof( Bool_t ), 0 ) != MBED_SUCCESS )
      return False;
   game->hasRamia = ( hasRamia > 0 ) ? True : False;

   return True;
}

internal Bool_t Storage_ReadPlayers( Game_t* game, u32 slot )
{
   u32 i, playerCount, exp, hp, mp;
   PlayerClass_t playerClass;
   Gender_t playerGender;
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

      // player gender
      sprintf( subkey, KVSTORE_PLAYER_GENDER_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey );
      if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &playerGender, sizeof( Gender_t ), 0 ) != MBED_SUCCESS )
         return False;
      if ( !Validate_PlayerGender( (i32)playerGender ) )
         return False;
      game->players[i].gender = playerGender;

      // player experience
      sprintf( subkey, KVSTORE_PLAYER_EXP_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey );
      if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &exp, sizeof( u32 ), 0 ) != MBED_SUCCESS )
         return False;
      if ( !Validate_PlayerExp( (i32)exp ) )
         return False;
      game->players[i].exp = exp;
      Player_UpdateLevel( game->players + i );

      // player stats
      sprintf( subkey, KVSTORE_PLAYER_STATS_HP_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey );
      if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &hp, sizeof( u32 ), 0 ) != MBED_SUCCESS )
         return False;
      sprintf( subkey, KVSTORE_PLAYER_STATS_MP_KEY, i );
      sprintf( key, "%s_%d_%s", KVSTORE_KEY_PREFIX, slot, subkey );
      if ( kv_get_info( key, &info ) != MBED_SUCCESS || kv_get( key, &mp, sizeof( u32 ), 0 ) != MBED_SUCCESS )
         return False;
      if ( !Validate_PlayerStats( hp, mp ) )
         return False;
      game->players[i].stats.hp = hp;
      game->players[i].stats.mp = mp;
   }

   if ( !Validate_SingleHero( game ) )
      return False;

   return True;
}
