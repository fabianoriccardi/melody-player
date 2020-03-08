/**
 * Load an RTTTL melody from file
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = D6;

String melodyFilePath = "/pokemon.rtttl";

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println();
  Serial.println("Melody Player - Load RTTTL Melody from file");
  
  // Remember of init the filesystem before loading a melody
  SPIFFS.begin();

  Serial.print("Loading melody... ");
  Melody melody = MelodyFactory.loadRtttl(melodyFilePath);
  if(!melody) {
    Serial.println("Melody is not valid");
    while(1) delay(1000);
  }
  Serial.println("Done!");

  player.playAsync(melody);
}

void loop() {

}
