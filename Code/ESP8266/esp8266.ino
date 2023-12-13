#include "ESP_MICRO.h" // importing our library

String data;
void setup()
{
  Serial.begin(9600);
  start("ENGJOY", "engjoy.coworking"); // Wifi details connect to
}

void loop()
{
  digitalWrite(1, HIGH);
  waitUntilNewReq(); // Waits until a new request from python come
  if (Serial.available() > 0)
  {
    data = Serial.readStringUntil('\n');
  }
  while (Serial.available() > 0)
  {
    Serial.read();
  }
  if (data == NULL)
    data = "";
  // convert data to int
  returnThisStr(data); // Returns the data to python
  digitalWrite(1, LOW);
}
