/***************************************************************************
 *   This file is part of Melody Player, a library for Arduino             *
 *   to play notes on piezoelectric buzzers.                               *
 *                                                                         *
 *   Copyright (C) 2020-2022  Fabiano Riccardi                             *
 *                                                                         *
 *   This library is free software; you can redistribute                   *
 *   it and/or modify it under the terms of the GNU Lesser General Public  *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this library; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/
#ifndef PITCHES_UNORDERED_MAP_H
#define PITCHES_UNORDERED_MAP_H

/*************************************************
 * Public Constants
 *************************************************/

#include <unordered_map>
#include <string>

/**
 * NOTE: Use carefully with Arduino: this map is quite large
 * (could be around 1Kbyte), so if you're running low of RAM,
 * switch to the original pitches.h file (based on #define)
 */
extern std::unordered_map<std::string, unsigned short> noteMapping;

#endif  // END PITCHES_UNORDERED_MAP_H
