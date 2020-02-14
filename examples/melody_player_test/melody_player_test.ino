/**
 * A sketch to test various functionalities of this library.
 */
#include <melody_player.h>
#include <melody_factory.h>

#include <FS.h>

#ifdef ESP8266
int buzzerPin = D6;
#else
int buzzerPin = 14;
#endif

// This file do not exist
String missingFilePath = "/missing.txt";
String wrong1FilePath = "/wrong1.txt";
String melodyFilePath = "/melody3.txt";


MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Melody test;
  if(test){
    Serial.print("valid");
  } else {
    Serial.print("not valid");
  }

  // Remember of init the filesystem by yourself
  SPIFFS.begin();

  Melody missing = MelodyFactory.load(missingFilePath);
  Serial.print(missingFilePath + " is ");
  if(missing){
    Serial.println("valid");
  } else {
    Serial.println("not valid");
  }

  Melody wrong1 = MelodyFactory.load(wrong1FilePath);
  Serial.print(wrong1FilePath + " is ");
  if(wrong1){
    Serial.println("valid");
  } else {
    Serial.println("not valid");
  }

  // Load and play a correct melody
  Melody melody = MelodyFactory.load(melodyFilePath);
  
  Serial.println(String("Title:") + melody.getTitle());
  Serial.println(String("Tempo:") + melody.getTempo());
  player.play(melody);
  Serial.println("Sync playing end!");
  
  delay(500);
  
  Serial.print("Calling the async play... ");
  player.playAsync();
  Serial.println("Immediate return!");
  while(player.isPlaying()){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nEnd async playing!");
}

void loop() {}
