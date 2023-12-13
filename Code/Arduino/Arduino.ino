#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
LinePin linePins = {6, 5, 4, 3, 2};
MotorPins motorPins = {7, 8, 9, 10, 11, 12};
// Test Parameters
float linear = 50;
float angular = 5;
bool lastTurnRight = 0;
float speed = 100;

void motorTest(float linear, float angular, MotorPins motorpins)
{
  motorControl(linear, 0, motorpins, 5);

  motorControl(-linear, 0, motorpins, 5);

  motorControl(0, angular, motorpins, 5);

  motorControl(0, -angular, motorpins, 5);

  motorControl(linear, angular, motorpins, 5);

  motorControl(linear, -angular, motorpins, 5);
}
void setup()
{
  Serial.begin(9600);
  for (int i = 2; i <= 6; i++){
    pinMode(i,INPUT);
  }
}

void loop()
{
  // double distance = readUltra(ultraPins);
  //lastTurnRight = lineFollowingAlgorithm(speed, linePins, motorPins, lastTurnRight, false);

  // send line reading to Serial
  Serial.print("Line Reading: ");
  LineReading lineReading = readLine(linePins);
  Serial.print(lineReading.LL);
  Serial.print(", ");
  Serial.print(lineReading.L);
  Serial.print(", ");
  Serial.print(lineReading.M);
  Serial.print(", ");
  Serial.print(lineReading.R);
  Serial.print(", ");
  Serial.println(lineReading.RR);
}
