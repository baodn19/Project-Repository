#include <Arduino.h>
#include <Wire.h>s

int speakerPin = 6;
int speakerPin2 = 9;

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  //int mag = analogRead(A0);
  //mag = map(mag, 0, 870, 31, 1000);
  //Serial.println(mag);
  tone(speakerPin, 150, 1000);
  tone(speakerPin2, 150, 1000);
  //analogWrite(speakerPin, mag);
   
  

  /* for (int cnt = 0; cnt < 10; cnt++) {
    num[cnt] = random(100);
  }

  for (int cnt = 0; cnt < 10; cnt++) {
    Serial.println(num[cnt]);
  }

  delay(1000);
  */

  /*
  while (Serial.available()) {
    delay(2);  //delay to allow byte to arrive in input buffer
    char c = Serial.read();
    readString += c;
  }

  if (readString.length() >0) {
    int val = readString.toInt();
    Serial.println(val);

    readString="";
  }
  */
}
