#ifndef AUDIO_H
#define AUDIO_H
#include <Arduino.h>

class Audio {
private:
    // Attributes
    int RX; //RX of Serial MP3 module connect to D7 of Arduino
    int TX; //TX to D8, note that D8 can not be used as RX on Mega2560, you should modify this if you donot use Arduino UNO
    int8_t folderLoc; // Set location of sound foler
    int8_t fileLoc[2]; // Set file wanted to play
    int8_t volume; // set the volume for the speaker
    int musicDone = 1; // check if the music is done playing
    unsigned long prevMillis = 0; // timestamp when the music is played
    unsigned long currMillis = 0; // current timestamp

    int rightLED;
    int leftLED;
    uint8_t photores;
    int lightThreshold;

public: 
    Audio(int RX_, int TX_, int8_t folderLoc_, int8_t (&fileLoc_)[2], int8_t volume_, int rightLED_, int leftLED_, uint8_t photores_);

    void setAudio();

    void changeLightThreshold(int lightThreshold_);

    void playMusic(unsigned int time, int audioFile);

    void readLight();
};




/* playMusic()
Description: play the audio file based on location and make sure the audio file is played completely before
Arguments: 
    time: the duration of audio file (seconds)
    foldername: location of folders in bits
    filename: location of file in bits
*/
void playMusic(unsigned int time, int8_t folderName, int8_t fileName);

/*
Description: turn on or off the headlights based on the photoresistor's readings
Arguments:
    rightLED, leftLED: pins for headlights
    PHOTORES: pin for photoresistor
*/
void readLight(int rightLED, int leftLED, uint8_t PHOTORES);

#endif 