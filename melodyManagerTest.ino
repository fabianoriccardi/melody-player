#include "melody_manager.h"

int buzzerPin = D6;

String melodyFilePath = "/melody2.txt";

void setup() {
  Serial.begin(115200);
  while(!Serial);

  // Remember of init the filesystem by yourself
  SPIFFS.begin();
  
  Melody mel(buzzerPin);
  // Alternatively, you can call set the buzzer pin after the object construcion
  mel.setPin(buzzerPin);
  
  mel.load(melodyFilePath);
  Serial.println(String("Title:") + mel.getTitle());
  Serial.println(String("Tempo:") + mel.getTempo());
  mel.playSync();
  Serial.println("Sync playing end!");

  delay(500);
  
  Serial.print("Calling the async play... ");
  mel.playAsync();
  Serial.println("Immediate return!");
  while(mel.isPlaying()){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nEnd async playing!");
}

void loop() {}

