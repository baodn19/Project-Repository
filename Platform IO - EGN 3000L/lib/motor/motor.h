#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


class Motor {
public:
    // Attributes
    int motorPin[6];
    uint8_t rightIR;
    uint8_t leftIR;
    
    Motor(int (&motorPin_)[6], uint8_t rightIR_, uint8_t leftIR_);

    void setMotor();

    void rotateMotor(int speed = 0);
};

#endif