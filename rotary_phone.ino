// Rotary Phone Hacking
// Tech Art Mini Project 
// January 28 
// Aidan and Hannah 

// DEFINES
#define DEBUG

// INCLUDES
#include <SPI.h> // SPI interface
#include <SD.h> // SD card 

#include <TMRpcm.h>

// CONSTANTS
// configuration 
const byte phonePin = 9; // send audio to phone from SD card 
const byte hookPin = 8; // detect receiver lift
const byte chipSelectPin = 10; // chip select 
const unsigned long debounceDelay = 5; // ms

const unsigned long maxPulseInterval = 250 // ms
const int numDigitsInPhoneNumber = 1; // look for one dialed number 

// GLOBALS
TMRpcm tmrpcm;
char number[numDigitsInPhoneNumber + 1]; // char rep of number dialled
int currentDigit; // digit number currently being dialled 
int pulseCount; // number of pulses detected for current digit (ie. 1 pulse for number 1, 5 pulses for number 5)
typedef enum { ON_HOOK, OFF_HOOK, DIALLING, CONNECTED } stateType; // telephone states
stateType state = ON_HOOK; // assume handset starts on hook 
int previousPinReading = HIGH; // keep track of last "pulse" pin reading 
unsigned long timePinChanged; // time when pin last changed value 
unsigned long now = millis (); // current time 

void setup() {
  // put your setup code here, to run once:
  pinMode(phonePin, INPUT_PULLUP); // change to output when send audio signal down 
  pinMode(hookPin, INPUT_PULLUP);

  // start the serial connection 
  Serial.begin(9600);
  Serial.println(F("Serial connection started"));

  // Open the connection to the SD card
  if(!SD.begin(chipSelectPin)) {
    Serial.println("SD card initialization failed!");
    return;
  }

  // volume range from 0 to 7 (play around with volume)
  tmrpcm.setVolume(4);
  // enable 2x oversampling (quality of audio)
  tmrpcm.quality(1);

  Serial.println("Setup complete");
}

void loop() {
  // put your main code here, to run repeatedly:

  // check if receiver lifted off the handset
  int hookValue = digitalRead(hookPin);

  if(hookValue == 0 && state == ON_HOOK) {

    // print debug info 
    #ifdef DEBUG
      Serial.println("Receiver Lifted");
    #endif

    // update phone state
    state = OFF_HOOK; // phone has been lifted and is ready to be dialed 
  }
  else if(hookValue == 1 && state != ON_HOOK) {

     // print debug info 
     #ifdef DEBUG
      Serial.println("Receiver Replaced");
     #endif

     // update phone state 
     state = ON_HOOK;

     // clear number dialled information 
     pulseCount = 0;
     currentDigit = 0;

     // stop playing audio 
     tmrpcm.stopPlayback();

     // pin mode should be back in input state (would be in output state for playing audio)
     pinMode(phonePin, INPUT_PULLUP);
  }
  if (state == OFF_HOOK || state == DIALLING) {
    
    // record current timestamp
    now = millis();

    // test value of phone pin 
    int pinReading = digitalRead(phonePin);

    // if value has changed, person is dialling a number
    if (pinReading != previousPinReading) {

      state = DIALLING;

      // use debouncing to be confident that there actually is a change 
      if (now - timePinChanged < debounceDelay) {
        return;
      }

      if (pinReading == HIGH) {
        // dialling pulse detected
        pulseCount++; // count the number of pulses to determine numbers dialled
      }

      timePinChanged = now;
      previousPinReading = pinReading;
    }

    if (((now - timePinChanged) >= maxPulseInterval) && pulseCount > 0) {

      if (currentDigit < numDigitsInPhoneNumber) {
        if (pulseCount == 10) { pulseCount = 0;}

        #ifdef DEBUG
          Serial.print(F("Digit dialled: "));
          Serial.println(pulseCount);
        #endif

        number[currentDigit] = pulseCount | '0';

        currentDigit++;

        number[currentDigit] = 0;
      }

      if(currentDigit == numDigitsInPhoneNumber) {

        #ifdef DEBUG
          Serial.print(F("Number dialled: "));
          Serial.println(number);
        #endif

        if(strcmp(number,"0") == 0) {
          #ifdef DEBUG 
            Serial.println(F("0 dialled"));
          #endif

          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("0_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"1") == 0) {
          #ifdef DEBUG
            Serial.println(F("1 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("1_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"2") == 0) {
          #ifdef DEBUG
            Serial.println(F("2 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("2_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"3") == 0) {
          #ifdef DEBUG
            Serial.println(F("3 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("3_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"4") == 0) {
          #ifdef DEBUG
            Serial.println(F("4 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("4_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"5") == 0) {
          #ifdef DEBUG
            Serial.println(F("5 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("5_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"6") == 0) {
          #ifdef DEBUG
            Serial.println(F("6 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("6_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"7") == 0) {
          #ifdef DEBUG
            Serial.println(F("7 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("7_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"8") == 0) {
          #ifdef DEBUG
            Serial.println(F("8 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("8_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        else if(strcmp(number,"9") == 0) {
          #ifdef DEBUG
            Serial.println(F("9 dialled"));
          #endif
  
          pinMode(phonePin,OUTPUT); // set the pin as output to play sound 
          tmrpcm.speakerPin = 9; // set the pin for output
          tmrpcm.play("9_file"); // play the sound file 
          while(!digitalRead(hookPin)) { delay(1000); } // play sound until receiver replaced 
        }

        state = CONNECTED; // puzzle state changed 
      }

    pulseCount = 0; // reset pulse counter for next digit
    }
  }
}
