#include "HCSR04.h"

void setup()
{
  Serial.begin(9600);
}

void loop()
{

  double *reading = HCSR04.measureDistanceMm();
  delay(100);
  if (reading[0] < 30)
  {
    Serial.print('o');
    delay(100);
  }
}