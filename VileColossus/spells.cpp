#include "spells.h"



const vector<string> ROMAN_NUMERALS = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX" };



const string getSpellName(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):      return "Arcane Bolt";
	case(Spell::BLINK):					return "Blink";
	case(Spell::CALL_LIGHTNING):        return "Lightning Strike";
	case(Spell::CONJURE_FLAME):			return "Conjure Flame";
	case(Spell::SMITE_EVIL):			return "Smite Undead";
	case(Spell::VENOMFANG):				return "Venomfang";

	case(Spell::ARCANE_PULSE):			return "Arcane Pulse";
	case(Spell::CHAIN_LIGHTNING):		return "Chain Lightning";
	case(Spell::FIREBALL):				return "Fireball";
	case(Spell::TELEPORT):				return "Teleport";
	case(Spell::TOXIC_RADIANCE):		return "Toxic Radiance";

	case(Spell::CRYSTAL_SPEAR):			return "Crystal Spear";
	case(Spell::FIRESTORM):				return "Firestorm";
	case(Spell::STATIC_FIELD):			return "Static Field";

	default:
		return "unknown_spell_name";
	}
}

const colorType getSpellColor(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):		
	case(Spell::ARCANE_PULSE):
	case(Spell::CRYSTAL_SPEAR):
		return getDamageTypeColor(DTYPE_ARCANE);

	case(Spell::BLINK):
	case(Spell::TELEPORT):
		return TCODColor::fuchsia;
	
	case(Spell::CALL_LIGHTNING):		
	case(Spell::CHAIN_LIGHTNING):
	case(Spell::STATIC_FIELD):
		return getDamageTypeColor(DTYPE_ELECTRIC);

	case(Spell::CONJURE_FLAME):
	case(Spell::FIREBALL):
	case(Spell::FIRESTORM):
		return getDamageTypeColor(DTYPE_FIRE);

	case(Spell::TOXIC_RADIANCE):
	case(Spell::VENOMFANG):
		return getDamageTypeColor(DTYPE_POISON);

	case(Spell::SMITE_EVIL):
		return TCODColor::gold;

	default:
		return COLOR_WHITE;
	}
}

const string getSpellDescription(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):
		return "Hurls a bolt of arcane damage at a random visible enemy.";
	case(Spell::ARCANE_PULSE):
		return "You release pulses of arcane damage for a short duration, damaging everything adjacent to you.";
	case(Spell::BLINK):
		return "Warp in a chosen direction. Cannot pass through solid walls or creatures.";
	case(Spell::CALL_LIGHTNING):
		return "Strikes a random visible enemy with a bolt of lightning.";
	case(Spell::CHAIN_LIGHTNING):
		return "Hurl lightning in a chosen direction; it then arcs to additional nearby enemies.";
	case(Spell::CONJURE_FLAME):
		return "Hurl fire in a chosen direction.";
	case(Spell::CRYSTAL_SPEAR):
		return "Hurl a spear of arcane energy that penetrates multiple creatures in a line.";
	case(Spell::FIREBALL):
		return "Hurl an exploding ball of flame.";
	case(Spell::FIRESTORM):
		return "Release an explosion of fire that ignites the area around you.";
	case(Spell::SMITE_EVIL):
		return "Your next weapon attack inflicts massive bonus damage to an undead target. Lasts for more hits at higher levels.";
	case(Spell::STATIC_FIELD):
		return "You emit pulses of electricity at random intervals, which can chain through multiple enemies.";
	case(Spell::TELEPORT):
		return "Teleport in a chosen direction. Can pass through monsters, but not solid walls or doors.";
	case(Spell::TOXIC_RADIANCE):
		return "Inflicts poison damage in a radius around you and temporarily buffs your Poison Affinity.";
	case(Spell::VENOMFANG):
		return "Adds poison damage to your attacks for a while.";
	default:
		return "unknown_spell_description";
	}
}


//	Use Roman numerals for spell level, so it looks cool and shit
const string getSpellNameFull(const Spell sp, const int lvl)
{
	return getSpellName(sp) + " L" + to_string(lvl);
}


