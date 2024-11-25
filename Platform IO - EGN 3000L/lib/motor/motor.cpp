#include "motor.h"

void rotateMotor(int motorPin[6], int speed, uint8_t rightIR, uint8_t leftIR) 
{ 
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
  analogWrite(motorPin[2], speed);
  analogWrite(motorPin[5], speed);

  // Checking the value of each IR sensors
  Serial.println("Right: " + String(rightIRSensorValue));
  Serial.println("Left: " + String(leftIRSensorValue));
  Serial.println();
}
