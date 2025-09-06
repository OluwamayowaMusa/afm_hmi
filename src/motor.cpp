#include "afm_hmi.h"


/**
 * motorMoveDistance - Rotate motor such that angular displacement translates to distance
 * @distance: linear displacement (can be positive or negative)
 * @motor: 'X' or 'Y' motor identifier
 */
void motorMoveDistance(int distance, char motor)
{
  // Power motor
  digitalWrite(ENABLE_PIN, LOW);
  digitalWrite(SLEEP_PIN, HIGH);


  // Select appropriate pins based on motor
  MotorPins pins;
  switch (motor) {
    case 'X':
      pins = {STEP_PIN_X, DIR_PIN_X};
      break;
    case 'Y':
      pins = {STEP_PIN_Y, DIR_PIN_Y};
      break;
    default:
      return;
  }

  // Set direction based on distance sign
  digitalWrite(pins.dirPin, (distance > 0) ? HIGH : LOW);

  // Calculate and execute steps
  int steps = STEPS_PER_DISTANCE * abs(distance);

  for (int i = 0; i < steps; i++) {
    digitalWrite(pins.stepPin, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(pins.stepPin, LOW);
    delayMicroseconds(STEP_DELAY);
  }

  delay(100); // Brief pause after movement


  // Power off Motors
  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(SLEEP_PIN, LOW);
}
