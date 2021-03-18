#pragma once

#include <string>
#include <vector>

#include "color_constants.h"

using namespace std;


enum Maptile
{
	MT_ALCHEMY, MT_ANVIL, MT_FORGE, MT_GEMSTONE_PRESS, MT_DEMONFORGE, MT_ABYSSAL_ALTAR,
	MT_BARREL, MT_BLOOD_POOL,
	MT_BUSH,
	MT_CHEST_SMALL, MT_CHEST_GLOWING, MT_CHEST_LUMINOUS, MT_CHEST_RADIANT,
	MT_DOOR_STONE, MT_DOOR_WOODEN,
	MT_FLOOR_CARPET, MT_FLOOR_HOT, MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_FLOOR_VOID,
	MT_GRASS,
	MT_HEALING_FOUNTAIN,
	MT_LAVA,
	MT_MEDITATION_SHRINE, MT_DRAINED_SHRINE,
	MT_PEW_WOODEN, MT_TABLE_WOODEN,
	MT_RUBBLE,
	MT_RUNE_IMPRINTER,
	MT_SAND,
	MT_SARCOPHAGUS,
	MT_STASH,
	MT_STATUE_MARBLE, MT_STATUE_BLOODY,
	MT_TOMBSTONE,
	MT_THORNS,
	MT_TREE_DEAD,
	MT_WALL_BLOODY, MT_WALL_FLESH, MT_WALL_ICE, MT_WALL_SANDSTONE, MT_WALL_STONE, MT_WALL_SUPER,
	MT_WATER,

	MT_STAIRS_DOWN, MT_STAIRS_UP,
	MT_STAIRS_UP_LONG, MT_STAIRS_DOWN_LONG,
	MT_HELLPORTAL_DOWN, MT_HELLPORTAL_UP,
	MT_TEMPLE_PORTAL,
	MT_ABYSSAL_GATE,
};

const string getMaptileName(const Maptile tl);
const int getMaptileGlyph(const Maptile tl);
const colorType getMaptileColor(const Maptile tl);

const bool isMaptileWalkable(const Maptile tl);
const bool isMaptileTransparent(const Maptile tl);

const bool isMaptileBreakable(const Maptile tl);

const bool doesMaptileFlicker(const Maptile tl);
const bool isMaptileStairs(const Maptile tl);
const bool drawMaptileOverSurface(const Maptile tl);


const bool isSpecialMaptile(const Maptile tl);
const string getMaptileDescription(const Maptile tl);


//	SURFACES

enum class Surface { BONES, CORPSE, FIRE, POISON_OOZE, SLUDGE, WEB, __NONE, };

const string getSurfaceName(const Surface sf);
const colorType getSurfaceColor(const Surface sf);
const int getSurfaceGlyph(const Surface sf);