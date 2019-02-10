// Rotary Phone Hacking
// Tech Art Mini Project 
// January 28 

// Dial test 

// DEFINES
#define DEBUG

#define INITPIN 12
#define NUMPIN 11

void setup() {
  // put your setup code here, to run once:
  
  // start the serial connection 
  Serial.begin(9600);
  Serial.println(F("Serial connection started"));

  pinMode(INITPIN, INPUT_PULLUP);
  pinMode(NUMPIN, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  int initRead = digitalRead (INITPIN);
  Serial.println(initRead);
}
