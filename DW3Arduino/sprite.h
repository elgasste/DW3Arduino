#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "vector.h"

#define STATIC_SPRITE_SIZE_PIXELS            16
#define STATIC_SPRITE_SIZE_UNITS             ( STATIC_SPRITE_SIZE_PIXELS * UNITS_PER_PIXEL )
#define ACTIVE_SPRITE_TEXTURE_WIDTH          32
#define ACTIVE_SPRITE_TEXTURE_HEIGHT         64
#define ACTIVE_SPRITE_FRAME_SIZE             16
#define ACTIVE_SPRITE_FRAME_PIXELS           ( ACTIVE_SPRITE_FRAME_SIZE * ACTIVE_SPRITE_FRAME_SIZE )
#define ACTIVE_SPRITE_FRAMES                 2
#define ACTIVE_SPRITE_FRAME_TOTAL_SECONDS    0.35f

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
