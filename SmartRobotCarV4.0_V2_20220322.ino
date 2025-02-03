#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"

// Create our control object
ApplicationFunctionSet Application_FunctionSet;

void setup()
{
  // Enable the watchdog timer (2 seconds) in case of a loop hang.
  wdt_enable(WDTO_2S);
  
  // Initialize everything (IR, motors, etc.)
  Application_FunctionSet.init();
}

void loop()
{
  // Reset the watchdog periodically.
  wdt_reset();

  // Continuously check the IR and update motor control.
  Application_FunctionSet.IRControlLoop();

  // (Other tasks can be added here if needed.)
}
