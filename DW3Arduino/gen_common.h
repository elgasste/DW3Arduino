// THIS FILE IS AUTO-GENERATED, PLEASE DO NOT MODIFY!

#if !defined( GEN_COMMON_H )
#define GEN_COMMON_H

#define UNITS_PER_PIXEL                         100

#define MAX_SAVE_SLOTS                          3

#define MAX_PLAYERS                             4
#define PLAYER_MAX_NAME_LENGTH                  8

#define CLOCK_FPS                               60
#define CLOCK_FRAME_MICROSECONDS                16666
#define CLOCK_FRAME_SECONDS                     0.01666667f

#define ANIMATION_CHAIN_MAX_ANIMATIONS          32

#define DAY_FACTOR_TOTAL_SECONDS                29.00f
#define DAY_FACTOR_LOW_CUTOFF                   0.15f
#define DAY_FACTOR_HIGH_CUTOFF                  0.85f
#define DAY_FACTOR_UNDERGROUND_THRESHOLD        0.40f

#define ENCOUNTER_CHANCE_LOW                    24   // 1 out of 24 chance
#define ENCOUNTER_CHANCE_MEDIUM                 16   // 1 out of 16 chance
#define ENCOUNTER_CHANCE_HIGH                   8   // 1 out of 8 chance

#define GENERIC_ENTITY_WIDTH                    1200
#define GENERIC_ENTITY_HEIGHT                   1200

#define NPC_MIN_ACTION_SECONDS                  1
#define NPC_MAX_ACTION_SECONDS                  4
#define NPC_MIN_VELOCITY                        8    // units per frame
#define NPC_MAX_VELOCITY                        40

#define PLAYER_MOVE_HISTORY_SIZE                16

#define SCREEN_WIDTH                            256
#define SCREEN_HEIGHT                           224
#define SCREEN_PIXELS                           57344
#define SCREEN_PALETTE_SIZE                     256
#define SCREEN_TEXT_TILE_COUNT                  85
#define SCREEN_TEXT_TILE_SIZE                   8
#define SCREEN_COLOR16_TRANSPARENT              0xF81F
#define SCREEN_MENU_BORDER_CHAR_TOPLEFT         1
#define SCREEN_MENU_BORDER_CHAR_TOPRIGHT        2
#define SCREEN_MENU_BORDER_CHAR_BOTTOMLEFT      3
#define SCREEN_MENU_BORDER_CHAR_BOTTOMRIGHT     4
#define SCREEN_MENU_BORDER_CHAR_LEFT            5
#define SCREEN_MENU_BORDER_CHAR_TOP             6
#define SCREEN_MENU_BORDER_CHAR_RIGHT           7
#define SCREEN_MENU_BORDER_CHAR_BOTTOM          8
#define SCREEN_DOWNWARD_CARAT_CHAR              9

#define STATIC_SPRITE_SIZE_PIXELS               16
#define STATIC_SPRITE_SIZE_UNITS                1600

#define ACTIVE_SPRITE_TEXTURE_WIDTH             32
#define ACTIVE_SPRITE_TEXTURE_HEIGHT            64
#define ACTIVE_SPRITE_FRAME_SIZE                16
#define ACTIVE_SPRITE_FRAME_PIXELS              256
#define ACTIVE_SPRITE_FRAMES                    2
#define ACTIVE_SPRITE_FRAME_TOTAL_SECONDS       0.35f

#define TILEMAP_TILE_SIZE_PIXELS                16
#define TILEMAP_TILE_SIZE_UNITS                 1600
#define TILEMAP_MAX_TILE_TEXTURES               32
#define TILEMAP_MAX_STATIC_SPRITE_TEXTURES      32
#define TILEMAP_MAX_STATIC_SPRITES              64
#define TILEMAP_MAX_ACTIVE_SPRITE_TEXTURES      16
#define TILEMAP_MAX_ACTIVE_SPRITES              16
#define TILEMAP_MAX_PORTALS                     32
#define TILEMAP_MAX_ENTITIES                    32
#define TILEMAP_MAX_NPCS                        24
#define TILEMAP_MAX_TILES_X                     256
#define TILEMAP_MAX_TILES_Y                     256
#define TILEMAP_SWAP_FADE_SECONDS               0.3f
#define TILEMAP_SWAP_PAUSE_SECONDS              0.2f

#define TILE_WALK_SPEED_NORMAL                  100   // units per frame
#define TILE_WALK_SPEED_SLOW                    80
#define TILE_WALK_SPEED_VERY_SLOW               60
#define TILE_WALK_SPEED_CRAWL                   40
#define TILE_WALK_SPEED_DIAGONAL_NORMAL         80
#define TILE_WALK_SPEED_DIAGONAL_SLOW           60
#define TILE_WALK_SPEED_DIAGONAL_VERY_SLOW      40
#define TILE_WALK_SPEED_DIAGONAL_CRAWL          20

#endif // GEN_COMMON_H
