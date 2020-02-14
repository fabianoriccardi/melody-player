#ifndef MELODY_H
#define MELODY_H

#include <Arduino.h>
#include <memory>

/**
 * This struct represents the couple between a note and 
 * its duration than respect to the "tempo" 
 */
struct NoteDuration {
  unsigned short frequency;
  unsigned short duration;
};

/**
 * This class stores the data of a melody. To eases the creation of a melody,
 * MelodyFactory class is provided.
 */
class Melody {
public:
  Melody():
   title(""), tempo(0), notes(nullptr), counter(0) {};
  
  Melody(String title, unsigned short tempo, std::shared_ptr<std::vector<NoteDuration>> notes):
    title(title), tempo(tempo), notes(notes), counter(0) {};

  /**
   * Return the title of the melody.
   */
  String getTitle() const {
    return title;
  };

  /**
   * Return the default tempo.
   */ 
  unsigned short getTempo() const {
    return tempo;
  };

  /**
   * Get number of notes.
   */
  unsigned short getLength() const {
    if(notes == nullptr) return 0;
    return (*notes).size();
  }

  /**
   * Get a note to be reproduced.
   * Check length or validity of melody before call this method.
   */
  NoteDuration getNote() {
    return (*notes)[counter++];
  };

  /**
   * Position the inner pointer to the first note.
   */
  void reset() {
    counter = 0;
  }

  /**
   * Tell if the object contains a valid melody.
   */
  bool isValid() const {
    return notes != nullptr && (*notes).size() != 0;
  }

  /**
   * Implement cast to bool, so you can do 'if(melody){...}'.
   */
  operator bool() const {
    return isValid();
  }
  
private:
  String title;
  unsigned short tempo;
  std::shared_ptr<std::vector<NoteDuration>> notes;
  const static unsigned short maxLength = 1000;

  unsigned short counter;
  
  const static unsigned short debug = 0;
};

#endif // END MELODY_H
