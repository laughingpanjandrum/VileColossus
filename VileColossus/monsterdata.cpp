#include "monsterdata.h"



//	Random Tier 3 monster
const MonsterType monsterdata::rollMinibossForLevel(const int dl)
{
	if (dl < 2)
		return MonsterType::__NONE;
	else if (dl >= 10)
	{
		int r = randint(1, 3);
		switch (r)
		{
		case(1):	return MonsterType::FLAME_CONJURER;
		case(2):	return MonsterType::SPIDER_PHASE;
		default:	return MonsterType::WORM_DEMON;
		}
	}
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
	vector<MonsterType> ops;
	
	//	Lower tier
	if (dl < 10)
	{
		ops = { MonsterType::CORPSE_COLOSSUS, MonsterType::CARRION_PRINCE, };
		if (dl > 4)
			ops.push_back(MonsterType::SKINLESS_KNIGHT);
	}

	//	Higher tier
	else
	{
		ops = { MonsterType::COLOSSUS_IRON };
		if (dl > 11)
		{
			ops.push_back(MonsterType::DEMON_PRINCE);
			ops.push_back(MonsterType::VAMPIRE_PRINCE);
			ops.push_back(MonsterType::SPIDER_TITAN);
		}
	}

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
		return { MonsterType::CULTIST_INFESTED, MonsterType::OOZE_ELECTRIC, MonsterType::RAT_GIANT, MonsterType::OGRE, 
				MonsterType::OOZE_SLUDGE, MonsterType::SKELETON, MonsterType::SKULL_EXPLODING, MonsterType::ZOMBIE };

	else if (dl < 12)
		return { MonsterType::LIGHTNING_SPIRE, MonsterType::IMP, MonsterType::QUASIT, MonsterType::WRETCH, };
	else
		return { MonsterType::CULTIST_ASCENDED, MonsterType::LIGHTNING_SPIRE, MonsterType::IMP, MonsterType::QUASIT, MonsterType::SPIDER, MonsterType::VAMPIRE_SPAWN, MonsterType::WRETCH, };
	
	//	fall-through (error)
	return { MonsterType::__NONE };
}


//	Returns a random monster type of the options available for the given depth.
const MonsterType monsterdata::rollMonsterForLevel(const int dl)
{
	auto moptions = getMonstersForLevel(dl);
	return moptions[randrange(moptions.size())];
}


//	Seoncdary monster types that can be mixed into a monster group.
const vector<MonsterType> monsterdata::getAltVersions(const MonsterType id, const int dl)
{
	vector<MonsterType> alts;
	switch (id)
	{
	case(MonsterType::BONES_BLOODY):
		alts.push_back(MonsterType::BONES_BURNING);
		break;

	case(MonsterType::CULTIST):
		if (dl > 1)
			alts.push_back(MonsterType::CULTIST_MUTTERING);
		break;

	case(MonsterType::OGRE):
		alts.push_back(MonsterType::OGRE_BRUTE);
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

	case(MonsterType::SPIDER):
		if (dl > 10)
			alts.push_back(MonsterType::SPIDER_FLAMING);
		break;

	case(MonsterType::WRAITH):
		alts.push_back(MonsterType::WRAITH_MOON);
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
	case(MonsterType::BLOOD_BLOB):
		return MonsterType::BLOOD_LEECH;

	case(MonsterType::CULTIST):
	case(MonsterType::CULTIST_INFESTED):
		if (dl > 3)
			return MonsterType::CULTIST_WINGED;
		break;

	case(MonsterType::IMP):
		return MonsterType::IMP_MEGA;

	case(MonsterType::OGRE):
		return MonsterType::OGRE_BERSERKER;

	case(MonsterType::SKELETON):	
		return MonsterType::SKELETON_GIANT;

	case(MonsterType::SPIDER):
		return MonsterType::SPIDER_OGRE;

	case(MonsterType::TENTACLE):
		return MonsterType::TENTACLE_HORROR;

	case(MonsterType::VAMPIRE_SPAWN):
		return MonsterType::VAMPIRE;

	case(MonsterType::WRAITH):
		return MonsterType::WRAITH_GREAT;
	
	case(MonsterType::ZOMBIE):		
		if (dl > 1)
			return MonsterType::ZOMBIE_LARGE;
		break;
	}

	return MonsterType::__NONE;
}



