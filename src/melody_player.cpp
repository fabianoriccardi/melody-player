#include "melody_player.h"
#include <Ticker.h>

void MelodyPlayer::play(){
  melody.reset();

  playing = true;
  setupPin();

  for(int i = 0; i < melody.getLength(); i++){
  	NoteDuration note(melody.getNote());
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

static Ticker ticker; 

/**
 * NOTE: this is the function executed in the Ticker, aka the Thread on ESP* platform.
 * So you can decide to use delay(..) function without blocking the others threads 
 * (like the main one dedicated to loop() function).
 */
void changeTone(MelodyPlayer* player){
  static int i = 0;
  if(i < player->melody.getLength()){
  	NoteDuration note(player->melody.getNote());
    if(player->debug) Serial.println(String("Playing async: freq=") + note.frequency + " dur=" + note.duration + " iteration=" + i);
#ifdef ESP32
    ledcWriteTone(player->pwmChannel, note.frequency);
    delay(player->melody.getTempo() * note.duration);
    ledcWriteTone(player->pwmChannel, 0);
    ticker.once_ms(0.3f * note.duration * player->melody.getTempo(), changeTone, player);
#else
    tone(player->pin, note.frequency, player->melody.getTempo() * note.duration);
    ticker.once_ms(1.3f * note.duration * player->melody.getTempo(), changeTone, player);
#endif
    i++;
  } else {
    i = 0;
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
  melody.reset();

  playing = true;
  setupPin();
  
  // Start immediately
  ticker.once(0, changeTone, this);
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
