#include "win_common.h"
#include "screen.h"

internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam );
internal void InitButtonMap();
internal void RenderScreen();
internal void DrawDiagnostics( HDC* dcMem );
internal void HandleKeyboardInput( u32 keyCode, LPARAM flags );
internal void FatalError( const char* message );

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
   TIMECAPS timeCaps;
   UINT timerResolution;
   WNDCLASSA mainWindowClass = { 0 };
   DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
   RECT expectedWindowRect = { 0 };
   LONG clientPaddingRight, clientPaddingTop;
   MSG msg;
   Game_t* game = &g_winGlobals.game;

   UNUSED_PARAM( hPrevInstance );
   UNUSED_PARAM( lpCmdLine );
   UNUSED_PARAM( nCmdShow );

   if ( !QueryPerformanceFrequency( &( g_winGlobals.performanceFrequency ) ) )
   {
      FatalError( "failed to query performance frequency." );
   }

   if ( timeGetDevCaps( &timeCaps, sizeof( TIMECAPS ) ) != TIMERR_NOERROR )
   {
      FatalError( "failed to set timer resolution." );
   }

   timerResolution = min( max( timeCaps.wPeriodMin, 1 ), timeCaps.wPeriodMax );
   timeBeginPeriod( timerResolution );

   mainWindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   mainWindowClass.lpfnWndProc = MainWindowProc;
   mainWindowClass.hInstance = hInstance;
   mainWindowClass.lpszClassName = "mainWindowClass";

   if ( !RegisterClassA( &mainWindowClass ) )
   {
      FatalError( "failed to register window class." );
   }

   expectedWindowRect.right = SCREEN_WIDTH;
   expectedWindowRect.bottom = SCREEN_HEIGHT;

   if ( !AdjustWindowRect( &expectedWindowRect, windowStyle, 0 ) )
   {
      FatalError( "failed to adjust window rect." );
   }

   clientPaddingRight = ( expectedWindowRect.right - expectedWindowRect.left ) - SCREEN_WIDTH;
   clientPaddingTop = ( expectedWindowRect.bottom - expectedWindowRect.top ) - SCREEN_HEIGHT;

   g_winGlobals.hWndMain = CreateWindowExA( 0,
                                            mainWindowClass.lpszClassName,
                                            "DW3Arduino Windows Development Tool",
                                            windowStyle,
                                            CW_USEDEFAULT,
                                            CW_USEDEFAULT,
                                            (int)( SCREEN_WIDTH * GRAPHICS_SCALE ) + clientPaddingRight,
                                            (int)( SCREEN_HEIGHT * GRAPHICS_SCALE ) + clientPaddingTop,
                                            0,
                                            0,
                                            hInstance,
                                            0 );

   if ( !g_winGlobals.hWndMain )
   {
      FatalError( "failed to create main window." );
   }

   SetCursor( LoadCursor( 0, IDC_ARROW ) );

   g_winGlobals.hFont = CreateFontA( 16, 0, 0, 0,
                                     FW_BOLD, FALSE, FALSE, FALSE,
                                     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
                                     "Consolas" );

   WinPixelBuffer_Init( &( g_winGlobals.screenBuffer ), SCREEN_WIDTH, SCREEN_HEIGHT );
   g_winGlobals.bmpInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
   g_winGlobals.bmpInfo.bmiHeader.biWidth = g_winGlobals.screenBuffer.w;
   g_winGlobals.bmpInfo.bmiHeader.biHeight = -(LONG)( g_winGlobals.screenBuffer.h );
   g_winGlobals.bmpInfo.bmiHeader.biPlanes = 1;
   g_winGlobals.bmpInfo.bmiHeader.biBitCount = 32;
   g_winGlobals.bmpInfo.bmiHeader.biCompression = BI_RGB;

   InitButtonMap();

   Game_Init( game, g_winGlobals.screenBuffer.memory16 );
   g_winGlobals.shutdown = False;

   g_winDebugFlags.showDiagnostics = False;
   g_winDebugFlags.noClip = False;
   g_winDebugFlags.fastWalk = False;
   g_winDebugFlags.showHitBoxes = False;

   while ( 1 )
   {
      Clock_StartFrame( &( game->clock ) );
      Input_ResetState( &( game->input ) );

      while ( PeekMessageA( &msg, g_winGlobals.hWndMain, 0, 0, PM_REMOVE ) )
      {
         TranslateMessage( &msg );
         DispatchMessageA( &msg );
      }

      Game_Tic( game );

      InvalidateRect( g_winGlobals.hWndMain, 0, FALSE );
      Clock_EndFrame( &game->clock );

      if ( g_winGlobals.shutdown )
      {
         break;
      }
   }

   WinPixelBuffer_CleanUp( &( g_winGlobals.screenBuffer ) );
   return 0;
}

