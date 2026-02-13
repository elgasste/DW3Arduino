#include <random.h>

#include "main.h"

u32 g_randomPercentResults[16];
u32 g_randomPercentIndex;
u32 g_randomPercentCount;

i32 g_random32iResults[16];
i32 g_random32iIndex;
i32 g_random32iCount;

u32 g_random32uResults[16];
u32 g_random32uIndex;
u32 g_random32uCount;

u32 Random_Percent()
{
   u32 result = g_randomPercentResults[g_randomPercentIndex];
   g_randomPercentIndex++;
   return result;
}

i32 Random_i32( i32 min, i32 max )
{
   UNUSED_PARAM( min );
   UNUSED_PARAM( max );

   i32 result = g_random32iResults[g_random32iIndex];
   g_random32iIndex++;
   return result;
}

u32 Random_u32( u32 min, u32 max )
{
   UNUSED_PARAM( min );
   UNUSED_PARAM( max );

   u32 result = g_random32uResults[g_random32uIndex];
   g_random32uIndex++;
   return result;
}
