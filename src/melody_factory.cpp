#include "melody_factory.h"
#include "pitches_unordered_map.h"

static void removeCarriageReturn(String& s){
  if(s.charAt(s.length()-1) == '\r'){
    s = s.substring(0, s.length()-1);
  }
}

Melody MelodyFactoryClass::load(String filepath){
  File f = SPIFFS.open(filepath, "r");
  
  if(!f){
    Serial.println("Opening file error");
    return Melody();
  }

  // Skip multi-line comments at the begin of the file
  String line = f.readStringUntil('\n');
  while(line.charAt(0) == '#'){
  	line = f.readStringUntil('\n');
  }

  bool success = false;
  success = loadTitle(line);
  if(!success){
    return Melody();
  }

  success = loadTempo(f.readStringUntil('\n'));
  if(!success){
    return Melody();
  }
  
  success = loadNumberOfNotes(f.readStringUntil('\n'));
  if(!success){
    return Melody();
  }

  NoteFormat noteFormat = loadNoteFormat(f.readStringUntil('\n'));
  if(noteFormat == NoteFormat::ERROR){
    return Melody();
  } else {
    this->noteFormat = noteFormat;
  }

  
  if(debug) Serial.println(String("This melody object will take at least: ") + (sizeof(NoteDuration) * nNotes) + "bytes");
  if(nNotes < maxLength){
    notes = std::make_shared<std::vector<NoteDuration>>();
    notes->reserve(nNotes);
    bool error = false;
    while(f.available() && notes->size() < nNotes && !error){
      // let's try to read a token
      String noteDuration = f.readStringUntil('|');
      error = !loadNote(noteDuration);
    }

    if(error){
      if(debug) Serial.println("error during the tokens loading!");
      return Melody();
    }
  }

  return Melody(title, tempo, notes);
}

Melody MelodyFactoryClass::load(String title, unsigned short tempo, String notesToLoad[], unsigned short nNotesToLoad){
  if(title.length() == 0 && tempo <= 20){
    return Melody();
  }
  if(nNotesToLoad == 0 || nNotesToLoad > maxLength ){
    return Melody();
  }

  if(notesToLoad == nullptr){
    return Melody();
  }

  notes = std::make_shared<std::vector<NoteDuration>>();
  notes->reserve(nNotesToLoad);
  noteFormat = NoteFormat::STRING;
  bool error = false;
  while(this->notes->size() < nNotesToLoad && !error){
    String noteDuration = notesToLoad[notes->size()] + ",1";
    error = !loadNote(noteDuration);
  }
  if(error){
    return Melody();
  }

  return Melody(title, tempo, notes);
}

Melody MelodyFactoryClass::load(String title, unsigned short tempo, int frequenciesToLoad[], unsigned short nFrequenciesToLoad){
  if(title.length() == 0 && tempo <= 20){
    return Melody();
  }
  if(nFrequenciesToLoad == 0 || nFrequenciesToLoad > maxLength ){
    return Melody();
  }

  if(frequenciesToLoad == nullptr){
    return Melody();
  }

  notes = std::make_shared<std::vector<NoteDuration>>();
  notes->reserve(nFrequenciesToLoad);
  noteFormat = NoteFormat::INTEGER;
  bool error = false;
  while(this->notes->size() < nFrequenciesToLoad && !error){
    String noteDuration = String(frequenciesToLoad[notes->size()]) + ",1";
    error = !loadNote(noteDuration);
  }
  if(error){
    return Melody();
  }

  return Melody(title, tempo, notes);
}

bool MelodyFactoryClass::loadTitle(String line){
  removeCarriageReturn(line);
  if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
  if(line.substring(0, 5) == "title"){
      String title = line.substring(6);
      this->title = title;
      return true;
  }
  return false;
}

bool MelodyFactoryClass::loadTempo(String line){
  removeCarriageReturn(line);
  if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
  if(line.substring(0, 5) == "tempo"){
    String t = line.substring(6);
    this->tempo = t.toInt();
    if(debug) Serial.println(this->tempo);
    if(this->tempo>20){
      return true;
    }
  }
  return false;
}

bool MelodyFactoryClass::loadNumberOfNotes(String line){
  removeCarriageReturn(line);
  if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
  if(line.substring(0, 6) == "length"){
    String len = line.substring(7);
    this->nNotes = len.toInt();
    if(debug) Serial.println(this->nNotes);
    return true;
  }
  return false;
}

MelodyFactoryClass::NoteFormat MelodyFactoryClass::loadNoteFormat(String line){
  removeCarriageReturn(line);
  if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
  String format;
  if(line.substring(0, 6) == "format"){
    format = line.substring(7);
    if(debug) Serial.println(format);
  }

  NoteFormat noteFormat = NoteFormat::ERROR;
  if(format == "string"){
    noteFormat = NoteFormat::STRING;
  }else if(format == "integer"){
    noteFormat = NoteFormat::INTEGER;
  }

  return noteFormat;
}

bool MelodyFactoryClass::loadNote(String token){
  token.trim();
  bool error = false;
  NoteDuration note;
  
  if(debug) Serial.println(String("note+duration: ") + token);
  
  String aux;
  int j = 0;

  // Get the frequency
  while(j < token.length() && token.charAt(j) != ','){
    aux += token.charAt(j);
    j++;
  }
  
  if(noteFormat == NoteFormat::STRING){
    std::string mystr(aux.c_str());
    std::unordered_map<std::string, unsigned short>::const_iterator n = noteMapping.find(mystr);
    if(n != noteMapping.end()){
      note.frequency = noteMapping[mystr];
    } else {
      if(debug) Serial.println(String("This note doesn't exist: ") + aux);
      return false;
    }
    
  }else if(noteFormat == NoteFormat::INTEGER){
    note.frequency = aux.toInt();
  }
  if(debug) Serial.println(String("freq: ") + note.frequency);

  j++;
  aux = "";
  while(j < token.length()){
    aux += token.charAt(j);
    j++;
  }
  
  note.duration = aux.toInt();
  if(debug) Serial.println(String("duration: ") + note.duration);

  notes->push_back(note);

  return true;
}

MelodyFactoryClass MelodyFactory;