void Program_Log( const char* msg )
{
   OutputDebugStringA( msg );
}

internal LRESULT CALLBACK MainWindowProc( _In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam )
{
   LRESULT result = 0;

   switch ( uMsg )
   {
      case WM_QUIT:
      case WM_CLOSE:
      case WM_DESTROY:
         g_winGlobals.shutdown = True;
         break;
      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_SYSKEYDOWN:
      case WM_SYSKEYUP:
         HandleKeyboardInput( (u32)wParam, lParam );
         break;
      case WM_PAINT:
         RenderScreen();
         break;
      default:
         result = DefWindowProcA( hWnd, uMsg, wParam, lParam );
   }

   return result;
}

internal void InitButtonMap()
{
   g_winGlobals.buttonMap[InputButton_Left] = VK_LEFT;
   g_winGlobals.buttonMap[InputButton_Up] = VK_UP;
   g_winGlobals.buttonMap[InputButton_Right] = VK_RIGHT;
   g_winGlobals.buttonMap[InputButton_Down] = VK_DOWN;
   g_winGlobals.buttonMap[InputButton_A] = 0x58; // X
   g_winGlobals.buttonMap[InputButton_B] = 0x5A; // Z
   g_winGlobals.buttonMap[InputButton_Start] = VK_RETURN;
   g_winGlobals.buttonMap[InputButton_Select] = VK_SHIFT;
}

// the double-buffering part of this came from Stack Overflow
internal void RenderScreen()
{
   HDC dc, dcMem;
   HBITMAP bmMem;
   HANDLE hOld;
   PAINTSTRUCT ps;
   int winWidth = (int)( SCREEN_WIDTH * GRAPHICS_SCALE );
   int winHeight = (int)( SCREEN_HEIGHT * GRAPHICS_SCALE );

   dc = BeginPaint( g_winGlobals.hWndMain, &ps );

   // create an off-screen DC for double-buffering
   dcMem = CreateCompatibleDC( dc );
   bmMem = CreateCompatibleBitmap( dc, winWidth, winHeight );
   hOld = SelectObject( dcMem, bmMem );

   // actually draw everything
   StretchDIBits( dcMem,
                  0, 0, (int)( SCREEN_WIDTH * GRAPHICS_SCALE ), (int)( SCREEN_HEIGHT * GRAPHICS_SCALE ), // dest
                  0, 0, g_winGlobals.screenBuffer.w, g_winGlobals.screenBuffer.h, // src
                  g_winGlobals.screenBuffer.memory32,
                  &( g_winGlobals.bmpInfo ),
                  DIB_RGB_COLORS, SRCCOPY );

   if ( g_winDebugFlags.showDiagnostics )
   {
      DrawDiagnostics( &dcMem );
   }

   // transfer the off-screen DC to the screen
   BitBlt( dc, 0, 0, winWidth, winHeight, dcMem, 0, 0, SRCCOPY );

   SelectObject( dcMem, hOld );
   DeleteObject( bmMem );
   DeleteDC( dcMem );
   EndPaint( g_winGlobals.hWndMain, &ps );
}

