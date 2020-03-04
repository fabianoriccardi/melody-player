#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include "melody.h"
#include <Ticker.h>
#include <memory>

class MelodyPlayer {
public:
#ifdef ESP32
    /**
     * pwmChannel is optional and you have to configure it only if you need to play
     * simultaneous melodies.
     */
    MelodyPlayer(unsigned char pin, unsigned char pwmChannel = 0);
#else
    MelodyPlayer(unsigned char pin);
#endif

  	/**
	 * Play the last melody played in a synchrounus (blocking) way.
     */
    void play();

    /**
     * Play the melody in a synchronous (blocking) way.
     * If the melody is not valid, this call has no effect.
     */
    void play(Melody& melody);

    /**
     * Play the last melody in asynchronous way, this function return immediately.
     * If the melody is not valid, this call has no effect.
     */
    void playAsync();

    /**
     * Play the melody in asynchronous way, this function return immediately.
     * If the melody is not valid, this call has no effect.
     */
    void playAsync(Melody& melody);

    /**
     * Stop the melody. 
     * After a stop(), if you call play() or playAsync(), the melody restarts from the begin.
     */
    void stop();

    /**
     * Tell if the melody is played.
     */
    bool isPlaying() const{
        return state == State::PLAY;
    }

    /**
     * The current melody and the source Player state is transferend to the destination Player.
     * The source gets stopped. You have to call play(Melody) to make it play again.
     */
    void transferMelodyTo(MelodyPlayer& destPlayer);

    /**
     * The current melody and source player state is cloned to the destiantion Player.
     * The two players remains indipendent (e.g. the melody can be stop/pause/played indipendently).
     */
    void duplicateMelodyTo(MelodyPlayer& destPlayer);

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
        MelodyState(): index(0), partialNoteReproduction(0) {};
        MelodyState(Melody& melody): melody(melody), index(0), partialNoteReproduction(0) {};
        Melody melody;
        unsigned short index;

        /**
         * Used to pause a melody, and exchange the State between Players.
         * Values expressed in ms.
         */
        unsigned short partialNoteReproduction;
        bool silence;
        
        /**
         * Reset the state (i.e. like a melody not yet reproduced).
         */
        void reset() {
            index = 0;
            partialNoteReproduction = 0;
        }

        /**
         * Save the current state of the melody, including the time to finish the current note.
         */
        void saveCurrentState(unsigned long supportSemiNote){
            // Rollback of the melodyState to reproduce the "partial" note
            partialNoteReproduction = supportSemiNote - millis();
            if(partialNoteReproduction < 10) {
              partialNoteReproduction = 0;
            } else {
              if(!silence) {
                index--;
              }
              silence = !silence;
            }
        }
    };

    enum class State { STOP, PLAY, PAUSE};

    State state;

    std::unique_ptr<MelodyState> melodyState;

    unsigned long supportSemiNote;

    Ticker ticker;

    const static bool debug = false;

    /**
     * Change the tone with to next note.
     */
    friend void changeTone(MelodyPlayer* melody);

    /**
     * Stop to play melody, but do not reset the melody index.
     */
    void stopPlay();

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