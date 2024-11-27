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

Audio::Audio(int RX_, int TX_, int8_t folderLoc_, int8_t (&fileLoc_)[2], int8_t volume_, int rightLED_, int leftLED_, uint8_t photores_) {
  // Array attribute
  for (int i = 0; i < 2; i++) {
    fileLoc[i] = fileLoc_[i];
  }
   
  // Other attributes
  RX = RX_;
  TX = TX_;
  folderLoc = folderLoc_;
  volume = volume_;
  rightLED = rightLED_;
  leftLED = leftLED_;
  photores = photores_;
}

void Audio::setAudio() {
  // Set output for headlights and input for the photoresistor
  pinMode(rightLED, OUTPUT);
  pinMode(leftLED, OUTPUT);
  pinMode(photores, INPUT);

  MP3 mp3(RX, TX);
  // Initialize the MP3 module
  delay(500);//Requires 500ms to wait for the MP3 module to initialize  
  mp3.setVolume(volume);
  delay(50);//you should wait for >=50ms between two commands
}

void Audio::changeLightThreshold(int lightThreshold_) {
  lightThreshold = lightThreshold_;
}

void Audio::playMusic(unsigned int time, int audioFile) {
  // Create mp3 object
  MP3 mp3(RX, TX);

  // Marking the current timestamp
  currMillis = millis();

  // Check if the audio is finished playing
  if (musicDone == 1) {
    mp3.playWithFileName(folderLoc,fileLoc[audioFile]); // Play the audio file  
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

void Audio::readLight() {
  // Read the environment's light intensity
  int photocellStatus = analogRead(photores);

  // Turn on the light when the value of photoresistor is greater than 500
  if (photocellStatus > lightThreshold) 
  {
    // Turning on 2 headlights
    digitalWrite(rightLED, HIGH);
    digitalWrite(leftLED, HIGH);

    // Play "Ka - Chow"
    playMusic(2, 0);

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