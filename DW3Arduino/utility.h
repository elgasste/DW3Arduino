#if !defined( UTILITY_H )
#define UTILITY_H

#include "common.h"
#include "vector.h"

#if defined( __cplusplus )
extern "C" {
#endif

Bool_t Utility_RectsIntersect32i( i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2 );
Bool_t Utility_RectsIntersect32r( r32 x1, r32 y1, r32 w1, r32 h1, r32 x2, r32 y2, r32 w2, r32 h2 );
Bool_t Utility_HorizontalLineIntersectsRect32i( i32 lineX1, i32 lineX2, i32 lineY, i32 rx, i32 ry, i32 rw, i32 rh );
Bool_t Utility_VerticalLineIntersectsRect32i( i32 lineX, i32 lineY1, i32 lineY2, i32 rx, i32 ry, i32 rw, i32 rh );
Bool_t Utility_PointInRect32r( r32 px, r32 py, r32 rx, r32 ry, r32 rw, r32 rh );

#if defined( __cplusplus )
}
#endif

#endif // UTILITY_H
