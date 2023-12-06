#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
UltraPin ultraPins = {3, 2};
LinePin linePins = {4, 5, 6, 7, 8};
MotorPins motorpins = {8, 7, 9, 10, 12, 11};
// Test Parameters
float linear = 50;
float angular = 5;

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
  Serial1.begin(9600);
}

void loop()
{
  // double distance = readUltra(ultraPins);
  LineReading line = readLine(linePins);

  Serial.println("Line Readings: ");
  Serial.println(line.LL);
  Serial.println(line.L);
  Serial.println(line.M);
  Serial.println(line.R);
  Serial.println(line.RR);
  motorTest(linear, angular, motorpins);
  /*
    if (distance > 150 || distance < 0)
    {
      Serial1.println("C");
    }
    else
    {
      Serial1.println("O");
    }
  */
  delay(1000);
}
