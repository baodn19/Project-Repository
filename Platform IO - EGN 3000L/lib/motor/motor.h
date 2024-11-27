#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


class Motor {
private:
    // Attributes
    int motorPin[6];
    uint8_t rightIR;
    uint8_t leftIR;
    int currentSpeed;

public:
    Motor(int (&motorPin_)[6], uint8_t rightIR_, uint8_t leftIR_);

    void setMotor();

    void rotateMotor(int endSpeed = 0, int speedChange = 0);

    void setCurrentSpeed(int currentSpeed);
};

#endif