/**
 * You can start a melody on a buzzer and during the play
 * you can move it to another buzzer.
 *
 * This can be depicted as follow:
 *
 * Player A ---OOOOOOOOOOOOOOOOOOO--------------
 * Player B ----------------------OOOOOOOOO-----
 *            ^                  ^
 *            |                  |
 *     playerA.play()     playerA.transferMelodyTo(playerB)
 *
 * where dash represents silence and 'O' is sound.
 * NOTE that the "source" player is always stopped after
 * a transfer and it loses its melody, hence successive playAsync()
 * won't restart that melody (call instead playAsync(melody)).
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
  Serial.println("Melody Player - Transfer playing melody");

  Serial.print("Loading melody... ");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody = MelodyFactory.load("Nice Melody (slow)", 700, notes, 8);
  Serial.println("Done!");

  Serial.println("Playing...");

  player1.playAsync(melody);
  start = millis();

  // NOTE: On ESP8266, a long delay(..) affects the timing of the melody.
  // This library makes use of "scheduled function" i.e. the played code
  // run in CONT/USER "thread" and delay() blocks this thread.
  //
  // HENCE, IF YOU ARE PLAYING A MELODY ON ESP8266, DO NOT WRITE THIS:
  // delay(2000);
}

void loop() {
  if (millis() - start > 2000 && !transfered) {
    transfered = true;
    Serial.println("Continuing the melody playback on the second buzzer...");
    player1.transferMelodyTo(player2);
  }

  if (transfered && !player2.isPlaying() && !end) {
    end = true;
    Serial.println("The end!");
  }
}
