#include "monster.h"


//	Generates a 'default' monster of the given level.
monster::monster(string name, int glyph, colorType color, int lvl, int tier, MonsterType tp) :
	creature(name, glyph, color), _doneDeathDrops(false), _type(tp)
{
	//	stats based on level
	const int base_hp = lootgen::getBaseHealthForLevel(lvl);
	const int base_dam = lootgen::getBaseDamageForLevel(lvl);

	//	scale up stats based on our tier
	switch (tier)
	{
	case(1):	
		_maxHealth = base_hp; 
		_baseDamage = base_dam / 2;
		break;
	case(2):	
		_maxHealth = base_hp + base_hp / 2; 
		_baseDamage = base_dam;
		break;
	case(3):	
		_maxHealth = base_hp * 2; 
		_baseDamage = base_dam;
		break;
	case(4):	
		_maxHealth = base_hp * 4; 
		_baseDamage = base_dam + base_dam / 2;
		break;
	case(5):
		_maxHealth = base_hp * 8;
		_baseDamage = base_dam + base_dam / 2;
		break;
	}

	//	remember level and tier
	_level = lvl;
	_tier = tier;

	//	other default attributes
	_attackRange = 0;
	_protection = 0;
	_defence = lootgen::getBaseDefenceForLevel(lvl);
	_accuracy = lootgen::getBaseAccuracyForLevel(lvl);
	_attackSpeed = 10;
	_spawnDelay = 0;
}


int monster::getDefenceValue() const
{
	int total = _defence;
	if (hasStatusEffect(STATUS_STAGGER))
		total -= total / 3;
	return total;
}

int monster::getKnockbackChance() const
{
	if (hasFlag("knockback"))
		return 50;
	else
		return 0;
}

int monster::getMoveEnergyCost() const
{
	int cost = ENERGY_COST_BASE;
	if (hasFlag("slow"))
		cost += cost / 2;
	else if (hasFlag("fast"))
		cost -= cost / 2;
	return cost;
}


int monster::getWeaponDamageOfType(const DamageType dt) const
{
	int total = 0;

	switch (dt)
	{
	case(DTYPE_ARCANE):
		if (hasFlag("arcane_attack"))
			total += getWeaponDamage();
		break;

	case(DTYPE_FIRE):
		if (hasFlag("fire_attack"))
			total += getWeaponDamage();
		break;

	case(DTYPE_ELECTRIC):
		if (hasFlag("electric_attack"))
			total += getWeaponDamage();
		break;

	case(DTYPE_POISON):
		if (hasFlag("poison_attack"))
			total += getWeaponDamage();
		break;
	}

	return total;
}

int monster::getResistance(const DamageType dt) const
{
	int total = 0;
	switch (dt)
	{
	case(DTYPE_ARCANE):
		if (hasFlag("immune_arcane"))
			total += 100;
		else if (hasFlag("resists_arcane"))
			total += 67;
		break;

	case(DTYPE_ELECTRIC):
		if (hasFlag("immune_electric"))
			total += 100;
		else if (hasFlag("resists_electric"))
			total += 67;
		break;

	case(DTYPE_FIRE):
		if (hasFlag("immune_fire"))
			total += 100;
		else if (hasFlag("resists_fire"))
			total += 67;
		break;

	case(DTYPE_POISON):
		if (hasFlag("immune_poison"))
			total += 100;
		else if (hasFlag("resists_poison"))
			total += 67;
		break;
	}
	return total;
}


//	Type of corpse we drop on death.
const Surface monster::getCorpseType() const
{
	if		(hasFlag("no_corpse"))	return Surface::__NONE;
	else if (hasFlag("bones"))		return Surface::BONES;
	else							return Surface::CORPSE;
}


//	Randomly selects a ray spell to cast. If we don't know any, returns an empty string.
//	IF we know more than one, picks one at random.
const string monster::rollRaySpellToCast() const
{
	if (!_spells.empty())
		return _spells[randrange(_spells.size())];
	return "";
}


