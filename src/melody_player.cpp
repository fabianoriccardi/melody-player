#include "melody_player.h"

void MelodyPlayer::play(){
  playing = true;
  setupPin();

  for(int i = 0; i < melody.getLength(); i++){
  	NoteDuration note(melody.getNote(i));
    if(debug) Serial.println(String("Playing: frequency:") + note.frequency + " duration:" + note.duration);
#ifdef ESP32
    ledcWriteTone(pwmChannel, note.frequency);
    delay(melody.getTempo() * note.duration);
    ledcWriteTone(pwmChannel, 0);
    delay(0.3f * (melody.getTempo() * note.duration));
#else
    tone(pin, note.frequency, melody.getTempo() * note.duration);
    delay(1.3f * (melody.getTempo() * note.duration));
#endif
  }
  playing = false;

#ifdef ESP32
  ledcWrite(pwmChannel, 0);
  ledcDetachPin(pin);
#endif

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void changeTone(MelodyPlayer* player){
  if(player->melodyIndex < player->melody.getLength()) {
  	NoteDuration note(player->melody.getNote(player->melodyIndex));
    int noteDur = player->melody.getTempo() * note.duration;
    if(player->debug) Serial.println(String("Playing async: freq=") + note.frequency + " dur=" + note.duration + " iteration=" + player->melodyIndex);
#ifdef ESP32
    if(player->silence) {
      ledcWriteTone(player->pwmChannel, 0);
      player->ticker.once_ms(0.3f * noteDur, changeTone, player);
      player->melodyIndex++;
      player->silence = false;
    } else {
      ledcWriteTone(player->pwmChannel, note.frequency);
      player->ticker.once_ms(1.0f * noteDur, changeTone, player);
      player->silence = true;
    }
#else
    tone(player->pin, note.frequency, noteDur);
    player->ticker.once_ms(1.3f * noteDur, changeTone, player);
    player->melodyIndex++;
#endif
  } else {
    player->melodyIndex = 0;
    player->playing = false;
#ifdef ESP32
    ledcWrite(player->pwmChannel, 0);
    ledcDetachPin(player->pin);
#endif
    // Need to avoid to waste energy: the buzzer will consume energy if turned "active"
    pinMode(player->pin, OUTPUT);
    digitalWrite(player->pin, HIGH);
  }
}

void MelodyPlayer::playAsync(){
  playing = true;
  silence = false;
  setupPin();
  
  // Start immediately
  ticker.once(0, changeTone, this);
}

void MelodyPlayer::stop(){
  ticker.detach();
  melodyIndex = 0;
  playing = false;

#ifdef ESP32
  ledcWrite(pwmChannel, 0);
  ledcDetachPin(pin);
#endif

  // Need to avoid to waste energy: the buzzer will consume energy
  // if turned "active".
  // Force pin mode OUTPUT again to overwrite PWM settings
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void MelodyPlayer::setupPin(){
#ifdef ESP32
  int resolution = 8;
  // 2000 is a frequency, it will be changed at the first play
  ledcSetup(pwmChannel, 2000, resolution);
  ledcAttachPin(pin, pwmChannel);
  ledcWrite(pwmChannel, 125);
#endif
}
