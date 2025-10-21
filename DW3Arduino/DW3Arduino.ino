#include "giga_shield.h"
#include "game.h"

Game_t g_game;
GigaShield g_gigaShield;

void Program_Log( const char* msg )
{
#if defined( DEBUG_SERIAL )
   Serial.println( msg );
#endif
}

void setup()
{
#if defined( DEBUG_SERIAL )
   Serial.begin( SERIAL_BAUD );
#endif

   g_gigaShield.begin();
   Game_Init( &g_game, g_gigaShield.getBuffer() );

   // for NES controller input
   pinMode( INPUT_NES_DATA_PIN, INPUT );
  
   pinMode( INPUT_NES_CLOCK_PIN, OUTPUT );
   pinMode( INPUT_NES_LATCH_PIN, OUTPUT );
  
   digitalWrite( INPUT_NES_CLOCK_PIN, LOW );
   digitalWrite( INPUT_NES_LATCH_PIN, LOW );
}

void loop()
{
   Clock_StartFrame( &( g_game.clock ) );
   Game_Tic( &g_game );
   Clock_EndFrame( &( g_game.clock ) );
}

void Screen_BlitBuffer( Screen_t* screen )
{
   g_gigaShield.drawScreen();
}
