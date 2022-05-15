/**
 * Play multiple melodies at the same time on different buzzers.
 *
 * NOTE: To use this sketch on ESP32 you need to specify the LEDC channel in MelodyPlayer
 * constructor. For more information visit the wiki:
 * https://github.com/fabianoriccardi/melody-player/wiki/Limitations-of-multi-buzzer-configuration
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

bool end;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("Melody Player - Play melodies simultaneouly");

  Serial.print("Loading melodies... ");
  String notes1[] = { "C4", "G3", "G3", "A3", "G3", "SILENCE", "B3", "C4" };
  Melody melody1 = MelodyFactory.load("Nice Melody", 250, notes1, 8);
  int notes2[] = { 500, 1000, 0, 2000 };
  Melody melody2 = MelodyFactory.load("Raw frequencies", 400, notes2, 4);
  Serial.println("Done!");

  Serial.print("Start playing... ");
  player1.playAsync(melody1);
  player2.playAsync(melody2);
  end = false;
}

void loop() {
  if (!player1.isPlaying() && !player2.isPlaying()) {
    if (!end) {
      end = true;
      Serial.println("The end!");
    }
  }
  delay(10);
}
