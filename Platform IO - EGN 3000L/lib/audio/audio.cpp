#include "audio.h" // include header file
#include <SoftwareSerial.h> // Needed for the playing audio
#include <RedMP3.h> // store all the function of the audio controller

// Variable
int musicDone = 1; // check if the music is done playing
unsigned long prevMillis = 0; // timestamp when the music is played
unsigned long currMillis = 0; // current timestamp

// MP3 Module
#define MP3_RX 7//RX of Serial MP3 module connect to D7 of Arduino
#define MP3_TX 8//TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO

int8_t folderName = 0x01; // Set location of sound foler
int8_t fileName = 0x01; // Set file wanted to play

void playMusic(unsigned int time, int8_t folderName, int8_t fileName) 
{
  // Create mp3 object
  MP3 mp3(MP3_RX, MP3_TX);

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
void readLight(int rightLED, int leftLED, uint8_t PHOTORES) {
  // Read the environment's light intensity
  int photocellStatus = analogRead(PHOTORES);

  // Turn on the light when the value of photoresistor is greater than 500
  if (photocellStatus > 500) 
  {
    // Turning on 2 headlights
    digitalWrite(rightLED, HIGH);
    digitalWrite(leftLED, HIGH);

    // Play "Ka - Chow"
    playMusic(2, folderName, fileName);

    // Serial print the photoresistor's value
    Serial.print("Its DARK, Turn on the LED : ");
    Serial.println(photocellStatus);
  } else 
  {
    // Turn off the 2 headlights
    digitalWrite(rightLED, LOW);
    digitalWrite(leftLED, LOW);

    // Serial print the photoresistor's value
    Serial.print("Its BRIGHT, Turn off the LED : ");
    Serial.println(photocellStatus);
  }
}