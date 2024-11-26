#include "motor.h"

// Create a class for motor function
Motor::Motor(int (&motorPin_)[6], uint8_t rightIR_, uint8_t leftIR_) {
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
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
    digitalWrite(motorPin[0],LOW);
    digitalWrite(motorPin[1],HIGH); 
    digitalWrite(motorPin[3],HIGH);
    digitalWrite(motorPin[4],LOW); 
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  { 
    digitalWrite(motorPin[0],HIGH);
    digitalWrite(motorPin[1],LOW); 
    digitalWrite(motorPin[3],LOW);
    digitalWrite(motorPin[4],HIGH);
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    digitalWrite(motorPin[0],LOW);
    digitalWrite(motorPin[1],LOW);
    digitalWrite(motorPin[3],LOW);
    digitalWrite(motorPin[4],LOW);
  }

  // Input speed into each motor
  analogWrite(motorPin[2], endSpeed);
  analogWrite(motorPin[5], endSpeed);

  // Checking the value of each IR sensors
  Serial.println("Right: " + String(rightIRSensorValue));
  Serial.println("Left: " + String(leftIRSensorValue));
  Serial.println();
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
}

void Motor::setCurrentSpeed(int currentSpeed_) {
  currentSpeed = currentSpeed_;
}


/*

*/