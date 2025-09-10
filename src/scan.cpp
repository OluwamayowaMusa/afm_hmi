#include "afm_hmi.h"

/**
 * setDefalutConfig - Set defalut scan configurations
 *
 * Return: Scan configurations
*/
ScanConfig setDefaultConfig(void)
{
  ScanConfig scan_config = {
    .head_type = F("AFM-LFM"),
    .scanner = 10,
    .head_mode = F("AFM"),
    .beam_bounce_cantilever = F("UL06B"),
    .electrochemistry = false,
    .voltage_mode = F("HI"),
  };

  return scan_config;
}


/**
 * setDefaultParameters - Set Default scan parameters 
 *
 * Return: Scan parameters
*/

ScanParameters setDefaultParameters(void)
{
  ScanParameters scan_parameters = {
    .scan_size = 5,
    .scan_rate = 1,
    .scan_direction = 'X',
    .x_slope = 0,
    .y_slope = 0,
    .gain = 0.01,
    .set_point = 0.4,
    .no_data_points = 256,
    .no_of_images = 1,
  };

  return scan_parameters;
}

/**
 * take_scan - Start taking the scan
*/
bool take_scan(void)
{
  setupDisplay();
  display.println("Taking Scan");
  display.println("Updating Gain parameter based on oscillations");
  display.display();
  
  unsigned long current_time = millis();

  char buffer[20];
  while (millis() - current_time <= SCAN_TIME) 
  {
    sprintf(buffer, "Gain: %d", 3);
    display.fillRect(0, 24, 128, 16, SSD1306_BLACK);
    display.setCursor(0, 24);
    display.println(buffer);
    display.display();

    delay(100);
  }

  return true;
}
