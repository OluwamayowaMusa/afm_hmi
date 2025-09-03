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
