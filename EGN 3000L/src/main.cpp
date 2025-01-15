// Library
#include <motor.h>
#include <audio.h>
#include <SoftwareSerial.h> // Needed for the playing audio
#include <RedMP3.h> // store all the function of the audio controller

// Variables
//Motor
int motorConfig[6] = {6, 5, 3, 9, 10, 11};
#define IR_SENSOR_RIGHT 12 // Pin for right IR sensor
#define IR_SENSOR_LEFT 13 // Pin for left IR sensor
#define MOTOR_SPEED 180 // Motors' speed

Motor yellowMotor(motorConfig, IR_SENSOR_RIGHT, IR_SENSOR_LEFT);

// Headlights and Photoresistor
#define RIGHT_LED 2 // Pin for right headlight
#define LEFT_LED 4 // Pin for left headlight
#define PHOTORES A0 // Pin for photoresistor

// MP3 Module
int8_t volume = 0x19; // set the volume for the speaker 0x19
#define MP3_RX 7//RX of Serial MP3 module connect to D7 of Arduino
#define MP3_TX 8//TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO
int8_t fileLoc[2] = {0x01, 0x02};
Audio speaker(MP3_RX, MP3_TX, 0x01, fileLoc, volume, RIGHT_LED, LEFT_LED, PHOTORES);

void setup() {
  speaker.setAudio();
  speaker.changeLightThreshold(600); // Set the light threshold for turning on and off the headlights

  // put your setup code here, to run once:
  yellowMotor.setMotor();
  yellowMotor.rotateMotor(); // Make sure the motor stop

  Serial.begin (9600) ; // use default of 9600 baud rate for serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  int envLight = speaker.isDark(); // Get the light intensity of the environment
  //Serial.println("Dark: " + String(envLight));
  yellowMotor.getLight(envLight); // Assign the light intensity of the environment
  yellowMotor.rotateMotor(MOTOR_SPEED, 5);

  int robotGo = yellowMotor.isStop(); // get the state of the robot (moving or not)
  int speedState = yellowMotor.speedState();
  speaker.controlLight(robotGo, speedState);
  delay(500);
}

