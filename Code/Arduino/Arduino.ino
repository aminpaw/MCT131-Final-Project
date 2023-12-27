#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
#include <Servo.h>
#include "TCS3200C.h"

LinePin linePins = {A5, A4, A3, A2, A1};
UltraPin ultraPins = {6, 7};
ColorPin colorPins = {4, 3, 2};
MotorPins motorPins = {6, 5, 8, 7, 9, 10};

TCS3200 CS = TCS3200(colorPins.S2, colorPins.S3, colorPins.OUT, 0);

int colorID;
Servo trigger;

void colorChanged(int color)
{
  if (CS.onChangeColor())
  {
    Serial.print("Color is: ");
    Serial.println(CS._ct[CS.readLastColorID()].name);
    colorID = CS.readLastColorID();
  }
  else
  {
    colorID = CS.readLastColorID();
  }
}

Ticker CSTicker(colorChanged, 10, 0, MILLIS);

// Test Parameters
bool lastTurnRight = 0;
float speed = 90;

void setup()
{
  // trigger.attach(11);
  // trigger.write(90);
  //// Serial.begin(9600);
  // CS.setTicker(CSTicker);
  // CS.begin();
  // CS.nSamples(40);
  // CS.setRefreshTime(50);
  // CS.loadBW();
  // CS.loadCT();
  for (int i = 6; i <= 13; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void loop()
{
  // CS.update();
  // if (CS.readLastColorID() == 4)
  //   turn(motorPins, linePins);
  lastTurnRight = lineFollowingAlgorithm(speed, linePins, motorPins, lastTurnRight);
  // motorControl(100, 0, motorPins);
}