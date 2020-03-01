#include "melody_player.h"

void MelodyPlayer::play(){
  turnOn();
  playing = true;

  for(int i = 0; i < melodyState.melody.getLength(); i++){
  	NoteDuration note(melodyState.melody.getNote(i));
    if(debug) Serial.println(String("Playing: frequency:") + note.frequency + " duration:" + note.duration);
#ifdef ESP32
    ledcWriteTone(pwmChannel, note.frequency);
    delay(melodyState.melody.getTempo() * note.duration);
    ledcWriteTone(pwmChannel, 0);
    delay(0.3f * (melodyState.melody.getTempo() * note.duration));
#else
    tone(pin, note.frequency, melodyState.melody.getTempo() * note.duration);
    delay(1.3f * (melodyState.melody.getTempo() * note.duration));
#endif
  }
  playing = false;

  turnOff();
}

void MelodyPlayer::play(Melody& melody){
    if(!melody){
        return;
    }
    melodyState = MelodyState(melody);
    play();
}

void changeTone(MelodyPlayer* player) {
  if(player->melodyState.index < player->melodyState.melody.getLength()) {
  	NoteDuration note(player->melodyState.melody.getNote(player->melodyState.index));
    int noteDur = player->melodyState.melody.getTempo() * note.duration;
    if(player->debug) Serial.println(String("Playing async: freq=") + note.frequency + " dur=" + note.duration + " iteration=" + player->melodyState.index);
#ifdef ESP32
    if(player->melodyState.silence) {
      ledcWriteTone(player->pwmChannel, 0);
      player->melodyState.index++;
      player->melodyState.silence = false;
      player->ticker.once_ms(0.3f * noteDur, changeTone, player);
    } else {
      ledcWriteTone(player->pwmChannel, note.frequency);
      player->melodyState.silence = true;
      player->ticker.once_ms(1.0f * noteDur, changeTone, player);
    }
#else
    tone(player->pin, note.frequency, noteDur);
    player->ticker.once_ms(1.3f * noteDur, changeTone, player);
    player->melodyState.index++;
#endif
  } else {
    player->melodyState.index = 0;
    player->playing = false;
   
    player->turnOff();
  }
}

void MelodyPlayer::playAsync(){
  playing = true;

#ifdef ESP32
  melodyState.silence = false;
#endif
  
  turnOn();
  
  // Start immediately
  ticker.once(0, changeTone, this);
}

void MelodyPlayer::playAsync(Melody& melody){
  if(!melody){
    return;
  }
  melodyState = MelodyState(melody);
  playAsync();
}

void MelodyPlayer::stop(){
  ticker.detach();
  melodyState.index = 0;
  playing = false;
  turnOff();
}

#ifdef ESP32
MelodyPlayer::MelodyPlayer(unsigned char pin, unsigned char pwmChannel):
    pin(pin), pwmChannel(pwmChannel), playing(false) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
};
#else
MelodyPlayer::MelodyPlayer(unsigned char pin):
    pin(pin), playing(false) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
};
#endif


void MelodyPlayer::turnOn(){
#ifdef ESP32
  const int resolution = 8;
  // 2000 is a frequency, it will be changed at the first play
  ledcSetup(pwmChannel, 2000, resolution);
  ledcAttachPin(pin, pwmChannel);
  ledcWrite(pwmChannel, 125);
#endif
}

void MelodyPlayer::turnOff() {
#ifdef ESP32
  ledcWrite(pwmChannel, 0);
  ledcDetachPin(pin);
#endif

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}
