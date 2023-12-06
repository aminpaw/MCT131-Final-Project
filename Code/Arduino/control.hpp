#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <Arduino.h>
#include "sensors.hpp"

// create parameters for motorControl function
// Wheel radius in cm
#define WHEEL_RADIUS 3.5
// Distance between wheels in cm
#define BASE_WIDTH 12.5

typedef struct MotorPins
{
    int rf;
    int rb;
    int lf;
    int lb;
    int speedRight;
    int speedLeft;
} MotorPins;
void turn(MotorPins motorPins);
void moveForward(float speed, MotorPins motorPins);
void turnLeft(float speed, MotorPins motorPins);
void turnRight(float speed, MotorPins motorPins);

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

    delay(10);

    // Stop motors
    analogWrite(motorPins.speedLeft, 0);
    analogWrite(motorPins.speedRight, 0);
}
void motorControl(float linearSpeed, float angularSpeed, MotorPins motorPins, float time)
{
    // Calculate left and right wheel speeds
    float leftSpeed = (linearSpeed - angularSpeed * BASE_WIDTH / 2) / WHEEL_RADIUS;
    float rightSpeed = (linearSpeed + angularSpeed * BASE_WIDTH / 2) / WHEEL_RADIUS;

    // map speeds to 150-255 range
    float leftSpeedMapped = map(abs(leftSpeed), 0, 100, 75, 255);
    float rightSpeedMapped = map(abs(rightSpeed), 0, 100, 75, 255);

    leftSpeed = leftSpeedMapped * leftSpeed / abs(leftSpeed);
    rightSpeed = rightSpeedMapped * rightSpeed / abs(rightSpeed);

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

    delay(time * 1000);

    // Stop motors
    analogWrite(motorPins.speedLeft, 0);
    analogWrite(motorPins.speedRight, 0);
}

bool lineFollowingAlgorithm(float speed, LinePin linePins, MotorPins motorPins, bool lastTurnRight, bool colorDetected)
{
    // Assuming sensor values are read into variables sensor1, sensor2, sensor3, sensor4, sensor5
    // Assuming color sensor value is read into variable colorSensor
    // Assuming motor control functions are turnSharpLeft(speed, motorPins), turnSharpRight(speed, motorPins), turnBack(speed, motorPins), moveForward(speed, motorPins)
    // Assuming a boolean variable lastTurnRight to keep track of the last turn direction

    // Read color sensor value
    LineReading lineReading = readLine(linePins);

    if (colorDetected)
    {
        do
        {
            turn(motorPins);
            lineReading = readLine(linePins);
        } while (~lineReading.LL && ~lineReading.L && lineReading.M && ~lineReading.R && ~lineReading.RR);
    }
    else
    {
        if (lineReading.LL && lineReading.L && lineReading.M && lineReading.R && lineReading.RR)
        {
            // All sensors are on the line, junction detected
            if (lastTurnRight)
            {
                do
                {
                    turnLeft(speed, motorPins);
                    lineReading = readLine(linePins);
                } while (~lineReading.LL && ~lineReading.L && lineReading.M && ~lineReading.R && ~lineReading.RR);

                lastTurnRight = false;
            }
            else
            {
                do
                {
                    turnRight(speed, motorPins);
                    lineReading = readLine(linePins);
                } while (~lineReading.LL && ~lineReading.L && lineReading.M && ~lineReading.R && ~lineReading.RR);
                lastTurnRight = true;
            }
        }
        else if (lineReading.RR && lineReading.M)
        {
            // Rightmost sensor detects the line
            turnRight(speed, motorPins);
            lastTurnRight = true;
        }
        else if (lineReading.LL && lineReading.M)
        {
            // Leftmost sensor detects the line
            turnLeft(speed, motorPins);
            lastTurnRight = false;
        }
        else if (lineReading.M)
        {
            // Middle sensor detects the line
            moveForward(speed, motorPins);
        }
        else if (lineReading.L)
        {
            // Left sensor detects the line
            turnLeft(speed, motorPins);
        }
        else if (lineReading.R)
        {
            // Right sensor detects the line
            turnRight(speed, motorPins);
        }
        else
        {
            // None of the sensors detect the line
            // Continue in the last known direction
            if (lastTurnRight)
            {
                turnRight(speed, motorPins);
            }
            else
            {
                turnLeft(speed, motorPins);
            }
        }
    }
    return lastTurnRight;
}

void turn(MotorPins motorPins)
{
    // Turn in place (linSpeed = 0, angSpeed = 75)
    motorControl(0, 75, motorPins);
}

void moveForward(float speed, MotorPins motorPins)
{
    // Move forward (linSpeed = 75, angSpeed = 0)
    motorControl(speed, 0, motorPins);
}

void turnLeft(float speed, MotorPins motorPins)
{
    // Turn left (linSpeed = 75, angSpeed = -75)
    motorControl(speed, -speed, motorPins);
}

void turnRight(float speed, MotorPins motorPins)
{
    // Turn right (linSpeed = 75, angSpeed = 75)
    motorControl(speed, speed, motorPins);
}

#endif
