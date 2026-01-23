#include "main.h"

#include <unity.h>

void setUp()
{
}

void tearDown()
{
}

int main( int argc, char** argv )
{
   UNITY_BEGIN();

   // input tests
   RUN_TEST( Input_Init_Always_SetsAllStatesToFalse );
   RUN_TEST( Input_AnyButtonPressed_NoButtonsPressed_ReturnsFalse );
   RUN_TEST( Input_AnyButtonPressed_ButtonWasPressed_ReturnsTrue );

   return UNITY_END();
}
