/*
Controlling a servo position using a potentiometer (variable resistor)
by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

modified on 8 Nov 2013
by Scott Fitzgerald
http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

String data;
int pos = 0;   // variable to store the servo position
Servo myservo; // create servo object to control a servo

int val; // variable to read the value from the analog pin

void setup()
{
    myservo.attach(9); // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);
    myservo.write(0); // tell servo to go to position in variable 'pos'
}

void loop()
{
    if (Serial.available() > 0)
    {
        char msg = Serial.read();
        if (msg == 'o')
        {
            myservo.write(90); // tell servo to go to position in variable 'pos'
            delay(100);
            delay(5000);
            myservo.write(0);
        }
    }