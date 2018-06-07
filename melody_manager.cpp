#include "melody_manager.h"

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

bool Melody::load(String fileName){
  Serial.println("Init melody... ");
  File f=SPIFFS.open(fileName,"r");
  bool error=false;
  if(f){
    int fileSize=f.size();
    String line;
    
    line=f.readStringUntil('\n');
    fileSize=fileSize-line.length()-1;
    line=line.substring(0,line.length()-1);
    Serial.println(String("Reading line:--") + line + "-- LEn:" + line.length());
    if(line.substring(0,5)=="title"){
        String title=line.substring(6);
        this->title=title;
    }else{
      error=true; 
    }

    line=f.readStringUntil('\n');
    fileSize=fileSize-line.length()-1;
    line=line.substring(0,line.length()-1);
    Serial.println(String("Reading line:--") + line + "-- LEn:" + line.length());
    if(!error && line.substring(0,5)=="tempo"){
         String t=line.substring(6);
         this->tempo=t.toInt();
         Serial.println(this->tempo);
    }else{
      error=true;
    }

    line=f.readStringUntil('\n');
    fileSize=fileSize-line.length()-1;
    line=line.substring(0,line.length()-1);
    Serial.println(String("Reading line:--") + line + "-- LEn:" + line.length());
    if(line.substring(0,6)=="length"){
        String len=line.substring(7);
        this->nNotes=len.toInt();
    }else{
      error=true; 
    }
    Serial.println(this->nNotes);

    String format;
    line=f.readStringUntil('\n');
    fileSize=fileSize-line.length()-1;
    line=line.substring(0,line.length()-1);
    Serial.println(String("Reading line:--") + line + "-- LEn:" + line.length());
    if(!error && line.substring(0,6)=="format"){
      format=line.substring(7);
    }else{
      error=true;
    }
    Serial.println(format);

    if(!error){
      nd=(NoteDuration*)malloc(sizeof(NoteDuration)*nNotes);
      int i=0;
      while(f.available() && i<nNotes && !error){
        // let's try to read a couple
        String couple=f.readStringUntil('|');
        Serial.println(couple);
        
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
        Serial.println(nd[i].frequency);

        j++;
        aux="";
        while(j<couple.length()){
          aux+=couple.charAt(j);
          j++;
        }
        nd[i].duration = aux.toInt();
        Serial.println(nd[i].duration);
        
        i++;
      }
    }
  }else{
    error=true;
    Serial.println("Opening file error");
  }
  if(error){
    return false;
  }
  
  Serial.println("Done!");
  return true;
}

void Melody::playSync(){
  for(int i=0;i<nNotes;i++){
    Serial.println(nd[i].frequency);
    Serial.println(nd[i].duration);
    tone(D6,nd[i].frequency,tempo*nd[i].duration);
    delay(1.30f * (tempo*nd[i].duration));
  }
}

