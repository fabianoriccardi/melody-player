#include "melody_manager.h"

int buzzerPin = D6;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  SPIFFS.begin();
  
  Melody mel;
  mel.load("/melody3.txt");
  Serial.println(String("Title:") + mel.getTitle());
  Serial.println(String("Tempo:") + mel.getTempo());
  mel.playSync();
}

void loop() {}
