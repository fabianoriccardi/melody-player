#include "melody_manager.h"
#include <Ticker.h>

bool Melody::loadTest(){
  Serial.println("Init melody... ");
  tempo=200;
  nNotes=3;
  nd=(NoteDuration*)malloc(sizeof(NoteDuration)*nNotes);
  
  std::string mel[]={"D4","D5","D6"};
  for(int i=0;i<3;i++){
    nd[i].frequency = m[mel[i]];
    nd[i].duration = 1;
  }
  Serial.println("Done!");
  return true;
}

void removeCarriageReturn(String& s){
  if(s.charAt(s.length()-1)=='\r'){
    s=s.substring(0,s.length()-1);
  }
}

bool Melody::load(String fileName){
  File f=SPIFFS.open(fileName,"r");
  bool error=false;
  if(f){
    String line;
    
    line=f.readStringUntil('\n');
    removeCarriageReturn(line);
    if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
    if(line.substring(0,5)=="title"){
        String title=line.substring(6);
        this->title=title;
    }else{
      error=true; 
    }

    line=f.readStringUntil('\n');
    removeCarriageReturn(line);
    if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
    if(!error && line.substring(0,5)=="tempo"){
         String t=line.substring(6);
         this->tempo=t.toInt();
    }else{
      error=true;
    }
    if(debug) Serial.println(this->tempo);
    
    line=f.readStringUntil('\n');
    removeCarriageReturn(line);
    if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
    if(line.substring(0,6)=="length"){
        String len=line.substring(7);
        this->nNotes=len.toInt();
    }else{
      error=true; 
    }
    if(debug) Serial.println(this->nNotes);

    String format;
    line=f.readStringUntil('\n');
    removeCarriageReturn(line);
    if(debug) Serial.println(String("Reading line:--") + line + "-- Len:" + line.length());
    if(!error && line.substring(0,6)=="format"){
      format=line.substring(7);
    }else{
      error=true;
    }
    if(debug) Serial.println(format);

    if(!error){
      if(debug) Serial.println(String("This melody object will take at least: ") + sizeof(NoteDuration)*nNotes + "bytes");
      if(nNotes<maxLength){
        nd=(NoteDuration*)malloc(sizeof(NoteDuration)*nNotes);
        int i=0;
        while(f.available() && i<nNotes && !error){
          // let's try to read a couple
          String couple=f.readStringUntil('|');
          if(debug) Serial.println(couple);
          
          String aux;
          int j=0;
          // Get the freq
          while(j<couple.length() && couple.charAt(j)!=','){
            aux+=couple.charAt(j);
            j++;
          }
          
          if(format=="string"){
            std::string mystr(aux.c_str());
            nd[i].frequency = m[mystr];
          }else if(format=="integer"){
            nd[i].frequency = aux.toInt();
          }else{
            error=true;
          }
          if(debug) Serial.println(nd[i].frequency);
  
          j++;
          aux="";
          while(j<couple.length()){
            aux+=couple.charAt(j);
            j++;
          }
          nd[i].duration = aux.toInt();
          if(debug) Serial.println(nd[i].duration);
          
          i++;
        }
      }else{
        Serial.println("Melody length too high, the load is not executed!");
        error=true;
      }
    }
  }else{
    error=true;
    Serial.println("Opening file error");
  }
  if(error){
    return false;
  }
  
  return true;
}

void Melody::playSync(){
  if(!checkReadyness()){
    return;
  }

  setupPin();

  state=1;
  for(int i=0;i<nNotes;i++){
    if(debug) Serial.println(String("Playing: freq=") + nd[i].frequency + " dur=" + nd[i].duration);
#ifdef ESP32
    ledcWriteTone(channel, nd[i].frequency);
    delay(tempo*nd[i].duration);
    ledcWriteTone(channel, 0);
    delay(0.30f * (tempo*nd[i].duration));
#else
    tone(pin,nd[i].frequency,tempo*nd[i].duration);
    delay(1.30f * (tempo*nd[i].duration));
#endif
  }
  state=2;
#ifdef ESP32
  ledcWrite(channel, 0);
  ledcDetachPin(pin);
#endif
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);
}

static Ticker ticker; 

/**
 * NOTE: this is the function executed in the Ticker, aka the Thread on ESP* platform.
 * So you can decide to put delay(..) function without blocking the others threads 
 * (e.g. the loop() function) instead of using the slightly complicated ticker.attach(..)
 * to delay a certain operation. Below you can see a mix between the 2 approaches
 */
void changeFreq(Melody* melody){
  static int i=0;
  if(i<melody->nNotes){
    if(melody->debug) Serial.println(String("Playing async: freq=") + melody->nd[i].frequency + " dur=" + melody->nd[i].duration + " iteration=" + i);
#ifdef ESP32
    ledcWriteTone(melody->channel, melody->nd[i].frequency);
    delay(melody->tempo*melody->nd[i].duration);
    ledcWriteTone(melody->channel, 0);
    ticker.once_ms(0.30f*melody->nd[i].duration*melody->tempo,changeFreq,melody);
#else
    tone(melody->pin,melody->nd[i].frequency,melody->tempo*melody->nd[i].duration);
    ticker.once_ms(1.30f*melody->nd[i].duration*melody->tempo,changeFreq,melody);
#endif
    i++;
  }else{
    i=0;
    melody->state=2;
#ifdef ESP32
    ledcWrite(melody->channel, 0);
    ledcDetachPin(melody->pin);
    // Need to avoid to waste energy: the buzzer will consume energy if turned "active"
    pinMode(melody->pin,OUTPUT);
    digitalWrite(melody->pin,HIGH);
#endif
  }
}

void Melody::playAsync(){
  state=1;
  setupPin();
  ticker.once(0,changeFreq,this);
}

bool Melody::isPlaying(){
  if(state==1){
    return true;
  }
  return false;
}

bool Melody::checkReadyness(){
  if(nNotes<1){
    Serial.println("Cannot play the melody, was the file loaded corretly or is the format wrong?");
    return false;
  }
  return true;
}

void Melody::setupPin(){
#ifdef ESP32
  int resolution = 8;
  // 2000 is a frequency, it will be change at the first play
  ledcSetup(channel, 2000, resolution);
  ledcAttachPin(pin, channel);
  ledcWrite(channel, 125);
#endif
}