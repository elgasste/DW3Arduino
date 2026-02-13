#include <random.h>

#include "main.h"

u32 g_mock_random_percentResults[16];
u32 g_mock_random_percentIndex;
u32 g_mock_random_percentCount;

i32 g_mock_random_i32Results[16];
i32 g_mock_random_i32Index;
i32 g_mock_random_i32Count;

u32 g_mock_random_u32Results[16];
u32 g_mock_random_u32Index;
u32 g_mock_random_u32Count;

void Random_Seed()
{
}

u32 Random_Percent()
{
   u32 result = g_mock_random_percentResults[g_mock_random_percentIndex];
   g_mock_random_percentIndex++;
   return result;
}

i32 Random_i32( i32 min, i32 max )
{
   UNUSED_PARAM( min );
   UNUSED_PARAM( max );

   i32 result = g_mock_random_i32Results[g_mock_random_i32Index];
   g_mock_random_i32Index++;
   return result;
}

u32 Random_u32( u32 min, u32 max )
{
   UNUSED_PARAM( min );
   UNUSED_PARAM( max );

   u32 result = g_mock_random_u32Results[g_mock_random_u32Index];
   g_mock_random_u32Index++;
   return result;
}
