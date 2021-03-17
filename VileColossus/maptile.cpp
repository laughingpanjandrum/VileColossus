#include "maptile.h"

const string getMaptileName(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ABYSSAL_ALTAR):		return "Abyssal Altar";
	case(MT_ABYSSAL_GATE):		return "Abyssal Gate";
	case(MT_ALCHEMY):			return "alembic of alchemy";
	case(MT_ANVIL):				return "anvil";
	case(MT_BARREL):			return "wooden barrel";
	case(MT_BUSH):              return "bush";
	case(MT_CHEST_GLOWING):		return "glowing chest";
	case(MT_CHEST_LUMINOUS):	return "luminous chest";
	case(MT_CHEST_RADIANT):		return "radiant chest";
	case(MT_CHEST_SMALL):		return "small chest";
	case(MT_DEMONFORGE):		return "THE DEMONFORGE";
	case(MT_DOOR_STONE):        return "stone door";
	case(MT_DOOR_WOODEN):		return "wooden door";
	case(MT_DRAINED_SHRINE):	return "drained shrine";
	case(MT_FLOOR_CARPET):      return "carpet";
	case(MT_FLOOR_HOT):			return "hellstone floor";
	case(MT_FLOOR_STONE):       
	case(MT_FLOOR_STONE2):		return "stone floor";
	case(MT_FLOOR_VOID):		return "void";
	case(MT_FORGE):				return "enchanted forge";
	case(MT_GEMSTONE_PRESS):	return "gemstone press";
	case(MT_GRASS):				return "grass";
	case(MT_HEALING_FOUNTAIN):	return "fountain of life";
	case(MT_HELLPORTAL_DOWN):	return "descending hellportal";
	case(MT_HELLPORTAL_UP):		return "ascending hellportal";
	case(MT_LAVA):				return "molten ooze";
	case(MT_MEDITATION_SHRINE):	return "Shrine of Meditation";
	case(MT_PEW_WOODEN):        return "wooden pew";
	case(MT_RUBBLE):            return "rubble";
	case(MT_RUNE_IMPRINTER):	return "rune imprinter";
	case(MT_SAND):              return "sand";
	case(MT_SARCOPHAGUS):		return "sarcophagus";
	case(MT_STAIRS_DOWN):       return "stairs down";
	case(MT_STAIRS_DOWN_LONG):	return "stairs down to depths";
	case(MT_STAIRS_UP):         return "stairs up";
	case(MT_STAIRS_UP_LONG):	return "stairs up to surface";
	case(MT_STASH):				return "stash";
	case(MT_STATUE_BLOODY):		return "bloody statue";
	case(MT_STATUE_MARBLE):     return "marble statue";
	case(MT_TABLE_WOODEN):      return "wooden table";
	case(MT_TEMPLE_PORTAL):		return "temple portal";
	case(MT_THORNS):			return "hellthorns";
	case(MT_TOMBSTONE):			return "tombstone";
	case(MT_TREE_DEAD):			return "dead tree";
	case(MT_WALL_BLOODY):		return "bloody wall";
	case(MT_WALL_ICE):			return "ice block";
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
		return 236;

	case(MT_GEMSTONE_PRESS):
		return 237;

	case(MT_ANVIL):
	case(MT_RUNE_IMPRINTER):
		return 251;

	case(MT_BARREL):
		return '0';

	case(MT_ABYSSAL_GATE):
	case(MT_HELLPORTAL_DOWN):
	case(MT_HELLPORTAL_UP):
	case(MT_TEMPLE_PORTAL):
		return 239;

	case(MT_DEMONFORGE):
	case(MT_FORGE):
		return 233;

	case(MT_BUSH):
		return '"';

	case(MT_CHEST_GLOWING):
	case(MT_CHEST_LUMINOUS):
	case(MT_CHEST_RADIANT):
	case(MT_CHEST_SMALL):
		return 8;

	case(MT_DOOR_STONE):
	case(MT_DOOR_WOODEN):
		return '+';

	case(MT_FLOOR_CARPET):
		return ':';

	case(MT_FLOOR_STONE2):
		return ',';

	case(MT_FLOOR_HOT):
	case(MT_FLOOR_STONE):
	case(MT_FLOOR_VOID):
	case(MT_GRASS):
	case(MT_SAND):
		return 250;

	case(MT_ABYSSAL_ALTAR):
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
	case(MT_STATUE_BLOODY):
	case(MT_STATUE_MARBLE):
		return 225;

	case(MT_TABLE_WOODEN):
		return 194;

	case(MT_TOMBSTONE):
		return 22;

	case(MT_WALL_BLOODY):
	case(MT_WALL_ICE):
	case(MT_WALL_SANDSTONE):
	case(MT_WALL_STONE):
	case(MT_WALL_SUPER):
		return '#';

	case(MT_LAVA):
	case(MT_WATER):
		return 247;

	case(MT_TREE_DEAD):
		return 5;

	case(MT_STASH):
		return 254;

	case(MT_DRAINED_SHRINE):
	case(MT_MEDITATION_SHRINE):
		return 244;

	case(MT_THORNS):
		return 15;

	default:
		return '?';
	}
}

