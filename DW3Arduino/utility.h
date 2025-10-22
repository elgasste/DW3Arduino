#if !defined( UTILITY_H )
#define UTILITY_H

#include "common.h"

#if defined( __cplusplus )
extern "C" {
#endif

Bool_t Utility_RectsIntersect32i( i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2 );
Bool_t Utility_PointInRectF( r32 px, r32 py, r32 rx, r32 ry, r32 rw, r32 rh );

#if defined( __cplusplus )
}
#endif

#endif // UTILITY_H