internal void DrawDiagnostics( HDC* dcMem )
{
   u32 gameSeconds, realSeconds, dayClockMinutes, hourHand;
   RECT r = { 10, 10, 0, 0 };
   char str[STRING_SIZE_DEFAULT];
   char sunMsg[STRING_SIZE_DEFAULT];
   HFONT oldFont = (HFONT)SelectObject( *dcMem, g_winGlobals.hFont );

   if ( g_winGlobals.game.isAM )
   {
      dayClockMinutes = (u32)( 720 * g_winGlobals.game.daylightFactor ); // 12 hours is 720 minutes
   }
   else
   {
      dayClockMinutes = (u32)( 720 * ( 1.0 - g_winGlobals.game.daylightFactor ) ); // 12 hours is 720 minutes
   }

   if ( g_winGlobals.game.daylightFactor < DAY_FACTOR_LOW_CUTOFF )
   {
      sprintf( sunMsg, "down" );
   }
   else if ( g_winGlobals.game.daylightFactor > DAY_FACTOR_HIGH_CUTOFF )
   {
      sprintf( sunMsg, "up" );
   }
   else
   {
      sprintf( sunMsg, g_winGlobals.game.isAM ? "rising" : "setting" );
   }

   hourHand = ( ( dayClockMinutes / 60 ) == 0 ) ? 12 : ( dayClockMinutes / 60 );

   SetTextColor( *dcMem, 0x00FFFFFF );
   SetBkMode( *dcMem, TRANSPARENT );

   sprintf_s( str, STRING_SIZE_DEFAULT, "      Frame Rate: %u", CLOCK_FPS );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "   Last Frame MS: %u", g_winGlobals.game.clock.lastFrameMicro / 1000 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "    Total Frames: %u", g_winGlobals.game.clock.frameCount );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   gameSeconds = g_winGlobals.game.clock.frameCount / CLOCK_FPS;
   sprintf_s( str, STRING_SIZE_DEFAULT, "   In-Game Timer: %u:%02u:%02u", gameSeconds / 3600, gameSeconds / 60, gameSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   realSeconds = ( g_winGlobals.game.clock.absoluteEndMicro - g_winGlobals.game.clock.absoluteStartMicro ) / 1000000;
   sprintf_s( str, STRING_SIZE_DEFAULT, "Real World Timer: %u:%02u:%02u", realSeconds / 3600, realSeconds / 60, realSeconds );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   gameSeconds = g_winGlobals.game.clock.frameCount / CLOCK_FPS;
   sprintf_s( str, STRING_SIZE_DEFAULT, "   In-Game Clock: %02u:%02u %s (sun is %s)", hourHand, dayClockMinutes % 60, g_winGlobals.game.isAM ? "AM" : "PM", sunMsg );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "        Position: %.3f, %.3f", g_winGlobals.game.player.entity->pos.x, g_winGlobals.game.player.entity->pos.y );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "      Tile Index: %u", g_winGlobals.game.player.tileIndex );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_Up].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "--" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_Left].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "   --" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_Right].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "      SEL" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_Select].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "          STR" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_Start].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "              B" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_B].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );

   sprintf_s( str, STRING_SIZE_DEFAULT, "                A" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_A].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   sprintf_s( str, STRING_SIZE_DEFAULT, "  |" );
   SetTextColor( *dcMem, g_winGlobals.game.input.buttonStates[InputButton_Down].down ? 0x00FFFFFF : 0x00333333 );
   DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
   r.top += 16;

   SetTextColor( *dcMem, 0x00FFFFFF );

   if ( g_winDebugFlags.noClip )
   {
      strcpy( str, "No clip" );
      DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
      r.top += 16;
   }

   if ( g_winDebugFlags.fastWalk )
   {
      strcpy( str, "Fast walk" );
      DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
      r.top += 16;
   }

   if ( g_winDebugFlags.showHitBoxes )
   {
      strcpy( str, "Show hit boxes" );
      DrawTextA( *dcMem, str, -1, &r, DT_SINGLELINE | DT_NOCLIP );
      r.top += 16;
   }

   SelectObject( *dcMem, oldFont );
}

internal void HandleKeyboardInput( u32 keyCode, LPARAM flags )
{
   Bool_t keyWasDown = ( flags & ( (LONG_PTR)1 << 30 ) ) != 0 ? True : False;
   Bool_t keyIsDown = ( flags & ( (LONG_PTR)1 << 31 ) ) == 0 ? True : False;
   u32 i;

   // ignore repeat presses
   if ( keyWasDown != keyIsDown )
   {
      if ( keyIsDown )
      {
         // ensure alt+F4 still closes the window
         if ( keyCode == VK_F4 && ( flags & ( (LONG_PTR)1 << 29 ) ) )
         {
            g_winGlobals.shutdown = True;
            return;
         }

         for ( i = 0; i < InputButton_Count; i++ )
         {
            if ( g_winGlobals.buttonMap[i] == keyCode )
            {
               Input_ButtonPressed( &g_winGlobals.game.input, i );
               break;
            }
         }

         // Windows-specific diagnostic/debug keys
         switch ( keyCode )
         {
            case VK_F8:
               TOGGLE_BOOL( g_winDebugFlags.showDiagnostics );
               break;
            case VK_NOCLIP:
               TOGGLE_BOOL( g_winDebugFlags.noClip );
               break;
            case VK_FASTWALK:
               TOGGLE_BOOL( g_winDebugFlags.fastWalk );
               break;
            case VK_SHOWHITBOXES:
               TOGGLE_BOOL( g_winDebugFlags.showHitBoxes );
               break;
         }
      }
      else
      {
         for ( i = 0; i < InputButton_Count; i++ )
         {
            if ( g_winGlobals.buttonMap[i] == keyCode )
            {
               Input_ButtonReleased( &g_winGlobals.game.input, i );
               break;
            }
         }
      }
   }
}

internal void FatalError( const char* message )
{
   char errorMsg[STRING_SIZE_DEFAULT];
   snprintf( errorMsg, STRING_SIZE_DEFAULT, "Windows error: %s", message );
   MessageBoxA( 0, message, "Error", MB_OK | MB_ICONERROR );
   exit( 1 );
}
