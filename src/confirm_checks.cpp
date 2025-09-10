#include "afm_hmi.h"

/**
 * confirm_all_checks - Confirm all system components are well connected.
 *
 * Return: true when all checks are passed
 */
bool confirm_all_checks(void)
{
  // Array of check items - category, question pairs
  const __FlashStringHelper* checks[][2] = {
    {F("Cable Connections"), F("Cable well connected?")},
    {F("System Components"), F("Probe Head Installed?")},
    {F("System Components"), F("Scanner Installed?")},
    {F("System Components"), F("Probe Loaded?")},
    {F("System Components"), F("Sample Loaded?")}
  };

  int numChecks = sizeof(checks) / sizeof(checks[0]);

  // Perform each check
  for (int i = 0; i < numChecks; i++) {
    if (!performSingleCheck(checks[i][0], checks[i][1])) {
      return false; // If any check fails, return false
    }
  }

  return true; // All checks passed
}

/**
 * performSingleCheck - Perform a single yes/no check
 *
 * @category: The category name to display (e.g., "Cable Connections")
 * @question: The question to ask (e.g., "Cable well connected?")
 * Return: true if user pressed YES, false if user pressed NO
 */
bool performSingleCheck(
  const __FlashStringHelper *category,
  const __FlashStringHelper *question)
{
  // Display the question
  setupDisplay();
  display.println(category);
  display.println(question);
  display.display();

  // Wait for button press (active low buttons)
  while (digitalRead(YES_BUTTON_PIN) && digitalRead(NO_BUTTON_PIN)) {
    delay(10); // Small delay to prevent excessive CPU usage
  }

  // Process the response
  if (!digitalRead(YES_BUTTON_PIN)) {
    display.println(F("Yes"));
    display.display();
    delay(2000);
    return true;
  }
  else if (!digitalRead(NO_BUTTON_PIN)) {
    display.println(F("No"));
    display.display();
    delay(2000);
    return false;
  }

  // This should never be reached, but just in case
  return false;
}
