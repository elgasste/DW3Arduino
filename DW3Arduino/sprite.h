#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "vector.h"

#define STATIC_SPRITE_SIZE              16
#define ACTIVE_SPRITE_TEXTURE_WIDTH     32
#define ACTIVE_SPRITE_TEXTURE_HEIGHT    64

typedef enum Direction_t
{
   Direction_Left = 0,
   Direction_Up,
   Direction_Right,
   Direction_Down
}
Direction_t;

typedef struct StaticSpriteTexture_t
{
   u8 paletteIndexes[STATIC_SPRITE_SIZE * STATIC_SPRITE_SIZE];
}
StaticSpriteTexture_t;

typedef struct StaticSprite_t
{
   u32 textureIndex;
   u32 tileIndex;
   Bool_t isPassable;
}
StaticSprite_t;

typedef struct ActiveSpriteTexture_t
{
   u8 paletteIndexes[ACTIVE_SPRITE_TEXTURE_WIDTH * ACTIVE_SPRITE_TEXTURE_HEIGHT];
}
ActiveSpriteTexture_t;

typedef struct ActiveSprite_t
{
   u32 textureIndex;
   Direction_t direction;
   Vector2u32_t offset;
}
ActiveSprite_t;

#endif // SPRITE_H
