#include "random.h"

u32 Random_Percent()
{
   return (u32)( rand() % 101 );
}

i32 Random_i32( i32 min, i32 max )
{
   // min and max inclusive
   return ( (i32)rand() % ( ( max + 1 ) - min ) ) + min;
}

u32 Random_u32( u32 min, u32 max )
{
   // min and max inclusive
   return ( (u32)rand() % ( ( max + 1 ) - min ) ) + min;
}
