#include "afm_hmi.h"

/**
 * configure_scan_paramters - Configure scan parameters based on  quick
 *                            or advanced scan
 */
bool configure_scan_parameters(void)
{
  String text = F(
    "Configure Scan Parameters\n"
    "-Press 1 for  quick scan, 2 for an advanced scan\n"
  );
  displayTextWithPagination(text);

  switch (waitForKeypadInput())
  {
    case '1':
      return quickScan();

    case '2':
      return advancedScan();

    default:
      return false;
  }

  return true;
}

/**
 * quickScan - Scan area by setting number of images
 *
 * Return: true when done unless operation Cancelled
 */
bool quickScan(void)
{
  int noOfImages = getValidatedInput(F(
    "Scan Parameters\n"
    "-No of Images: \n"
  ));
  if (noOfImages == -1) return false; 


  ScanConfig scan_config = setDefaultConfig();
  ScanParameters scan_parameters = setDefaultParameters();
  scan_parameters.no_of_images = noOfImages;
  
  // Update gain Parameter
  gain_paramter = scan_parameters.gain;

  String summary = 
    "Config summary\n"
//    "Head Mode: " + String(scan_config.head_mode) + "\n"
//    "Scan size: " + String(scan_parameters.scan_size) + "µm\n"
//    "Scan rate:" + String(scan_parameters.scan_rate) + "Hz\n"
//    "Data Points: " + String(scan_parameters.no_data_points) + "\n"
//    "Images: " + String(scan_parameters.no_of_images) + "\n"
    "Press any key to continue\n"
    ;
  displayTextWithPagination(summary);
  waitForKeypadInput();

  return true;
}

/**
 * advancedScan - Scan area by setting all parameters
 */
bool advancedScan(void)
{
  return false;
}

