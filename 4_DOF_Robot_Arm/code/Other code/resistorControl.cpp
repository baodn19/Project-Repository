// Library
#include <AccelStepper.h>
#include <Arduino.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <SimpleKalmanFilter.h>
#include <Wire.h>

// Stepper motors
#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
SimpleKalmanFilter filterX(2, 2, 0.01);
AccelStepper stepX(1, X_STEP_PIN, X_DIR_PIN, X_ENABLE_PIN); // X axis

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
SimpleKalmanFilter filterY(2, 2, 0.01);
AccelStepper stepY(1, Y_STEP_PIN, Y_DIR_PIN, Y_ENABLE_PIN); // Y axis

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62
SimpleKalmanFilter filterZ(2, 2, 0.01);
AccelStepper stepZ(1, Z_STEP_PIN, Z_DIR_PIN, Z_ENABLE_PIN); // Z axis

int curPosX = 0;
int curPosY = 0;
int curPosZ = 0;

Servo servo; // Servo

// END_STOP
#define END_X 3
#define END_Y 14
#define END_Z 18

// Variable resistor
#define RES_X A3
#define RES_Y A4
#define RES_Z A5
#define BUTTON A10

int valResX;
int valResY;
int valResZ;
int valBut;

int cordX;
int cordY;
int cordZ;
int butDefault = 1; // 1 is open and 0 is closed
unsigned int cnt;

// Move Y and Z axes to home position
void homeYZ() {
  int homeY = 0;
  int homeZ = 0;
  stepY.setMaxSpeed(100);
  stepY.setAcceleration(100);
  stepY.enableOutputs();
  stepZ.setMaxSpeed(100);
  stepZ.setAcceleration(100);
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

void home() {
  homeYZ();
  homeX();
}

void setup() {
  Serial.begin(250000); // Terminal

  servo.attach(4); // Servo

  // Stepper motor
  stepX.setEnablePin(X_ENABLE_PIN);
  stepX.setPinsInverted(false, false, true);
  stepY.setEnablePin(Y_ENABLE_PIN);
  stepY.setPinsInverted(false, false, true);
  stepZ.setEnablePin(Z_ENABLE_PIN);
  stepZ.setPinsInverted(false, false, true);

  // End stops
  pinMode(END_X, INPUT_PULLUP);
  pinMode(END_Y, INPUT_PULLUP);
  pinMode(END_Z, INPUT_PULLUP);

  // Variable resistors and button
  pinMode(RES_X, INPUT);
  pinMode(RES_Y, INPUT);
  pinMode(RES_Z, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // Return to home position
  home();

  // Return to current position
  valResX = analogRead(RES_X);
  valResY = analogRead(RES_Y);
  valResZ = analogRead(RES_Z);

  if (valResX != 0 || valResY != 0 || valResZ != 0) {
    cordX = map(valResX, 0, 1023, 0, -500);
    cordY = map(valResY, 0, 1023, 0, -300);
    cordZ = map(valResZ, 0, 1023, 0, 250);
    stepX.moveTo(cordX);
    stepY.moveTo(cordY);
    stepZ.moveTo(cordZ);

    while (stepX.distanceToGo() != 0 || stepY.distanceToGo() != 0 ||
           stepZ.distanceToGo() != 0) {
      stepX.run();
      stepY.run();
      stepZ.run();
    }
  }

  // Set faster speed
  stepX.setMaxSpeed(200);
  stepX.setAcceleration(200);
  stepY.setMaxSpeed(100);
  stepY.setAcceleration(100);
  stepZ.setMaxSpeed(100);
  stepZ.setAcceleration(100);
}

void loop() {
  // Button controlling the servo
  valBut = digitalRead(BUTTON);
  if (valBut != butDefault) {
    if (valBut == 0)
      cnt++;
    butDefault = valBut;
  }
  if (cnt % 2 == 0)
    servo.write(180);
  else
    servo.write(0);

  // Variable resistor controlling the stepper motor
  valResX = filterX.updateEstimate(analogRead(RES_X));
  valResY = filterY.updateEstimate(analogRead(RES_Y));
  valResZ = filterZ.updateEstimate(analogRead(RES_Z));

  if ((valResX > curPosX) || (valResX < curPosX) or (valResY > curPosY) ||
      (valResY < curPosY) or (valResZ > curPosZ) || (valResZ < curPosZ)) {
    stepX.enableOutputs();
    stepY.enableOutputs();
    stepZ.enableOutputs();

    cordX = map(valResX, 0, 1023, 0, -500);
    cordY = map(valResY, 0, 1023, 0, -300);
    cordZ = map(valResZ, 0, 1023, 0, 250);
    stepX.moveTo(cordX);
    stepY.moveTo(cordY);
    stepZ.moveTo(cordZ);

    while (stepX.distanceToGo() != 0 || stepY.distanceToGo() != 0 ||
           stepZ.distanceToGo() != 0) {
      stepX.run();
      stepY.run();
      stepZ.run();
    }

    curPosX = valResX;
    curPosY = valResY;
    curPosZ = valResZ;
  }

  // Print current statistics
  Serial.print("Res X: " + String(valResX) + " - Res Y: " + String(valResY) +
               " - Res Z: " + String(valResZ) + " - Button: " + String(valBut));
  Serial.println(" | Counter: " + String(cnt) + " - Cord X: " + String(cordX) +
                 " - Cord Y: " + String(cordY) + " - Cord Z: " + String(cordZ));
}