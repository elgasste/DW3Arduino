#include <game.h>

u32 g_mock_clock_frameStartMicro;
u32 g_mock_clock_lagFrameCount;

void Clock_Init( Clock_t* clock )
{
   clock->frameStartMicro = g_mock_clock_frameStartMicro;
   clock->lagFrameCount = g_mock_clock_lagFrameCount;
}

void Clock_StartFrame( Clock_t* clock )
{
   UNUSED_PARAM( clock );
}

void Clock_EndFrame( Clock_t* clock )
{
   UNUSED_PARAM( clock );
}
