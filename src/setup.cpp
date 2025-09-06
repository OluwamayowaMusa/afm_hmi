#include "WString.h"
#include "afm_hmi.h"

/**
 * setupDisplay - Clear and setup display for writing
*/

void setupDisplay(void)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

/**
 * displayMainMenu - display main menu
*/
void displayMainMenu()
{
  setupDisplay();
  display.println(F("HMI System Ready"));
  display.println(F("1: Show Device Manual"));
  display.println(F("2: Take an AFM Image"));
  display.println(F("Select option:"));
  display.display();
}

/**
 * displayError - Display errorr
*/
void displayError(void)
{
  setupDisplay();
  display.println(F("Invalid Input\n-Enter digit 0-9\n-Try again..."));
  display.display();
  waitForKeypadInput(); // Wait for acknowledgment
}

/**
 * displayCustomizedError - Display customised error
 *
 * @message - Error message
*/

void displayCustomizedError(const __FlashStringHelper *message)
{
  setupDisplay();
  display.println(message);
  display.println(F("Press G-BTN to redo, R-BTN to cancel"));
  display.display();
}

/**
 * displayMovementComplete - Show movement completion summary
 *
 * @x_val: X displacement
 * @y_val: Y displacement
 */
void displayMovementComplete(int x_val, int y_val)
{
  setupDisplay();

  display.println(F("Movement Complete"));
  display.print(F("X: "));
  display.print(x_val);
  display.println(F("mm"));
  display.print(F("Y: "));
  display.print(y_val);
  display.println(F("mm"));
  display.println(F("Press any key..."));

  display.display();
  waitForKeypadInput();
}
