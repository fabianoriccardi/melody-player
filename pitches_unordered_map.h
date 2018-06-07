#ifndef PITCHES_UNORDERED_MAP_H
#define PITCHES_UNORDERED_MAP_H

/*************************************************
 * Public Constants
 *************************************************/

#include <unordered_map>

/**
 * NOTE: Use carefully with arduino: this map is quite large
 * (could be around 1Kbyte), so if you're running low of RAM,
 * switch to the original pitches.h file (based on #define)
 */
extern std::unordered_map <std::string,int >  m;

#endif // END PITCHES_UNORDERED_MAP_H
