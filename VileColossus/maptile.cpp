#include "maptile.h"

const string getMaptileName(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ALCHEMY):			return "alembic of alchemy";
	case(MT_ANVIL):				return "anvil";
	case(MT_BARREL):			return "wooden barrel";
	case(MT_BUSH):              return "bush";
	case(MT_CHEST_GLOWING):		return "glowing chest";
	case(MT_CHEST_SMALL):		return "small chest";
	case(MT_DOOR_STONE):        return "stone door";
	case(MT_DOOR_WOODEN):		return "wooden door";
	case(MT_DRAINED_SHRINE):	return "drained shrine";
	case(MT_FLOOR_CARPET):      return "carpet";
	case(MT_FLOOR_STONE):       
	case(MT_FLOOR_STONE2):		return "stone floor";
	case(MT_FORGE):				return "enchanted forge";
	case(MT_GEMSTONE_PRESS):	return "gemstone press";
	case(MT_GRASS):				return "grass";
	case(MT_HEALING_FOUNTAIN):	return "fountain of life";
	case(MT_MEDITATION_SHRINE):	return "Shrine of Meditation";
	case(MT_PEW_WOODEN):        return "wooden pew";
	case(MT_RUBBLE):            return "rubble";
	case(MT_RUNE_IMPRINTER):	return "rune imprinter";
	case(MT_SAND):              return "sand";
	case(MT_SARCOPHAGUS):		return "sarcophagus";
	case(MT_STAIRS_DOWN):       return "stairs down";
	case(MT_STAIRS_DOWN_LONG):	return "stairs down (long)";
	case(MT_STAIRS_UP):         return "stairs up";
	case(MT_STAIRS_UP_LONG):	return "stairs up (long)";
	case(MT_STASH):				return "stash";
	case(MT_STATUE_MARBLE):     return "marble statue";
	case(MT_TABLE_WOODEN):      return "wooden table";
	case(MT_TOMBSTONE):			return "tombstone";
	case(MT_WALL_SANDSTONE):	return "sandstone wall";
	case(MT_WALL_STONE):        return "stone wall";
	case(MT_WALL_SUPER):        return "unbreakable wall";
	case(MT_WATER):             return "water";
	default:
		return "error_tile_" + to_string(tl);
	}
}

const int getMaptileGlyph(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ALCHEMY):
	case(MT_GEMSTONE_PRESS):
		return 237;

	case(MT_ANVIL):
	case(MT_RUNE_IMPRINTER):
		return 251;

	case(MT_BARREL):
		return '0';

	case(MT_FORGE):
		return 233;

	case(MT_BUSH):
		return '"';

	case(MT_CHEST_GLOWING):
	case(MT_CHEST_SMALL):
		return 8;

	case(MT_DOOR_STONE):
	case(MT_DOOR_WOODEN):
		return '+';

	case(MT_FLOOR_CARPET):
		return ':';

	case(MT_FLOOR_STONE2):
		return ',';

	case(MT_FLOOR_STONE):
	case(MT_GRASS):
	case(MT_SAND):
		return 249;

	case(MT_SARCOPHAGUS):
	case(MT_PEW_WOODEN):
		return 220;

	case(MT_RUBBLE):
		return ',';

	case(MT_STAIRS_DOWN):
	case(MT_STAIRS_DOWN_LONG):
		return '>';
	case(MT_STAIRS_UP):
	case(MT_STAIRS_UP_LONG):
		return '<';

	case(MT_HEALING_FOUNTAIN):
	case(MT_STATUE_MARBLE):
		return 225;

	case(MT_TABLE_WOODEN):
		return 194;

	case(MT_TOMBSTONE):
		return 239;

	case(MT_WALL_SANDSTONE):
	case(MT_WALL_STONE):
	case(MT_WALL_SUPER):
		return '#';

	case(MT_WATER):
		return 247;

	case(MT_STASH):
		return 254;

	case(MT_DRAINED_SHRINE):
	case(MT_MEDITATION_SHRINE):
		return 244;

	default:
		return '?';
	}
}

