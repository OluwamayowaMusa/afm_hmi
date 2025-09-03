#include "Arduino.h"
#include "Key.h"
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
        Serial.println(F("In Show Manual"));
        show_manual();
        break;

      case '2':
        Serial.println(F("In Take an AFM image"));
        take_an_afm_image();
        break;

      default:
        Serial.print(F("The default expression, key: "));
        Serial.println(key);
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
  Serial.println(F("MUsaaa"));
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
        if (confirm_all_checks()){
          Serial.println(F("All Checks performed, naviagting"));
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
  const char* checks[][2] = {
    {"Cable Connections", "Cable well connected?"},
    {"System Components", "Probe Head Installed?"},
    {"System Components", "Scanner Installed?"},
    {"System Components", "Probe Loaded?"},
    {"System Components", "Sample Loaded?"}
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
bool performSingleCheck(const char *category, const char *question)
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
