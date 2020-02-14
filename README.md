# Melody Player for Arduino
A library to store and manage melody on buzzer/piezo. The melody can be stored on file and dinamically loaded in ram memory.

### Details about the format
You can write your own melody on a simple file (extension .mel), accordigly to the following format:
    
    title={Name of the melody}
    tempo={Base time in millisecond}
    length={Array length}
    format={A value between "integer" or "string", it specifies the format about frequency in array}
    {Array composed by pair <{frequency, as integer number or as note in English convention}, {duration, in tempo unit}>}, semicolon-separated}
Between 2 pairs there is always a small pause.
It is very naive, it was designed to be as minimal as possible. In hindsight, I should have used the RTTTL format designed by Nokia many year ago to encode ringtones on mobile phone. This format would guarantee lots of available free melodies and a better portability among codebases. Among many good libraries supporting RTTTL on buzzer, I would point out [Non-blocking playback of RTTTL melodies
](https://github.com/cefn/non-blocking-rtttl-arduino) and [Arduino RTTTL Player](https://github.com/ponty/arduino-rtttl-player).


An example, it codifies two simple beep:
    
    title=Bip
    tempo=200
    length=1
    format=integer
    3000,3
