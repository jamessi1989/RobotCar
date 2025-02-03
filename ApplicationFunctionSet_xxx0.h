#ifndef APPLICATIONFUNCTIONSET_XXX0_H
#define APPLICATIONFUNCTIONSET_XXX0_H

#include "Arduino.h"
#include "DeviceDriverSet_xxx0.h"

/**
 * A simplified class that:
 *   - Reads IR codes
 *   - Moves the motors while a button is held
 *   - Does a "dance" on a separate button press
 *   - Gives "forward" priority over "backward" if both are held
 */
class ApplicationFunctionSet
{
public:
    void init();           // call in setup()
    void IRControlLoop();  // call in loop()

private:
    // Move, stop, or dance
    void moveCar(uint8_t direction, uint8_t speed);
    void stopCar();
    void doDance();

    // Inactivity time for IR in ms
    const unsigned long IR_INACTIVITY_TIMEOUT = 200;

    // Track the last code/time to detect "button release"
    unsigned long lastIRTime = 0;
    uint8_t       lastIRCode = 0;

    // For priority logic (if needed)
    // We'll store the current "active" direction so we can ignore
    // contradictory commands (e.g. backward while in forward).
    uint8_t currentActiveDirection = 0; // 1=fwd,2=back,3=left,4=right,0=stop

    // Our hardware drivers
    DeviceDriverSet_IRrecv  AppIRrecv;
    DeviceDriverSet_Motor   AppMotor;
};

#endif
  