const colorType getMaptileColor(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ABYSSAL_ALTAR):
		return TCODColor::lightPurple;

	case(MT_ALCHEMY):
		return TCODColor::celadon;

	case(MT_GEMSTONE_PRESS):
		return TCODColor::lightPurple;

	case(MT_ANVIL):
		return TCODColor::grey;

	case(MT_FORGE):
		return TCODColor::flame;
	case(MT_DEMONFORGE):
		return TCODColor::darkOrange;

	case(MT_STASH):
		return TCODColor::sepia;

	case(MT_CHEST_SMALL):
		return TCODColor::lightSepia;
	case(MT_CHEST_GLOWING):
		return TCODColor::lighterBlue;
	case(MT_CHEST_LUMINOUS):
		return TCODColor::lighterPurple;
	case(MT_CHEST_RADIANT):
		return TCODColor::lighterOrange;

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

	case(MT_FLOOR_VOID):
		return TCODColor::darkerGrey;

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
		return TCODColor::lightSea;
	case(MT_STAIRS_DOWN_LONG):
	case(MT_STAIRS_UP_LONG):
		return TCODColor::gold;

	case(MT_STATUE_BLOODY):
		return TCODColor::crimson;

	case(MT_STATUE_MARBLE):
	case(MT_TOMBSTONE):
		return TCODColor::lighterGrey;

	case(MT_TREE_DEAD):
		return TCODColor::lightGreen;

	case(MT_WALL_SUPER):
		return TCODColor::darkGrey;

	case(MT_WALL_BLOODY):
		return TCODColor::darkCrimson;

	case(MT_WALL_ICE):
		return TCODColor::darkCyan;

	case(MT_FLOOR_HOT):
	case(MT_LAVA):
		return TCODColor::flame;

	case(MT_WATER):
		return TCODColor::lightBlue;

	case(MT_HEALING_FOUNTAIN):
		return TCODColor::lighterBlue;

	case(MT_RUNE_IMPRINTER):
	case(MT_MEDITATION_SHRINE):
		return TCODColor::lightPurple;
	case(MT_DRAINED_SHRINE):
		return TCODColor::grey;

	case(MT_THORNS):
		return TCODColor::lightGreen;

	case(MT_ABYSSAL_GATE):
		return TCODColor::lightPurple;
	case(MT_HELLPORTAL_DOWN):
		return TCODColor::orange;
	case(MT_HELLPORTAL_UP):
		return TCODColor::lightBlue;
	case(MT_TEMPLE_PORTAL):
		return TCODColor::lightCrimson;

	default:
		return COLOR_WHITE;
	}
}

const bool isMaptileWalkable(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ABYSSAL_ALTAR):
	case(MT_ALCHEMY):
	case(MT_ANVIL):
	case(MT_BARREL):
	case(MT_CHEST_GLOWING):
	case(MT_CHEST_LUMINOUS):
	case(MT_CHEST_RADIANT):
	case(MT_CHEST_SMALL):
	case(MT_DEMONFORGE):
	case(MT_FORGE):
	case(MT_GEMSTONE_PRESS):
	case(MT_PEW_WOODEN):
	case(MT_HEALING_FOUNTAIN):
	case(MT_MEDITATION_SHRINE):
	case(MT_RUNE_IMPRINTER):
	case(MT_SARCOPHAGUS):
	case(MT_STASH):
	case(MT_STATUE_BLOODY):
	case(MT_STATUE_MARBLE):
	case(MT_TABLE_WOODEN):
	case(MT_TOMBSTONE):
	case(MT_TREE_DEAD):
	case(MT_WALL_BLOODY):
	case(MT_WALL_ICE):
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
	case(MT_TREE_DEAD):
	case(MT_WALL_BLOODY):
	case(MT_WALL_SANDSTONE):
	case(MT_WALL_STONE):
	case(MT_WALL_SUPER):
		return false;

	default:
		return true;
	}
}

