#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
#include <Servo.h>
#include "TCS3200C.h"

LinePin linePins = {A5, A4, A3, A2, A1};
UltraPin ultraPins = {13, 12};
ColorPin colorPins = {2, 3, 4};
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

Ticker CSTicker(colorChanged, 30, 0, MILLIS);

// Test Parameters
bool lastTurnRight = 0;
float speed = 90;

void setup()
{
  trigger.attach(11);
  trigger.write(90);
  Serial.begin(9600);
  CS.setTicker(CSTicker);
  CS.begin();
  CS.nSamples(40);
  CS.setRefreshTime(50);
  CS.loadBW();
  CS.loadCT();
  for (int i = 6; i <= 13; i++)
  {
    pinMode(i, OUTPUT);
  }
}

void loop()
{
  CS.update();
  double distance = readUltra(ultraPins);
  if (CS.readLastColorID() == 0)
  {
    Serial.println("WHITE");
  }
  else if (CS.readLastColorID() == 1)
  {
    Serial.println("BLACK");
  }
  else if (CS.readLastColorID() == 2)
  {
    Serial.println("YELLOW");
  }
  else if (CS.readLastColorID() == 3)
  {
    Serial.println("RED");
    for (int i = 90; i >= 60; i--)
    {
      trigger.write(i);
      delay(20);
    }
    for (int i = 60; i <= 90; i++)
    {
      trigger.write(i);
      delay(20);
    }
  }
  else if (CS.readLastColorID() == 4)
  {
    Serial.println("GREEN");
    for (int i = 90; i <= 120; i++)
    {
      trigger.write(i);
      delay(20);
    }
    for (int i = 120; i >= 90; i--)
    {
      trigger.write(i);
      delay(20);
    }
  }
  else
  {
    Serial.println("NONE");
  }
}
