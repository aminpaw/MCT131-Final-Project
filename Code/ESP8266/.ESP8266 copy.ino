#include "ESP_MICRO.h" // importing our library

String data;
void setup()
{
  Serial.begin(9600);
  // start("WE_1C415D", "cdce9ca7"); // Wifi details connect to
}

void loop()
{
  digitalWrite(2, HIGH);
  delay(500);
  // waitUntilNewReq(); // Waits until a new request from python come
  if (Serial.available() > 0)
  {
    data = Serial.read();
    if (data == "O")
    {
      digitalWrite(1, LOW);
    }
    else
    {
      digitalWrite(1, HIGH);
    }
  }
  while (Serial.available() > 0)
  {
    Serial.read();
  }
  // convert data to int
  // returnThisStr(data); // Returns the data to python
  digitalWrite(2, LOW);
  delay(500);
}
