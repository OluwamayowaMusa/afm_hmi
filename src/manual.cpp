#include "afm_hmi.h"

/**
 * show_manual - Show device Manual
 *
 */

void show_manual(void)
{
  setupDisplay();
  display.println(F("AutoProbe CP system must Grounded"));
  display.println(F("AutoProbe CP components must be well connected"));
  display.display();
  waitForKeypadInput();
}
