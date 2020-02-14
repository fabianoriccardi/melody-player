#ifndef MELODY_FACTORY_H
#define MELODY_FACTORY_H

#include "melody.h"

#include <FS.h>
#ifdef ESP32
#include <SPIFFS.h>
#endif

class MelodyFactoryClass {
public:
  /**
   * Load the melody from file.
   */
  Melody load(String filePath);

  /**
   * Load the melody from code.
   * Notes are represented as string accordigly to english notation (i.e. "C4", "G3", "G6").
   * This method assumes that each note lasts 1 beat.
   */
  Melody load(String title, unsigned short tempo, String notesToLoad[], unsigned short nNotes);

private:
  enum class NoteFormat {ERROR, STRING, INTEGER};
  
  String title;
  unsigned short tempo;
  NoteFormat noteFormat;
  std::shared_ptr<std::vector<NoteDuration>> notes;
  // Used to check how many note are stored in a file.
  unsigned short nNotes;
  const unsigned short maxLength = 1000;

  /**
   * Get title from line.
   * Return true on success.
   */
  bool loadTitle(String line);

  /**
   * Get tempo from line.
   * Return true on success.
   */
  bool loadTempo(String line);

  /**
   * Get number of notes.
   * Return true on success.
   */
  bool loadNumberOfNotes(String line);

  /**
   * Get how the note are represented in the file.
   */
  NoteFormat loadNoteFormat(String line);

  /**
   * Parse a token that is a note and its duration.
   * The format of this token is:
   * <note> + ',' + <duration>.
   * Return true if parsing succeed, false otherwise.
   */
  bool loadNote(String token);

  /**
   * Set this value to true to enable debug messages over serial port.
   */
  static const bool debug = false;
};

extern MelodyFactoryClass MelodyFactory;

#endif // END MELODY_FACTORY_H