#if !defined( MOCK_RANDOM_H )
#define MOCK_RANDOM_H

#include <common.h>

extern u32 g_mock_random_percentResults[16];
extern u32 g_mock_random_percentIndex;
extern u32 g_mock_random_percentCount;

extern i32 g_mock_random_i32Results[16];
extern u32 g_mock_random_i32Index;
extern u32 g_mock_random_i32Count;

extern u32 g_mock_random_u32Results[16];
extern u32 g_mock_random_u32Index;
extern u32 g_mock_random_u32Count;

inline void Mock_Random_ResetAll()
{
   g_mock_random_percentCount = 0;
   g_mock_random_percentIndex = 0;

   g_mock_random_i32Index = 0;
   g_mock_random_i32Count = 0;

   g_mock_random_u32Index = 0;
   g_mock_random_u32Count = 0;
}

inline void Mock_Random_PushPercent( u32 percent )
{
   g_mock_random_percentResults[g_mock_random_percentCount] = percent;
   g_mock_random_percentCount++;
}

inline void Mock_Random_ResetAndPushPercent( u32 val )
{
   g_mock_random_percentIndex = 0;
   g_mock_random_percentCount = 0;
   Mock_Random_PushPercent( val );
}

inline void Mock_Random_Push32i( i32 val )
{
   g_mock_random_i32Results[g_mock_random_i32Count] = val;
   g_mock_random_i32Count++;
}

inline void Mock_Random_ResetAndPush32i( i32 val )
{
   g_mock_random_i32Index = 0;
   g_mock_random_i32Count = 0;
   Mock_Random_Push32i( val );
}

inline void Mock_Random_Push32u( u32 val )
{
   g_mock_random_u32Results[g_mock_random_u32Count] = val;
   g_mock_random_u32Count++;
}

inline void Mock_Random_ResetAndPush32u( u32 val )
{
   g_mock_random_u32Index = 0;
   g_mock_random_u32Count = 0;
   Mock_Random_Push32u( val );
}

#endif // MOCK_RANDOM_H
