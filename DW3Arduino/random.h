#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

u32 Random_Percent();
i32 Random_i32( i32 min, i32 max );
u32 Random_u32( u32 min, u32 max );

// platform-specific
void Random_Seed();

#if defined( __cplusplus )
}
#endif

#endif // RANDOM_H
