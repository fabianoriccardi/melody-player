/**
 * Play 2 different melodies at the same time on 2 buzzers.
 * 
 * This works only with playAsync(), otherwise the melodies 
 * are played sequentially.
 * 
 * For more information about enabling multiple PWM output and
 * limitations of your board, check the wiki.
 * https://github.com/fabiuz7/melody-player-arduino/wiki
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin1 = 4;
int buzzerPin2 = 5;

MelodyPlayer player1(buzzerPin1);
MelodyPlayer player2(buzzerPin2);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("Melody Player - Play Melodies Simultaneouly");
  
  Serial.print("Loading melodies... ");
  String notes1[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody1 = MelodyFactory.load("Nice Melody", 250, notes1, 8);
  int notes2[] = { 500, 1000, 0, 2000 };
  Melody melody2 = MelodyFactory.load("Raw frequencies", 400, notes2, 4);
  Serial.println("Done!");

  Serial.print("Playing... ");
  player1.playAsync(melody1);
  player2.playAsync(melody2);

  Serial.println("The end!");
}

void loop() {}
