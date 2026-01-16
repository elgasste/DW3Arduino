#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "vector.h"

typedef struct StaticSpriteTexture_t
{
   u8 paletteIndexes[STATIC_SPRITE_SIZE_PIXELS * STATIC_SPRITE_SIZE_PIXELS];
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
   Vector2u32_t offset;
   Direction_t direction;
   u32 frame;
   r32 frameSeconds;
}
ActiveSprite_t;

#if defined( __cplusplus )
extern "C" {
#endif

void ActiveSprite_Init( ActiveSprite_t* sprite, u32 textureIndex, u32 offsetX, u32 offsetY, Direction_t direction );
void ActiveSprite_SetDirection( ActiveSprite_t* sprite, Direction_t direction );
void ActiveSprite_Tic( ActiveSprite_t* sprite );

#if defined( __cplusplus )
}
#endif

#endif // SPRITE_H
