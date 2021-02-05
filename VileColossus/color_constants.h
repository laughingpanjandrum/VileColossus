#pragma once

#include "libtcod.hpp"
#include "utility.h"


typedef TCODColor colorType;

//		Returns colours corresponding to string inputs.
inline colorType translateColour(string code)
{
	auto split1 = splitLine(code, ',');
	auto split2 = splitLine(split1.second, ',');
	int c1 = stoi(split1.first);
	int c2 = stoi(split2.first);
	int c3 = stoi(split2.second);
	return TCODColor(c1, c2, c3);
}


#define COLOR_WHITE				TCODColor::white
#define COLOR_BLACK				TCODColor::black

#define COLOR_HIGHLIGHT			TCODColor::white
#define COLOR_HIGHLIGHT_POS		TCODColor::desaturatedGreen
#define COLOR_HIGHLIGHT_NEG		TCODColor::desaturatedRed

#define COLOR_POSITIVE			TCODColor::darkGreen
#define COLOR_NEGATIVE			TCODColor::darkRed
#define COLOR_WARNING			TCODColor::darkOrange

#define COLOR_LIGHT				TCODColor::lightSepia
#define COLOR_MEDIUM			TCODColor::sepia
#define COLOR_DARK				TCODColor::darkSepia


#define COLOR_HEALTH			TCODColor::darkCrimson
#define COLOR_MAGIC				TCODColor::lighterBlue
#define COLOR_MISC_STAT			TCODColor::darkAmber