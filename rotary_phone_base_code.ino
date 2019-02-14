// DEFINES
//#define DEBUG // uncomment when want to print to serial monitor
#define wheelTurnPin 7
#define dialPin 8
#define hookPin 6
#define SD_ChipSelectPin 4

// INCLUDES
#include <SD.h> // SD card
#include <TMRpcm.h> // audio 
#include <SPI.h> // SPI interface for SD card reader

TMRpcm tmrpcm;

// VARIABLES
int counter; // count the pulses for each dial spin
int currentValue = 0; // current value of the dial pin
int numberDialled; // the number recorded after the dial has returned to the home position 
long lastDebounceTime = 0; // the last time the dial pin changed value 
long debounceDelay = 5; // the debounce time

void setup(){
  
  Serial.begin(9600); //start serial connection
  #ifdef DEBUG
    Serial.println(F("Serial connection started"));
  #endif
  
  // configure the three inputs
  pinMode(wheelTurnPin, INPUT_PULLUP);
  pinMode(dialPin, INPUT_PULLUP);
  pinMode(hookPin, INPUT);

  tmrpcm.speakerPin = 9; // configure the speaker and SD card 

  // check that the SD card reader is connected
  if (!SD.begin(SD_ChipSelectPin)) {
    #ifdef DEBUG
      Serial.println("SD fail");
    #endif
  return;
  }
  else {
    #ifdef DEBUG
      Serial.println("SD succeeded");
    #endif
  }

  tmrpcm.quality(1); // set the audio quality 

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
    // play dial tone when receiver has been lifted and no number has been dialled 
    if (counter == 0) {
      tmrpcm.setVolume(4); // set the dial tone volume
      tmrpcm.play("dial.wav"); // play dial tone
      while(digitalRead(wheelTurnPin) && !digitalRead(hookPin)) { delay(1000); } // play sound until wheel turned 
    }
   
    if (initRead == LOW) {  // if the rotary wheel is turning 
      tmrpcm.stopPlayback(); // stop playing the dial tone
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
            #ifdef DEBUG
              Serial.println(0);
            #endif
          } else {
            #ifdef DEBUG
              Serial.println(counter);
            #endif
          }
          
          if (counter == 1) {
            tmrpcm.setVolume(5); // set the audio volume
            tmrpcm.play("phonehm.wav"); // play the audio file
            while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
          }
          else if (counter == 2) {
            tmrpcm.setVolume(5);
            tmrpcm.play("alarm.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 3) {
            tmrpcm.setVolume(5);
            tmrpcm.play("calendar.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 4) {
            tmrpcm.setVolume(5);
            tmrpcm.play("maps.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 5) {
            tmrpcm.setVolume(5);
            tmrpcm.play("weather.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 6) {
            tmrpcm.setVolume(5);
            tmrpcm.play("fb.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 7) {
            tmrpcm.setVolume(5);
            tmrpcm.play("ig.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 8) {
            tmrpcm.setVolume(5);
            tmrpcm.play("twitter.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 9) {
            tmrpcm.setVolume(5);
            tmrpcm.play("snap.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          }
          else if (counter == 10) {
            tmrpcm.setVolume(5);
            tmrpcm.play("sorry.wav");
            while(!digitalRead(hookPin)) { delay(1000); } 
          } 
        }
        counter = 0; // reset the counter
    }
  }  
  else { // receiver replaced on handset 
    tmrpcm.stopPlayback(); // stop playing audio 
    #ifdef DEBUG
      Serial.println("Receiver Replaced");
    #endif
  }
}
