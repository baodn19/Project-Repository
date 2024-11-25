// Library
#include <motor.h>
#include <audio.h>
#include <SoftwareSerial.h> // Needed for the playing audio
#include <RedMP3.h> // store all the function of the audio controller

// Variables
// IR Sensors
#define IR_SENSOR_RIGHT 12 // Pin for right IR sensor
#define IR_SENSOR_LEFT 13 // Pin for left IR sensor
#define MOTOR_SPEED 130 // Motors' speed

// Headlights and Photoresistor
#define RIGHT_LED 2 // Pin for right headlight
#define LEFT_LED 4 // Pin for left headlight
#define PHOTORES A0 // Pin for photoresistor

// MP3 Module
int8_t volume = 0x1a; // set the volume for the speaker
#define MP3_RX 7//RX of Serial MP3 module connect to D7 of Arduino
#define MP3_TX 8//TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO
MP3 mp3(MP3_RX, MP3_TX);

//Motor
int motorConfig[6] = {6, 5, 3, 9, 10, 11};

void setup() {
  //The problem with TT gear motors is that, at very low pwm value it does not even rotate.
  //If we increase the PWM value then it rotates faster and our robot is not controlled in that speed and goes out of line.
  //For that we need to increase the frequency of analogWrite.
  //Below line is important to change the frequency of PWM signal on pin D5 and D6
  //Because of this, motor runs in controlled manner (lower speed) at high PWM value.
  //This sets frequecny as 7812.5 hz
  TCCR0B = TCCR0B & (B11111000 | B00000010);

  // put your setup code here, to run once:
  // Set output for right motor's pin
  for (int i = 0; i < 6; i++) {
    pinMode(motorConfig[i], OUTPUT);
  }

  // Set input for ir sensors
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  rotateMotor(motorConfig, 0, IR_SENSOR_RIGHT, IR_SENSOR_LEFT); // make sure the wheels aren't moving

  // Set output for headlights and input for the photoresistor
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(PHOTORES, INPUT);

  // Initialize the MP3 module
  delay(500);//Requires 500ms to wait for the MP3 module to initialize  
  mp3.setVolume(volume);
  delay(50);//you should wait for >=50ms between two commands

  Serial.begin (9600) ; // use default of 9600 baud rate for serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  rotateMotor(motorConfig, MOTOR_SPEED, IR_SENSOR_RIGHT, IR_SENSOR_LEFT);
  readLight(RIGHT_LED, LEFT_LED, PHOTORES);
}