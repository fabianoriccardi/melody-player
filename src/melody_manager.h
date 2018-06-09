#ifndef MELODY_MANAGER_H
#define MELODY_MANAGER_H

#include <Arduino.h>
#include <FS.h>

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
 * This class aims to manage the melody in term of memory/storage and playing
 */
class Melody{
public:
  Melody(int pin = -1): pin(pin), nNotes(0), tempo(0){};

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

  void setPin(int pin){
    this->pin=pin;
  }

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
  int pin=-1;
  NoteDuration *nd;
  String title;
  int nNotes;
  int maxLength = 1000;
  int tempo;
  /**
   * 0, never played
   * 1, playing
   * 2, stopped
   */
  int state = 0;
  const int debug = 0;

  friend void changeFreq(Melody* melody);

  /**
   * Check is there the condition to play the melody, that is:
   *  - a melody is loaded
   *  - the pin is set
   */
  bool checkReadyness();
};

#endif
