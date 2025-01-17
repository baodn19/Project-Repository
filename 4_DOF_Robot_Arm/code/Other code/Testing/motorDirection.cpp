#include <AccelStepper.h>
#include <Arduino.h>

// var
#define STEP 54
#define DIR 55
#define ENA 38

AccelStepper motor(1, STEP, DIR, ENA);

void setup() {
  motor.setMaxSpeed(200);

  motor.setEnablePin(ENA);
  motor.setPinsInverted(false, false, true);
  motor.enableOutputs();
}

void loop() {
  motor.setCurrentPosition(0);
  while (motor.currentPosition() != -200) {
    motor.setSpeed(-20);
    motor.runSpeed();
  }
}