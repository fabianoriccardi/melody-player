#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include "melody.h"
#include <Ticker.h>

class MelodyPlayer {
public:
#ifdef ESP32
    MelodyPlayer(unsigned char pin, unsigned char pwmChannel = 0);
#else
    MelodyPlayer(unsigned char pin);
#endif

  	/**
	 * Play the last melody successfully played in a synchrounus (blocking) way.
     */
    void play();

    /**
     * Play the melody in a synchronous (blocking) way.
     * If the melody is not valid, this call has no effect.
     */
    void play(Melody& melody);

    /**
     * Play the last melody in asynchronous way, this function return immediately.
     */
    void playAsync();

    /**
     * Play the melody in asynchronous way, this function return immediately.
     * If the melody is not valid, this call has no effect.
     */
    void playAsync(Melody& melody);

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
#endif

    /**
     * Class to store the current state of a Melody.
     */
    class MelodyState {
    public:
        MelodyState() {};
        MelodyState(Melody& melody): melody(melody), index(0), partialNoteReproduction(0) {};
        Melody melody;
        unsigned short index;

        /**
         * Used to pause a melody, and exchange the State between Players.
         * Values expressed in ms.
         */
        unsigned short partialNoteReproduction;

#ifdef ESP32
        bool silence;
#endif
    };

    /**
     * Variable to contain the state of the Player
     */
    bool playing;

    MelodyState melodyState;

    Ticker ticker;

    const static bool debug = false;

    /**
     * Change the tone with to next note.
     */
    friend void changeTone(MelodyPlayer* melody);

    /**
     * Configure pin to emit PWM.
     */
    void turnOn();

    /**
     * Disable PWM and put the buzzer is low power state.
     * 
     * This calls will fails if PWM is not initialized!
     */
    void turnOff();
};

#endif // END MELODY_PLAYER_H