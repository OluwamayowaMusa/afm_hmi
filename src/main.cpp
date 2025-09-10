#include "afm_hmi.h"


// Declaration for SSD1306 display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declaration for Joystick
#define X_PIN A0
#define Y_PIN A1

// Declaration for Keypad
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 11, 10, 9, 8 };  //connect to the row pinouts of the keypad
byte colPins[COLS] = { 7, 6, 5};  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Text and pagination variables
String pages[5]; // Array to store pages (adjust size as needed)
int totalPages = 0;
int currentPage = 0;
bool displayingText = false; // Flag to track if we're in text display mode

// Scan Parameters
float gain_paramter = 0.0;

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

  switch (waitForKeypadInput())
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

  displayMainMenu();
}
