#include "animation.h"
#include "screen.h"
#include "clock.h"

internal void Animation_TicPause( Animation_t* animation );

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
}

void AnimationChain_Tic( AnimationChain_t* chain )
{
   Animation_t* animation = chain->curAnimation;

   if ( animation )
   {
      switch ( animation->type )
      {
         case AnimationType_TotalPause:
         case AnimationType_ActivePause:
            Animation_TicPause( animation );
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
   if ( chain->isRunning && chain->curAnimation )
   {
      switch ( chain->curAnimation->type )
      {
         case AnimationType_ActivePause:
         case AnimationType_TotalPause:
            return True;
         default:
            return False;
      }
   }
   
   return False;
}

Bool_t AnimationChain_PausesAction( AnimationChain_t* chain )
{
   if ( chain->isRunning && chain->curAnimation )
   {
      switch ( chain->curAnimation->type )
      {
         case AnimationType_ActivePause:
            return False;
         case AnimationType_TotalPause:
            return True;
         default:
            return False;
      }
   }

   return False;
}

internal void Animation_TicPause( Animation_t* animation )
{
   animation->elapsedSeconds += CLOCK_FRAME_SECONDS;
}
