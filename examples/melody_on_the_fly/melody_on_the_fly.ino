/**
 * Play a simple melody hardcoded in the sketch.
 */
#include <melody_player.h>
#include <melody_factory.h>

#ifdef ESP8266
int buzzerPin = D6;
#else
int buzzerPin = 14;
#endif

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Loading melody...");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  // Load and play a correct melody
  Melody melody = MelodyFactory.load("Nice Melody", 175, notes, 8);
  
  Serial.println(String(" Title:") + melody.getTitle());
  Serial.println(String(" Tempo:") + melody.getTempo());
  player.play(melody);
  Serial.println("The end!");
}

void loop() {}