//	Decide what type of monster to spawn. Based on our type.
const MonsterType monster::rollMonsterToSpawn() const
{
	int r;
	switch (_type)
	{
	case(MonsterType::BOSS_PALLID_ROTKING):
	case(MonsterType::BOSS_ROTKING_BURNED):
	case(MonsterType::BOSS_ROTKING_DESOLATE):
	case(MonsterType::BOSS_ROTKING_INFUSED):
		r = randint(1, 3);
		if		(r == 1)	return MonsterType::BLOAT;
		else if (r == 2)	return MonsterType::ZOMBIE_ROTTED;
		else if (r == 3)	return MonsterType::CULTIST_INFESTED;
		break;

	case(MonsterType::BOSS_DEMON_LORD):
		r = randint(1, 4);
		if		(r <= 3)	return MonsterType::IMP;
		else if (r == 4)	return MonsterType::IMP_MEGA;
		break;

	case(MonsterType::BOSS_WRAITH_KING):
		r = randint(1, 3);
		if (r <= 2)		return MonsterType::WRAITH;
		else			return MonsterType::WRAITH_MOON;
		break;

	case(MonsterType::BOSS_WRETCHED_PRINCE):
		r = randint(1, 3);
		if (r == 1)	return MonsterType::WRETCH_PUKING;
		else		return MonsterType::WRETCH;
		break;

	case(MonsterType::BOSS_DROWNED_DOGOSSA):
		return MonsterType::TENTACLE_HORROR;

	case(MonsterType::BOSS_TOMB_LORD_AMOG):
		return MonsterType::BLOOD_LEECH;

	case(MonsterType::BOSS_VIRIDIAN_PRINCE):
		return MonsterType::LIGHTNING_SPIRE;

		
	case(MonsterType::BOSS_VILE_COLOSSUS):
		return MonsterType::VILESPAWN;



	case(MonsterType::BLOOD_LEECH):
		return MonsterType::BLOOD_BLOB;

	case(MonsterType::CARRION_PRINCE):
		r = randint(1, 2);
		if (r == 1)	return MonsterType::CORPSEFLY;
		else		return MonsterType::ROACH;

	case(MonsterType::CORPSE_COLOSSUS):
		r = randint(1, 7);
		if		(r <= 3)	return MonsterType::ZOMBIE;
		else if (r <= 6)	return MonsterType::ZOMBIE_ROTTED;
		else				return MonsterType::ZOMBIE_LARGE;
		break;

	case(MonsterType::CULTIST_ASCENDED):
		return MonsterType::WRETCH;

	case(MonsterType::CULTIST_INFESTED):
		return MonsterType::ROACH;

	case(MonsterType::FLAME_CONJURER):
		return MonsterType::ORB_FLAME;

	case(MonsterType::RAT_KING):
		return MonsterType::RAT_GIANT;

	case(MonsterType::SKULL_PILE):
		return MonsterType::SKULL_FLOATING;

	case(MonsterType::SPIDER_TITAN):
		r = randint(1, 10);
		if		(r == 1)	return MonsterType::SPIDER_OGRE;
		else if (r <= 5)	return MonsterType::SPIDER_FLAMING;
		else				return MonsterType::SPIDER;
		break;

	case(MonsterType::ZOMBIE_MASS):
		if (roll_one_in(4))
			return MonsterType::ZOMBIE_ROTTED;
		else
			return MonsterType::ZOMBIE;

	default:
		return MonsterType::__NONE;
	}
}


//	Add a new flag. This may cause our stats to change immediately, or it may have an active effect.
//	If we already have this flag, nothing happens.
void monster::addFlag(const string flag)
{
	if (!hasFlag(flag))
	{
		//	Remember that we have this flag
		_flags.push_back(flag);


		//	Apply stat-altering effects if necessary
		if (flag == "inaccurate")
			_accuracy -= _accuracy / 2;
		else if (flag == "less_damage")
			_baseDamage -= _baseDamage / 2;
		else if (flag == "less_defence")
			_defence /= 2;
		else if (flag == "less_health")
			_maxHealth = MAX(1, _maxHealth - _maxHealth / 2);
		else if (flag == "more_health")
			_maxHealth += _maxHealth / 2;
		else if (flag == "more_damage")
			_baseDamage += _baseDamage / 2;
		else if (flag == "protected")
			_protection = lootgen::getBaseProtectionForLevel(_level);
		else if (flag == "protected_heavy")
			_protection = lootgen::getHeavyProtectionForLevel(_level);
		else if (flag == "ranged_attack")
			_attackRange = 8;
		else if (flag == "defended")
			_defence += _defence / 2;


		//	Sepcial attacks reduce base damage
		/*else if (flag == "arcane_attack")
			_baseDamage /= 2;*/
		else if (flag == "electric_attack")
			_baseDamage /= 2;
		else if (flag == "fire_attack")
			_baseDamage /= 2;
		else if (flag == "poison_attack")
			_baseDamage /= 2;


		//	Indicates that we know a spell
		else if (flag == "casts_poison_spit")
			_spells.push_back("poison_spit");
		else if (flag == "casts_arcane_bolt")
			_spells.push_back("arcane_bolt");
		else if (flag == "casts_firebolt")
			_spells.push_back("firebolt");
		else if (flag == "casts_fireblast")
			_spells.push_back("fireblast");
		else if (flag == "casts_lightning")
			_spells.push_back("lightning");
		else if (flag == "spits_sludge")
			_spells.push_back("sludge");
	}
}


bool monster::hasFlag(const string flag) const
{
	return find(_flags.begin(), _flags.end(), flag) != _flags.end();
}
