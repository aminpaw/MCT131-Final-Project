// test ultrasonic sensor and hc05

#include <Arduino.h>
#include "sensors.hpp"

UltraPin ultraPin = {13, 12};

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    Serial.println(readUltra(ultraPin));
    delay(1000);
}