//	Monster appears alone, rather than in a group.
bool monsterdata::isSoloMonster(const MonsterType id)
{
	switch (id)
	{
	case(MonsterType::CARRION_PRINCE):
	case(MonsterType::CULTIST_ASCENDED):
	case(MonsterType::DEMON_PUTRESCENT):
	case(MonsterType::GRIM_KNIGHT):
	case(MonsterType::LIGHTNING_SPIRE):
	case(MonsterType::PALE_KNIGHT):
	case(MonsterType::SPIDER_PHASE):
	case(MonsterType::SKINLESS_KNIGHT):
	case(MonsterType::SKULL_PILE):
	case(MonsterType::STARSPAWN):
	case(MonsterType::STAR_VAMPIRE):
	case(MonsterType::CRAB_TITAN):
	case(MonsterType::WALKING_SKULL_PILE):
	case(MonsterType::ZOMBIE_MASS):
		return true;

	default:
		return false;
	}
}




//  List of flags that a monster with the given ID has by default.
vector<string> monsterdata::getMonsterFlags(const MonsterType id)
{
	switch (id)
	{
	case(MonsterType::ABYSSAL_WRAITH):
		return { "flits", "ethereal", "undead", "immune_poison", "casts_ice_wall", };

	case(MonsterType::BLOAT):
		return { "flits", "poison_burst", "less_health", "less_defence", "flying", };

	case(MonsterType::BLOOD_BLOB):
		return { "flits", "vampiric", "less_health", "inaccurate" };
	case(MonsterType::BLOOD_LEECH):
		return { "flits", "vampiric", "spawner", "more_health", };

	case(MonsterType::BONES_BLOODY):
		return { "protected", "more_damage", "fast", "undead", "immune_poison" };
	case(MonsterType::BONES_BURNING):
		return { "protected", "more_damage", "undead", "immune_poison", "immune_fire", "casts_fireblast", "fire_attack" };

	case(MonsterType::CARRION_PRINCE):
		return { "spawner", "more_defence", "undead", "poison_attack", "resists_poison", };

	case(MonsterType::COLOSSUS_IRON):
		return { "protected_heavy", "more_damage", "slow", "resists_fire", "resists_poison", "resists_electric" };

	case(MonsterType::CORPSE_COLOSSUS):
		return { "slow", "spawner", "more_damage", "less_defence", "more_health", "undead", };

	case(MonsterType::CORPSEFLY):
		return { "fast", "flits", "poison_attack", "flying" };

	case(MonsterType::CRAB_TITAN):
		return { "grapples", "more_damage", "protected_heavy", "flits", "resists_fire", };

	case(MonsterType::CULTIST):
		return { "less_health", "less_damage" };
	case(MonsterType::CULTIST_ASCENDED):
		return { "flying", "casts_arcane_bolt", "resists_arcane", "spawner" };
	case(MonsterType::CULTIST_INFESTED):
		return { "less_health", "spawner", "infested", };
	case(MonsterType::CULTIST_MUTTERING):
		return { "less_health", "casts_arcane_bolt" };
	case(MonsterType::CULTIST_WINGED):
		return { "casts_arcane_bolt", "flying", "flits", "casts_poison_spit" };

	case(MonsterType::CULTIST_DOGGOSAN):
		return { "resists_poison", "casts_poison_spit", "poison_burst", "less_health", };

	case(MonsterType::DEMON_PRINCE):
		return { "casts_fireblast", "immune_fire", "fire_damage", "protected", "slow" };

	case(MonsterType::DEMON_PUTRESCENT):
		return { "poison_attack", "casts_poison_spit", "resists_poison", "protected", };

	case(MonsterType::FLAME_CONJURER):
		return { "spawner", "resists_fire", "flits", "casts_firebolt" };

	case(MonsterType::GRIM_KNIGHT):
		return { "protected_heavy", "arcane_attack", "more_damage", "slow", "defended", };

	case(MonsterType::IMP):
		return { "protected", "less_health", "immune_fire", "casts_firebolt", "less_damage", };
	case(MonsterType::IMP_MEGA):
		return { "protected", "immune_fire", "casts_firebolt", "less_damage", };

	case(MonsterType::KNIGHT_DREAD):
		return { "protected_heavy", "slow", "more_damage", "electric_attack", "resists_fire", "resists_electric" };

	case(MonsterType::LIGHTNING_SPIRE):
		return { "immobile", "immune_electric", "protected", "casts_lightning", "no_attack", };

	case(MonsterType::NIGHTGAUNT):
		return { "fast", "flits", "defended", };

	case(MonsterType::OGRE):
		return { "slow", "ranged_attack", "less_defence", "protected", "more_health" };
	case(MonsterType::OGRE_BERSERKER):
		return { "slow", "less_defence", "protected", "more_health", "more_damage", "berserker", "knockback" };
	case(MonsterType::OGRE_BRUTE):
		return { "slow", "less_defence", "protected", "more_health", "more_damage", "knockback" };

	case(MonsterType::OOZE_ELECTRIC):
		return { "slow", "electric_attack", "immune_electric", };
	case(MonsterType::OOZE_SLUDGE):
		return { "flits", "spits_sludge", "more_health", "resists_electric" };

	case(MonsterType::ORB_FLAME):
		return { "immobile", "flying", "less_health", "immune_fire", "casts_firebolt", "no_corpse", };

	case(MonsterType::PALE_KNIGHT):
		return { "protected_heavy", "ethereal", "arcane_attack", "undead", "more_damage", "more_health", "resists_fire", };
	case(MonsterType::PALE_SCHOLAR):
		return { "resists_arcane", "casts_arcane_bolt", "defended", "ethereal", "electric_attack", "more_damage", };

	case(MonsterType::QUASIT):
		return { "flying", "flits", "casts_arcane_bolt", "casts_lightning", "teleports", "less_health", "less_damage", };

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
		return { "protected", "fire_attack", "bones", "resists_fire", "undead", "immune_poison" };
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
		return { "protected", "more_health", "no_attack", "immobile", "spawner", "undead", "immune_poison", };
	case(MonsterType::WALKING_SKULL_PILE):
		return { "flits", "protected", "throws_bones", "immune_poison", "undead", };

	case(MonsterType::SPIDER):
		return { "webs", "defended", "poison_attack" };
	case(MonsterType::SPIDER_FLAMING):
		return { "webs", "defended", "fire_attack", "resists_fire" };
	case(MonsterType::SPIDER_OGRE):
		return { "webs", "protected", "poison_attack", };
	case(MonsterType::SPIDER_PHASE):
		return { "webs", "defended", "teleports", "immune_arcane", "arcane_attack" };
	case(MonsterType::SPIDER_TITAN):
		return { "webs", "protected_heavy", "spawner", "poison_attack", "slow", "inaccurate", "more_health", };

	case(MonsterType::STARSPAWN):
		return { "more_health", "resists_electric", "resists_fire", "resists_poison", "arcane_attack", "casts_arcane_bolt", };

	case(MonsterType::STAR_VAMPIRE):
		return { "vampiric", "arcane_attack", "unseen", };

	case(MonsterType::TENTACLE):
		return { "less_health", "grapples" };
	case(MonsterType::TENTACLE_HORROR):
		return { "spawner", "more_health", "more_damage", "grapples", };

	case(MonsterType::VAMPIRE):
		return { "more_damage", "more_health", "vampiric", "undead", "resists_arcane", };
	case(MonsterType::VAMPIRE_PRINCE):
		return { "more_damage", "more_health", "vampiric", "spawner", "casts_arcane_bolt", "undead", "resists_arcane", };
	case(MonsterType::VAMPIRE_SPAWN):
		return { "vampiric", "undead", "resists_arcane", };

	case(MonsterType::VILESPAWN):
		return { "casts_poison_spit", "resists_poison", "flits", "fast", };

	case(MonsterType::WORM_DEMON):
		return { "protected_heavy", "slow", "more_damage", "inaccurate", "more_health", };

	case(MonsterType::WRAITH):
		return { "ethereal", "arcane_attack", "immune_poison", "undead", };
	case(MonsterType::WRAITH_GREAT):
		return { "ethereal", "arcane_attack", "more_damage", "immune_poison", "undead", };
	case(MonsterType::WRAITH_MOON):
		return { "ethereal", "electric_attack", "immune_poison", "immune_electric", "casts_lightning", "undead", };

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


		//	Bosses

	case(MonsterType::BOSS_PALLID_ROTKING):
		return { "slow", "poison_attack", "undead", "spawner", "spit_sludge", "casts_poison_spit", "more_health" };
	case(MonsterType::BOSS_ROTKING_BURNED):
		return { "slow", "fire_attack", "undead", "spawner", "spit_sludge", "casts_firebolt", "more_health", "immune_fire", };
	case(MonsterType::BOSS_ROTKING_DESOLATE):
		return { "slow", "more_damage", "poison_attack", "undead", "spawner", "protected", "spit_sludge", "casts_poison_spit", "more_health", "immune_poison" };
	case(MonsterType::BOSS_ROTKING_INFUSED):
		return { "slow", "arcane_attack", "undead", "spawner", "spit_sludge", "casts_arcane_bolt", "more_health", "immune_arcane" };


	case(MonsterType::BOSS_DEMON_LORD):
		return { "immune_fire", "resists_poison", "fire_attack", "casts_fireblast", "more_health", "spawner", "protected", "casts_firestorm", };

	case(MonsterType::BOSS_WRAITH_KING):
		return { "immune_poison", "ethereal", "casts_arcane_bolt", "more_health", "spawner", "undead", "arcane_attack", };

	case(MonsterType::BOSS_WRETCHED_PRINCE):
		return { "spawner", "megaspawner", "more_health", "protected_heavy", "flits", "casts_lightning", };


	case(MonsterType::BOSS_DROWNED_DOGOSSA):
		return { "immune_poison", "casts_poison_spit", "more_health", "more_damage", "grapples", "defended", "poison_attack", "spawner", "avatar", };
	case(MonsterType::BOSS_TOMB_LORD_AMOG):
		return { "undead", "casts_arcane_bolt", "more_health", "more_damage", "arcane_attack", "spawner", "avatar", };
	case(MonsterType::BOSS_VIRIDIAN_PRINCE):
		return { "ethereal", "casts_lightning", "spawner", "immune_electric", "electric_attack", "teleports", "avatar", };
		
	case(MonsterType::BOSS_VILE_COLOSSUS):
		return { "more_health", "more_damage", "defended", "protected", "resists_arcane", "resists_electric", "resists_fire", "resists_poison", "megaspawner", "flits", "casts_lightning", };

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
	case(MonsterType::BOSS_ROTKING_BURNED):
	case(MonsterType::BOSS_ROTKING_DESOLATE):
	case(MonsterType::BOSS_ROTKING_INFUSED):

	case(MonsterType::BOSS_DEMON_LORD):
	case(MonsterType::BOSS_WRETCHED_PRINCE):
	case(MonsterType::BOSS_WRAITH_KING):

	case(MonsterType::BOSS_DROWNED_DOGOSSA):
	case(MonsterType::BOSS_TOMB_LORD_AMOG):
	case(MonsterType::BOSS_VIRIDIAN_PRINCE):

	case(MonsterType::BOSS_VILE_COLOSSUS):
		
		return 5;

	case(MonsterType::CARRION_PRINCE):
	case(MonsterType::COLOSSUS_IRON):
	case(MonsterType::CORPSE_COLOSSUS):
	case(MonsterType::DEMON_PRINCE):
	case(MonsterType::SKINLESS_KNIGHT):
	case(MonsterType::SPIDER_TITAN):
	case(MonsterType::TENTACLE_HORROR):
	case(MonsterType::VAMPIRE_PRINCE):
		return 4;

	case(MonsterType::CRAB_TITAN):
	case(MonsterType::DEMON_PUTRESCENT):
	case(MonsterType::FLAME_CONJURER):
	case(MonsterType::GRIM_KNIGHT):
	case(MonsterType::KNIGHT_DREAD):
	case(MonsterType::OGRE_BERSERKER):
	case(MonsterType::PALE_KNIGHT):
	case(MonsterType::RAT_KING):
	case(MonsterType::SPIDER_PHASE):
	case(MonsterType::STARSPAWN):
	case(MonsterType::STAR_VAMPIRE):
	case(MonsterType::VAMPIRE):
	case(MonsterType::WALKING_SKULL_PILE):
	case(MonsterType::WORM_DEMON):
	case(MonsterType::WRAITH_GREAT):
	case(MonsterType::ZOMBIE_MASS):
		return 3;

	case(MonsterType::BLOOD_LEECH):
	case(MonsterType::BONES_BURNING):
	case(MonsterType::CULTIST_ASCENDED):
	case(MonsterType::CULTIST_DOGGOSAN):
	case(MonsterType::CULTIST_WINGED):
	case(MonsterType::IMP_MEGA):
	case(MonsterType::LIGHTNING_SPIRE):
	case(MonsterType::OGRE_BRUTE):
	case(MonsterType::SKELETON_GIANT):
	case(MonsterType::SKELETON_GOLD_PLATED):
	case(MonsterType::SKULL_PILE):
	case(MonsterType::SPIDER_OGRE):
	case(MonsterType::WRAITH):
	case(MonsterType::WRAITH_MOON):
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
	case(MonsterType::ABYSSAL_WRAITH):			return make_tuple("abyssal wraith", 'w', TCODColor::cyan);
	case(MonsterType::BLOAT):					return make_tuple("bloat", 'b', TCODColor::lightLime);
	case(MonsterType::BLOOD_BLOB):				return make_tuple("blood blob", 'o', TCODColor::lighterCrimson);
	case(MonsterType::BLOOD_LEECH):				return make_tuple("blood leech", 'o', TCODColor::crimson);
	case(MonsterType::BONES_BLOODY):			return make_tuple("bloody bones", 'k', TCODColor::crimson);
	case(MonsterType::BONES_BURNING):			return make_tuple("burning bones", 'k', TCODColor::lightFlame);
	case(MonsterType::CARRION_PRINCE):			return make_tuple("CARRION PRINCE", 'P', TCODColor::lime);
	case(MonsterType::COLOSSUS_IRON):			return make_tuple("IRON COLOSSUS", 'N', TCODColor::silver);
	case(MonsterType::CORPSEFLY):				return make_tuple("corpsefly", 'f', TCODColor::lightLime);
	case(MonsterType::CORPSE_COLOSSUS):			return make_tuple("CORPSE COLOSSUS", 'C', TCODColor::pink);
	case(MonsterType::CRAB_TITAN):				return make_tuple("Titan Crab", 'B', TCODColor::orange);
	case(MonsterType::CULTIST):					return make_tuple("cultist", 'u', TCODColor::pink);
	case(MonsterType::CULTIST_ASCENDED):		return make_tuple("ascended cultist", 'U', TCODColor::purple);
	case(MonsterType::CULTIST_DOGGOSAN):		return make_tuple("cultist of Dogossa", 'u', TCODColor::green);
	case(MonsterType::CULTIST_INFESTED):		return make_tuple("infested cultist", 'u', TCODColor::lime);
	case(MonsterType::CULTIST_MUTTERING):		return make_tuple("muttering cultist", 'u', TCODColor::lightPurple);
	case(MonsterType::CULTIST_WINGED):			return make_tuple("winged cultist", 'U', TCODColor::pink);
	case(MonsterType::DEMON_PRINCE):			return make_tuple("DEMON PRINCE", 'P', TCODColor::flame);
	case(MonsterType::DEMON_PUTRESCENT):		return make_tuple("Putrescent Demon", 'P', TCODColor::lightGreen);
	case(MonsterType::FLAME_CONJURER):			return make_tuple("Flame Conjurer", 'F', TCODColor::flame);
	case(MonsterType::GRIM_KNIGHT):				return make_tuple("Grim Knight", 'N', TCODColor::lightPurple);
	case(MonsterType::IMP):						return make_tuple("imp", 'i', TCODColor::flame);
	case(MonsterType::IMP_MEGA):				return make_tuple("mega-imp", 'I', TCODColor::flame);
	case(MonsterType::NIGHTGAUNT):				return make_tuple("nightgaunt", 'n', TCODColor::lightBlue);
	case(MonsterType::KNIGHT_DREAD):			return make_tuple("Dread Knight", 'N', TCODColor::red);
	case(MonsterType::OGRE):					return make_tuple("ogre rockthrower", 'O', TCODColor::pink);
	case(MonsterType::OGRE_BERSERKER):			return make_tuple("Ogre Berserker", 'O', TCODColor::darkRed);
	case(MonsterType::OGRE_BRUTE):				return make_tuple("ogre brute", 'O', TCODColor::orange);
	case(MonsterType::OOZE_ELECTRIC):			return make_tuple("electric ooze", 'o', TCODColor::yellow);
	case(MonsterType::OOZE_SLUDGE):				return make_tuple("sludge ooze", 'o', TCODColor::lightSepia);
	case(MonsterType::LIGHTNING_SPIRE):			return make_tuple("lightning spire", 234, TCODColor::yellow);
	case(MonsterType::ORB_FLAME):				return make_tuple("orb of flame", '*', TCODColor::flame);
	case(MonsterType::PALE_KNIGHT):				return make_tuple("Pale Knight", 'N', TCODColor::cyan);
	case(MonsterType::PALE_SCHOLAR):			return make_tuple("pale scholar", 's', TCODColor::lightCyan);
	case(MonsterType::QUASIT):					return make_tuple("quasit", 'q', TCODColor::lightPurple);
	case(MonsterType::RAT_GIANT):				return make_tuple("giant rat", 'r', TCODColor::darkGreen);
	case(MonsterType::RAT_KING):				return make_tuple("Rat King", 'R', TCODColor::lightSepia);
	case(MonsterType::ROACH):					return make_tuple("roach", 'r', TCODColor::sepia);
	case(MonsterType::SKELETON):				return make_tuple("skeleton", 'k', TCODColor::desaturatedYellow);
	case(MonsterType::SKELETON_ARCHER):			return make_tuple("skeleton archer", 'k', TCODColor::yellow);
	case(MonsterType::SKELETON_BURNING):		return make_tuple("burning skeleton", 'k', TCODColor::flame);
	case(MonsterType::SKELETON_KNIGHT):			return make_tuple("skeleton knight", 'k', TCODColor::lighterGrey);
	case(MonsterType::SKELETON_GOLD_PLATED):	return make_tuple("gold-plated skeleton", 'k', TCODColor::gold);
	case(MonsterType::SKELETON_MAGE):			return make_tuple("skeleton mage", 'k', TCODColor::fuchsia);
	case(MonsterType::SKELETON_GIANT):			return make_tuple("giant skeleton", 'K', TCODColor::desaturatedYellow);
	case(MonsterType::SKINLESS_KNIGHT):			return make_tuple("SKINLESS KNIGHT", 'N', TCODColor::lighterOrange);
	case(MonsterType::SKULL_EXPLODING):			return make_tuple("exploding skull", 's', TCODColor::lightOrange);
	case(MonsterType::SKULL_FLAMING):			return make_tuple("flaming skull", 's', TCODColor::darkFlame);
	case(MonsterType::SKULL_FLOATING):			return make_tuple("floating skull", 's', TCODColor::lightestYellow);
	case(MonsterType::SKULL_PILE):				return make_tuple("skull pile", 234, TCODColor::lightestYellow);
	case(MonsterType::SPIDER):					return make_tuple("giant spider", 'm', TCODColor::lime);
	case(MonsterType::SPIDER_FLAMING):			return make_tuple("flaming spider", 'm', TCODColor::flame);
	case(MonsterType::SPIDER_OGRE):				return make_tuple("ogre spider", 'M', TCODColor::lime);
	case(MonsterType::SPIDER_PHASE):			return make_tuple("phase spider", 'M', TCODColor::lightPurple);
	case(MonsterType::SPIDER_TITAN):			return make_tuple("TITAN SPIDER", 'M', TCODColor::silver);
	case(MonsterType::STARSPAWN):				return make_tuple("starspawn", 'X', TCODColor::pink);
	case(MonsterType::STAR_VAMPIRE):			return make_tuple("Star Vampire", 'V', TCODColor::white);
	case(MonsterType::TENTACLE):				return make_tuple("tentacle", 't', TCODColor::green);
	case(MonsterType::TENTACLE_HORROR):			return make_tuple("Tentacle Horror", 'T', TCODColor::green);
	case(MonsterType::VAMPIRE):					return make_tuple("vampire", 'V', TCODColor::lightRed);
	case(MonsterType::VAMPIRE_PRINCE):			return make_tuple("VAMPIRE PRINCE", 'V', TCODColor::fuchsia);
	case(MonsterType::VAMPIRE_SPAWN):			return make_tuple("Vampire spawn", 'v', TCODColor::red);
	case(MonsterType::VILESPAWN):				return make_tuple("vilespawn", 'v', TCODColor::lime);
	case(MonsterType::WALKING_SKULL_PILE):		return make_tuple("Walking Skull Pile", 'S', TCODColor::desaturatedYellow);
	case(MonsterType::WORM_DEMON):				return make_tuple("Worm Demon", 'O', TCODColor::lightGrey);
	case(MonsterType::WRAITH):					return make_tuple("wraith", 'w', TCODColor::lightPurple);
	case(MonsterType::WRAITH_GREAT):			return make_tuple("great wraith", 'W', TCODColor::lighterPurple);
	case(MonsterType::WRAITH_MOON):				return make_tuple("moon wraith", 'w', TCODColor::lightYellow);
	case(MonsterType::WRETCH):					return make_tuple("wretch", 'e', TCODColor::crimson);
	case(MonsterType::WRETCH_PUKING):			return make_tuple("puking wretch", 'e', TCODColor::lime);
	case(MonsterType::ZOMBIE):					return make_tuple("zombie", 'z', TCODColor::green);
	case(MonsterType::ZOMBIE_INFUSED):			return make_tuple("infused zombie", 'z', TCODColor::fuchsia);
	case(MonsterType::ZOMBIE_LARGE):			return make_tuple("large zombie", 'Z', TCODColor::lightGreen);
	case(MonsterType::ZOMBIE_MASS):				return make_tuple("Zombie Mass", 'Z', TCODColor::lightPink);
	case(MonsterType::ZOMBIE_ROTTED):			return make_tuple("rotted zombie", 'z', TCODColor::darkLime);

	case(MonsterType::BOSS_PALLID_ROTKING):		return make_tuple("= PALLID ROTKING =", 'A', TCODColor::lightLime);
	case(MonsterType::BOSS_ROTKING_BURNED):		return make_tuple("= BURNED ROTKING =", 'A', TCODColor::flame);
	case(MonsterType::BOSS_ROTKING_DESOLATE):	return make_tuple("= DESOLATE ROTKING =", 'A', TCODColor::green);
	case(MonsterType::BOSS_ROTKING_INFUSED):	return make_tuple("= INFUSED ROTKING =", 'A', TCODColor::lightPurple);

	case(MonsterType::BOSS_DEMON_LORD):			return make_tuple("= DEMON LORD =", 'D', TCODColor::flame);
	case(MonsterType::BOSS_WRAITH_KING):		return make_tuple("= WRAITH KING =", 'W', TCODColor::pink);
	case(MonsterType::BOSS_WRETCHED_PRINCE):	return make_tuple("= WRETCHED PRINCE =", 'L', TCODColor::crimson);

	case(MonsterType::BOSS_DROWNED_DOGOSSA):	return make_tuple("= AVATAR OF DROWNED DOGOSSA =", 'G', TCODColor::lime);
	case(MonsterType::BOSS_TOMB_LORD_AMOG):		return make_tuple("= AVATAR OF THE TOMB LORD =", 'T', TCODColor::brass);
	case(MonsterType::BOSS_VIRIDIAN_PRINCE):	return make_tuple("= LOST PRINCE OF VIRIDIA =", 'P', TCODColor::silver);

	case(MonsterType::BOSS_VILE_COLOSSUS):		return make_tuple("== THE VILE COLOSSUS =", 'X', TCODColor::gold);

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


//	Generates a unique abyssal boss.
monsterPtr monsterdata::generate_AbyssLord(int level)
{
	auto mon = monsterPtr(new monster("ABYSSAL LORD", 'A', TCODColor::lightPurple, level, 5, MonsterType::BOSS_ABYSS_LORD));

	//	Choices of additional flags.
	vector<string> flags = { "more_health", "defended", "protected" };
	mon->addFlag(flags[randrange(flags.size())]);

	flags = { "arcane_attack", "electric_attack", "fire_attack", "poison_attack" };
	mon->addFlag(flags[randrange(flags.size())]);

	flags = { "more_damage", "fast", "ranged_attack" };
	mon->addFlag(flags[randrange(flags.size())]);

	flags = { "casts_arcane_bolt", "casts_firebolt", "casts_lightning", "casts_poison_spit" };
	mon->addFlag(flags[randrange(flags.size())]);

	mon->addFlag("abysslord");
	return mon;
}


//	ULTIMATE omega endgame bosses.
monsterPtr monsterdata::generate_Viledragon(const int level)
{
	//	damage type (determines associated + name)
	vector<DamageType> dtypes = { DTYPE_ARCANE, DTYPE_ELECTRIC, DTYPE_FIRE, DTYPE_POISON };
	auto dt = dtypes[randrange(dtypes.size())];

	//	generate monster name
	string name = "VILEDRAGON";
	switch (dt)
	{
	case(DTYPE_ARCANE):		name = "INFUSED " + name; break;
	case(DTYPE_ELECTRIC):	name = "ELECTRIC " + name; break;
	case(DTYPE_FIRE):		name = "CHARRED " + name; break;
	case(DTYPE_POISON):		name = "FETID " + name; break;
	}
	name = "= " + name + " =";

	//	generate the monster itself
	auto mon = monsterPtr(new monster(name, 'D', getDamageTypeColor(dt), level, 5, MonsterType::BOSS_VILEDRAGON));
	mon->addFlag("viledragon");

	//	Choice of special flags.
	vector<string> flags = { "more_health", "defended", "protected_heavy" };
	for (unsigned i = 0; i < 2; i++)
	{
		auto f = randrange(flags.size());
		mon->addFlag(flags[f]);
		flags.erase(flags.begin() + f);
	}

	//	Elemental association.
	switch (dt)
	{
	case(DTYPE_ARCANE):
		mon->addFlag("arcane_attack");
		mon->addFlag("resists_arcane");
		mon->addFlag("casts_arcane_bolt");
		break;

	case(DTYPE_ELECTRIC):
		mon->addFlag("electric_attack");
		mon->addFlag("immune_electric");
		mon->addFlag("casts_lightning");
		break;

	case(DTYPE_FIRE):
		mon->addFlag("fire_attack");
		mon->addFlag("immune_fire");
		mon->addFlag("casts_firebolt");
		break;

	case(DTYPE_POISON):
		mon->addFlag("poison_attack");
		mon->addFlag("immune_poison");
		mon->addFlag("casts_poison_spit");
	}

	//	additional abilities, granted at higher levels
	flags = { "more_damage", "vampiric", "unseen", "fast", };
	auto bonus_count = (level - 40) / 4;
	while (bonus_count > 0 && !flags.empty())
	{
		auto f = randrange(flags.size());
		mon->addFlag(flags[f]);
		flags.erase(flags.begin() + f);
		bonus_count--;
	}

	return mon;
}


//	Rolls a Viledragon encounter, plus allies if applicable
vector<monsterPtr> monsterdata::generate_ViledragonGang(const int level)
{
	vector<monsterPtr> mlist;
	vector<MonsterType> mtypes = { MonsterType::VILESPAWN };

	string name = "VILEDRAGON";

	//	base type
	const int t = randint(1, 5);
	switch (t)
	{
	case(1):	
		name = "BIRTHING " + name; 
		mtypes.push_back(MonsterType::VILESPAWN);	
		break;
	case(2):	
		name = "TWIN " + name; 
		break;
	case(3):	
		name = "TITANIC " + name; 
		mtypes.push_back(MonsterType::CRAB_TITAN);
		break;
	case(4):	
		name = "PHASE " + name; 
		mtypes.push_back(MonsterType::WRAITH_GREAT);
		break;
	case(5):	
		name = "THIRSTING " + name; 
		mtypes.push_back(MonsterType::VAMPIRE_SPAWN);
		break;
	}

	//	elemental affinity
	const DamageType dt = (roll_one_in(5)) ? DTYPE_NORMAL : SPECIAL_DAMAGE_TYPES[randrange(SPECIAL_DAMAGE_TYPES.size())];
	switch (dt)
	{
	case(DTYPE_ARCANE):		
		name = "INFUSED " + name; 
		mtypes.push_back(MonsterType::QUASIT);
		break;
	case(DTYPE_ELECTRIC):	
		name = "ELECTRIC " + name; 
		mtypes.push_back(MonsterType::WRAITH_MOON);
		break;
	case(DTYPE_FIRE):		
		name = "CHARRED " + name; 
		mtypes.push_back(MonsterType::ORB_FLAME);
		break;
	case(DTYPE_POISON):		
		name = "ROTTED " + name;
		mtypes.push_back(MonsterType::BONES_BLOODY);
		break;
	default:				
		name = "PETRIFIED " + name; 
		mtypes.push_back(MonsterType::STARSPAWN);
		break;
	}


	//	QTY of monsters is variable
	int count = 1;
	if (t == 2) count = 2;


	while (count-- > 0)
	{
		//	generate the base viledragon
		auto mon = monsterPtr(new monster(name, 'D', getDamageTypeColor(dt), level, 5, MonsterType::BOSS_VILEDRAGON));
		mon->addFlag("viledragon");
		mon->addFlag("more_health");

		//	elemental association
		switch (dt)
		{
		case(DTYPE_ARCANE):
			mon->addFlag("resists_arcane");
			mon->addFlag("arcane_attack");
			mon->addFlag("casts_arcane_bolt");
			mon->addFlag("protected");
			break;

		case(DTYPE_ELECTRIC):
			mon->addFlag("immune_electric");
			mon->addFlag("electric_attack");
			mon->addFlag("casts_lightning");
			mon->addFlag("protected");
			break;

		case(DTYPE_FIRE):
			mon->addFlag("immune_fire");
			mon->addFlag("fire_attack");
			mon->addFlag("casts_fireblast");
			mon->addFlag("protected");
			break;

		case(DTYPE_POISON):
			mon->addFlag("immune_poison");
			mon->addFlag("poison_attack");
			mon->addFlag("casts_poison_spit");
			mon->addFlag("protected");
			break;

		default:
			mon->addFlag("protected_heavy");
			mon->addFlag("more_damage");
			break;
		}

		//	flags based on type
		switch (t)
		{
		case(1):
			mon->addFlag("megaspawner");
			break;

		case(2):

			break;

		case(3):
			mon->addFlag("defended");
			mon->addFlag("slow");
			break;

		case(4):
			mon->addFlag("teleports");
			mon->addFlag("flits");
			break;

		case(5):
			mon->addFlag("vampiric");
			break;
		}

		//	add to list
		mlist.push_back(mon);
	}

	//	his gang
	count = dieRoll(3, 4);
	while (count-- > 0)
		mlist.push_back(generate(mtypes[randrange(mtypes.size())], level));

	return mlist;
}
