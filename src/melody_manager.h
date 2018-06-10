#ifndef MELODY_MANAGER_H
#define MELODY_MANAGER_H

#include <Arduino.h>
#include <FS.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif

#include "pitches_unordered_map.h"

/**
 * This struct represents the couple between a note and 
 * its duration than respect to the "tempo" 
 */
struct NoteDuration{
  short frequency;
  short duration;
};

/**
 * This class aims to manage the melody in term of memory/storage and playing.
 * 
 * The general idea is providing a simple library to manage the melodies. These can be stored
 * on the flash memory to decouple the code from the data. This allows to change the melody
 * without a new compilation. The melody file format is very trivial and not efficient: 
 * in fact, all the data are stored in a human readable way. Still, this format doesn't
 * aim to compete with MIDI format, because it doesn't support multiple streaming nor 
 * precise timestamp (just to make a couple of example). 
 */
class Melody{
public:
  Melody(int pin): pin(pin), nNotes(0), tempo(0){
  	pinMode(pin,OUTPUT);
  	digitalWrite(pin,HIGH);
  };

  /**
   * Load just 3 notes, to test the buzzer,
   * Return always true
   */
  bool loadTest();

  /**
   * Load the melody from file
   * Return true in case of success
   */
  bool load(String filePath);
  
  String getTitle(){
    return title;
  };

  void setTitle(String title){
    this->title=title;
  };

  int getTempo(){
    return tempo;
  };

  void setTempo(int tempo){
    this->tempo=tempo;
  };

  /**
   * Play the melody in a synchrounus (blocking) way.
   */
  void playSync();

  /**
   * Play the music using a thread, or something similar,
   * to melody the melody while performing other action
   */
   void playAsync();

   /**
    * return if the solg is still playing, valid both async and sync
    */
    bool isPlaying();
  
private:
  int pin;
  NoteDuration *nd;
  String title;
  int nNotes;
  int maxLength = 1000;
  int tempo;
  int channel = 0;
  
  /**
   * 0, never played
   * 1, playing
   * 2, stopped
   */
  int state = 0;
  const int debug = 0;

  friend void changeFreq(Melody* melody);

  /**
   * Check if there is the condition to play the melody, that is:
   *  - a melody is loaded
   */
  bool checkReadyness();

  void setupPin();
};

#endif
