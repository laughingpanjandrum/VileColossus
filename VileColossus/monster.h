#pragma once

#include "creature.h"
#include "lootgen.h"
#include "maptile.h"


enum class MonsterType
{
	//	Cathedral monsters
	BLOAT,
	CARRION_PRINCE,
	CORPSE_COLOSSUS,
	CORPSEFLY,
	CULTIST, CULTIST_INFESTED, CULTIST_MUTTERING, CULTIST_WINGED,
	OOZE_ELECTRIC, OOZE_SLUDGE,
	GRIM_KNIGHT,
	OGRE, OGRE_BERSERKER, OGRE_BRUTE,
	RAT_GIANT, RAT_KING, ROACH,
	SKELETON, SKELETON_ARCHER, SKELETON_BURNING, SKELETON_GOLD_PLATED, SKELETON_KNIGHT, SKELETON_MAGE, SKELETON_GIANT,
	SKULL_EXPLODING, SKULL_FLAMING, SKULL_FLOATING, SKULL_PILE,
	SKINLESS_KNIGHT,
	WALKING_SKULL_PILE,
	ZOMBIE, ZOMBIE_INFUSED, ZOMBIE_LARGE, ZOMBIE_MASS, ZOMBIE_ROTTED,

	BOSS_PALLID_ROTKING, BOSS_ROTKING_BURNED, BOSS_ROTKING_INFUSED, BOSS_ROTKING_DESOLATE,


	//	Hell monsters
	COLOSSUS_IRON,
	CULTIST_ASCENDED,
	DEMON_PRINCE,
	FLAME_CONJURER,
	IMP, IMP_MEGA,
	LIGHTNING_SPIRE,
	ORB_FLAME,
	QUASIT,
	SPIDER, SPIDER_FLAMING, SPIDER_OGRE, SPIDER_PHASE, SPIDER_TITAN,
	WORM_DEMON,
	VAMPIRE, VAMPIRE_PRINCE, VAMPIRE_SPAWN,
	WRAITH, WRAITH_GREAT, WRAITH_MOON,
	WRETCH, WRETCH_PUKING,

	BOSS_DEMON_LORD, BOSS_WRAITH_KING, BOSS_WRETCHED_PRINCE,


	//	Abyss monsters

	ABYSSAL_WRAITH,
	BLOOD_BLOB, BLOOD_LEECH, 
	BONES_BLOODY, BONES_BURNING,
	CRAB_TITAN,
	CULTIST_DOGGOSAN, 
	DEMON_PUTRESCENT,
	KNIGHT_DREAD,
	NIGHTGAUNT,
	PALE_KNIGHT, PALE_SCHOLAR,
	STARSPAWN,
	STAR_VAMPIRE,
	TENTACLE_HORROR, TENTACLE,
	VILESPAWN,

	BOSS_ABYSS_LORD,
	BOSS_DROWNED_DOGOSSA, BOSS_VIRIDIAN_PRINCE, BOSS_TOMB_LORD_AMOG,
	BOSS_VILE_COLOSSUS,
	BOSS_VILEDRAGON,


	__NONE,
};





class monster : public creature
{
public:

	monster() {}
	monster(string name, int glyph, colorType color, int lvl, int tier, MonsterType tp);
	~monster() {}


	//	changing attributes of active monsters
	void adjustDamage(const int adj) { _baseDamage += adj; }
	void adjustMaxHealth(const int adj) { _maxHealth += adj; }


	//	overriden from base class

	virtual int getMaxHealth() const { return _maxHealth; }
	virtual int getMaxSpellShield() const { return _maxSpellShield; }

	virtual int getAccuracy() const { return _accuracy; }
	virtual int getDefenceValue() const;
	virtual int getArmourValue() const { return _protection; }

	virtual int getAttackSpeed() const { return _attackSpeed; }
	virtual int getWeaponDamage() const { return _baseDamage; }
	virtual int getCriticalChance() const { return 0; }
	virtual int getCriticalMultiplier() const { return 0; }

	virtual int getKnockbackChance() const;

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

	virtual bool isEthereal() const { return hasFlag("ethereal"); }
	virtual bool isFlying() const { return hasFlag("flying"); }
	virtual bool isInvisible() const { return hasFlag("unseen") && _damageTaken == 0; }
	virtual bool isUndead() const { return hasFlag("undead"); }


	//	Flags

	const MonsterType getType() const { return _type; }

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