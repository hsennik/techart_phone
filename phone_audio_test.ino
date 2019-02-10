#include <SD.h>
#define SD_ChipSelectPin 4
#include <TMRpcm.h>
#include <SPI.h>

TMRpcm tmrpcm;

void setup(){
tmrpcm.speakerPin = 9;
Serial.begin(9600);
if (!SD.begin(SD_ChipSelectPin)) {
  Serial.println("SD fail");
  return;
}

Serial.println("SD succeeded");

tmrpcm.setVolume(6);
tmrpcm.quality(1);
tmrpcm.play("phone.wav");
}

void loop(){  }
