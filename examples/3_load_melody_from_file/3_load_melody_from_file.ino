/**
 * Melody Player library can load melodies from file.
 * On ESP8266/ESP32 there is an integrated flash memory (sometimes referred as SPIFFS),s
 * which can read and write files like a traditional file system.
 * 
 * Remember to upload the "data" folder on the MCU through ESPxx Sketch Data Upload plugin.
 * On ESP8266:
 * https://github.com/esp8266/arduino-esp8266fs-plugin
 * On ESP32:
 * https://github.com/me-no-dev/arduino-esp32fs-plugin
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = D6;

// This file do not exist
String missingMelodyFilePath = "missing.mel";
String melodyFilePath = "/jingle.mel";

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("Melody Player - Load Melody from File");
  
  // Remember of init the filesystem before loading a melody
  SPIFFS.begin();

  Serial.println("Loading melody...");
  Melody missingMelody = MelodyFactory.load(melodyFilePath);
  // Check if the melody was successfully loaded
  if(!missingMelody){
    Serial.println(missingMelodyFilePath + " is really missing, trying to load another one....");
  }

  // Load and play a correct melody
  Melody melody = MelodyFactory.load(melodyFilePath);
  if(melody){
    Serial.println(melodyFilePath + " loaded!");
  } else {
    Serial.println("error");
    while(1) delay(1000);
  }
  Serial.println(String(" Title: ") + melody.getTitle());
  Serial.println(String(" Tempo: ") + melody.getTempo() + " milliseconds");
  Serial.println(String(" Number of notes: ") + melody.getLenght());
  
  Serial.print("Play in blocking mode...");
  player.play(melody);
  Serial.println("The end!");
}

void loop() {}
