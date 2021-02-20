#include "monsterdata.h"



//	Monster appears alone, rather than in a group.
bool monsterdata::isSoloMonster(const MonsterType id)
{
	switch (id)
	{
	case(MonsterType::CARRION_PRINCE):
	case(MonsterType::GRIM_KNIGHT):
	case(MonsterType::SKINLESS_KNIGHT):
	case(MonsterType::SKULL_PILE):
	case(MonsterType::WALKING_SKULL_PILE):
	case(MonsterType::ZOMBIE_MASS):
		return true;

	default:
		return false;
	}
}


//	Random Tier 3 monster
const MonsterType monsterdata::rollMinibossForLevel(const int dl)
{
	if (dl < 2)
		return MonsterType::__NONE;
	else
	{
		int r = randint(1, 4);
		switch (r)
		{
		case(1):	return MonsterType::ZOMBIE_MASS;
		case(2):	return MonsterType::GRIM_KNIGHT;
		case(3):	return MonsterType::RAT_KING;
		default:	return MonsterType::WALKING_SKULL_PILE;
		}
	}
}

//	Tier-4 boss monster
const MonsterType monsterdata::rollBossForLevel(const int dl)
{
	//	Assemble list of options.
	vector<MonsterType> ops = { MonsterType::CORPSE_COLOSSUS, MonsterType::CARRION_PRINCE, };
	if (dl > 4)
		ops.push_back(MonsterType::SKINLESS_KNIGHT);

	//	Pick one.
	return ops[randrange(ops.size())];
}


//	Returns a list of monster types that can appear on the given level.
const vector<MonsterType> monsterdata::getMonstersForLevel(const int dl)
{
	if (dl < 2)
		return { MonsterType::CULTIST, MonsterType::ZOMBIE };

	else if (dl < 6)
		return { MonsterType::BLOAT, MonsterType::CORPSEFLY, MonsterType::CULTIST, MonsterType::SKULL_FLOATING,
				MonsterType::OOZE_ELECTRIC, MonsterType::SKELETON, MonsterType::SKULL_PILE, MonsterType::ZOMBIE };

	else if (dl < 10)
		return { MonsterType::CULTIST_INFESTED, MonsterType::OOZE_ELECTRIC, MonsterType::RAT_GIANT, MonsterType::OOZE_SLUDGE, MonsterType::SKELETON, MonsterType::SKULL_EXPLODING, MonsterType::ZOMBIE };

	else
		return { MonsterType::IMP, MonsterType::WRETCH, };
	
	//	fall-through (error)
	return { MonsterType::__NONE };
}


//	Seoncdary monster types that can be mixed into a monster group.
const vector<MonsterType> monsterdata::getAltVersions(const MonsterType id, const int dl)
{
	vector<MonsterType> alts;
	switch (id)
	{
	case(MonsterType::CULTIST):
		if (dl > 1)
			alts.push_back(MonsterType::CULTIST_MUTTERING);
		break;

	case(MonsterType::SKULL_FLOATING):
		if (dl > 3)
			alts.push_back(MonsterType::SKULL_FLAMING);
		if (dl > 5)
			alts.push_back(MonsterType::SKULL_EXPLODING);
		break;

	case(MonsterType::SKELETON):
		alts.push_back(MonsterType::SKELETON_ARCHER);
		if (dl > 2)
			alts.push_back(MonsterType::SKELETON_KNIGHT);
		if (dl > 3)
			alts.push_back(MonsterType::SKELETON_MAGE);
		if (dl > 4)
			alts.push_back(MonsterType::SKELETON_BURNING);
		if (dl > 5)
			alts.push_back(MonsterType::SKELETON_GOLD_PLATED);
		break;

	case(MonsterType::WRETCH):
		alts.push_back(MonsterType::WRETCH_PUKING);
		break;

	case(MonsterType::ZOMBIE):
		if (dl > 2)
			alts.push_back(MonsterType::ZOMBIE_ROTTED);
		if (dl > 5)
			alts.push_back(MonsterType::ZOMBIE_INFUSED);
		break;
	}
	return alts;
}


