/**
 * Load an RTTTL melody from hardcoded literal string and from file.
 */
#include <melody_player.h>
#include <melody_factory.h>

int buzzerPin = 4;

String melodyFilePath = "/pokemon.rtttl";

const char melodyString[] = "Pokemon:d=16,o=5,b=112:32p,f,a#,c6,c#6,c6,c#6,d#6,2f6,a#,c6,8c#6,8f6,"
                            "8d#6,32c#.6,32d#.6,32c#.6,8c6,8g#.,f,a#,c6,c#6,c6,c#6,d#6,2f6,8a#,c#6,"
                            "8f6,a,d#6,4g#6";

MelodyPlayer player(buzzerPin);

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  Serial.println();
  Serial.println("Melody Player - Load RTTTL melody");

  Serial.print("Loading hardcoded melody... ");
  Melody melody2 = MelodyFactory.loadRtttlString(melodyString);
  if (melody2) {
    Serial.println("Done!");
    Serial.print("Playing... ");
    player.play(melody2);
    Serial.println("Done!");
  } else {
    Serial.println("Error");
  }

  delay(1000);
  Serial.println();

  // Remember to init the filesystem before loading a melody
  SPIFFS.begin();

  Serial.print("Loading melody from file... ");
  Melody melody = MelodyFactory.loadRtttlFile(melodyFilePath);
  if (!melody) {
    Serial.println("Error");
  } else {
    Serial.println("Done!");
    Serial.print("Playing... ");
    player.play(melody);
    Serial.println("Done!");
  }
}

void loop() {}
