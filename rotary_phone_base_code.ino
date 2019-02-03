//#define DEBUG
#define wheelTurnPin 7
#define dialPin 8
#define hookPin 6
#define SD_ChipSelectPin 4

#include <SD.h> // SD card
#include <TMRpcm.h> // audio 
#include <SPI.h> // SPI interface for SD card reader

TMRpcm tmrpcm;

int counter; // count the pulses for each dial spin
int currentValue = 0; // current value of the dial pin
int numberDialled; // the number recorded after the dial has returned to the home position 
long lastDebounceTime = 0; // the last time the dial pin changed value 
long debounceDelay = 5; // the debounce time

void setup(){
  
  //start serial connection
  Serial.begin(9600);
//  Serial.println(F("Serial connection started"));
  
  // configure the three inputs
  pinMode(wheelTurnPin, INPUT_PULLUP);
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(hookPin, INPUT);

// configure the speaker and SD card 
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin)) {
//   Serial.println("SD fail");
  return;
  }
//  Serial.println("SD succeeded");

  tmrpcm.setVolume(6);
  tmrpcm.quality(1);
//  tmrpcm.play("ted11.wav");

//  Serial.println("Setup complete");
}

void loop(){
  int initRead = digitalRead (wheelTurnPin); // detects if the rotary wheel is turning 
  int hookValue = digitalRead(hookPin); // detect if receiver lifted off the handset
  static int lastValue = HIGH;  // holds the last read from the dial pin 
  
  if (hookValue == 0) { // if receiver lifted off handset 
//    Serial.println("Receiver Lifted");
    if (initRead == LOW) {  // If the rotary wheel is turning 
//      Serial.println("Rotary wheel turning");
      int dialValue = digitalRead (dialPin); // check the pulse pin.
      if (dialValue != lastValue) { // check if the pulse pin value has changed 
        lastDebounceTime = millis(); // record the time that the pulse pin value changed 
      }
      if ((millis() - lastDebounceTime) > debounceDelay) { // check if enough time has passed since the pulse pin value changed 
        if (currentValue != dialValue) { // if the current value of the pulse pin is different then when it was last updated 
          currentValue = dialValue; // 
          if (dialValue == 1) { // If the dial value is a 1 that means another pulse has occurred
            counter++; // add to the pulse counter 
          }
        }
      }
  
      lastValue = dialValue; // Your dial value becomes the old one for comparison.
    } 
    else { // the rotary dial is in the home position (no more pulses to record)
//        Serial.println("Rotary wheel in home position");
        if (counter > 0) {
          if (counter == 10) { // 10 pulses represents "0"
            Serial.println(0);
          } else {
            Serial.println(counter);
          }
          if (counter == 9) {
            tmrpcm.play("ted11.wav");
            while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
          }
          else if (counter == 7) {
            tmrpcm.play("snow16.wav");
            while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
          }
        }

        counter = 0; // reset the counter
    }
  }  
  else { // receiver replaced on handset 
    tmrpcm.stopPlayback(); // stop playing audio 
//    Serial.println("Receiver Replaced");
  }
}
