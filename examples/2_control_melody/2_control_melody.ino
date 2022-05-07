/**
 * A melody can be played, paused or stopped.
 * To stop or pause a melody, it must be started with playAsync(..) method.
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = 4;

MelodyPlayer player(buzzerPin);

unsigned long startPlay = millis();
int state = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("Melody Player - Control melody");

  Serial.println("Loading melody...");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  // Create a melody
  Melody melody = MelodyFactory.load("Nice Melody", 250, notes, 8);

  player.playAsync(melody);
  startPlay = millis();
  Serial.print(melody.getTitle() + " is playing...");
}

void loop() {
  if (millis() - startPlay > 1000 && state == 0) {
    state++;
    // Stop after 1 second since playAync(..)
    player.stop();
  } else if (millis() - startPlay > 2000 && state == 1) {
    state++;
    // The melody was stopped, hence it starts from the begin.
    player.playAsync();
    // Let it play until the end
  } else if (!player.isPlaying() && state == 2) {
    state++;

    // A small delay before restarting the melody
    delay(1000);

    // The melody has reached the end and it is automatically stopped,
    // hence the melody restarts from the begin
    player.playAsync();
    startPlay = millis();
  } else if (millis() - startPlay > 1400 && state == 3) {
    state++;
    // Pause after 1.5s
    player.pause();
  } else if (millis() - startPlay > 2000 && state == 4) {
    state++;
    // Continue to playing from where it was paused
    player.playAsync();
  }

  Serial.println(String("Playing: ") + (player.isPlaying() ? "true" : "false"));
  delay(50);
}
