#include "audio.h" // include header file
#include <SoftwareSerial.h> // Needed for the playing audio
#include <RedMP3.h> // store all the function of the audio controller

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

  // musicDone = 1;
  // prevMillis = 0;
  // currMillis = 0;
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
  // Serial.print("MusicDone: ");
  // Serial.println(musicDone);
  

  // Check if the audio is finished playing or the robot change its state
  if (musicDone == 1) {
    // Serial.print("Current Mill: ");
    // Serial.println(currMillis);
    // Serial.println("Prev Mill: " + String(prevMillis));
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

void Audio::controlLight(int isStop_, int speedState) {

  // Turn on the light when the value of photoresistor is greater than 500
  if (this->isDark() == 1) 
  {
    // Turning on 2 headlights
    digitalWrite(rightLED, HIGH);
    digitalWrite(leftLED, HIGH);

    if (isStop_ == 1) { // 1 means the robot stopped
      // Play "Ka - Chow" as 10 seconds
      this->playMusic(10, 0);
    }
  } else 
  {
    // Turn off the 2 headlights
    digitalWrite(rightLED, LOW);
    digitalWrite(leftLED, LOW);

    if (speedState == 1) { 
      //unsigned int time ;
      // this->playMusic(9999, 1);
    }
  }
}

int Audio::isDark() {
  // Read the environment's light intensity
  int photocellStatus = analogRead(photores);

  if (photocellStatus > lightThreshold) {
    // Serial print the photoresistor's value
    // Serial.print("Its DARK, Turn on the LED : ");
    // Serial.println(photocellStatus);
    return  1; // 1 means the room is dark

    
  } else {
    // Serial.print("Its BRIGHT, Turn off the LED : ");
    // Serial.println(photocellStatus);
    return 0; // 0 means the room is bright
  }
}

void Audio::getRobotStatus(int isStop_, int speedState) {
  isStop = isStop_;
  if (speedState != prevState) {
    changeState = 1; // robot change state
  } else { 
    changeState = 0; // robot didn't change state
  }

  prevState = speedState; // update prev state

  Serial.println("Stop?: " + String(isStop));
  Serial.println("State changed?: " + String(speedState));
  Serial.println("");
}