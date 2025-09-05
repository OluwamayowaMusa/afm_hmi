#include "Arduino.h"
#include "afm_hmi.h"


/**
 * motorMoveDistance - Rotate motor such that angular displacement translates to distance
 *
 * @distance: linear displacement
 * @motor: X or Y motor
 *
*/
void motorMoveDistance(int distance, char motor)
{
  int stepPin, dirPin;

  if (motor == 'X')
  {
    stepPin = STEP_PIN_X;
    dirPin = DIR_PIN_X;
  }
  else
  {
    stepPin = STEP_PIN_Y;
    dirPin = DIR_PIN_Y;
  }

  if (distance > 0)
    digitalWrite(dirPin, HIGH);
  else
    digitalWrite(dirPin, LOW);

  int steps = STEPS_PER_DISTANCE * abs(distance); 

  for (int x = 0; x < steps; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }

  delay(100);
}
