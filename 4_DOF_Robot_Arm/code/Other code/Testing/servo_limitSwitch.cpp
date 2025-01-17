#include <Arduino.h>
#include <Servo.h>

// Variable
Servo myservo;

#define E_X 3
#define E_Y 14
#define E_Z 18

bool gt_E_X;
bool gt_E_Y;
bool gt_E_Z;

// Test limit switches
void limitSwitch() {
  gt_E_X = digitalRead(E_X);
  gt_E_Y = digitalRead(E_Y);
  gt_E_Z = digitalRead(E_Z);

  Serial.println(String(gt_E_X) + " " + String(gt_E_Y) + " " + String(gt_E_Z));
}

// Test servo
void servo() {
  myservo.write(180);
  delay(500);
  myservo.write(60);
  delay(500);
  myservo.write(0);
  delay(500);
}

void setup() {
  Serial.begin(250000);

  pinMode(E_X, INPUT_PULLUP);
  pinMode(E_Y, INPUT_PULLUP);
  pinMode(E_Z, INPUT_PULLUP);

  myservo.attach(4);
}

void loop() { servo(); }