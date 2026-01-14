#if !defined( ENUMS_H )
#define ENUMS_H

#include "common.h"

typedef enum Direction_t
{
   Direction_Left = 0,
   Direction_Up,
   Direction_Right,
   Direction_Down,

   Direction_Count
}
Direction_t;

typedef enum InputButton_t
{
   InputButton_A = 0,
   InputButton_B,
   InputButton_Select,
   InputButton_Start,
   InputButton_Up,
   InputButton_Down,
   InputButton_Left,
   InputButton_Right,

   InputButton_Count
}
InputButton_t;

typedef enum AnimationType_t
{
   AnimationType_Pause = 0,
   AnimationType_ActivePause,
   AnimationType_FadeOut,
   AnimationType_ActiveFadeOut,
   AnimationType_FadeIn,
   AnimationType_ActiveFadeIn,

   AnimationType_Count
}
AnimationType_t;

typedef enum PlayerClass_t
{
   PlayerClass_Hero = 0,
   PlayerClass_Soldier,
   PlayerClass_Pilgrim,
   PlayerClass_Wizard,
   PlayerClass_Fighter,
   PlayerClass_Merchant,
   PlayerClass_GoofOff,

   PlayerClass_Count
}
PlayerClass_t;

typedef enum EncounterRate_t
{
   EncounterRate_None = 0,
   EncounterRate_Low,
   EncounterRate_Medium,
   EncounterRate_High,

   EncounterRate_Count
}
EncounterRate_t;

typedef enum TileDamageRate_t
{
   TileDamageRate_None = 0,
   TileDamageRate_Low,
   TileDamageRate_Medium,
   TileDamageRate_High,

   TileDamageRate_Count
}
TileDamageRate_t;

#endif // ENUMS_H
