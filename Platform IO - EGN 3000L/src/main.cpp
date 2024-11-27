// Library
#include <motor.h>
#include <audio.h>
#include <SoftwareSerial.h> // Needed for the playing audio
#include <RedMP3.h> // store all the function of the audio controller

// Variables
//Motor
int motorConfig[6] = {6, 5, 3, 9, 10, 11};
#define IR_SENSOR_RIGHT A2 // Pin for right IR sensor
#define IR_SENSOR_LEFT A1 // Pin for left IR sensor
#define MOTOR_SPEED 130 // Motors' speed

Motor yellowMotor(motorConfig, IR_SENSOR_RIGHT, IR_SENSOR_LEFT);

// Headlights and Photoresistor
#define RIGHT_LED 2 // Pin for right headlight
#define LEFT_LED 4 // Pin for left headlight
#define PHOTORES A0 // Pin for photoresistor

// MP3 Module
int8_t volume = 0x1a; // set the volume for the speaker
#define MP3_RX 7//RX of Serial MP3 module connect to D7 of Arduino
#define MP3_TX 8//TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO
int8_t fileLoc[2] = {0x01, 0x02};
Audio speaker(MP3_RX, MP3_TX, 0x01, fileLoc, 0x1a, RIGHT_LED, LEFT_LED, PHOTORES);




void setup() {
  speaker.setAudio();

  // put your setup code here, to run once:
  yellowMotor.setMotor();
  yellowMotor.rotateMotor(); // Make sure the motor stop

  Serial.begin (9600) ; // use default of 9600 baud rate for serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  yellowMotor.rotateMotor(MOTOR_SPEED);
  speaker.readLight();
  delay(500);
}