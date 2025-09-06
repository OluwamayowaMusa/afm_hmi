#include "Arduino.h"
#include "USBAPI.h"
#include "WString.h"
#include "afm_hmi.h"


// Declaration for SSD1306 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declaration for Joystick
#define X_PIN A0
#define Y_PIN A1

// Declaration for Keypad
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 11, 10, 9, 8 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 7, 6, 5, 4 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Text and pagination variables
String pages[5]; // Array to store pages (adjust size as needed)
int totalPages = 0;
int currentPage = 0;
bool displayingText = false; // Flag to track if we're in text display mode

// Button debouncing variables
unsigned long lastButtonPress = 0;
bool buttonPressed = false;

/**
 * setup - Configures I/O pins and Peripherals
 *
*/
void setup() {
  Serial.begin(9600);

  // initialize the OLED object
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  //initialize buttons
  pinMode(PAGE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(YES_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NO_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SLEEP_PIN, OUTPUT);

  // initialize Motor pins
  pinMode(STEP_PIN_X, OUTPUT);
  pinMode(DIR_PIN_X, OUTPUT);
  pinMode(STEP_PIN_Y, OUTPUT);
  pinMode(DIR_PIN_Y, OUTPUT);

  // Power off Motors
  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(SLEEP_PIN, LOW);

  displayMainMenu();
}

/**
 * loop - Executes Program continuously
 */
void loop() {
  // Handle pagination if we're displaying text
  if (displayingText) {
    handlePagination();
    return; // Don't process keypad while in text display mode
  }


  char key = keypad.getKey();  // Read the key

  if (key)
  {
    switch (key)
    {
      case '1':
        Serial.println(F("A: Show Manual"));
        show_manual();
        break;

      case '2':
        Serial.println(F("A: Take an AFM image"));
        take_an_afm_image();
        break;

      default:
        break;
    }
  }
}


/**
 * show_manual - Show device Manual
 *
 */

void show_manual(void)
{
}

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

  Serial.println(F("Waiting for keyboard input"));
  char key = waitForKeypadInput();

  if (key)
  {
    switch (key)
    {
      case '1':
          Serial.println(F("Confirm all checks"));
          if (confirm_all_checks())
          {
            Serial.println(F("All Checks performed, naviagting"));
            navigate_to_scan_area();
          }
          else
          {
            display.println(F("Ensure checks pass"));
            display.display();
          }
      default:
          Serial.println(F("Going back to Main Menu"));
          displayMainMenu();
          break;
    }
  }
}


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
//    {F("System Components"), F("Probe Head Installed?")},
//    {F("System Components"), F("Scanner Installed?")},
//    {F("System Components"), F("Probe Loaded?")},
//    {F("System Components"), F("Sample Loaded?")}
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

/**
 * navigate_to_scan_area - Navigate to area on sample to scan
 */
void navigate_to_scan_area(void)
{
  Serial.println(F("In Navigation"));
  String text = F("Navigate scan Area\n"
               "-Use the keypad or joystick to navigate to interested area under sample.\n"
               "-Press 1 for keypad, 2 for joystick.\n"
  );
  displayTextWithPagination(text);

  Serial.println(F("Waiting for keyboard input"));
  char key = waitForKeypadInput();

  switch (key) {
    case '1':
      keypadNavigation();
      break;
    case '2':
      joystickNavigation();
      break;
    default:
      Serial.println(F("Invalid selection"));
      // Could recursively call navigate_to_scan_area() or return to menu
      break;
  }
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
  Serial.print(F("Move X by "));
  Serial.println(x_displacement);
  motorMoveDistance(x_displacement, 'X');

  Serial.print(F("Move Y by "));
  Serial.println(y_displacement);
  motorMoveDistance(y_displacement, 'Y');

  // Show completion message
  displayMovementComplete(x_displacement, y_displacement);
}

/**
 * displayMovementComplete - Show movement completion summary
 *
 * @x_val: X displacement
 * @y_val: Y displacement
 */
void displayMovementComplete(int x_val, int y_val)
{
  String completion_text = "Movement Complete\n"
                          "X: " + String(x_val) + "mm\n"
                          "Y: " + String(y_val) + "mm\n"
                          "Press any key...";
  displayTextWithPagination(completion_text);
  waitForKeypadInput();
}

/**
 * getAxisInput - Get and validate input for a specific axis
 * @axis: 'X' or 'Y' axis identifier
 *
 * Return: validated integer displacement value
 */
int getAxisInput(char axis)
{
  String prompt_text = "Keypad Nav\n-Enter " + String(axis) + " (mm): ";

  while (true) {
    displayTextWithPagination(prompt_text);
    display.setCursor(0, 16);
    char input_value = waitForKeypadInput();

    // Validate input
    if (isdigit(input_value)) {
      Serial.println(input_value);
      display.println(input_value);
      display.display();
      delay(1500); // Reduced delay

      return (int(input_value) - 48); // Convert ASCII to integer
    } else {
      // Invalid input handling
      Serial.println(F("Enter valid digit (0-9)"));
      String error_text = "Invalid Input\n-Enter digit 0-9 for " + String(axis) + " axis\n-Try again...";
      displayTextWithPagination(error_text);
      waitForKeypadInput(); // Wait for acknowledgment
    }
  }
}
/**
 * joystickNavigation - Navigate to scan area using the joystick
 */

void joystickNavigation(void)
{
  Serial.println(F("In Joystick"));
  setupDisplay();
  display.println(F("Joystick Nav"));
  display.println(F("Press W-BTN to exit"));
  display.display();

  
  char x_buffer[10];
  char y_buffer[10];

  int x_dis = 0;
  int y_dis = 0;
  int last_x_dis = 0;
  int last_y_dis = 0;
  while (digitalRead(PAGE_BUTTON_PIN))
  {
    int x_raw = analogRead(JOYSTICK_X);
    int y_raw = analogRead(JOYSITCK_Y);

    int x_val = map(x_raw, 0, 1023, 0, 1);
    int y_val = map(y_raw, 0, 1023, 0, 1);

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
      String text = F(
        "Joystick Nav\n"
        "Maximum translation distance for X and Y Reached\n"
      );
      displayTextWithPagination(text);
      waitForKeypadInput();
      break;
    }

    sprintf(x_buffer, "X (mm): %d", x_dis);
    sprintf(y_buffer, "Y (mm): %d", y_dis);
    display.fillRect(0, 16, 128, 16, SSD1306_BLACK);
    display.setCursor(0, 16);
    display.println(x_buffer);
    display.println(y_buffer);
    display.display();
  }

  displayMovementComplete(x_dis, y_dis);
}


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

  Serial.print(F("Key pressed: "));
  Serial.println(key);
  return key;
}
