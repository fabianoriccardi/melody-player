#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include "melody.h"
#include <Ticker.h>

class MelodyPlayer {
public:
    #ifdef ESP32
        MelodyPlayer(unsigned char pin, unsigned char pwmChannel = 0): pin(pin), pwmChannel(pwmChannel), playing(false) {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
        };
    #else
        MelodyPlayer(unsigned char pin, unsigned char pwmChannel = 0): pin(pin), playing(false) {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
        };
    #endif
	

  	/**
	 * Play the last melody successfully played in a synchrounus (blocking) way.
     */
    void play();

    /**
     * Play the melody in a synchrounus (blocking) way.
     * If the melody is not valid, this call has no effect.
     */
    void play(Melody melody){
        if(!melody){
            return;
        }
        this->melody = melody;
        play();
    }

    /**
     * Play the last melody using a "thread", to reproduce the melody
     * while performing other actions.
     */
    void playAsync();

    /**
     * Play the music using a "thread", to reproduce the melody
     * while performing other actions.
     */
    void playAsync(Melody melody){
        if(!melody){
            return;
        }
        this->melody = melody;
        playAsync();
    }

    /**
     * Stop the melody. 
     * After a stop(), if you call play(), the melody restarts from the begin.
     */
    void stop();

    /**
     * Tell if the melody is played.
     */
    bool isPlaying(){
        return playing;
    }

private:
	unsigned char pin;

#ifdef ESP32
    unsigned char pwmChannel;
    bool silence;
#endif

    // Variable to contain the state of the Player
    bool playing;

    Melody melody;
    int melodyIndex;

    Ticker ticker;

    const static bool debug = false;

    /**
     * Change the tone with to next note.
     */
    friend void changeTone(MelodyPlayer* melody);

    void setupPin();
};

#endif // END MELODY_PLAYER_H