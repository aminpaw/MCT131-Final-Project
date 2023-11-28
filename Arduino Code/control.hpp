#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <Arduino.h>
#include "sensors.hpp"

typedef struct MotorPins
{
    int enPin;
    int in1Pin;
    int in2Pin;
} MotorPins;

typedef enum Direction
{
    FORWARD,
    STOP,
    LEFT,
    RIGHT
} Direction;

class Controller
{
private:
    /* data */
public:
    Controller(int enL, int in1L, int in2L, int enR, int in1R, int in2R, float kp, float ki, float kd);
    void update(LineReading lineReading);
    void movePID();
    void moveStep(Direction direction);
    void outMotor(float longSpeed, float angSpeed);
    void stopMotor();

    // class for controlling a line follower robot with two motors and 5 sensors
    MotorPins leftMotor;
    MotorPins rightMotor;

    float kp;
    float ki;
    float kd;

    float error;
    float lastError;
    float integral;
    float derivative;

    float maxLongSpeed = 120;

    LineReading lineReading;
    int dirBias = 0; // 0 for forward, 1 for left, -1 for right

    float wheelRadius = 0.03;  // in meters
    float wheelDistance = 0.1; // in meters
};

Controller::Controller(int enL, int in1L, int in2L, int enR, int in1R, int in2R, float kp, float ki, float kd)
{
    leftMotor.enPin = enL;
    leftMotor.in1Pin = in1L;
    leftMotor.in2Pin = in2L;

    rightMotor.enPin = enR;
    rightMotor.in1Pin = in1R;
    rightMotor.in2Pin = in2R;

    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    pinMode(leftMotor.enPin, OUTPUT);
    pinMode(leftMotor.in1Pin, OUTPUT);
    pinMode(leftMotor.in2Pin, OUTPUT);

    pinMode(rightMotor.enPin, OUTPUT);
    pinMode(rightMotor.in1Pin, OUTPUT);
    pinMode(rightMotor.in2Pin, OUTPUT);

    analogWrite(leftMotor.enPin, 0);
    analogWrite(rightMotor.enPin, 0);
}

void Controller::update(LineReading lineReading)
{
    this->lineReading = lineReading;
    error = (lineReading.LL * 4 + lineReading.L * 2 + lineReading.M * -1 * this->dirBias + lineReading.R * -2 + lineReading.RR * -4) + this->dirBias;
    integral += error;
    derivative = error - lastError;
    lastError = error;
}

void Controller::movePID()
{
    float angSpeed = kp * error + ki * integral + kd * derivative;
    float longSpeed = maxLongSpeed;
    outMotor(longSpeed, angSpeed);
}

void Controller::moveStep()
{
    switch (lineReading)
    {
    case LineReading::LL:
        outMotor(maxLongSpeed, -maxLongSpeed);
        break;
    case LineReading::L:
        outMotor(maxLongSpeed, -maxLongSpeed / 2);
        break;
    case LineReading::M:
        outMotor(maxLongSpeed, 0);
        break;
    case LineReading::R:
        outMotor(maxLongSpeed / 2, maxLongSpeed);
        break;
    case LineReading::RR:
        outMotor(-maxLongSpeed, maxLongSpeed);
        break;
    default:
        outMotor(0, 0);
        break;
    }
}

void Controller::stopMotor()
{
    analogWrite(leftMotor.enPin, 0);
    analogWrite(rightMotor.enPin, 0);
}

void Controller::outMotor(float longSpeed, float angSpeed)
{
    // change the longSpeed and angSpeed to the motor speed using differential drive
    float leftSpeed = longSpeed - angSpeed * wheelDistance / 2;
    float rightSpeed = longSpeed + angSpeed * wheelDistance / 2;

    // change the motor speed to the PWM value
    int leftPWM = map(leftSpeed, -maxLongSpeed, maxLongSpeed, -255, 255);
    int rightPWM = map(rightSpeed, -maxLongSpeed, maxLongSpeed, -255, 255);

    // set the motor speed
    if (leftPWM > 0)
    {
        analogWrite(leftMotor.in1Pin, leftPWM);
        analogWrite(leftMotor.in2Pin, 0);
    }
    else
    {
        analogWrite(leftMotor.in1Pin, 0);
        analogWrite(leftMotor.in2Pin, -leftPWM);
    }

    if (rightPWM > 0)
    {
        analogWrite(rightMotor.in1Pin, rightPWM);
        analogWrite(rightMotor.in2Pin, 0);
    }
    else
    {
        analogWrite(rightMotor.in1Pin, 0);
        analogWrite(rightMotor.in2Pin, -rightPWM);
    }
}

#endif // CONTROL_HPP