/**
 * Play a simple melody from SPIFFS.
 * Remember to upload the "data" folder on the MCU. If you are using ESP8266,
 * you can use this plugin:
 * https://github.com/esp8266/arduino-esp8266fs-plugin
 */
#include <melody_player.h>
#include <melody_factory.h>

#ifdef ESP8266
int buzzerPin = D6;
#else
int buzzerPin = 14;
#endif

// This file do not exist
String melodyFilePath = "/melody3.txt";

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  // Remember of init the filesystem before loading a melody
  SPIFFS.begin();

  Serial.println("Loading melody...");
  // Load and play a correct melody
  Melody melody = MelodyFactory.load(melodyFilePath);
  
  Serial.println(String(" Title:") + melody.getTitle());
  Serial.println(String(" Tempo:") + melody.getTempo());
  player.play(melody);
  Serial.println("The end!");
}

void loop() {}
