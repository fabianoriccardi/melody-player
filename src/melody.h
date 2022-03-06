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
#ifndef MELODY_H
#define MELODY_H

#include <Arduino.h>

#include <memory>
#include <vector>

/**
 * A note and the relative duration. 
 */
struct NoteDuration {
  // the frequency of a note
  unsigned short frequency;
  // duration of the frequency, expressed as number of "time unit"
  unsigned short duration;
};

/**
 * This class stores the data to play a melody. 
 * To eases the creation of a melody, you may use MelodyFactory class.
 */
class Melody {
public:
  Melody():
   notes(nullptr) {};
  
  Melody(String title, unsigned short timeUnit, std::shared_ptr<std::vector<NoteDuration>> notes, bool automaticSilence):
    title(title), timeUnit(timeUnit), notes(notes), automaticSilence(automaticSilence) {};

  /**
   * Return the title of the melody.
   */
  String getTitle() const {
    return title;
  };

  /**
   * Return the base time unit (i.e. the shortest possible
   * note in this melody), expressed in millisecond.
   */ 
  unsigned short getTimeUnit() const {
    return timeUnit;
  };

  /**
   * Get number of notes.
   */
  unsigned short getLength() const {
    if(notes == nullptr) return 0;
    return (*notes).size();
  }

  /**
   * Get a note at a given position.
   * Check length or validity of melody before call this method.
   */
  NoteDuration getNote(unsigned short i) const {
    if(i < (*notes).size()) {
      return (*notes)[i];
    }
    return (*notes).back();
  };

  bool getAutomaticSilence() const {
    return automaticSilence;
  }

  /**
   * Tell if the object contains a valid melody.
   */
  bool isValid() const {
    return notes != nullptr && (*notes).size() != 0;
  }

  /**
   * Return true if contains a valid melody, false otherwise.
   */
  explicit operator bool() const {
    return isValid();
  }
  
private:
  String title;
  unsigned short timeUnit;
  std::shared_ptr<std::vector<NoteDuration>> notes;
  const static unsigned short maxLength = 1000;
  bool automaticSilence;
  
  const static bool debug = false;
};

#endif // END MELODY_H
