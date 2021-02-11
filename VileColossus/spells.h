#pragma once

#include "enums.h"


enum class Spell
{
	//	Tier 1
	ARCANE_EMANATION,
	BLINK,
	CALL_LIGHTNING,
	CONJURE_FLAME,
	SMITE_EVIL,
	VENOMFANG,


	//	Tier 2
	ARCANE_PULSE,
	CHAIN_LIGHTNING,
	FIREBALL,

	__NONE,
};


//	Flavour data

const string getSpellName(const Spell sp);
const colorType getSpellColor(const Spell sp);
const string getSpellDescription(const Spell sp);;

const string getSpellNameFull(const Spell sp, const int lvl);



//	Effect data

int getSpellTier(const Spell sp);
int getSpellMPCost(const Spell sp, const int lvl);

bool isSpellTargeted(const Spell sp);
const int getSpellRange(const Spell sp, const int lvl);

bool doesSpellHaveDuration(const Spell sp);
const int getSpellDuration(const Spell sp, const int lvl);

bool doesSpellInflictDamage(const Spell sp);
const DamageType getSpellDamageType(const Spell sp);
const intpair getSpellDamage(const Spell sp, const int lvl);