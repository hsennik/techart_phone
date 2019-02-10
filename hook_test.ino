// Rotary Phone Hacking
// Tech Art Mini Project 
// January 28 

// Hook test 

// DEFINES
#define DEBUG

// CONSTANTS
// configuration 
const byte hookPin = 10; // detect receiver lift

void setup() {
  // put your setup code here, to run once:
  
  // start the serial connection 
  Serial.begin(9600);
  Serial.println(F("Serial connection started"));

  pinMode(hookPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // check if receiver lifted off the handset
  int hookValue = digitalRead(hookPin);

  Serial.print("Hook Value: ");
  Serial.print(hookValue);
  Serial.print("\n");

  if(hookValue == 0) {

    // print debug info 
    #ifdef DEBUG
      Serial.println("Receiver Lifted");
    #endif
  }
  else if(hookValue == 1) {
    
    // print debug info 
    #ifdef DEBUG
      Serial.println("Receiver Replaced");
    #endif
    
  }
}
