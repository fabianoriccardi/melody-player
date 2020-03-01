/**
 * Play a melody on 2 buzzers.
 */
#include <melody_player.h>
#include <melody_factory.h>

#ifndef ESP32
#error "Multiple buzzer are supported only on ESP32"
#endif

int buzzerPin1 = 13;
int pwmChannel1 = 0;
int buzzerPin2 = 14;
int pwmChannel2 = 1;

MelodyPlayer player1(buzzerPin1, pwmChannel1);
MelodyPlayer player2(buzzerPin2, pwmChannel2);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("Melody Player - Multiple Buzzers");
  
  Serial.print("Loading melody... ");
  String notes[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody = MelodyFactory.load("Nice Melody", 250, notes, 8);
  Serial.println("Done!");

  Serial.print("Playing... ");
  player1.playAsync(melody);
  player2.playAsync(melody);

  Serial.println("The end!");
}

void loop() {}
