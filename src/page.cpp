#include "afm_hmi.h"

/**
 * handlePagination - handle the pages
*/
void handlePagination(void) {
  // Check for pagination button press
  if (digitalRead(PAGE_BUTTON_PIN) == LOW && !buttonPressed &&
      (millis() - lastButtonPress > DEBOUNCE_DELAY)) {

    buttonPressed = true;
    lastButtonPress = millis();

    // Go to next page (wrap around to first page if at end)
    currentPage = (currentPage + 1) % totalPages;
    displayCurrentPage();
  }

  // Reset button state when released
  if (digitalRead(PAGE_BUTTON_PIN) == HIGH) {
    buttonPressed = false;
  }
}

/**
 * displayCurrentPage - Display text on current page
 */
void displayCurrentPage(void)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Display the current page text
  display.print(pages[currentPage]);

  // Display navigation info at bottom
  display.setCursor(0, SCREEN_HEIGHT - 8);
  display.print(F("Btn:Next "));
  display.setCursor(SCREEN_WIDTH - 30, SCREEN_HEIGHT - 8);
  display.print(currentPage + 1);
  display.print(F("/"));
  display.print(totalPages);

  display.display();
}

/**
* splitTextIntoPages - Split the text into various page based on the screen size
*
* @text: Text to be splited
*/
void splitTextIntoPages(String text) {
  // Calculate characters per line and lines per page
  int charsPerLine = SCREEN_WIDTH / 6; // Each character is ~6 pixels wide
  int linesPerPage = (SCREEN_HEIGHT - 8) / 8; // Reserve bottom line for page info

  totalPages = 0;
  int textPos = 0;
  int textLength = text.length();

  while (textPos < textLength && totalPages < 20) {
    String page = "";
    int linesUsed = 0;

    // Fill page line by line
    while (linesUsed < linesPerPage && textPos < textLength) {
      String currentLine = "";
      int lineLength = 0;

      // Build current line character by character
      while (lineLength < charsPerLine && textPos < textLength) {
        char currentChar = text.charAt(textPos);

        // Handle newline character
        if (currentChar == '\n') {
          textPos++; // Skip the newline character
          break;     // End this line
        }

        currentLine += currentChar;
        lineLength++;
        textPos++;
      }

      // If we filled the line without hitting a newline, try to break at word boundary
      if (lineLength == charsPerLine && textPos < textLength) {
        char nextChar = text.charAt(textPos);

        // If next character isn't a space or newline, try to break at last space
        if (nextChar != ' ' && nextChar != '\n') {
          int lastSpace = currentLine.lastIndexOf(' ');
          if (lastSpace > charsPerLine * 0.6) { // More lenient for line breaks
            // Break at the space
            String beforeSpace = currentLine.substring(0, lastSpace);
            int charsToBacktrack = currentLine.length() - lastSpace - 1;
            textPos -= charsToBacktrack; // Backtrack to after the space
            currentLine = beforeSpace;
          }
        }
      }

      // Add line to page
      if (linesUsed > 0) page += "\n"; // Add newline between lines (except first)
      page += currentLine;
      linesUsed++;

      // Skip any spaces at the beginning of next line
      while (textPos < textLength && text.charAt(textPos) == ' ') {
        textPos++;
      }
    }

    pages[totalPages] = page;
    totalPages++;
  }
}

/**
 * displayTextWithPagination - diplay the provided text with pages
 *
 * @text: Provided text
 */

void displayTextWithPagination(String text)
{
  splitTextIntoPages(text);
  currentPage = 0;
  displayingText = true;
  displayCurrentPage();
}
