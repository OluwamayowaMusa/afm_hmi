#include "afm_hmi.h"

/**
* waitForKeypadInput - Wait for keypad input and return the key
*
* Return: key
*/
char waitForKeypadInput() {
  char key = 0;
  while (!key) {
    // Continue handling pagination while waiting for keypad input
    if (displayingText) {
      handlePagination();
    }

    key = keypad.getKey();
    delay(10); // Small delay to prevent excessive CPU usage
  }

  // Only exit pagination mode after keypad input is received
  displayingText = false;

  return key;
}

/**
 * getValidatedInput - Validate the input 
 *
 * @query: Paramter to get
*/
int getValidatedInput(const __FlashStringHelper *query)
{
  while (true)
  {
    setupDisplay();
    display.println(query);
    display.display();

    display.setCursor(0, 16);
    char input = waitForKeypadInput();

    // validate Input
    if (input == '0')
    {
      display.println("Cancelled By User");
      display.display();
      delay(1500);
      return -1;
    }
    else if (isdigit(input))
    {
      display.println(input);
      display.display();
      delay(1500);

      return (int(input) - 48);
    }
    else
    {
      displayError();
    }

  }
}

/**
 * getAxisInput - Get and validate input for a specific axis
 * @axis: 'X' or 'Y' axis identifier
 *
 * Return: validated integer displacement value
 */
int getAxisInput(char axis)
{
  char prompt_text[28];
  sprintf(prompt_text, "Keypad Nav\n-Enter %c  (mm):", axis);

  while (true) {
    setupDisplay();
    display.println(prompt_text);
    display.display();

    display.setCursor(0, 16);
    char input_value = waitForKeypadInput();

    // Validate input
    if (isdigit(input_value)) {
      display.println(input_value);
      display.display();
      delay(1500); // Reduced delay

      return (int(input_value) - 48); // Convert ASCII to integer
    } else {
      // Invalid input handling
      displayError();
    }
  }
}
