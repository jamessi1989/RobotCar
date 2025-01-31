#include "ApplicationFunctionSet_xxx0.h"
#include "Arduino.h"

// Keep any other includes you want, e.g. QMI8658C, ArduinoJson, etc.
// #include "QMI8658C.h"
// #include "ArduinoJson-v6.11.1.h"

// Global driver objects
DeviceDriverSet_IRrecv  AppIRrecv;
DeviceDriverSet_Motor   AppMotor;

// ---------------------------------------------------------------------------
// Initialize function
// ---------------------------------------------------------------------------
void ApplicationFunctionSet::init()
{
  // Initialize serial if needed
  Serial.begin(9600);

  // Initialize your IR receiver
  AppIRrecv.DeviceDriverSet_IRrecv_Init();

  // Initialize your motor driver
  AppMotor.DeviceDriverSet_Motor_Init();

  // If you want to initialize anything else (voltage sensor, servo, LED, etc.)
  // you can still do it here if you wish.
}

// ---------------------------------------------------------------------------
// Simplified motor-movement helper
// ---------------------------------------------------------------------------
void ApplicationFunctionSet::moveCar(uint8_t direction, uint8_t speed)
{
  // For clarity:
  //   direction_void = stops the motor
  //   direction_just = forward
  //   direction_back = reverse
  // 
  // We'll interpret IR codes: 
  //   1 -> FORWARD
  //   2 -> BACKWARD
  //   3 -> LEFT
  //   4 -> RIGHT
  //   anything else -> STOP
  switch (direction) {
    case 1:  // Forward
      AppMotor.DeviceDriverSet_Motor_control(direction_just, speed,
                                             direction_just, speed,
                                             control_enable);
      break;

    case 2:  // Backward
      AppMotor.DeviceDriverSet_Motor_control(direction_back, speed,
                                             direction_back, speed,
                                             control_enable);
      break;

    case 3:  // Left
      // One easy way is left wheel reverse, right wheel forward
      // or you can do left wheel stop, right wheel forward, etc.
      AppMotor.DeviceDriverSet_Motor_control(direction_back,  speed,
                                             direction_just,  speed,
                                             control_enable);
      break;

    case 4:  // Right
      AppMotor.DeviceDriverSet_Motor_control(direction_just,  speed,
                                             direction_back,  speed,
                                             control_enable);
      break;

    default: // Stop
      AppMotor.DeviceDriverSet_Motor_control(direction_void, 0,
                                             direction_void, 0,
                                             control_enable);
      break;
  }
}

// ---------------------------------------------------------------------------
// Main IR-reading loop
// ---------------------------------------------------------------------------
void ApplicationFunctionSet::IRControlLoop()
{
  // If there's a valid IR code, read it:
  uint8_t IRrecv_button;
  if (AppIRrecv.DeviceDriverSet_IRrecv_Get(&IRrecv_button)) 
  {
    // Just map the IR code to movement:
    switch (IRrecv_button) {
      case 1: // forward
      case 2: // backward
      case 3: // left
      case 4: // right
        moveCar(IRrecv_button, 150);
        break;
      
      default:
        // On any other button, we can stop:
        moveCar(0, 0);
        break;
    }
    // Debug printing, if you like:
    Serial.print("IR Code: ");
    Serial.println(IRrecv_button);
  }
}
