/**
 * Play a simple melody directly hardcoded in the sketch.
 * 
 * You can observe the difference between the blocking play(..) method,
 * which block the sketch for the entire duration of the melody, and
 * playAsync(..) which returns immediately.
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = 4;

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("Melody Player - Simple Play (blocking vs non-blocking play");
  
  Serial.println("Loading melody...");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  // Load and play a correct melody
  Melody melody = MelodyFactory.load("Nice Melody", 175, notes, 8);
  
  Serial.println(String(" Title:") + melody.getTitle());
  Serial.println(String(" Time unit:") + melody.getTimeUnit());
  Serial.print("Play in blocking mode...");
  player.play(melody);
  Serial.println("The end!");

  delay(1000);

  Serial.print("Play in non-blocking mode...");
  player.playAsync(melody);
  Serial.println(" not the end!");
}

void loop() {}
