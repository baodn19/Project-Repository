#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

/*
Description: controling the motors' speed and direction based on the IR sensors readings
Arguments:
    motorpin (int array): the motors' pins specification in the format of {Right1, Right2, RightEnable, Left1, Left2, LeftEnable}
    speed: speed of two motors
    rightIR, leftIR: pins of two IR sensors 
*/
void rotateMotor(int motorPin[6], int speed, uint8_t rightIR, uint8_t leftIR);

#endif