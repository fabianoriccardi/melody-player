/**
 * Melody Player library can load melodies from file.
 *
 * REMEMBER to upload the file system containing the files in the "data" directory before running
 * this sketch. On ESP8266 and ESP32 there is an embedded portion of flash memory to store files and
 * folders organized with a file system, usually LittleFS or SPIFFS. You can upload files through
 * specific plugins. For more info, look at:
 * - On ESP8266: https://github.com/esp8266/arduino-esp8266fs-plugin
 * - On ESP32: https://github.com/me-no-dev/arduino-esp32fs-plugin
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = 4;

// This file does not exist
String missingMelodyFilePath = "/missing.mel";
String melodyFilePath = "/jingle.mel";

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("Melody Player - Load melody from file");

  // Remember to init the filesystem before loading a melody
  SPIFFS.begin();

  Serial.println(String("Loading melody ") + missingMelodyFilePath + "...");
  Melody missingMelody = MelodyFactory.load(missingMelodyFilePath);
  // Check if the melody was successfully loaded
  if (!missingMelody) {
    Serial.println(missingMelodyFilePath + " not found, try to load another one...");
  }

  // Load and play an existing melody
  Serial.println(String("Loading melody ") + melodyFilePath + "...");
  Melody melody = MelodyFactory.load(melodyFilePath);
  if (melody) {
    Serial.println(melodyFilePath + " loaded!");
  } else {
    Serial.println("error");
    while (1) delay(1000);
  }
  Serial.println(String(" Title: ") + melody.getTitle());
  Serial.println(String(" Time unit: ") + melody.getTimeUnit() + " milliseconds");
  Serial.println(String(" Number of notes: ") + melody.getLength());

  Serial.print("Start playing in blocking mode... ");
  player.play(melody);
  Serial.println("Melody ends!");
}

void loop() {}
