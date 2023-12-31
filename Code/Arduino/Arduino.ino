#include "sensors.hpp"
#include "control.hpp"
#include <Arduino.h>
#include <Servo.h>
#include "TCS3200C.h"

LinePin linePins = {A1, A2, A3, A4, A5};
UltraPin ultraPins = {13, 12};
ColorPin colorPins = {2, 3, 4};
MotorPins motorPins = {10, 9, 7, 8, 5, 6};
TCS3200 CS = TCS3200(colorPins.S2, colorPins.S3, colorPins.OUT, 0);

int colorID;
Servo trigger;
bool lastTurnRight = 0;
float speed = 90;

void colorChanged(int color)
{
  // read color
  CS.onChangeColor();
  colorID = CS.readLastColorID();
}

Ticker CSTicker(colorChanged, 10, 0, MILLIS);

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
  pinMode(ultraPins.trigPin, OUTPUT);
  pinMode(ultraPins.echoPin, INPUT);
  pinMode(motorPins.rf, OUTPUT);
  pinMode(motorPins.rb, OUTPUT);
  pinMode(motorPins.lf, OUTPUT);
  pinMode(motorPins.lb, OUTPUT);
  pinMode(motorPins.speedLeft, OUTPUT);
  pinMode(motorPins.speedRight, OUTPUT);
}

void loop()
{
  CS.update();
  if (CS.readLastColorID() == 0 || CS.readLastColorID() == 1) // 0 is black, 1 is white
  {
    lastTurnRight = lineFollowingAlgorithm(speed, linePins, motorPins, lastTurnRight);
  }
  else if (CS.readLastColorID() == 2) // 2 is yellow
  {
    if (speed != 100)
    {
      speed = 100;
      motorControl(speed, 0, motorPins);
      delay(100);
    }
    if (speed == 100)
    {
      speed = 80;
      motorControl(speed, 0, motorPins);
      delay(100);
    }
  }
  else if (CS.readLastColorID() == 3) // 3 is red
  {
    shootJoker(trigger);
  }
  else if (CS.readLastColorID() == 4) // 4 is green
  {
    shootRiddler(trigger);
  }
}