//	Will break if we shoot/bump into it.
const bool isMaptileBreakable(const Maptile tl)
{
	return tl == MT_BARREL || tl == MT_PEW_WOODEN || tl == MT_TABLE_WOODEN || tl == MT_TOMBSTONE || tl == MT_TREE_DEAD || tl == MT_WALL_ICE;
}

const bool doesMaptileFlicker(const Maptile tl)
{
	return tl == MT_WATER || tl == MT_HEALING_FOUNTAIN || tl == MT_ALCHEMY || tl == MT_FORGE || tl == MT_MEDITATION_SHRINE || tl == MT_LAVA || tl == MT_THORNS || tl == MT_ABYSSAL_ALTAR ||
		tl == MT_HELLPORTAL_DOWN || tl == MT_HELLPORTAL_UP || tl == MT_CHEST_LUMINOUS || tl == MT_DEMONFORGE || tl == MT_TEMPLE_PORTAL || tl == MT_FLOOR_VOID || tl == MT_ABYSSAL_GATE;
}

const bool isMaptileStairs(const Maptile tl)
{
	return tl == MT_STAIRS_DOWN || tl == MT_STAIRS_UP || tl == MT_STAIRS_UP_LONG || tl == MT_STAIRS_DOWN_LONG || tl == MT_HELLPORTAL_DOWN || tl == MT_HELLPORTAL_UP || tl == MT_TEMPLE_PORTAL || tl == MT_ABYSSAL_GATE;
}

//	Returns True if a maptile should have draw priority over surfaces.
const bool drawMaptileOverSurface(const Maptile tl)
{
	return isMaptileStairs(tl) || tl == MT_CHEST_GLOWING || tl == MT_CHEST_LUMINOUS || tl == MT_CHEST_RADIANT || tl == MT_CHEST_SMALL || tl == MT_THORNS || tl == MT_LAVA || tl == MT_WATER;
}

const bool isSpecialMaptile(const Maptile tl)
{
	return tl == MT_ANVIL || tl == MT_DEMONFORGE || tl == MT_FORGE || tl == MT_GEMSTONE_PRESS || tl == MT_HEALING_FOUNTAIN || tl == MT_MEDITATION_SHRINE || tl == MT_RUNE_IMPRINTER || tl == MT_STASH ||
		tl == MT_ABYSSAL_GATE;
}

const string getMaptileDescription(const Maptile tl)
{
	switch (tl)
	{
	case(MT_ABYSSAL_ALTAR):		return "Move into this tile to place ritual materials to open gates into the Abyss.";
	case(MT_ANVIL):				return "Move into this tile to open the Anvil. Here you can disassemble and repair items in your inventory.";
	case(MT_DEMONFORGE):		return "Move into this tile to open the Demonforge. Here you can fabricate items and transmute materials.";
	case(MT_FORGE):				return "Move into this tile to open the Enchanted Forge. Here you can repair and enhance your equipped items.";
	case(MT_GEMSTONE_PRESS):	return "Move into this tile to open the Gemstone Press. Here you can fabricate gems and embed them into your equipment.";
	case(MT_HEALING_FOUNTAIN):	return "Move into this tile to heal yourself to full and replenished your flask's charges.";
	case(MT_MEDITATION_SHRINE):	return "Move into this tile to save your game.";
	case(MT_RUNE_IMPRINTER):	return "Move into this tile to memorize or enhance spell runes.";
	case(MT_STASH):				return "Move into this tile to manage your stash. Here you can store and retrieve items for use later. Items in the stash are safe from loss.";
	}
	return "";
}



const string getSurfaceName(const Surface sf)
{
	switch (sf)
	{
	case(Surface::BONES):		return "Bones";
	case(Surface::CORPSE):		return "Corpse";
	case(Surface::FIRE):		return "Fire";
	case(Surface::POISON_OOZE):	return "Poison Ooze";
	case(Surface::SLUDGE):		return "Sludge";
	case(Surface::WEB):			return "Web";
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
	case(Surface::FIRE):		return TCODColor::flame;
	case(Surface::POISON_OOZE):	return TCODColor::lime;
	case(Surface::SLUDGE):		return TCODColor::sepia;
	case(Surface::WEB):			return TCODColor::lightestGrey;
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
	case(Surface::WEB):
		return '%';

	case(Surface::FIRE):
		return '^';

	case(Surface::POISON_OOZE):
	case(Surface::SLUDGE):
		return '~';

	default:
		return '?';
	}
}
