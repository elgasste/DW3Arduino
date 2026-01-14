#if !defined( ANIMATION_H )
#define ANIMATION_H

#define ANIMATION_CHAIN_MAX_ANIMATIONS    32

#include "common.h"

typedef struct Screen_t Screen_t;

typedef struct Animation_t
{
   AnimationType_t type;
   r32 totalSeconds;
   r32 elapsedSeconds;
   void ( *callback )( void*, void* );
   void* callbackPayload1;
   void* callbackPayload2;
}
Animation_t;

typedef struct AnimationChain_t
{
   Screen_t* screen;
   Animation_t animations[ANIMATION_CHAIN_MAX_ANIMATIONS];
   u32 animationCount;
   Animation_t* curAnimation;
   u32 curAnimationIndex;
   Bool_t isRunning;
}
AnimationChain_t;

#if defined( __cplusplus )
extern "C" {
#endif

void AnimationChain_Init( AnimationChain_t* chain, Screen_t* screen );
void AnimationChain_Reset( AnimationChain_t* chain );
void AnimationChain_Push( AnimationChain_t* chain, AnimationType_t type, r32 seconds );
void AnimationChain_PushWithCallback( AnimationChain_t* chain, AnimationType_t type, r32 seconds,
                                      void ( *callback )( void*, void* ), void* callbackPayload1, void* callbackPayload2 );
void AnimationChain_Start( AnimationChain_t* chain );
void AnimationChain_Tic( AnimationChain_t* chain );
Bool_t AnimationChain_BlocksInput( AnimationChain_t* chain );
Bool_t AnimationChain_PausesAction( AnimationChain_t* chain );

#if defined( __cplusplus )
}
#endif

#endif // ANIMATION_H
