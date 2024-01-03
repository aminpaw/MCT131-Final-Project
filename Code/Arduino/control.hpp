#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <Arduino.h>
#include "sensors.hpp"
#include <Servo.h>

// Wheel radius in cm
#define WHEEL_RADIUS 3.5
// Distance between wheels in cm
#define BASE_WIDTH 19.5
float steerRatio = 0;
typedef struct MotorPins
{
    int rf;
    int rb;
    int lf;
    int lb;
    int speedRight;
    int speedLeft;
} MotorPins;

void turn(MotorPins motorPins, LinePin linePins);
void shootJoker(Servo servo);
void shootRiddler(Servo servo);

void motorControl(float linearSpeed, float angularSpeed, MotorPins motorPins)
{
    // Calculate left and right wheel speeds
    float leftSpeed = (linearSpeed - angularSpeed * BASE_WIDTH / 2) / WHEEL_RADIUS;
    float rightSpeed = (linearSpeed + angularSpeed * BASE_WIDTH / 2) / WHEEL_RADIUS;

    // map speeds to 0-255 range
    leftSpeed = map(leftSpeed, -100, 100, -255, 255);
    rightSpeed = map(rightSpeed, -100, 100, -255, 255);

    // change direction of wheels if speed is negative
    if (leftSpeed < 0)
    {
        digitalWrite(motorPins.lf, LOW);
        digitalWrite(motorPins.lb, HIGH);
        leftSpeed = -leftSpeed;
    }
    else
    {
        digitalWrite(motorPins.lf, HIGH);
        digitalWrite(motorPins.lb, LOW);
    }

    if (rightSpeed < 0)
    {
        digitalWrite(motorPins.rf, LOW);
        digitalWrite(motorPins.rb, HIGH);
        rightSpeed = -rightSpeed;
    }
    else
    {
        digitalWrite(motorPins.rf, HIGH);
        digitalWrite(motorPins.rb, LOW);
    }

    // Generate PWM signals at speed pins
    analogWrite(motorPins.speedLeft, leftSpeed);
    analogWrite(motorPins.speedRight, rightSpeed);
}

bool lineFollowingAlgorithm(float speed, LinePin linePins, MotorPins motorPins, bool lastTurnRight)
{
    LineReading lineReading = readLine(linePins);

    if (lineReading.L)
        steerRatio += 0.1;
    if (lineReading.LL)
        steerRatio += 0.15;
    if (lineReading.R)
        steerRatio += -0.1;
    if (lineReading.RR)
        steerRatio += -0.1;
    if (lineReading.M)
        steerRatio /= 4;

    if (lineReading.LL && lineReading.L && !lineReading.R && !lineReading.RR)
        lastTurnRight = false;
    if (!lineReading.LL && !lineReading.L && lineReading.R && lineReading.RR)
        lastTurnRight = true;

    if (lineReading.LL && lineReading.L && lineReading.R && lineReading.RR && !lineReading.M)
    {
        steerRatio = 0;
        // All sensors are on the line, junction detected
        if (lastTurnRight)
        {

            steerRatio += -0.25;
        }
        else
        {
            steerRatio += 0.25;
        }
    }
    motorControl(speed * (1 - abs(steerRatio)), speed * steerRatio, motorPins);

    if (lineReading.L || lineReading.R || lineReading.M || lineReading.LL || lineReading.RR)
        steerRatio = 0;

    return lastTurnRight;
}

void turn(MotorPins motorPins, LinePin linePins)
{
    LineReading readings;
    do
    {
        readings = readLine(linePins);
        motorControl(0, 75, motorPins);
    } while (readings.LL == 0 && readings.L == 0);
}

void shootJoker(Servo servo)
{
    for (int i = 90; i < 120; i++)
    {
        servo.write(i);
        delay(20);
    }

    for (int i = 120; i > 90; i--)
    {
        servo.write(i);
        delay(20);
    }
}

void shootRiddler(Servo servo)
{
    for (int i = 90; i > 60; i--)
    {
        servo.write(i);
        delay(20);
    }

    for (int i = 60; i < 90; i++)
    {
        servo.write(i);
        delay(20);
    }
}
#endif