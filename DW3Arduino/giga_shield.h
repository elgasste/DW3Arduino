
#if !defined( GIGA_SHIELD_H )
#define GIGA_SHIELD_H

#include "Adafruit_GFX.h"
#include "Arduino_H7_Video.h"
#include "screen.h"

#define GIGA_SHIELD_WIDTH           480
#define GIGA_SHIELD_HEIGHT          800
#define GIGA_SHIELD_PIXELS          384000
#define GIGA_PLAY_AREA_OFFSET_X     30
#define GIGA_PLAY_AREA_OFFSET_Y     280

class GigaShield : public Adafruit_GFX {
   public:
      GigaShield();
      ~GigaShield() { };

      void begin();
      // we have to implement this, even if it does nothing
      void drawPixel( i16 x, i16 y, u16 color ) { }
      void drawScreen();
      void wipeScreen( u16 color );
      u16* getBuffer() { return _buffer; }

   private:
      void refreshThreadWorker();

   private:
      Arduino_H7_Video* _display;
      u16 _buffer[SCREEN_PIXELS];
      rtos::Thread* _refreshThread;
};

#endif // GIGA_SHIELD_H
