#include "random.h"

u32 Random_Percent()
{
   return (u32)( rand() % 101 );
}

u32 Random_u32( u32 min, u32 max )
{
   // min and max inclusive
   return ( (u32)rand() % ( ( max + 1 ) - min ) ) + min;
}
