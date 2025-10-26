#include "utility.h"

Bool_t Utility_RectsIntersect32i( i32 x1, i32 y1, i32 w1, i32 h1, i32 x2, i32 y2, i32 w2, i32 h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Utility_RectsIntersect32r( r32 x1, r32 y1, r32 w1, r32 h1, r32 x2, r32 y2, r32 w2, r32 h2 )
{
   return ( x1 < ( x2 + w2 ) && ( x1 + w1 ) > x2 && y1 < ( y2 + h2 ) && ( y1 + h1 ) > y2 ) ? True : False;
}

Bool_t Utility_HorizontalLineIntersectsRect32r( r32 lineX1, r32 lineX2, r32 lineY, r32 rx, r32 ry, r32 rw, r32 rh )
{
   return ( lineY > ry ) && ( lineY < ( ry + rh ) ) && ( lineX1 < ( rx + rw ) ) && ( lineX2 > rx ) ? True : False;
}

Bool_t Utility_VerticalLineIntersectsRect32r( r32 lineX, r32 lineY1, r32 lineY2, r32 rx, r32 ry, r32 rw, r32 rh )
{
   return ( lineX > rx ) && ( lineX < ( rx + rw ) ) && ( lineY1 < ( ry + rh ) ) && ( lineY2 > ry ) ? True : False;
}

Bool_t Utility_PointInRect32r( r32 px, r32 py, r32 rx, r32 ry, r32 rw, r32 rh )
{
   return ( px > rx ) && ( px < ( rx + rw ) ) && ( py > ry ) && py < ( ry + rh ) ? True : False;
}