//	Returns a randomly-rolled 'group leader' monster type.
//	A result of '__NONE' is also possible, indicating no leader.
const MonsterType monsterdata::getGroupLeaderType(const MonsterType id, const int dl)
{
	switch (id)
	{
	case(MonsterType::CULTIST):
	case(MonsterType::CULTIST_INFESTED):
		if (dl > 3)
			return MonsterType::CULTIST_WINGED;
		break;

	case(MonsterType::IMP):
		return MonsterType::IMP_MEGA;

	case(MonsterType::SKELETON):	
		return MonsterType::SKELETON_GIANT;
	
	case(MonsterType::ZOMBIE):		
		if (dl > 1)
			return MonsterType::ZOMBIE_LARGE;
		break;
	}

	return MonsterType::__NONE;
}

//  List of flags that a monster with the given ID has by default.
vector<string> monsterdata::getMonsterFlags(const MonsterType id)
{
	switch (id)
	{
	case(MonsterType::BLOAT):
		return { "flits", "poison_burst", "less_health", "less_defence", "flying", };

	case(MonsterType::CARRION_PRINCE):
		return { "spawner", "more_defence", "undead", "poison_attack", "resists_poison", };

	case(MonsterType::CORPSE_COLOSSUS):
		return { "slow", "spawner", "more_damage", "less_defence", "more_health", "undead", };

	case(MonsterType::CORPSEFLY):
		return { "fast", "flits", "poison_attack", "flying" };

	case(MonsterType::CULTIST):
		return { "less_health", "less_damage" };
	case(MonsterType::CULTIST_INFESTED):
		return { "less_health", "spawner", "infested", };
	case(MonsterType::CULTIST_MUTTERING):
		return { "less_health", "casts_arcane_bolt" };
	case(MonsterType::CULTIST_WINGED):
		return { "casts_arcane_bolt", "flying", "flits", "casts_poison_spit" };

	case(MonsterType::GRIM_KNIGHT):
		return { "protected_heavy", "arcane_attack", "more_damage", "slow", "defended", };

	case(MonsterType::IMP):
		return { "protected", "less_health", "resists_fire", "casts_firebolt" };
	case(MonsterType::IMP_MEGA):
		return { "protected", "immune_fire", "casts_firebolt", };

	case(MonsterType::OOZE_ELECTRIC):
		return { "slow", "electric_attack", "immune_electric", };
	case(MonsterType::OOZE_SLUDGE):
		return { "flits", "spits_sludge", "more_health", "resists_electric" };

	case(MonsterType::RAT_GIANT):
		return { "less_health", "poison_attack", "fast", "defended", };
	case(MonsterType::RAT_KING):
		return { "spawner", "more_health", "inaccurate", "flits", "megaspawner", };

	case(MonsterType::ROACH):
		return { "fast", "flits", "poison_attack", "less_damage", "less_health", "less_defence" };

	case(MonsterType::SKELETON):
		return { "protected", "bones", "immune_poison", "undead", };
	case(MonsterType::SKELETON_ARCHER):
		return { "protected", "less_damage", "ranged_attack", "immune_poison", "bones", "undead", };
	case(MonsterType::SKELETON_BURNING):
		return { "protected", "fire_attack", "bones", "resists_fire", "undead" };
	case(MonsterType::SKELETON_GOLD_PLATED):
		return { "protected_heavy", "bones", "immune_poison", "electric_attack", "bones", "undead", "more_damage", };
	case(MonsterType::SKELETON_KNIGHT):
		return { "protected_heavy", "more_damage", "bones", "immune_poison", "defended", "undead" };
	case(MonsterType::SKELETON_MAGE):
		return { "protected_heavy", "less_health", "bones", "immune_poison", "casts_lightning", "undead" };
	case(MonsterType::SKELETON_GIANT):
		return { "protected_heavy", "more_damage", "bones", "immune_poison", "undead" };

	case(MonsterType::SKINLESS_KNIGHT):
		return { "protected_heavy", "electric_attack", "more_damage", "resists_electric", "knockback", };

	case(MonsterType::SKULL_EXPLODING):
		return { "flits", "less_health", "protected", "less_damage", "immune_poison", "undead", "flying", "fire_burst" };
	case(MonsterType::SKULL_FLAMING):
		return { "flits", "less_health", "protected", "less_damage", "bones", "immune_poison", "resists_fire", "flying", "fire_attack", "undead" };
	case(MonsterType::SKULL_FLOATING):
		return { "flits", "less_health", "protected", "less_damage", "bones", "immune_poison", "flying", "undead" };
	case(MonsterType::SKULL_PILE):
		return { "protected", "more_health", "no_attack", "immobile", "spawner", "undead", };
	case(MonsterType::WALKING_SKULL_PILE):
		return { "flits", "protected", "throws_bones", "immune_poison", "undead", };

	case(MonsterType::WRETCH):
		return { "slow", "more_health", "undead", };
	case(MonsterType::WRETCH_PUKING):
		return { "slow", "more_health", "undead", "casts_poison_spit", "poison_attack", "resists_poison" };

	case(MonsterType::ZOMBIE):
		return { "slow", "more_damage", "inaccurate", "undead", };
	case(MonsterType::ZOMBIE_INFUSED):
		return { "slow", "more_damage", "inaccurate", "undead", "arcane_attack", "resists_arcane" };
	case(MonsterType::ZOMBIE_LARGE):
		return { "more_health", "slow", "more_damage", "inaccurate", "undead" };
	case(MonsterType::ZOMBIE_MASS):
		return { "more_health", "slow", "more_damage", "spawner", "inaccurate", "flits", "undead", };
	case(MonsterType::ZOMBIE_ROTTED):
		return { "slow", "inaccurate", "poison_attack", "undead" };


	case(MonsterType::BOSS_PALLID_ROTKING):
		return { "slow", "poison_attack", "undead", "spawner", "spit_sludge", "casts_poison_spit", "more_health" };

	default:
		return {};
	}
}


