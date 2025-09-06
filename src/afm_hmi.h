#ifndef AFM_HMI_H
#define AFM_HMI_H

/* Define Libraries */
#include "Arduino.h"
#include "WString.h"
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "Keypad.h"

/* Global Varibles */
extern Adafruit_SSD1306 display;
extern String pages[5];
extern int totalPages;
extern int currentPage;
extern bool displayingText;
extern unsigned long lastButtonPress;
extern bool buttonPressed;

/* Global Macros */
#define DEBOUNCE_DELAY 50
#define PAGE_BUTTON_PIN 0
#define YES_BUTTON_PIN 1
#define NO_BUTTON_PIN 12
#define ENABLE_PIN 4
#define SLEEP_PIN 13

#define JOYSTICK_X A1 /*Inverted due to design*/
#define JOYSITCK_Y A0
#define STEP_PIN_X A2
#define DIR_PIN_X A3
#define STEP_PIN_Y A4
#define DIR_PIN_Y A5
#define STEPS_PER_DISTANCE 400 /*Microstep-16, distance-8mm*/
#define STEP_DELAY 1000

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define ROWS 4
#define COLS 3


/**
 * struct MotorPins - Describe the pins required to operate Motor
 *
 * @stepPin: Control Motor Movement
 * @dirPin: Control Direction
 *
 * Description: The Motor is controlled with stepPin and dirPin
 */
struct MotorPins
{
	int stepPin;
	int dirPin;
};

/* Function Prototypes */
void show_manual(void);
void take_an_afm_image(void);
bool confirm_all_checks(void);
bool navigate_to_scan_area(void);
void keypadNavigation(void);
int getAxisInput(char axis);
void displayMovementComplete(int x_val, int y_val);
void joystickNavigation(void);
void motorMoveDistance(int distance, char motor);
void displayMainMenu(void);
void setupDisplay(void);
bool performSingleCheck(
	const __FlashStringHelper *category,
	const __FlashStringHelper *question);

void handlePagination(void);
void displayCurrentPage(void);
void displayTextWithPagination(String text);
void splitTextIntoPages(String text);
char waitForKeypadInput(void);

#endif /* AFM_HMI_H */
