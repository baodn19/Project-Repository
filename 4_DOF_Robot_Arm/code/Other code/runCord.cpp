// Library
#include <AccelStepper.h>
#include <Arduino.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <Wire.h>

// Variables
// Stepper motors
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

Servo servo; // Servo

// END_STOP & BUTTON
#define END_X 3
#define END_Y 14
#define END_Z 18
#define BUTTON A10

// Other
int valButton;
int buttonDef = 1;
int isPressed = 0;

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

void runCord(byte servoAngle, int cordX, int cordY, int cordZ) {
  servo.write(servoAngle);

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

void setup() {
  Serial.begin(250000);

  servo.attach(4);

  // Button and endstop
  pinMode(END_X, INPUT_PULLUP);
  pinMode(END_Y, INPUT_PULLUP);
  pinMode(END_Z, INPUT_PULLUP);
  pinMode(BUTTON, INPUT_PULLUP);

  // Stepper motor
  stepX.setEnablePin(X_ENABLE_PIN);
  stepX.setPinsInverted(false, false, true);
  stepY.setEnablePin(Y_ENABLE_PIN);
  stepY.setPinsInverted(false, false, true);
  stepZ.setEnablePin(Z_ENABLE_PIN);
  stepZ.setPinsInverted(false, false, true);

  // home
  home();

  // Calibrate stepper motor
  stepX.setMaxSpeed(200);
  stepX.setAcceleration(200);
  stepY.setMaxSpeed(100);
  stepY.setAcceleration(100);
  stepZ.setMaxSpeed(100);
  stepZ.setAcceleration(100);
}

void loop() {
  valButton = digitalRead(BUTTON); // Read button value

  // Check if button is pressed
  if (valButton != buttonDef) {
    if (valButton == 0)
      isPressed = 1;
    buttonDef = valButton;
  }

  // Cases for button
  if (isPressed == 1) {
    home(); // Go back to home state
    delay(500);

    runCord(180, -235, -171, 142); // Position 1
    delay(500);
    runCord(180, -348, -171, 142); // Position 2
    delay(500);
    runCord(0, -348, -294, 228); // Position 3
    delay(500);
    runCord(0, -348, -153, 112); // Position 4
    delay(500);
    runCord(0, -450, -153, 112); // Position 5
    delay(500);
    runCord(180, -450, -300, 216); // Position 6
    delay(500);
    runCord(180, -216, -147, 101); // Position 7
    delay(500);
    runCord(0, -56, -289, 229); // Position 8
    delay(500);
    runCord(0, -259, -164, 122); // Position 9
    delay(500);
    runCord(180, -259, -295, 234); // Position 10
    delay(1000);

    isPressed = 0;
  } else {
    servo.write(0);
    stepX.disableOutputs();
    stepY.disableOutputs();
    stepZ.disableOutputs();
  }

  Serial.println("Button: " + String(valButton));
}