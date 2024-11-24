#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

/*
Description: controling the motors' speed and direction based on the IR sensors readings
Arguments:
    motorpin (int array): the motors' pins specification in the format of {Right1, Right2, RightEnable, Left1, Left2, LeftEnable}
    rightMotorSpeed, leftMotorSpeed: speed of two motors
    rightIR, leftIR: pins of two IR sensors 
*/
void rotateMotor(int motorPin[6], int rightMotorSpeed, int leftMotorSpeed, int rightIR, int leftIR);

#endif