//  Default tier of a monster of the given id, from 1-4.
int monsterdata::getDefaultMonsterTier(const MonsterType id)
{
	switch (id)
	{
	case(MonsterType::BOSS_PALLID_ROTKING):
		return 5;

	case(MonsterType::CARRION_PRINCE):
	case(MonsterType::CORPSE_COLOSSUS):
	case(MonsterType::SKINLESS_KNIGHT):
		return 4;

	case(MonsterType::GRIM_KNIGHT):
	case(MonsterType::RAT_KING):
	case(MonsterType::WALKING_SKULL_PILE):
	case(MonsterType::ZOMBIE_MASS):
		return 3;

	case(MonsterType::CULTIST_WINGED):
	case(MonsterType::IMP_MEGA):
	case(MonsterType::SKELETON_GIANT):
	case(MonsterType::SKELETON_GOLD_PLATED):
	case(MonsterType::SKULL_PILE):
	case(MonsterType::ZOMBIE_INFUSED):
	case(MonsterType::ZOMBIE_LARGE):
		return 2;

	default:
		return 1;
	}
}

//  Returns a triplet containing flavour information for a monster with the given ID (its name, glyph, & color)
monsterdata::flavourdat monsterdata::get_flavourdat_for_monster_id(MonsterType id)
{
	switch (id)
	{
	case(MonsterType::BLOAT):				return make_tuple("bloat", 'b', TCODColor::lightLime);
	case(MonsterType::CARRION_PRINCE):		return make_tuple("CARRION PRINCE", 'P', TCODColor::lime);
	case(MonsterType::CORPSEFLY):			return make_tuple("corpsefly", 'f', TCODColor::lightLime);
	case(MonsterType::CORPSE_COLOSSUS):		return make_tuple("CORPSE COLOSSUS", 'C', TCODColor::pink);
	case(MonsterType::CULTIST):				return make_tuple("cultist", 'u', TCODColor::pink);
	case(MonsterType::CULTIST_INFESTED):	return make_tuple("infested cultist", 'u', TCODColor::lime);
	case(MonsterType::CULTIST_MUTTERING):	return make_tuple("muttering cultist", 'u', TCODColor::lightPurple);
	case(MonsterType::CULTIST_WINGED):		return make_tuple("winged cultist", 'U', TCODColor::pink);
	case(MonsterType::GRIM_KNIGHT):			return make_tuple("Grim Knight", 'K', TCODColor::lightPurple);
	case(MonsterType::IMP):					return make_tuple("imp", 'i', TCODColor::flame);
	case(MonsterType::IMP_MEGA):			return make_tuple("mega-imp", 'I', TCODColor::flame);
	case(MonsterType::OOZE_ELECTRIC):		return make_tuple("electric ooze", 'o', TCODColor::yellow);
	case(MonsterType::OOZE_SLUDGE):			return make_tuple("sludge ooze", 'o', TCODColor::lightSepia);
	case(MonsterType::RAT_GIANT):			return make_tuple("giant rat", 'r', TCODColor::darkGreen);
	case(MonsterType::RAT_KING):			return make_tuple("Rat King", 'R', TCODColor::lightSepia);
	case(MonsterType::ROACH):				return make_tuple("roach", 'r', TCODColor::sepia);
	case(MonsterType::SKELETON):			return make_tuple("skeleton", 'k', TCODColor::desaturatedYellow);
	case(MonsterType::SKELETON_ARCHER):		return make_tuple("skeleton archer", 'k', TCODColor::yellow);
	case(MonsterType::SKELETON_BURNING):	return make_tuple("burning skeleton", 'k', TCODColor::flame);
	case(MonsterType::SKELETON_KNIGHT):		return make_tuple("skeleton knight", 'k', TCODColor::lighterGrey);
	case(MonsterType::SKELETON_GOLD_PLATED):return make_tuple("gold-plated skeleton", 'k', TCODColor::gold);
	case(MonsterType::SKELETON_MAGE):		return make_tuple("skeleton mage", 'k', TCODColor::fuchsia);
	case(MonsterType::SKELETON_GIANT):		return make_tuple("giant skeleton", 'K', TCODColor::desaturatedYellow);
	case(MonsterType::SKINLESS_KNIGHT):		return make_tuple("skinless knight", 'K', TCODColor::lighterOrange);
	case(MonsterType::SKULL_EXPLODING):		return make_tuple("exploding skull", 's', TCODColor::lightOrange);
	case(MonsterType::SKULL_FLAMING):		return make_tuple("flaming skull", 's', TCODColor::darkFlame);
	case(MonsterType::SKULL_FLOATING):		return make_tuple("floating skull", 's', TCODColor::lightestYellow);
	case(MonsterType::SKULL_PILE):			return make_tuple("skull pile", 234, TCODColor::lightestYellow);
	case(MonsterType::WALKING_SKULL_PILE):	return make_tuple("Walking Skull Pile", 'S', TCODColor::desaturatedYellow);
	case(MonsterType::WRETCH):				return make_tuple("wretch", 'w', TCODColor::crimson);
	case(MonsterType::WRETCH_PUKING):		return make_tuple("puking wretch", 'w', TCODColor::lime);
	case(MonsterType::ZOMBIE):				return make_tuple("zombie", 'z', TCODColor::green);
	case(MonsterType::ZOMBIE_INFUSED):		return make_tuple("infused zombie", 'z', TCODColor::fuchsia);
	case(MonsterType::ZOMBIE_LARGE):		return make_tuple("large zombie", 'Z', TCODColor::lightGreen);
	case(MonsterType::ZOMBIE_MASS):			return make_tuple("Zombie Mass", 'Z', TCODColor::lightPink);
	case(MonsterType::ZOMBIE_ROTTED):		return make_tuple("rotted zombie", 'z', TCODColor::darkLime);

	case(MonsterType::BOSS_PALLID_ROTKING):	return make_tuple("= THE PALLID ROTKING =", 'A', TCODColor::lightLime);

	default:
		return make_tuple("error_id", '?', COLOR_WHITE);
	}
}
		



//  Generate a monster with the given ID at the given level.
monsterPtr monsterdata::generate(MonsterType id, int level)
{
	//  get flavour information & generate the base monster
	string name; int glyph; colorType color;
	tie(name, glyph, color) = get_flavourdat_for_monster_id(id);   
	auto mon = monsterPtr(new monster(name, glyph, color, level, getDefaultMonsterTier(id), id));

	//  add default flags to the monster
	for (auto f : getMonsterFlags(id))
		mon->addFlag(f);

	return mon;
}
