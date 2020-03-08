#include "melody_factory.h"
#include "notes.h"

const uint16_t sourceNotes[] =
{ 
  0,
  NOTE_C4,
  NOTE_CS4,
  NOTE_D4,
  NOTE_DS4,
  NOTE_E4,
  NOTE_F4,
  NOTE_FS4,
  NOTE_G4,
  NOTE_GS4,
  NOTE_A4,
  NOTE_AS4,
  NOTE_B4,

  NOTE_C5,
  NOTE_CS5,
  NOTE_D5,
  NOTE_DS5,
  NOTE_E5,
  NOTE_F5,
  NOTE_FS5,
  NOTE_G5,
  NOTE_GS5,
  NOTE_A5,
  NOTE_AS5,
  NOTE_B5,

  NOTE_C6,
  NOTE_CS6,
  NOTE_D6,
  NOTE_DS6,
  NOTE_E6,
  NOTE_F6,
  NOTE_FS6,
  NOTE_G6,
  NOTE_GS6,
  NOTE_A6,
  NOTE_AS6,
  NOTE_B6,

  NOTE_C7,
  NOTE_CS7,
  NOTE_D7,
  NOTE_DS7,
  NOTE_E7,
  NOTE_F7,
  NOTE_FS7,
  NOTE_G7,
  NOTE_GS7,
  NOTE_A7,
  NOTE_AS7,
  NOTE_B7,

  2*NOTE_C7,
  2*NOTE_CS7,
  2*NOTE_D7,
  2*NOTE_DS7,
  2*NOTE_E7,
  2*NOTE_F7,
  2*NOTE_FS7,
  2*NOTE_G7,
  2*NOTE_GS7,
  2*NOTE_A7,
  2*NOTE_AS7,
  2*NOTE_B7,
};

Melody MelodyFactoryClass::loadRtttl(String filepath) {
  File f = SPIFFS.open(filepath, "r");
  f.setTimeout(0);

  if(!f){
    Serial.println("Opening file error");
    return Melody();
  }

  String title = f.readStringUntil(':');
  title.trim();
  if(debug) Serial.println(String("Title:") + title);
  if(title.length() == 0 ){
    return Melody();
  }
  
  String values = f.readStringUntil(':');
  values.trim();
  if(debug) Serial.println(String("Default values:") + values);
  if(values.length() == 0) {
    return Melody();
  }

  parseDefaultValues(values);

  // 32 because it is the shortest note!
  int timeUnit = 60 * 1000 * 4 / beat / 32;

  notes = std::make_shared<std::vector<NoteDuration>>();
  bool result = true;
  while(f.available() && notes->size() < maxLength && result) {
    String s = f.readStringUntil(',');
    s.trim();
    result = parseRtttlNote(s);
  }
  if(result && notes->size() > 0) {
    return Melody(title, timeUnit, notes);
  }

  return Melody();
}

/**
 * Start from point to the next char to analyze.
 * Zero means error.
 */
unsigned int getUnsignedInt(String& s, int& startFrom) {
  unsigned int temp = 0;
  while(isDigit(s.charAt(startFrom))) {
    temp = (temp * 10) + s.charAt(startFrom) - '0';
    startFrom++;
  }
  return temp;
}

unsigned int MelodyFactoryClass::parseDuration(String& s, int& startFrom) {
  // Skip '='
  startFrom++;
  unsigned int temp = getUnsignedInt(s, startFrom);
  if(temp != 1 && temp != 2 && temp != 4 && temp != 8 && temp != 16 && temp != 32){
    return 0;
  }
  // Discard ','
  startFrom++;
  return temp;
}

unsigned int MelodyFactoryClass::parseOctave(String& s, int& startFrom) {
  // Skip '='
  startFrom++;
  unsigned int temp = getUnsignedInt(s, startFrom);
  if(temp < 4 || temp > 7){
    return 0;
  }
  // Discard ','
  startFrom++;
  return temp;
}

unsigned int MelodyFactoryClass::parseBeat(String& s, int& startFrom) {
  // Skip '='
  startFrom++;
  unsigned int temp = getUnsignedInt(s, startFrom);
  if(!(temp >= 10 && temp <= 200)){
    return 0;
  }
  // Discard ','
  startFrom++;
  return temp;
}

bool MelodyFactoryClass::parseRtttlNote(String s){
  int i = 0;

  unsigned int relativeDuration = this->duration;
  // Optional number: note duration (e.g 4=quarter note, ...)
  if(isdigit(s.charAt(i))) {
    unsigned int temp = getUnsignedInt(s, i);
    if(temp){
      relativeDuration = temp;
    }
  }

  // To match struct NoteDuration format, I need the direct
  // note length, instead RTTTL provides the denominator
  // of the whole note
  if (relativeDuration == 32) {
    relativeDuration = 1;
  } else if (relativeDuration == 16) {
    relativeDuration = 2;
  } else if (relativeDuration == 8) {
    relativeDuration = 4;
  } else if (relativeDuration == 4) {
    relativeDuration = 8;
  } else if (relativeDuration == 2) {
    relativeDuration = 16;
  } else if (relativeDuration == 1) {
    relativeDuration = 32;
  } else {
    relativeDuration = 0;
  }


  // note (p is silence)
  int note = 0;
  switch (s.charAt(i)){
    case 'c':
      note = 1;
      break;
    case 'd':
      note = 3;
      break;
    case 'e':
      note = 5;
      break;
    case 'f':
      note = 6;
      break;
    case 'g':
      note = 8;
      break;
    case 'a':
      note = 10;
      break;
    case 'b':
      note = 12;
      break;
    case 'p':
    default:
      note = 0;
  }

  i++;

  // Optional #
  if (s.charAt(i) == '#') {
    note++;
    i++;
  }

  // get optional '.' dotted note
  // This note will last 50% more
  if (s.charAt(i) == '.') {
    relativeDuration += relativeDuration / 2;
    i++;
  }

  int scale;
  // now, get scale
  if (isdigit(s.charAt(i))) {
    scale = s.charAt(i) - '0';
    i++;
  } else {
    scale = octave;
  }

  unsigned short freq;
  if (note) {
    freq = sourceNotes[(scale - 4) * 12 + note];
  } else {
    freq = 0;
  }

  notes->push_back({.frequency = freq, .duration = relativeDuration});
  return true;
}

void MelodyFactoryClass::parseDefaultValues(String values){
  int i = 0;

  if(values.charAt(i) == 'd'){
    i++;
  }
  duration = parseDuration(values, i);
  if(duration == 0) {
    duration = defaultDuration;
  }

  if(values.charAt(i) == 'o'){
    i++;
  }
  octave = parseOctave(values, i);
  if(octave == 0) {
    octave = defaultOctave;
  }

  if(values.charAt(i) == 'b'){
    i++;
    beat = parseBeat(values, i);
  }
  if(beat == 0) {
    beat = defaultBeat;
  }
}
