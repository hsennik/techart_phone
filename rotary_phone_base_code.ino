//#define DEBUG
#define wheelTurnPin 9
#define dialPin 8
#define hookPin 10

#include <SPI.h> // SPI interface
#include <SD.h> // SD card 
// audio includes 
#include <pcmRF.h>
#include <pcmConfig.h>
#include <TMRpcm.h>

int counter; // count the pulses for each dial spin
int currentValue = 0; // current value of the dial pin
int numberDialled; // the number recorded after the dial has returned to the home position 
long lastDebounceTime = 0; // the last time the dial pin changed value 
long debounceDelay = 5; // the debounce time

//TMRpcm tmrpcm;

void setup(){
  //start serial connection
  Serial.begin(9600);
  #ifdef DEBUG
    Serial.println(F("Serial connection started"));
  #endif
  
  // configure the three inputs
  pinMode(wheelTurnPin, INPUT_PULLUP);
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(hookPin, INPUT);

//  // volume range from 0 to 7 (play around with volume)
//  tmrpcm.setVolume(4);
//  // enable 2x oversampling (quality of audio)
//  tmrpcm.quality(1);

  #ifdef DEBUG
    Serial.println("Setup complete");
  #endif
}

void loop(){
  int initRead = digitalRead (wheelTurnPin); // detects if the rotary wheel is turning 
  int hookValue = digitalRead(hookPin); // detect if receiver lifted off the handset
  static int lastValue = HIGH;  // holds the last read from the dial pin 

  if (hookValue == 0) { // if receiver lifted off handset 
    #ifdef DEBUG
      Serial.println("Receiver Lifted");
    #endif
    if (initRead == LOW) {  // If the rotary wheel is turning 
      #ifdef DEBUG
        Serial.println("Rotary wheel turning");
      #endif
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
        #ifdef DEBUG
          Serial.println("Rotary wheel in home position");
        #endif
        if (counter > 0) {
          if (counter == 10) { // 10 pulses represents "0"
            numberDialled = 0;
          } else {
            numberDialled = counter;
          }
        }

        Serial.println(numberDialled);
        counter = 0; // reset the counter
    
        // Play the audio files that correspond to numberDialled 
        
        #ifdef DEBUG
          Serial.println("Ready to play audio files");
        #endif
    }
  }  
  else { // receiver replaced on handset 
//    tmrpcm.stopPlayback(); // stop playing audio 
    #ifdef DEBUG
      Serial.println("Receiver Replaced");
    #endif
  }
}
