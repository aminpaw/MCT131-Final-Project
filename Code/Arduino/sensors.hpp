#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <HCSR04.h>
#include <tcs3200.h>

typedef enum Color
{
    RED,
    YELLOW,
    GREEN,
    BLACK
} Color;

typedef struct LineReading
{
    bool LL;
    bool L;
    bool M;
    bool R;
    bool RR;
} LineReading;

typedef struct LinePin
{
    int LLPin;
    int LPin;
    int MPin;
    int RPin;
    int RRPin;
} LinePin;

typedef struct UltraPin
{
    int echoPin;
    int trigPin;
} UltraPin;

typedef struct ColorPin
{
    int S3;
    int S2;
    int OUT;
} ColorPin;

LineReading readLine(LinePin pins);
void readColor(ColorPin pins);
double readUltra(UltraPin pins);

LineReading readLine(LinePin pins)
{
    LineReading reading = {digitalRead(pins.LLPin),
                           digitalRead(pins.LPin),
                           digitalRead(pins.MPin),
                           digitalRead(pins.RPin),
                           digitalRead(pins.RRPin)};

    return reading;
}

double readUltra(UltraPin pins)
{
    HCSR04.begin(pins.trigPin, pins.echoPin);
    double *reading = HCSR04.measureDistanceMm();
    return reading[0];
}

#endif
