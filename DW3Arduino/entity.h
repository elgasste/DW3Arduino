#if !defined( ENTITY_H )
#define ENTITY_H

#include "common.h"
#include "vector.h"

typedef struct ActiveSprite_t ActiveSprite_t;

typedef struct Entity_t
{
   Vector4i32_t pos;
   Vector4i32_t prevPos;
   u32 tileIndex;
   Vector2i32_t velocity;
   ActiveSprite_t* sprite;
}
Entity_t;

#endif // ENTITY_H