const colorType getMaptileColor(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ALCHEMY):
		return TCODColor::celadon;

	case(MT_GEMSTONE_PRESS):
		return TCODColor::lightPurple;

	case(MT_ANVIL):
		return TCODColor::grey;

	case(MT_FORGE):
		return TCODColor::flame;

	case(MT_STASH):
		return TCODColor::sepia;

	case(MT_CHEST_SMALL):
		return TCODColor::lightSepia;
	case(MT_CHEST_GLOWING):
		return TCODColor::lighterBlue;

	case(MT_FLOOR_CARPET):
		return TCODColor::darkRed;

	case(MT_BUSH):
	case(MT_GRASS):
		return TCODColor::darkGreen;

	case(MT_DOOR_STONE):
	case(MT_FLOOR_STONE):
	case(MT_FLOOR_STONE2):
	case(MT_SARCOPHAGUS):
	case(MT_WALL_STONE):
		return TCODColor::grey;

	case(MT_BARREL):
	case(MT_DOOR_WOODEN):
	case(MT_PEW_WOODEN):
	case(MT_RUBBLE):
	case(MT_TABLE_WOODEN):
		return TCODColor::sepia;

	case(MT_SAND):
	case(MT_WALL_SANDSTONE):
		return TCODColor::darkAmber;

	case(MT_STAIRS_DOWN):
	case(MT_STAIRS_UP):
		return TCODColor::lightestBlue;
	case(MT_STAIRS_DOWN_LONG):
	case(MT_STAIRS_UP_LONG):
		return TCODColor::gold;

	case(MT_STATUE_MARBLE):
	case(MT_TOMBSTONE):
		return TCODColor::lighterGrey;

	case(MT_WALL_SUPER):
		return TCODColor::darkGrey;

	case(MT_WATER):
		return TCODColor::lightBlue;

	case(MT_HEALING_FOUNTAIN):
		return TCODColor::lighterBlue;

	case(MT_RUNE_IMPRINTER):
	case(MT_MEDITATION_SHRINE):
		return TCODColor::lightPurple;
	case(MT_DRAINED_SHRINE):
		return TCODColor::grey;

	default:
		return COLOR_WHITE;
	}
}

const bool isMaptileWalkable(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ALCHEMY):
	case(MT_ANVIL):
	case(MT_BARREL):
	case(MT_CHEST_GLOWING):
	case(MT_CHEST_SMALL):
	case(MT_FORGE):
	case(MT_GEMSTONE_PRESS):
	case(MT_PEW_WOODEN):
	case(MT_HEALING_FOUNTAIN):
	case(MT_MEDITATION_SHRINE):
	case(MT_RUNE_IMPRINTER):
	case(MT_SARCOPHAGUS):
	case(MT_STASH):
	case(MT_STATUE_MARBLE):
	case(MT_TABLE_WOODEN):
	case(MT_TOMBSTONE):
	case(MT_WALL_SANDSTONE):
	case(MT_WALL_STONE):
	case(MT_WALL_SUPER):
		return false;
   
	default:
		return true;
	}
}

const bool isMaptileTransparent(const Maptile tl)
{
	switch (tl)
	{
	case(MT_DOOR_STONE):
	case(MT_DOOR_WOODEN):
	case(MT_WALL_SANDSTONE):
	case(MT_WALL_STONE):
	case(MT_WALL_SUPER):
		return false;

	default:
		return true;
	}
}

const bool doesMaptileFlicker(const Maptile tl)
{
	return tl == MT_WATER || tl == MT_HEALING_FOUNTAIN || tl == MT_ALCHEMY || tl == MT_FORGE || tl == MT_MEDITATION_SHRINE;
}

const bool isMaptileStairs(const Maptile tl)
{
	return tl == MT_STAIRS_DOWN || tl == MT_STAIRS_UP || tl == MT_STAIRS_UP_LONG || tl == MT_STAIRS_DOWN_LONG;
}

const string getSurfaceName(const Surface sf)
{
	switch (sf)
	{
	case(Surface::BONES):		return "Bones";
	case(Surface::CORPSE):		return "Corpse";
	case(Surface::POISON_OOZE):	return "Poison Ooze";
	case(Surface::SLUDGE):		return "Sludge";
	default:
		return "surface?";
	}
}

const colorType getSurfaceColor(const Surface sf)
{
	switch (sf)
	{
	case(Surface::BONES):		return TCODColor::desaturatedYellow;
	case(Surface::CORPSE):		return TCODColor::darkRed; 
	case(Surface::POISON_OOZE):	return TCODColor::lime;
	case(Surface::SLUDGE):		return TCODColor::sepia;
	default:
		return COLOR_WHITE;
	}
}

const int getSurfaceGlyph(const Surface sf)
{
	switch (sf)
	{
	case(Surface::BONES):
	case(Surface::CORPSE):
		return '%';

	case(Surface::POISON_OOZE):
	case(Surface::SLUDGE):
		return '~';

	default:
		return '?';
	}
}
