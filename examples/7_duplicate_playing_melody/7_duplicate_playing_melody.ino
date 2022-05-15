/**
 * You can start a melody on a buzzer and during the play you can duplicate and
 * play it on another buzzer.
 *
 * This can be depicted as follow:
 *
 * Player A ---OOOOOOOOOOOOOOOOOOOOOOOOOOOO-----
 * Player B ----------------------OOOOOOOOO-----
 *            ^                  ^
 *            |                  |
 *     playerA.play()     playerA.duplicateMelodyTo(playerB)
 *
 * where dash represents silence and 'O' is sound.
 * NOTE that the state of the player (playing/stopped/paused)
 * is duplicated to the new player. For example, if the melody
 * is playing, it continues seamlessly on the new buzzer.
 * After the duplication, the 2 melodies are indipendent,
 * hence you can stop/pause/start one without affecting the other.
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin1 = 4;
int buzzerPin2 = 5;

MelodyPlayer player1(buzzerPin1);

#ifdef ESP32
MelodyPlayer player2(buzzerPin2, 2);
#else
MelodyPlayer player2(buzzerPin2);
#endif

unsigned long start = 0;
bool transfered = false;
bool end = false;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("Melody Player - Duplicate playing melody");

  Serial.print("Loading melody... ");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody = MelodyFactory.load("Nice Melody (slow)", 700, notes, 8);
  Serial.println("Done!");

  Serial.println("Playing...");

  player1.playAsync(melody);
  start = millis();
}

void loop() {
  if (millis() - start > 2000 && !transfered) {
    transfered = true;
    Serial.println("Continuing the melody playback on both buzzers...");
    player1.duplicateMelodyTo(player2);
  }

  if (transfered && !player2.isPlaying() && !end) {
    end = true;
    Serial.println("The end!");
  }
}
