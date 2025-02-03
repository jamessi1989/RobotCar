#include "ApplicationFunctionSet_xxx0.h"

// Define the IR receiver pin
#define IR_RECV_PIN 11

#ifndef IR_INACTIVITY_TIMEOUT
  #define IR_INACTIVITY_TIMEOUT 200  // Try 200ms for snappier stop; adjust as desired.
#endif

// ---------------------------------------------------------
// init() - Initialize IR + motor driver
// ---------------------------------------------------------
void ApplicationFunctionSet::init()
{
    Serial.begin(9600);

    // IR
    AppIRrecv.DeviceDriverSet_IRrecv_Init();
    pinMode(IR_RECV_PIN, INPUT);

    // Motors
    AppMotor.DeviceDriverSet_Motor_Init();

    Serial.println("Robot initialization complete.");
}

// ---------------------------------------------------------
// stopCar() - Helper to stop both motors
// ---------------------------------------------------------
void ApplicationFunctionSet::stopCar()
{
    AppMotor.DeviceDriverSet_Motor_control(
        direction_void, 0,
        direction_void, 0,
        control_enable
    );
}

// ---------------------------------------------------------
// moveCar() - Map codes to movement:
//    1 => Forward
//    2 => Backward
//    3 => Spin Left
//    4 => Spin Right
//    anything else => stop
// ---------------------------------------------------------
void ApplicationFunctionSet::moveCar(uint8_t direction, uint8_t speed)
{
    switch (direction)
    {
        case 1: // Forward
            AppMotor.DeviceDriverSet_Motor_control(
                direction_just, speed,
                direction_just, speed,
                control_enable
            );
            break;

        case 2: // Backward
            AppMotor.DeviceDriverSet_Motor_control(
                direction_back, speed,
                direction_back, speed,
                control_enable
            );
            break;

        case 3: // Spin Left
            AppMotor.DeviceDriverSet_Motor_control(
                direction_just, speed,
                direction_back, speed,
                control_enable
            );
            break;

        case 4: // Spin Right
            AppMotor.DeviceDriverSet_Motor_control(
                direction_back, speed,
                direction_just, speed,
                control_enable
            );
            break;

        default:
            stopCar();
            break;
    }
}

// ---------------------------------------------------------
// doDance() - A fun multi-step routine.
// ---------------------------------------------------------
void ApplicationFunctionSet::doDance()
{
    Serial.println("Dance routine start...");

    // Spin left 700ms
    AppMotor.DeviceDriverSet_Motor_control(
        direction_just, 180,
        direction_back, 180,
        control_enable
    );
    delay(700);

    // Spin right 700ms
    AppMotor.DeviceDriverSet_Motor_control(
        direction_back, 180,
        direction_just, 180,
        control_enable
    );
    delay(700);

    // Forward 500ms
    AppMotor.DeviceDriverSet_Motor_control(
        direction_just, 180,
        direction_just, 180,
        control_enable
    );
    delay(500);

    // Backward 500ms
    AppMotor.DeviceDriverSet_Motor_control(
        direction_back, 180,
        direction_back, 180,
        control_enable
    );
    delay(500);

    // Spin left 1 second
    AppMotor.DeviceDriverSet_Motor_control(
        direction_just, 200,
        direction_back, 200,
        control_enable
    );
    delay(1000);

    // Spin right 1 second
    AppMotor.DeviceDriverSet_Motor_control(
        direction_back, 200,
        direction_just, 200,
        control_enable
    );
    delay(1000);

    stopCar();
    Serial.println("Dance complete!");
}

// ---------------------------------------------------------
// IRControlLoop()
//   - Decodes IR codes
//   - Keeps driving as long as "movement" codes continue
//   - Stops if no new code arrives within IR_INACTIVITY_TIMEOUT
//   - Code 5 triggers doDance()
// ---------------------------------------------------------
void ApplicationFunctionSet::IRControlLoop()
{
    uint8_t currentIRCode = 0;
    static unsigned long lastIRTime = 0;  // Track when we got the last valid IR command
    static uint8_t lastIRCodeSaved = 0;   // Track the last non-zero IR code

    // Try to get a new IR code
   bool gotCode = AppIRrecv.DeviceDriverSet_IRrecv_Get(&currentIRCode);

    if (gotCode) {
      Serial.print("Raw IR code: ");
      Serial.println(currentIRCode, HEX);  // Print in hex for clarity

      // If the code is a repeat code
      if (currentIRCode == 0xFF || currentIRCode == 0xFFFFFFFF /* or 0 */) {
        // Keep using lastIRCodeSaved, but update time so it doesn't time out:
        Serial.println("Repeat code detected");
        currentIRCode = lastIRCodeSaved; 
      } else {
        // If it's a brand-new code, store it for future 'repeat'
        lastIRCodeSaved = currentIRCode;
      }

      // Always update lastIRTime if we got *any* code
      lastIRTime = millis();
    }


    // If we have not received any code in the last X ms, stop
    if ((millis() - lastIRTime) > IR_INACTIVITY_TIMEOUT)
    {
        // No recent IR signals -> Stop the car
        stopCar();
        // Also clear the last saved code if you want it to require a new press
        lastIRCodeSaved = 0;
        return;
    }

    // Otherwise, if we do have a valid last code, handle it
    if (lastIRCodeSaved != 0)
    {
        if (lastIRCodeSaved == 5)
        {
            // Special case: dance routine
            doDance();
            stopCar();
            // Clear the code so we don't keep dancing
            lastIRCodeSaved = 0;
        }
        else
        {
            // Move according to the last known code
            moveCar(lastIRCodeSaved, 150);
        }
    }
    else
    {
        // If there's no valid code at all, just stop
        stopCar();
    }
}
