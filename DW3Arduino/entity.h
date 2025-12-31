#if !defined( ENTITY_H )
#define ENTITY_H

#include "common.h"
#include "vector.h"

typedef struct ActiveSprite_t ActiveSprite_t;

typedef struct Entity_t
{
   Vector4r32_t pos;
   Vector4r32_t prevPos;
   Vector2r32_t velocity;
   ActiveSprite_t* sprite;
}
Entity_t;

#endif // ENTITY_H
