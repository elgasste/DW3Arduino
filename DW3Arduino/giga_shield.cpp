
#include "giga_shield.h"
#include "Adafruit_SPITFT.h"
#include "dsi.h"

GigaShield::GigaShield() : Adafruit_GFX( GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT )
{
}

void GigaShield::begin()
{
   _display = new Arduino_H7_Video( GIGA_SHIELD_WIDTH, GIGA_SHIELD_HEIGHT, GigaDisplayShield );
   _display->begin();

   memset( (void*)_buffer, 0, SCREEN_PIXELS * sizeof( u16 ) );

   _refreshThread = new rtos::Thread( osPriorityHigh );
   _refreshThread->start( mbed::callback( this, &GigaShield::refreshThreadWorker ) );
}

void GigaShield::refreshThreadWorker()
{
   while ( 1 )
   {
      rtos::ThisThread::flags_wait_any( 0x1 );
      
      u16* shieldBuffer = (u16*)( dsi_getActiveFrameBuffer() );
      shieldBuffer += ( ( GIGA_SHIELD_WIDTH * GIGA_PLAY_AREA_OFFSET_Y ) + GIGA_PLAY_AREA_OFFSET_X );

      dsi_lcdDrawImage( (void*)_buffer, (void*)shieldBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, DMA2D_INPUT_RGB565 );
   }
}

void GigaShield::drawScreen()
{
   _refreshThread->flags_set( 0x1 );
}
