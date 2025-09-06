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
