#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"

// Create our simplified control object
ApplicationFunctionSet Application_FunctionSet;

void setup()
{
  // Enable the watchdog at 2 seconds, so if the loop locks,
  // the chip will reset.
  wdt_enable(WDTO_2S);

  // Initialize everything
  Application_FunctionSet.init();
}

void loop()
{
  // Must periodically reset the watchdog
  wdt_reset();

  // Continuously read IR and drive motors based on IR input
  Application_FunctionSet.IRControlLoop();

  // You can do other tasks here if you like...
  // e.g. check a sensor, do some debug prints, etc.
}
