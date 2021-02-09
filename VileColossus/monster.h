#pragma once

#include "creature.h"
#include "lootgen.h"
#include "maptile.h"


enum class MonsterType
{
	BLOAT,
	CARRION_PRINCE,
	CORPSE_COLOSSUS,
	CORPSEFLY,
	CULTIST, CULTIST_INFESTED, CULTIST_MUTTERING, CULTIST_WINGED,
	OOZE_ELECTRIC, OOZE_SLUDGE,
	GRIM_KNIGHT,
	ROACH,
	SKELETON, SKELETON_ARCHER, SKELETON_BURNING, SKELETON_GOLD_PLATED, SKELETON_KNIGHT, SKELETON_MAGE, SKELETON_GIANT,
	SKULL_EXPLODING, SKULL_FLAMING, SKULL_FLOATING, SKULL_PILE,
	WALKING_SKULL_PILE,
	ZOMBIE, ZOMBIE_INFUSED, ZOMBIE_LARGE, ZOMBIE_MASS, ZOMBIE_ROTTED,
	__NONE,
};





class monster : public creature
{
public:

	monster() {}
	monster(string name, int glyph, colorType color, int lvl, int tier, MonsterType tp);
	~monster() {}

	//	to be overriden

	virtual int getMaxHealth() const { return _maxHealth; }
	virtual int getMaxSpellShield() const { return _maxSpellShield; }

	virtual int getAccuracy() const { return _accuracy; }
	virtual int getDefenceValue() const { return _defence; }
	virtual int getArmourValue() const { return _protection; }

	virtual int getAttackSpeed() const { return _attackSpeed; }
	virtual int getWeaponDamage() const { return _baseDamage; }
	virtual int getCriticalChance() const { return 0; }
	virtual int getCriticalMultiplier() const { return 0; }

	virtual int getMoveEnergyCost() const;

	virtual bool usingRangedWeapon() const { return _attackRange > 0; }
	virtual int getMaxAttackRange() const { return _attackRange; }

	virtual int getWeaponDamageOfType(const DamageType dt) const;

	virtual int getResistance(const DamageType dt) const;


	//	Corpse details

	const Surface getCorpseType() const;


	//	Spells/abilities

	const string rollRaySpellToCast() const;
	const MonsterType rollMonsterToSpawn() const;

	virtual bool isFlying() const { return hasFlag("flying"); }
	virtual bool isUndead() const { return hasFlag("undead"); }


	//	Flags

	void addFlag(const string flag);
	bool hasFlag(const string flag) const;

	const vector<string> getAllFlags() const { return _flags; }


	//	Targeting

	creaturePtr _target;
	intpair _targetPt;

	bool hasTarget() const { return _target != nullptr; }


	//	Public data

	int _tier;
	bool _doneDeathDrops;

	int _spawnDelay;


protected:

	int _maxHealth;
	int _maxSpellShield;

	int _accuracy;
	int _defence;
	int _protection;
	int _attackRange;

	int _attackSpeed;
	int _baseDamage;

	vector<string> _flags;
	MonsterType _type;

	vector<string> _spells;

};
typedef shared_ptr<monster> monsterPtr;