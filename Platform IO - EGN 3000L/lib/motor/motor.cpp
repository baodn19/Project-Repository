#include "motor.h"
#include <audio.h>

// Create a class for motor function
Motor::Motor(int (&motorPin_)[6], int rightIR_, int leftIR_) {
  // Assign value to array
    for (int i = 0; i < 6; i++) {
      motorPin[i] = motorPin_[i];
    }

    // Assign other value
    rightIR = rightIR_;
    leftIR = leftIR_;
}

void Motor::rotateMotor(int endSpeed, int speedChange) {
  // Read from both IR sensors
  int rightIRSensorValue = digitalRead(rightIR);
  int leftIRSensorValue = digitalRead(leftIR);

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    digitalWrite(motorPin[0],HIGH);
    digitalWrite(motorPin[1],LOW); 
    digitalWrite(motorPin[3],HIGH);
    digitalWrite(motorPin[4],LOW);
    
    doGo = 1; // robot detects a line and moves
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
    digitalWrite(motorPin[0],LOW);
    digitalWrite(motorPin[1],HIGH); 
    digitalWrite(motorPin[3],HIGH);
    digitalWrite(motorPin[4],LOW); 

    doGo = 1; // robot detects a line and moves
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  { 
    digitalWrite(motorPin[0],HIGH);
    digitalWrite(motorPin[1],LOW); 
    digitalWrite(motorPin[3],LOW);
    digitalWrite(motorPin[4],HIGH);

    doGo = 1; // robot detects a line and moves
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    digitalWrite(motorPin[0],LOW);
    digitalWrite(motorPin[1],LOW);
    digitalWrite(motorPin[3],LOW);
    digitalWrite(motorPin[4],LOW);

    doGo = 0; // robot detects no line/ black line and does not move
  }

  if (isDark == 1) {
    this->speedControl(stopSpeed, speedChange); // Deccelerate
     stateChange = -1; 
  } else if (doGo == 1 && currentSpeed != endSpeed) {
    this->speedControl(endSpeed, speedChange); // Accelerate
     stateChange = 1;
  } else {
    analogWrite(motorPin[2], endSpeed); // Run at constant speed
    analogWrite(motorPin[5], endSpeed);
     stateChange = 0;
  }

  // Checking the value of each IR sensors
  // Serial.println("State: " + String(stateChange));
  // Serial.println("Speed: " + String(currentSpeed));
  // Serial.println();
}

void Motor::setMotor() {
  //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
  //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
  //For that we need to increase the frequency of analogWrite.
  //Below line is important to change the frequency of PWM signal on pin D5 and D6
  //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
  //This sets frequecny as 7812.5 hz
  TCCR0B = TCCR0B & (B11111000 | B00000010);
  
  for (int i = 0; i < 6; i++) {
    pinMode(motorPin[i], OUTPUT);
    }

    // Set input for ir sensors
    pinMode(rightIR, INPUT);
    pinMode(leftIR, INPUT);
    this->setCurrentSpeed(0); // Set current speed to 0
    this->setStopSpeed(0);
}

void Motor::setCurrentSpeed(int currentSpeed_) {
  currentSpeed = currentSpeed_;
}

void Motor::setStopSpeed(int stopSpeed_) { 
  stopSpeed = stopSpeed_; 
}

void Motor::speedControl(int endSpeed, int speedChange) {
  // Acceleration
  if (currentSpeed < endSpeed) {
    currentSpeed += speedChange;
    // Input speed into each motor
    analogWrite(motorPin[2], currentSpeed);
    analogWrite(motorPin[5], currentSpeed);
  }

  // Decceleration
  if (currentSpeed > endSpeed) {
    currentSpeed -= speedChange;
    // Input speed into each motor
    analogWrite(motorPin[2], currentSpeed);
    analogWrite(motorPin[5], currentSpeed);
  }
  
  // Serial.println("Dark: " + String(isDark));
  // Serial.println("Current speed: " + String(currentSpeed));
  // Serial.println("");
}

// Decide whether the robot stopped or is still moving
int Motor::isStop() {
  int status;
  if (currentSpeed == stopSpeed || doGo == 0) {
    status =  1; // robot stop
  } else {
    status = 0; // robot still going
  }

  // Serial.println("Stop?: " + String(status));
  // Serial.println("Speed: " + String(currentSpeed));
  // Serial.println("");

  return status;
}

void Motor::getLight(int isDark_) {
  isDark = isDark_;
}

int Motor::speedState() {
  return stateChange;
}