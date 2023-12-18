#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
LinePin linePins = {6, 5, 4, 3, 2};
MotorPins motorPins = {10, 9, 7, 8, 11, 12};
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
}

void loop()
{
  // double distance = readUltra(ultraPins);

  // send line reading to Serial
  Serial.print("Line Reading: ");
  LineReading lineReading = readLine(linePins);
  // motorControl(linear, 0, motorPins);
  Serial.print(lineReading.LL);
  Serial.print(", ");
  Serial.print(lineReading.L);
  Serial.print(", ");
  Serial.print(lineReading.M);
  Serial.print(", ");
  Serial.print(lineReading.R);
  Serial.print(", ");
  Serial.println(lineReading.RR);
  // lastTurnRight = lineFollowingAlgorithm(speed, linePins, motorPins, lastTurnRight, false);
  motorControl(speed, 0, motorPins);
}
