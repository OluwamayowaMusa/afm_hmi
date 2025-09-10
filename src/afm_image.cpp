#include "afm_hmi.h"

/**
 * take_an_afm_image - Take an AFM Image
 *
 */
void take_an_afm_image(void)
{
  String text = F("Take an AFM Image\n"
                "-Confirm all Checks\n"
                "-Navigate to scan area\n"
                "-Configure the scan parameters\n"
                "-Take scan and update parameters\n"
                "-Press 1 to begin\n"
  );
  displayTextWithPagination(text);

  char key = waitForKeypadInput();
  switch (key)
  {
    case '1':
    {
      if (!confirm_all_checks())
      {
        display.println(F("Ensure checks pass"));
        display.display();
        delay(2000);
        return;
      }
      
      bool status = navigate_to_scan_area();
      if (!status)
      {
        displayCustomizedError(F("Navigation Cancelled.\n"));

        while (digitalRead(NO_BUTTON_PIN) && digitalRead(YES_BUTTON_PIN)) delay(10);

        if (!digitalRead(YES_BUTTON_PIN))  status = navigate_to_scan_area();
        if (!digitalRead(NO_BUTTON_PIN)) return;
      }

      if (!status) return;

      status = configure_scan_parameters();
      if (!status)
      {
        displayCustomizedError(F("Configuration Cancelled"));

        while (digitalRead(NO_BUTTON_PIN) && digitalRead(YES_BUTTON_PIN)) delay(10);

        if (!digitalRead(YES_BUTTON_PIN)) status = configure_scan_parameters();
        if (!digitalRead(NO_BUTTON_PIN)) return;
      }

      if (!status) return;

      if (take_scan())
      {
          setupDisplay();
          display.println(F("Scan all done"));
          display.println(F("Press any key to continue"));
          display.display();
          waitForKeypadInput();
      }
    }
    default:
      break;
  }
}
