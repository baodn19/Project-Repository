// Library
#include <AccelStepper.h>
#include <Arduino.h>
#include <Wire.h>

// Variables
#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
AccelStepper stepX(1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN); // X axis

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
AccelStepper stepY(1, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN); // Y axis

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62
AccelStepper stepZ(1, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN); // Z axis

// END_STOP
#define END_X 3
#define END_Y 14
#define END_Z 18

// Move Y and Z axes to home position
void homeYZ() {
  int homeY = 0;
  int homeZ = 0;
  stepY.setMaxSpeed(100);
  stepZ.setMaxSpeed(100);
  stepY.setAcceleration(100);
  stepZ.setAcceleration(100);
  stepY.enableOutputs();
  stepZ.enableOutputs();

  // Move bit by bit allow to check state of end switch
  while (digitalRead(END_Y) == 1 && digitalRead(END_Z) == 1) {
    homeY++;
    homeZ--;
    stepY.moveTo(homeY);
    stepZ.moveTo(homeZ);
    stepY.run();
    stepZ.run();
  }

  // Check if Y axis have touched the end switch
  while (digitalRead(END_Y) == 1) {
    homeY++;
    stepY.moveTo(homeY);
    stepY.run();
  }
  stepY.setCurrentPosition(0);

  // Check if Z axis have touched the end switch
  while (digitalRead(END_Z) == 1) {
    homeZ--;
    stepZ.moveTo(homeZ);
    stepZ.run();
  }
  stepZ.setCurrentPosition(0);
}

// Move X axes to home position
void homeX() {
  int homeX = 0;
  stepX.setMaxSpeed(100);
  stepX.setAcceleration(100);
  stepX.enableOutputs();

  // Check if X axis have touched the end switch
  while (digitalRead(END_X) == 1) {
    homeX++;
    stepX.moveTo(homeX);
    stepX.run();
  }
  stepX.setCurrentPosition(0);
}

void setup() {
  // Setup motor
  stepX.setEnablePin(X_ENABLE_PIN);
  stepY.setEnablePin(Y_ENABLE_PIN);
  stepZ.setEnablePin(Z_ENABLE_PIN);
  stepX.setPinsInverted(false, false, true);
  stepY.setPinsInverted(false, false, true);
  stepZ.setPinsInverted(false, false, true);

  // Setup end stops
  pinMode(END_X, INPUT_PULLUP);
  pinMode(END_Y, INPUT_PULLUP);
  pinMode(END_Z, INPUT_PULLUP);

  // Return to home position
  homeYZ();
  homeX();
}

void loop() {}