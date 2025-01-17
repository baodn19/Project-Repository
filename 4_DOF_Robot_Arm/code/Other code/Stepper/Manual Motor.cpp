#include <Arduino.h>

// Variables
int dir = 4;
int step = 5;
int ena = 6;

void setup() {
  pinMode(dir, OUTPUT);
  pinMode(step, OUTPUT);
  pinMode(ena, OUTPUT);

  digitalWrite(ena, LOW); // Activate the motor
}

void loop() {
  digitalWrite(dir, LOW);         // Identify the rotating direction
  for (int x = 0; x < 200; x++) { // 1 loop - 500 speed
    digitalWrite(step, HIGH);
    delayMicroseconds(1000); // Rotation speed
    digitalWrite(step, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
}