int getSpellTier(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):
	case(Spell::BLINK):
	case(Spell::CALL_LIGHTNING):
	case(Spell::CONJURE_FLAME):
	case(Spell::SMITE_EVIL):
	case(Spell::VENOMFANG):
		return 1;

	case(Spell::ARCANE_PULSE):
	case(Spell::CHAIN_LIGHTNING):
	case(Spell::FIREBALL):
	case(Spell::TELEPORT):
	case(Spell::TOXIC_RADIANCE):
		return 2;

	case(Spell::CRYSTAL_SPEAR):
	case(Spell::FIRESTORM):
	case(Spell::STATIC_FIELD):
		return 3;

	default:
		return 0;
	}
}

int getSpellMPCost(const Spell sp, const int lvl)
{
	return getSpellTier(sp) + (lvl / 5);
}


//	True if the spell requires a direction to cast. False otherwise.
bool isSpellTargeted(const Spell sp)
{
	switch (sp)
	{
	case(Spell::BLINK):
	case(Spell::CHAIN_LIGHTNING):
	case(Spell::CONJURE_FLAME):
	case(Spell::CRYSTAL_SPEAR):
	case(Spell::FIREBALL):
	case(Spell::TELEPORT):
		return true;

	default:
		return false;
	}
}

const int getSpellRange(const Spell sp, const int lvl)
{
	switch (sp)
	{
	case(Spell::BLINK):
		return 3 + lvl / 2;

	case(Spell::CHAIN_LIGHTNING):
		return 5;

	case(Spell::CONJURE_FLAME):
		return 6;

	case(Spell::CRYSTAL_SPEAR):
		return 10 + lvl / 3;

	case(Spell::FIREBALL):
		return 8 + lvl / 5;

	case(Spell::TELEPORT):
		return 3 + lvl / 2;

	case(Spell::TOXIC_RADIANCE):
		return 4;

	default:
		return 0;
	}
}

bool doesSpellHaveDuration(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_PULSE):
	case(Spell::SMITE_EVIL):
	case(Spell::STATIC_FIELD):
	case(Spell::TOXIC_RADIANCE):
	case(Spell::VENOMFANG):
		return true;

	default:
		return false;
	}
}

const int getSpellDuration(const Spell sp, const int lvl)
{
	switch (sp)
	{
	case(Spell::ARCANE_PULSE):		return lvl * 2;
	case(Spell::SMITE_EVIL):		return 1 + lvl / 2;
	case(Spell::STATIC_FIELD):		return lvl * 2;
	case(Spell::TOXIC_RADIANCE):	return lvl * 5;
	case(Spell::VENOMFANG):			return 2 + lvl;

	default:
		return 0;
	}
}

bool doesSpellInflictDamage(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_PULSE):
	case(Spell::BLINK):
	case(Spell::SMITE_EVIL):
	case(Spell::STATIC_FIELD):
	case(Spell::TELEPORT):
	case(Spell::VENOMFANG):
		return false;

	default:
		return true;
	}
}

const DamageType getSpellDamageType(const Spell sp)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):	
	case(Spell::ARCANE_PULSE):
	case(Spell::CRYSTAL_SPEAR):
		return DTYPE_ARCANE;

	case(Spell::CALL_LIGHTNING):	
	case(Spell::CHAIN_LIGHTNING):
		return DTYPE_ELECTRIC;

	case(Spell::CONJURE_FLAME):
	case(Spell::FIREBALL):
	case(Spell::FIRESTORM):
		return DTYPE_FIRE;

	case(Spell::TOXIC_RADIANCE):
		return DTYPE_POISON;

	default:
		return DTYPE_NORMAL;
	}
}

//	Normal damage range of the given spell at the given level.
const intpair getSpellDamage(const Spell sp, const int lvl)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):	return intpair(1, lvl * 5 + 1);
	case(Spell::CALL_LIGHTNING):	return intpair(1 + lvl, 2 + lvl * 2);
	case(Spell::CHAIN_LIGHTNING):	return intpair(3 + lvl, 3 + lvl * 3);
	case(Spell::CONJURE_FLAME):		return intpair(1 + lvl, 2 + lvl * 2);
	case(Spell::CRYSTAL_SPEAR):		return intpair(1, lvl * 20);
	case(Spell::FIREBALL):			return intpair(lvl * 2, lvl * 3);
	case(Spell::FIRESTORM):			return intpair(5, 5 + lvl * 2);
	case(Spell::TOXIC_RADIANCE):	return intpair(1, lvl * 4);
	default:
		return intpair(0, 0);
	}
}
