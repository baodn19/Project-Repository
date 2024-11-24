#ifndef AUDIO_H
#define AUDIO_H
#include <Arduino.h>

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