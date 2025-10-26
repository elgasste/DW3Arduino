#if !defined( ENTITY_H )
#define ENTITY_H

#include "common.h"
#include "vector.h"

typedef struct Entity_t
{
   Vector4r32_t hitBox;
   Vector4r32_t prevHitBox;
   Vector2r32_t velocity;
}
Entity_t;

#endif // ENTITY_H
