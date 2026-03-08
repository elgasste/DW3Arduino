#if !defined( GIGA_COMMON_H )
#define GIGA_COMMON_H

#define KVSTORE_KEY_PREFIX            "dw3"
#define KVSTORE_PLAYER_COUNT_KEY      "playerCount"
#define KVSTORE_PLAYER_NAME_KEY       "player%d_name"
#define KVSTORE_PLAYER_CLASS_KEY      "player%d_class"
#define KVSTORE_PLAYER_STATS_HP_KEY   "player%d_stats_hp"
#define KVSTORE_PLAYER_STATS_MP_KEY   "player%d_stats_mp"

#define KVSTORE_ERROR_PLAYER_WRITE    "could not write player data to KV Store"
#define KVSTORE_ERROR_PLAYER_READ     "could not read player data from KV Store"
#define KVSTORE_ERROR_DELETE_SLOT     "could not find any KV Store data to delete for slot %d"

#endif // GIGA_COMMON_H
