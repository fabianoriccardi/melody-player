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
  int frequency;
  int duration;
};

class Melody{
public:
  Melody(): nNotes(0), tempo(0){};

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
  
private:
  NoteDuration *nd;
  String title;
  int nNotes;
  int tempo;
  int debug=0;
};

#endif
