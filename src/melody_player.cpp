#include "melody_player.h"

/**
 * https://stackoverflow.com/questions/24609271/errormake-unique-is-not-a-member-of-std
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

void MelodyPlayer::play(){
  if(melodyState == nullptr) {
    return;
  }

  turnOn();
  state = State::PLAY;

  for(int i = 0; i < melodyState->melody.getLength(); i++){
  	NoteDuration note(melodyState->melody.getNote(i));
    if(debug) Serial.println(String("Playing: frequency:") + note.frequency + " duration:" + note.duration);
#ifdef ESP32
    ledcWriteTone(pwmChannel, note.frequency);
    delay(melodyState->melody.getTempo() * note.duration);
    ledcWriteTone(pwmChannel, 0);
    delay(0.3f * (melodyState->melody.getTempo() * note.duration));
#else
    tone(pin, note.frequency, melodyState->melody.getTempo() * note.duration);
    delay(1.3f * (melodyState->melody.getTempo() * note.duration));
#endif
  }
  state = State::STOP;

  turnOff();
}

void MelodyPlayer::play(Melody& melody){
    if(!melody){
        return;
    }
    melodyState = make_unique<MelodyState>(melody);
    play();
}

void changeTone(MelodyPlayer* player) {
  if(player->melodyState->index < player->melodyState->melody.getLength()) {
  	NoteDuration note(player->melodyState->melody.getNote(player->melodyState->index));
    int noteDur = player->melodyState->melody.getTempo() * note.duration;
    if(player->debug) Serial.println(String("Playing async: freq=") + note.frequency + " dur=" + note.duration + " iteration=" + player->melodyState->index);

    if(player->melodyState->silence) {
#ifdef ESP32
      ledcWriteTone(player->pwmChannel, 0);
#else
      tone(player->pin, 0);
#endif
      player->melodyState->index++;
      player->melodyState->silence = false;

      float duration = 0.3f * noteDur;
      player->supportSemiNote = millis() + duration;
      player->ticker.once_ms(duration, changeTone, player);
#ifdef ESP32
      player->ticker.once_ms(duration, changeTone, player);
#else
      player->ticker.once_ms_scheduled(duration, std::bind(changeTone, player));
#endif
    } else {
#ifdef ESP32
      ledcWriteTone(player->pwmChannel, note.frequency);
#else
      tone(player->pin, note.frequency);
#endif
      player->melodyState->silence = true;

      float duration = 1.0f * noteDur;
      player->supportSemiNote = millis() + duration;
#ifdef ESP32
      player->ticker.once_ms(duration, changeTone, player);
#else
      player->ticker.once_ms_scheduled(duration, std::bind(changeTone, player));
#endif
    }
  } else {
    player->melodyState->index = 0;
    player->state = MelodyPlayer::State::STOP;
   
    player->turnOff();
  }
}

void MelodyPlayer::playAsync(){
  if(melodyState == nullptr) {
    return;
  }

  turnOn();
  state = State::PLAY;

  // Start immediately
#ifdef ESP32
  ticker.once(0, changeTone, this);
#else
  ticker.once_scheduled(0, std::bind(changeTone, this));
#endif
}

void MelodyPlayer::playAsync(Melody& melody){
  if(!melody){
    return;
  }
  melodyState = make_unique<MelodyState>(melody);
  playAsync();
}

void MelodyPlayer::stop(){
  if(melodyState == nullptr) {
    return;
  }

  stopPlay();
  melodyState->reset();
}

void MelodyPlayer::transferMelodyTo(MelodyPlayer& destPlayer){
  if(melodyState == nullptr) {
    return;
  }

  destPlayer.stop();

  if(isPlaying()) {
    stopPlay();
    melodyState->saveCurrentState(supportSemiNote);

    destPlayer.melodyState = std::move(melodyState);
    destPlayer.playAsync();
  } else {
    stopPlay();
    melodyState->saveCurrentState(supportSemiNote);

    destPlayer.melodyState = std::move(melodyState);
    destPlayer.state = state;
  }
}

void MelodyPlayer::duplicateMelodyTo(MelodyPlayer& destPlayer){
  if(melodyState == nullptr) {
    return;
  }

  destPlayer.stop();
  destPlayer.melodyState = make_unique<MelodyState>(*(this->melodyState));
  destPlayer.melodyState->saveCurrentState(supportSemiNote);

  if(isPlaying()) {
    destPlayer.playAsync();
  } else {
    destPlayer.state = state;
  }
}

#ifdef ESP32
MelodyPlayer::MelodyPlayer(unsigned char pin, unsigned char pwmChannel):
    pin(pin), pwmChannel(pwmChannel), state(State::STOP), melodyState(nullptr) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
};
#else
MelodyPlayer::MelodyPlayer(unsigned char pin):
    pin(pin), state(State::STOP), melodyState(nullptr) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
};
#endif

void MelodyPlayer::stopPlay(){
  // Stop player, but do not reset the melodyState
  ticker.detach();
  state = State::STOP;
  turnOff();
}

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
#else
  // Remember that this will set LOW output
  noTone(pin);
#endif

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}
