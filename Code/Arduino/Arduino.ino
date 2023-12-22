#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
#include "TCS3200C.h"

#define TCS3200_S2 25
#define TCS3200_S3 24
#define TCS3200_LED 21
#define TCS3200_OUT 27
#define TCS3200_S0 23
#define TCS3200_S1 22

int colorID;

TCS3200 CS = TCS3200(TCS3200_S2, TCS3200_S3, TCS3200_OUT, TCS3200_S0, TCS3200_S1,
                     TCS3200_LED, 0);

void colorChanged(int color)
{
  if (CS.onChangeColor())
  {
    // Serial.print("Color is: ");
    // Serial.println(CS._ct[CS.readLastColorID()].name);
    colorID = CS.readLastColorID();
  }
  else
  {
    colorID = CS.readLastColorID();
  }
}

Ticker CSTicker(colorChanged, CS.refreshTime, 0, MILLIS);

LinePin linePins = {6, 5, 4, 3, 2};
MotorPins motorPins = {8, 7, 10, 9, 11, 12};
// Test Parameters
float linear = 100;
float angular = 5;
bool lastTurnRight = 0;
float speed = 80;

void motorTest(float linear, float angular, MotorPins motorpins)
{
  motorControl(linear, 0, motorpins);
  delay(1000);
  motorControl(-linear, 0, motorpins);
  delay(1000);
  motorControl(0, angular, motorpins);
  delay(1000);
  motorControl(0, -angular, motorpins);
  delay(1000);
  motorControl(linear, angular, motorpins);
  delay(1000);
  motorControl(linear, -angular, motorpins);
}
void setup()
{
  Serial.begin(9600);
  for (int i = 2; i <= 6; i++)
  {
    pinMode(i, INPUT);
  }
  pinMode(13, OUTPUT);
  CS.setTicker(CSTicker);
  CS.begin();

  CS.setFrequency(0);
  CS.nSamples(40);
  CS.setRefreshTime(100);
  CS.loadBW();
  CS.loadCT();
}

void loop()
{
  CS.update();
  LineReading lineReading = readLine(linePins);
  if (colorID == 0 || colorID == 1 || lineReading.M == 1)
  {
    digitalWrite(13, HIGH);
    Serial.println(CS._ct[colorID].name);
    lastTurnRight = lineFollowingAlgorithm(speed, linePins, motorPins, lastTurnRight, false);
  }
  else
  {
    digitalWrite(13, LOW);
    Serial.println(CS._ct[colorID].name);
    motorControl(0, 0, motorPins);
  }
}
