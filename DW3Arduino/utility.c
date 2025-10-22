#include "utility.h"

Bool_t Utility_RectsIntersect32i( i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Utility_PointInRectF( r32 px, r32 py, r32 rx, r32 ry, r32 rw, r32 rh )
{
   return ( px > rx ) && ( px < ( rx + rw ) ) && ( py > ry ) && py < ( ry + rh ) ? True : False;
}
