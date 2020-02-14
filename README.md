# Melody Player for Arduino
An Arduino library to play melodies on buzzers. Melodies can be easily written on a file on directly into the code as a sequence on human-readable notes.

### Details about the format
You can write your own melody on a simple file (extension .mel), accordigly to the following format:
    
    title={Name of the melody}
    tempo={Base time in millisecond}
    length={Array length}
    format={This value can be "integer" or "string", and it specifies how the tone frequency is represent in the following array}
    {Array composed by pair <{frequency, as integer number or as string accordingly to note codification in English convention (E5, F1)} {duration, an integer representing time in "tempo" unit}>}, semicolon-separated}
2 consecutive pairs there is a '|' (pipe character), and there is always a small pause.

Example 1: this melody codifies 2 "beeps" using the "note" codification:

    title=Beep
    tempo=200
    length=1
    format=string
    G7,3|SILENCE,1|G7,3

Example 2: the same melody codifying explicitly the frequency reproduced by the buzzer:

    title=Beep
    tempo=200
    length=1
    format=integer
    3136,3|0,1|3136,3

### Remarks
This library is very minimal, indeed it was designed to be as simply as possible. In hindsight, I may have used the RTTTL format introduced by Nokia many years ago to encode ringtones on mobile phone. This format would guarantee lots of available free melodies and a better portability among codebases. Among many good libraries supporting RTTTL on buzzer, I would point out [Non-blocking playback of RTTTL melodies
](https://github.com/cefn/non-blocking-rtttl-arduino) and [Arduino RTTTL Player](https://github.com/ponty/arduino-rtttl-player).


