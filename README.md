# Melody Player for ESP
Melody Player library allows to play melodies on buzzers on ESP8266 and ESP32 in non-blocking manner. Melodies can be written directly in code or stored in file. It supports Ring Tone Text Transfer Language (RTTTL) format, as well as an alternative format developed specifically for this library.

### Features

- Support to ESP8266 and ESP32
- Support multiple playing buzzers
- Support RTTTL format (allow reuse of old ringtones)
- Support dedicated format to allow finer control of frequencies
- Load melody from file (SPIFFS)
- Non-blocking play
- Control the melody advancement through traditional *play, pause, stop* methods
- Dynamic *migration*/*duplication* of melodies among buzzers

### Details about the custom format
You can write your own melody on a simple file (extension .mel), accordigly to the following format:
    
    title={Name of the melody}
    timeUnit={Base time in millisecond}
    length={Array length}
    format={This value can be "integer" or "string", and it specifies how the tone frequency is represent in the following array}
    {Array composed by pair <{frequency, as integer number or as string accordingly to note codification in English convention (E5, F1)} {duration, an integer representing number of "timeUnit"s}>}, semicolon-separated}
2 consecutive pairs there is a '|' (pipe character), and there is always a small pause.

Example 1: this melody codifies 2 "beeps" using the "note" codification:

    title=Beep
    timeUnit=200
    length=1
    format=string
    G7,3|SILENCE,1|G7,3

Example 2: the same melody codifying explicitly the frequency reproduced by the buzzer:

    title=Beep
    timeUnit=200
    length=1
    format=integer
    3136,3|0,1|3136,3

### Remarks
This library, targeting Arduino users, was designed to be as intuitive as possible, without giving up advanced features. i.e. I prefered to use simple *object* instead of more advanced constructs like *smart pointers* and *references*. 

