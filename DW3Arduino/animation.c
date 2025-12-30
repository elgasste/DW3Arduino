#include "animation.h"
#include "screen.h"
#include "clock.h"

internal void Animation_Start( Animation_t* animation, Screen_t* screen );
internal void Animation_TicPause( Animation_t* animation );
internal void Animation_TicFadeOut( Animation_t* animation, Screen_t* screen );
internal void Animation_TicFadeIn( Animation_t* animation, Screen_t* screen );

void AnimationChain_Init( AnimationChain_t* chain, Screen_t* screen )
{
   chain->screen = screen;
   AnimationChain_Reset( chain );
}

void AnimationChain_Reset( AnimationChain_t* chain )
{
   chain->animationCount = 0;
   chain->curAnimation = 0;
   chain->curAnimationIndex = 0;
   chain->isRunning = False;
}

void AnimationChain_Push( AnimationChain_t* chain, AnimationType_t type, r32 seconds )
{
   AnimationChain_PushWithCallback( chain, type, seconds, 0, 0, 0 );
}

void AnimationChain_PushWithCallback( AnimationChain_t* chain, AnimationType_t type, r32 seconds,
                                      void ( *callback )( void*, void* ), void* callbackPayload1, void* callbackPayload2 )
{
   Animation_t* newAnimation = chain->animations + chain->animationCount;

   newAnimation->type = type;
   newAnimation->callback = callback;
   newAnimation->callbackPayload1 = callbackPayload1;
   newAnimation->callbackPayload2 = callbackPayload2;
   newAnimation->totalSeconds = seconds;
   newAnimation->elapsedSeconds = 0.0f;

   chain->animationCount++;
}

void AnimationChain_Start( AnimationChain_t* chain )
{
   chain->curAnimation = chain->animations;
   chain->curAnimationIndex = 0;
   chain->isRunning = True;

   Animation_Start( chain->curAnimation, chain->screen );
}

void AnimationChain_Tic( AnimationChain_t* chain )
{
   Animation_t* animation = chain->curAnimation;

   if ( animation )
   {
      switch ( animation->type )
      {
         case AnimationType_Pause:
         case AnimationType_ActivePause:
            Animation_TicPause( animation );
            break;
         case AnimationType_FadeOut:
         case AnimationType_ActiveFadeOut:
            Animation_TicFadeOut( animation, chain->screen );
            break;
         case AnimationType_FadeIn:
         case AnimationType_ActiveFadeIn:
            Animation_TicFadeIn( animation, chain->screen );
            break;
      }

      if ( animation->elapsedSeconds > animation->totalSeconds )
      {
         if ( animation->callback )
         {
            animation->callback( animation->callbackPayload1, animation->callbackPayload2 );
         }

         chain->curAnimationIndex++;

         if ( chain->curAnimationIndex >= chain->animationCount )
         {
            chain->isRunning = False;
         }
         else
         {
            chain->curAnimation = chain->animations + chain->curAnimationIndex;
            Animation_Start( chain->curAnimation, chain->screen );
         }
      }
   }
   else
   {
      chain->isRunning = False;
   }
}

Bool_t AnimationChain_BlocksInput( AnimationChain_t* chain )
{
   // TODO: currently all animations should block input, but there might be
   // some that won't (like if some non-player entity has an animation).
   return ( chain->isRunning && chain->curAnimation ) ? True : False;
}

Bool_t AnimationChain_PausesAction( AnimationChain_t* chain )
{
   if ( chain->isRunning && chain->curAnimation )
   {
      switch ( chain->curAnimation->type )
      {
         case AnimationType_ActivePause:
         case AnimationType_ActiveFadeIn:
         case AnimationType_ActiveFadeOut:
            return False;
         default:
            return True;
      }
   }

   return False;
}

internal void Animation_Start( Animation_t* animation, Screen_t* screen )
{
   switch ( animation->type )
   {
      case AnimationType_FadeOut:
      case AnimationType_ActiveFadeOut:
         Screen_BackupPalette( screen );
         break;
   }
}

internal void Animation_TicPause( Animation_t* animation )
{
   animation->elapsedSeconds += CLOCK_FRAME_SECONDS;
}

internal void Animation_TicFadeOut( Animation_t* animation, Screen_t* screen )
{
   u32 i;
   u16 rangeR, rangeB, rangeG;
   r32 p;

   animation->elapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->elapsedSeconds > animation->totalSeconds )
   {
      Screen_ClearPalette( screen, SCREEN_COLOR16_BLACK );
   }
   else
   {
      for ( i = 0; i < screen->paletteColorCount; i++ )
      {
         if ( screen->backupPalette[i] != SCREEN_COLOR16_TRANSPARENT )
         {
            rangeR = screen->backupPalette[i] >> 11;
            rangeG = ( screen->backupPalette[i] & 0x7E0 ) >> 5;
            rangeB = screen->backupPalette[i] & 0x1F;
            p = 1.0f - ( animation->elapsedSeconds / animation->totalSeconds );
            screen->palette[i] = ( (u16)( rangeR * p ) << 11 ) | ( (u16)( rangeG * p ) << 5 ) | (u16)( rangeB * p );
         }
      }
   }
}

internal void Animation_TicFadeIn( Animation_t* animation, Screen_t* screen )
{
   u32 i;
   u16 rangeR, rangeB, rangeG;
   r32 p;

   animation->elapsedSeconds += CLOCK_FRAME_SECONDS;

   if ( animation->elapsedSeconds > animation->totalSeconds )
   {
      Screen_RestorePalette( screen );
   }
   else
   {
      for ( i = 0; i < screen->paletteColorCount; i++ )
      {
         if ( screen->backupPalette[i] != SCREEN_COLOR16_TRANSPARENT )
         {
            rangeR = screen->backupPalette[i] >> 11;
            rangeG = ( screen->backupPalette[i] & 0x7E0 ) >> 5;
            rangeB = screen->backupPalette[i] & 0x1F;
            p = animation->elapsedSeconds / animation->totalSeconds;
            screen->palette[i] = ( (u16)( rangeR * p ) << 11 ) | ( (u16)( rangeG * p ) << 5 ) | (u16)( rangeB * p );
         }
      }
   }
}
