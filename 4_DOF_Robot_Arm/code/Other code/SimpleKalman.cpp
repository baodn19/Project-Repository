// Lib
#include <Arduino.h>
#include <SimpleKalmanFilter.h>

// Var
SimpleKalmanFilter filter(1, 1, 0.01);

float resVal;
float resFil;

void setup() {
  Serial.begin(250000);

  pinMode(A0, INPUT);
}

void loop() {
  resVal = analogRead(A0);
  resFil = filter.updateEstimate(resVal);

  Serial.println("Measured: " + String(resVal) +
                 " - Filtered: " + String(resFil));
}