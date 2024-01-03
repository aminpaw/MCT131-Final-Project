// test ultrasonic sensor and hc05

#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    analogWrite(5, 255);
}