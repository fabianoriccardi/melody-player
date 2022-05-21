/**
 * Play a simple melody hard-coded in the sketch.
 *
 * You can observe the difference between the blocking play(..),
 * which blocks the sketch for the entire duration of the melody, and
 * playAsync(..) which returns immediately.
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = 4;

// specify the buzzer's pin and the standby voltage level
MelodyPlayer player(buzzerPin, HIGH);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("Melody Player - Simple play (blocking vs non-blocking play)");

  Serial.println("Loading melody...");
  const int nNotes = 8;
  String notes[nNotes] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  const int timeUnit = 175;
  // create a melody
  Melody melody = MelodyFactory.load("Nice Melody", timeUnit, notes, nNotes);

  // get basic info about the melody
  Serial.println(String(" Title:") + melody.getTitle());
  Serial.println(String(" Time unit:") + melody.getTimeUnit());

  Serial.print("Start playing in blocking mode... ");
  player.play(melody);
  Serial.println("Melody ends!");

  delay(1000);

  Serial.print("Start playing in non-blocking mode...");
  player.playAsync(melody);
  Serial.println(" Melody is playing!");
}

void loop() {}
