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
    int isStop; // store the state of the robot (moving or stop)

public: 
    /*
    Description: assign values to the object
    Input: 
        RX_: pin for RX
        TX_: pin for TX
        folderLoc_: location of the folder in hexadecimal
        fileLoc_: an array for file location in the folder in hexadecimal
        volume_: volume of speaker in hexadecimal
        rightLED_: pin of the right LED
        leftLED_: pin of the left LED
        photores_: pin of photoresistor
    */
    Audio(int RX_, int TX_, int8_t folderLoc_, int8_t (&fileLoc_)[2], int8_t volume_, int rightLED_, int leftLED_, uint8_t photores_);

    /*
    Description: setup all the audio pin in void setup() in main.cpp
    */
    void setAudio();

    /*
    Description: change the boundary for light to turn on and off
    Input - lightThreshold_: the value of the boundary
    */
    void changeLightThreshold(int lightThreshold_);

    /*
    Description: play the audio file and wait for the audio to finish
    Input:
        time: the actual duration * 5
        audioFile: the index of the audio file in the fileLoc array
    */
    void playMusic(unsigned int time, int audioFile);
    
    /*
    Description: turn the light on and off based on the photoresistor value
    */
    void controlLight();

    /*
    Description: decide if the room is dark or bright
    Output:
        1: the room is dark, light is turned on
        0: the room is bright, light is turned off
    */
    int isDark();

    /*
    Description: assign the value of the robot status
    Input: the status of the robot
        1: stopped
        0: moving
    */
    void getRobotStatus(int status);
};

#endif