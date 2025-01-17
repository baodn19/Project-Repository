#include <AccelStepper.h>
#include <Arduino.h>

// Variables
int dir = 4;
int step = 5;
int ena = 6;
AccelStepper motor(1, step, dir, ena);

void constSpeed() {
  motor.setSpeed(-500);
  motor.runSpeed();
  Serial.println("Speed: " + String(motor.speed()));
}

void rotation() {
  motor.setCurrentPosition(0);
  while (motor.currentPosition() != 600) { // 3 rounds CCW
    motor.setSpeed(300);
    motor.runSpeed();
  }
  delay(1000);

  Serial.println("Speed: " + String(motor.speed()));
  Serial.println("Position: " + String(motor.currentPosition()));
}

void acceleration() {
  // Accelerate forward
  motor.setCurrentPosition(0);
  motor.moveTo(1000);
  Serial.println("Destination 1: " + String(motor.targetPosition()));
  motor.runToPosition();
  Serial.println("Postion: " + String(motor.currentPosition()));
  delay(1000);

  // Accelerate backward
  Serial.println("Destination 2: 0");
  motor.runToNewPosition(0);
  Serial.println("Position: " + String(motor.currentPosition()));
  delay(1000);

  // Use while loop for changing destination at constant speed
  motor.moveTo(-1000);
  motor.setSpeed(500);
  Serial.println("Destination 3: " + String(motor.targetPosition()));
  Serial.println("Distance: " + String(motor.distanceToGo()));
  while (motor.distanceToGo() != 0) {
    motor.runSpeedToPosition();
    Serial.println("Distance: " + String(motor.distanceToGo()));
  }
  Serial.println("Position: " + String(motor.currentPosition()));
  delay(1000);
}

void setup() {
  Serial.begin(250000);
  pinMode(dir, OUTPUT);
  pinMode(step, OUTPUT);
  pinMode(ena, OUTPUT);

  digitalWrite(ena, LOW);  // Activate the motor
  motor.setMaxSpeed(1000); // Maximum speed
  // motor.setAcceleration(-1000);
}

void loop() { acceleration(); }