#include "afm_hmi.h"

/**
 * navigate_to_scan_area - Navigate to area on sample to scan
 *
 * Return: true if success else false
 */
bool navigate_to_scan_area(void)
{
  String text = F("Navigate scan Area\n"
               "-Use the keypad or joystick to navigate to interested area under sample.\n"
               "-Press 1 for keypad, 2 for joystick.\n"
  );
  displayTextWithPagination(text);

  char key = waitForKeypadInput();

  switch (key) {
    case '1':
      keypadNavigation();
      break;
    case '2':
      joystickNavigation();
      break;
    default:
      // Could recursively call navigate_to_scan_area() or return to menu
      return false;
      break;
  }
  return true;
}

/**
 * keypadNavigation - Navigate to scan area using the keypad.
 */
void keypadNavigation(void)
{
  // Get X and Y coordinates using helper function
  int x_displacement = getAxisInput('X');
  int y_displacement = getAxisInput('Y');

  // Execute the movements
  motorMoveDistance(x_displacement, 'X');
  motorMoveDistance(y_displacement, 'Y');

  // Show completion message
  displayMovementComplete(x_displacement, y_displacement);
}

/**
 * joystickNavigation - Navigate to scan area using the joystick
 */

void joystickNavigation(void)
{
  setupDisplay();
  display.println(F("Joystick Nav"));
  display.println(F("Press W-BTN to exit"));
  display.display();

  // Set up buffer
  char x_buffer[10];
  char y_buffer[10];

  // displacement varibales 
  int x_dis = 0;
  int y_dis = 0;
  int last_x_dis = 0;
  int last_y_dis = 0;

  while (digitalRead(PAGE_BUTTON_PIN))
  {
    // Read and map input
    int x_val = map(analogRead(JOYSTICK_X), 0, 1023, 0, 1);
    int y_val = map(analogRead(JOYSITCK_Y), 0, 1023, 0, 1);

    // Confirm the stage has room to move
    if (x_dis + x_val <= 8 && y_dis +y_val <= 8)
    {
      x_dis += x_val;
      y_dis += y_val;

      if (x_dis != last_x_dis)
        motorMoveDistance(x_val, 'X');
      if (y_dis != last_y_dis)
        motorMoveDistance(y_val, 'Y');
      last_x_dis = x_dis;
      last_y_dis = y_dis;
    }
    else
    {
      setupDisplay();
      display.println(F("Joystick Nav"));
      display.println(F("Maximum translation for X or Y"));
      display.display();
      waitForKeypadInput();
      break;
    }

    // Update display based on current value of X and Y
    sprintf(x_buffer, "X (mm): %d", x_dis);
    sprintf(y_buffer, "Y (mm): %d", y_dis);
    display.fillRect(0, 16, 128, 16, SSD1306_BLACK);
    display.setCursor(0, 16);
    display.println(x_buffer);
    display.println(y_buffer);
    display.display();

    delay(100); // Prevent excessive CPU Usage
  }

  displayMovementComplete(x_dis, y_dis);
}

