#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>


class Motor {
private:
    // Attributes
    int motorPin[6];
    int rightIR;
    int leftIR;
    int currentSpeed; // store the current speed of both motor
    int stopSpeed; // store the speed to stop the motor
    int isDark; // determine if the room is dark or not
    int doGo; // whether the robot detects a line and go or not
    int stateChange; // tells  if robot is accelerating, decelerating, or moving constantly

public:
    /*
    Description: assign values to the object
    Input: 
        motorPin_: the motors' pin in the order of {right1, right2, rightEnable, left1, left2. leftEnable}
        rightIR_: pin of right IR sensor
        leftIR_: pin of left IR sensor
    */
    Motor(int (&motorPin_)[6], int rightIR_, int leftIR_);

    /*
    Description: set the mode for all the motors' pin in the void setup() in main.cpp
    */
    void setMotor();

    /*
    Description: determine the rotation of each motor based on the values read by IR sensors
    Input: 
        endSpeed: default value is 0; is the top speed
        speedChange: default value is 0; is the change in speed when accelerating or deccelerating
    */
    void rotateMotor(int endSpeed = 0, int speedChange = 0);

    /*
    Description: assign value to the current speed
    Input - currentSpeed: value of current speed
    */
    void setCurrentSpeed(int currentSpeed);

    /*
    Description: control the motors' speed
    Input: 
        endSpeed: the speed that we want to reach
        speedChange: the speed rate of change
    */
    void speedControl(int endSpeed, int speedChange);

    /*
    Description: return the robot's status (moving or stopped)
    Output: 
        1: stopped
        0: moving
    */
    int isStop();

    /*
    Description: Get value of the room's light intensity
    Input: 
        1: dark; light is turned on, slowing down
        0: bright
    */
    void getLight(int isDark_);
    /*
    Description: assign stop speed value
    Input - stopSpeed: speed when the motor stop moving
    */
    void setStopSpeed(int stopSpeed_);

    /*
    Description: return the state of the robot
    Output: 
        -1: Decelerating
        0: moving constantly
        1: Accelerating
    */
    int speedState();
};

#endif