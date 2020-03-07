/**
 * While a melody is playing, it can be duplicated to
 * another buzzer. This can be depicted as follow:
 * 
 * Player A ---OOOOOOOOOOOOOOOOOOOOOOOOOOOO-----
 * Player B ----------------------OOOOOOOOO-----
 *            ^                  ^
 *            |                  |
 *     playerA.play()     playerA.duplicateMelodyTo(playerB)
 * 
 * where dash is silence and O is melody.
 * NOTE that even the state of the player (playing, paused, stop)
 * is applied to the new player. Hence, if the melody was playing,
 * it continues seamlessly on the new buzzer.
 * The 2 melodies are indipendent, hence they can be stopped/paused
 * without affecting each other.
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
  Serial.println("Melody Player - Duplicate Playing Melody");
  
  Serial.print("Loading melody... ");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody = MelodyFactory.load("Nice Melody", 700, notes, 8);
  Serial.println("Done!");
  
  Serial.println("Playing...");

  player1.playAsync(melody);
  start = millis();
}

void loop() {
  if(millis() - start > 2000 && !transfered) {
    transfered = true;
    Serial.println("Continue to play the melody on a different buzzer...");
    player1.duplicateMelodyTo(player2);
  }
  
  if(transfered && !player2.isPlaying() && !end) {
    end = true;
    Serial.println("The end!");
  }
}
