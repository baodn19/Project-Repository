// Library
#include <Arduino.h>
#include <SoftwareSerial.h> // Needed for the playing audio
#include "RedMP3.h" // store all the function of the audio controller

// Headlights and Photoresistor
#define RIGHT_LED 2 // Pin for right headlight
#define LEFT_LED 4 // Pin for left headlight
#define PHOTORES A0 // Pin for photoresistor

// MP3 Module
#define MP3_RX 7//RX of Serial MP3 module connect to D7 of Arduino
#define MP3_TX 8//TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO
MP3 mp3(MP3_RX, MP3_TX);

int8_t volume = 0x1a; // set the volume for the speaker
int8_t folderName = 0x01; // Set location of sound foler
int8_t fileName = 0x01; // Set file wanted to play

int musicDone = 1; // check if the music is done playing
unsigned long prevMillis = 0; // timestamp when the music is played
unsigned long currMillis = 0; // current timestamp

// IR Sensors
#define IR_SENSOR_RIGHT 12 // Pin for right IR sensor
#define IR_SENSOR_LEFT 13 // Pin for left IR sensor
#define MOTOR_SPEED 150 // Motors' speed

//Right motor
int enableRightMotor=3; // Pin for right motor speed control
int rightMotorPin1=6; // Pin for right motor directional control 1
int rightMotorPin2=5; // Pin for right motor directional control 2

//Left motor
int enableLeftMotor=11; // Pin for left motor speed control
int leftMotorPin1=9; // Pin for right motor directional control 1
int leftMotorPin2=10; // Pin for right motor directional control 2

// Function for controling the motors' speed and direction
// Input: the speed of 2 motors
void rotateMotor(int rightMotorSpeed, int leftMotorSpeed) 
{ 
  // Control rotation based on the right motor's speed
  if (rightMotorSpeed < 0) // Rotate Counter Clock Wise
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);
  }
  else if (rightMotorSpeed > 0) // Rotate Clock Wise
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else // Stop
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }

  // Control rotation based on the left motor's speed
  if (leftMotorSpeed < 0) // Rotate Counter Clock Wise
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0) // Rotate Clock Wise
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else // Stop
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }

  // Input speed into each motor
  analogWrite(enableRightMotor, abs(rightMotorSpeed));
  analogWrite(enableLeftMotor, abs(leftMotorSpeed));    
}

void followLine() // Use IR sensors value to follow black-and-white lines
{
  // Read from both IR sensors
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  int leftIRSensorValue = digitalRead(IR_SENSOR_LEFT);

  // Checking the value of each IR sensors
  Serial.println("Right: " + String(rightIRSensorValue));
  Serial.println("Left: " + String(leftIRSensorValue));
  Serial.println();
  delay(500);

  //If none of the sensors detects black line, then go straight
  if (rightIRSensorValue == LOW && leftIRSensorValue == LOW)
  {
    rotateMotor(MOTOR_SPEED, MOTOR_SPEED);
  }
  //If right sensor detects black line, then turn right
  else if (rightIRSensorValue == HIGH && leftIRSensorValue == LOW )
  {
      rotateMotor(-MOTOR_SPEED, MOTOR_SPEED); 
  }
  //If left sensor detects black line, then turn left  
  else if (rightIRSensorValue == LOW && leftIRSensorValue == HIGH )
  {
      rotateMotor(MOTOR_SPEED, -MOTOR_SPEED); 
  } 
  //If both the sensors detect black line, then stop 
  else 
  {
    rotateMotor(0, 0);
  }
}

// Play music from SD Card
// Input: time of the audio, the folder the audio is in, and the position of the audio file
void playMusic(unsigned int time, int8_t folderName, int8_t fileName) 
{
  // Marking the current timestamp
  currMillis = millis();

  // Check if the audio is finished playing
  if (musicDone == 1) {
    mp3.playWithFileName(0x01,0x01); // Play the audio file  
    musicDone = 0; // Mark as 0 because the music is playing
    prevMillis = currMillis; // Mark prevMillis as the time the audio played
  }


  // Compare if the audio has finished playing 
  // by subtracing the current timestamp to prevMillis 
  // and compare to the audio length
  if ((currMillis - prevMillis) > (time * 1000)) {
    // Mark as done because the audio is done playing
    musicDone = 1;
  }
}

// Read the light intensity of environment to turn on/ off LED
void readLight() {
  // Read the environment's light intensity
  int photocellStatus = analogRead(PHOTORES);

  // Turn on the light when the value of photoresistor is greater than 500
  if (photocellStatus > 500) 
  {
    // Turning on 2 headlights
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);

    // Play "Ka - Chow"
    playMusic(2, folderName, fileName);

    // Serial print the photoresistor's value
    Serial.print("Its DARK, Turn on the LED : ");
    Serial.println(photocellStatus);
  } else 
  {
    // Turn off the 2 headlights
    digitalWrite(RIGHT_LED, LOW);
    digitalWrite(LEFT_LED, LOW);

    // Serial print the photoresistor's value
    Serial.print("Its BRIGHT, Turn off the LED : ");
    Serial.println(photocellStatus);
  }
}

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
  pinMode(enableRightMotor, OUTPUT); 
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
  
  // Set output for left motor's pin
  pinMode(enableLeftMotor, OUTPUT);
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);

  // Set input for ir sensors
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  rotateMotor(0,0); // make sure the wheels aren't moving

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
  followLine();
  readLight();
}