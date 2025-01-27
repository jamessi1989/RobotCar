/*
 * @Author: ELEGOO
 * @Date: 2019-10-22 11:59:09
 * @LastEditTime: 2021-05-25 13:57:10
 * @LastEditors: Changhua
 * @Description: Smart Robot Car V4.0
 */

#include <avr/wdt.h>
#include "ApplicationFunctionSet_xxx0.h"

// 1) Define the pin for your buzzer
#define BUZZER_PIN 9 

void setup()
{
  // 2) Initialize buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);

  // 3) Optional: Play a beep at startup for 500 ms
  tone(BUZZER_PIN, 1000, 500); // beep at 1000 Hz, lasting 500 ms

  // 4) Proceed with the normal robot init
  Application_FunctionSet.ApplicationFunctionSet_Init();
  wdt_enable(WDTO_2S);
}

void loop()
{
  // Required to prevent the watchdog from resetting the MCU
  wdt_reset();

  // Normal RobotCar tasks
  Application_FunctionSet.ApplicationFunctionSet_SensorDataUpdate();
  Application_FunctionSet.ApplicationFunctionSet_KeyCommand();
  Application_FunctionSet.ApplicationFunctionSet_RGB();
  Application_FunctionSet.ApplicationFunctionSet_Follow();
  Application_FunctionSet.ApplicationFunctionSet_Obstacle();
  Application_FunctionSet.ApplicationFunctionSet_Tracking();
  Application_FunctionSet.ApplicationFunctionSet_Rocker();
  Application_FunctionSet.ApplicationFunctionSet_Standby();
  Application_FunctionSet.ApplicationFunctionSet_IRrecv();
  Application_FunctionSet.ApplicationFunctionSet_SerialPortDataAnalysis();

  Application_FunctionSet.CMD_ServoControl_xxx0();
  Application_FunctionSet.CMD_MotorControl_xxx0();
  //Application_FunctionSet.CMD_CarControlTimeLimit_xxx0();
  Application_FunctionSet.CMD_CarControlNoTimeLimit_xxx0();
  Application_FunctionSet.CMD_MotorControlSpeed_xxx0();
  //Application_FunctionSet.CMD_LightingControlTimeLimit_xxx0();
  Application_FunctionSet.CMD_LightingControlNoTimeLimit_xxx0();
  Application_FunctionSet.CMD_ClearAllFunctions_xxx0();
}
