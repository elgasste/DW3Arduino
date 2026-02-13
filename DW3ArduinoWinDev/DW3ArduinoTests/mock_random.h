#if !defined( MOCK_RANDOM_H )
#define MOCK_RANDOM_H

#include <common.h>

extern u32 g_randomPercentResults[16];
extern u32 g_randomPercentIndex;
extern u32 g_randomPercentCount;

extern i32 g_random32iResults[16];
extern u32 g_random32iIndex;
extern u32 g_random32iCount;

extern u32 g_random32uResults[16];
extern u32 g_random32uIndex;
extern u32 g_random32uCount;

inline void Mock_Random_ResetAll()
{
   g_randomPercentCount = 0;
   g_randomPercentIndex = 0;

   g_random32iIndex = 0;
   g_random32iCount = 0;

   g_random32uIndex = 0;
   g_random32uCount = 0;
}

inline void Mock_Random_PushPercent( u32 percent )
{
   g_randomPercentResults[g_randomPercentCount] = percent;
   g_randomPercentCount++;
}

inline void Mock_Random_ResetAndPushPercent( u32 val )
{
   g_randomPercentIndex = 0;
   g_randomPercentCount = 0;
   Mock_Random_PushPercent( val );
}

inline void Mock_Random_Push32i( i32 val )
{
   g_random32iResults[g_random32iCount] = val;
   g_random32iCount++;
}

inline void Mock_Random_ResetAndPush32i( i32 val )
{
   g_random32iIndex = 0;
   g_random32iCount = 0;
   Mock_Random_Push32i( val );
}

inline void Mock_Random_Push32u( u32 val )
{
   g_random32uResults[g_random32uCount] = val;
   g_random32uCount++;
}

inline void Mock_Random_ResetAndPush32u( u32 val )
{
   g_random32uIndex = 0;
   g_random32uCount = 0;
   Mock_Random_Push32u( val );
}

#endif // MOCK_RANDOM_H
