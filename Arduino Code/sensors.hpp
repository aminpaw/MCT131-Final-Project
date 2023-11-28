#include "Arduino.h"

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

class Sensors
{

public:
    Sensors(int llPin, int lPin, int mPin, int rPin, int rrPin, int colorPin);
    LineReading readLine();
    Color readColor();
    float rgb2hue(float r, float g, float b);

    LinePin lineSensorPin;
    int colorSensorPin;
    LineReading lineReading = {0, 0, 0, 0, 0};
    Color colorReading = BLACK;
};

Sensors::Sensors(int llPin, int lPin, int mPin, int rPin, int rrPin, int colorPin)
{
    this->lineSensorPin.LLPin = llPin;
    this->lineSensorPin.LPin = lPin;
    this->lineSensorPin.MPin = mPin;
    this->lineSensorPin.RPin = rPin;
    this->lineSensorPin.RRPin = rrPin;
    this->colorSensorPin = colorPin;
}

LineReading Sensors::readLine()
{
    this->lineReading.LL = digitalRead(this->lineSensorPin.LLPin);
    this->lineReading.L = digitalRead(this->lineSensorPin.LPin);
    this->lineReading.M = digitalRead(this->lineSensorPin.MPin);
    this->lineReading.R = digitalRead(this->lineSensorPin.RPin);
    this->lineReading.RR = digitalRead(this->lineSensorPin.RRPin);

    return this->lineReading;
}

Color Sensors::colorReading()
{
    float r, g, b;
    // convert to hue easier to get color from
    float hue = rgb2hue(r, g, b);

    if (r >= 240 && g >= 240 && b >= 240)
        this->colorReading = BLACK;
    else if (hue <= 22 || hue >= 350)
        this->colorReading = RED;
    else if (/* condition */)
        this->colorReading = YELLOW;
    else if ()
        this->colorReading = GREEN;
    else
        this->colorReading = BLACK;

    return this->colorReading;
}