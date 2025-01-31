#ifndef APPLICATIONFUNCTIONSET_XXX0_H
#define APPLICATIONFUNCTIONSET_XXX0_H

#include "DeviceDriverSet_xxx0.h"

/**
 * Simple class that initializes the motor and IR drivers
 * and provides a function to read IR commands and drive the car.
 */
class ApplicationFunctionSet
{
public:
    // Initialize all drivers (motors, IR, etc.)
    void init();

    // Continuously read IR and drive accordingly
    void IRControlLoop();

private:
    // Helper: sets both motors according to a "direction" code
    // For simplicity, let's say:
    //   1 -> FORWARD
    //   2 -> BACKWARD
    //   3 -> LEFT
    //   4 -> RIGHT
    //   anything else -> STOP
    void moveCar(uint8_t direction, uint8_t speed = 150);
};

#endif
