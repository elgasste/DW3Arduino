#include <random.h>

#include "main.h"

u32 g_randomPercentResult = 0;
i32 g_random32iResult = 0;
u32 g_random32uResult = 0;

u32 Random_Percent()
{
   return g_randomPercentResult;
}

i32 Random_i32( i32 min, i32 max )
{
   UNUSED_PARAM( min );
   UNUSED_PARAM( max );
   return g_random32iResult;
}

u32 Random_u32( u32 min, u32 max )
{
   UNUSED_PARAM( min );
   UNUSED_PARAM( max );
   return g_random32uResult;
}
