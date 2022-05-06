/***************************************************************************
 *   This file is part of Melody Player, a library for Arduino             *
 *   to play notes on piezoelectric buzzers.                               *
 *                                                                         *
 *   Copyright (C) 2020-2022  Fabiano Riccardi                             *
 *                                                                         *
 *   This library is free software; you can redistribute                   *
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this library; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/
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
   * Load melody from RTTTL format from file.
   */
  Melody loadRtttlFile(String filePath);

  /**
   * Load melody from RTTTL format string.
   */
  Melody loadRtttlString(const char rtttlMelody[]);

  /**
   * Load the melody from code.
   * Notes are represented as string accordigly to english notation (i.e. "C4", "G3", "G6").
   * This method assumes that each note lasts 1 beat.
   * The last parameter, automaticSilence, if true, automatically inserts a small silence between 2
   * consecutive notes.
   */
  Melody load(String title, unsigned short timeUnit, String notesToLoad[],
              unsigned short nNotesToLoad, bool autoSilence = true);

  /**
   * Load the melody from code.
   * frequenciesToLoad are intenger numbers expressing the real reproduced frequency.
   * This method assumes that each note lasts 1 beat.
   * The last parameter, automaticSilence, if true, automatically inserts a small silence between 2
   * consecutive notes.
   */
  Melody load(String title, unsigned short timeUnit, int frequenciesToLoad[],
              unsigned short nFrequenciesToLoad, bool autoSilence = true);

private:
  enum class NoteFormat { ERROR, STRING, INTEGER };

  String title;
  unsigned short timeUnit;
  NoteFormat noteFormat;
  std::shared_ptr<std::vector<NoteDuration>> notes;
  // Used to check how many note are stored in a file.
  unsigned short nNotes;
  const unsigned short maxLength = 1000;

  ///////////// RTTTL helpers
  /**
   * It is the default duration of a note. For example,
   * "4" means that each note with no duration specifier
   * is by default considered a quarter note. Possibile values:
   * 1 - whole note
   * 2 - half note
   * 4 - quarter note
   * 8 - eighth note
   * 16 - sixteenth note
   * 32 - thirty-second note
   */
  const unsigned short defaultDuration = 4;
  unsigned short duration;

  /**
   * The default octave. There are four octaves in the RTTTL format [4-7].
   */
  const unsigned short defaultOctave = 6;
  unsigned short octave;

  /**
   * Beats per minute (BPM).
   */
  const unsigned short defaultBeat = 63;
  unsigned short beat;

  /**
   * Try to parse the default parameters of RTTTL melody.
   * If not found, it sets the default values (defined by rtttl specification).
   */
  void parseDefaultValues(String values);

  unsigned int parseDuration(String& s, int& startFrom);
  unsigned int parseOctave(String& s, int& startFrom);
  unsigned int parseBeat(String& s, int& startFrom);
  bool parseRtttlNote(String s);

  //////////// END RTTTL helpers

  /**
   * Get title from line.
   * Return true on success.
   */
  bool loadTitle(String line);

  /**
   * Get time unit from line.
   * Return true on success.
   */
  bool loadTimeUnit(String line);

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

#endif  // END MELODY_FACTORY_H