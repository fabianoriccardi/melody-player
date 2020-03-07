/**
 * Melody can be started from a buzzer, but during play
 * it can be "moved" seamlessly to another buzzer.
 * 
 * This can be depicted as follow:
 * 
 * Player A ---OOOOOOOOOOOOOOOOOOO--------------
 * Player B ----------------------OOOOOOOOO-----
 *            ^                  ^
 *            |                  |
 *     playerA.play()     playerA.transferMelodyTo(playerB)
 * 
 * where dash is silence and O is melody.
 * NOTE that the "source" player is always stopped after
 * a transfer and it loses its melody, hence successive playAsync()
 * won't have any effects (call instead playAsync(melody)).
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin1 = D5;
int buzzerPin2 = D6;

MelodyPlayer player1(buzzerPin1);
MelodyPlayer player2(buzzerPin2);

unsigned long start = 0;
bool transfered = false;
bool end = false;
 
void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("Melody Player - Transfer Playing Melody");
  
  Serial.print("Loading melody... ");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody = MelodyFactory.load("Nice Melody", 700, notes, 8);
  Serial.println("Done!");
  
  Serial.println("Playing...");

  player1.playAsync(melody);
  start = millis();

  // NOTE: On ESP8266, a long delay(..) affects the timing of the melody.
  // This library makes use of "scheduled function" i.e. the played code
  // run in CONT/USER "thread" and delay() blocks this thread.
  // 
  // HENCE, IF YOU ARE PLAY A MELODY, DO NOT WRITE THIS ON ESP8266:
  // delay(2000);
}

void loop() {
  if(millis() - start > 2000 && !transfered) {
    transfered = true;
    Serial.println("Continue to play the melody on a different buzzer...");
    player1.transferMelodyTo(player2);
  }
  
  if(transfered && !player2.isPlaying() && !end) {
    end = true;
    Serial.println("The end!");
  }
}
