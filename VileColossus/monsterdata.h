#pragma once

#include <tuple>

#include "enums.h"
#include "monster.h"



/*
TABLE OF ALL MONSTER FLAGS
	
	defended				Has +50% Defence Value.
	electric_attack			Deals 50% of damage as electric.
	fast					Reduced move energy cost.
	fire_attack				Deals 50% of damage as fire.
	flits					Moves erratically.
	flying					Ignores stuff on the ground, like poison pools.
	inaccurate				-50% base accuracy.
	immobile				Can't move. Immune to effects that cause movement.
	less_damage				Inflicts -50% damage.
	less_defence			Defence Value reduced by 50%.
	less_health				-50% health.
	megaspawner				Spawns more frequently.
	minion					Grants no XP and doesn't drop anything. (Added to monsters spawned by other monsters.)
	more_health				+50% health.
	more_damage				Inflicts +50% damage.
	no_attack				Cannot make normal weapon attacks. (Can still have spells/etc)
	poison_attack			Deals 50% of damage as poison.
	poison_burst			On death, spray poison ooze.
	protected				Has the default 'light armour' value for its level.
	protected_heavy			Has the default 'heavy armour' value for its level.
	ranged_attack			Default attack is ranged.
	slow					Move energy cost is increased by 50%.
	spawner					Spawns monsters (based on its type)
	vampiric				Regains health on hit

	casts_arcane_bolt		Ranged arcane damage.
	casts_firebolt			Ranged fire damage.
	casts_fireblast			Leaves flames behind on hit.
	casts_firestorm			Like the player spell.
	casts_lightning			Ranged electric damage.
	spit_sludge				Hurls sludge pools.
	throws_bones			Ranged attack that can spawn a skull.

	Resistances: resists_ is 50% resistance; immune_ is 100%, naturally
		resists_arcane		/	immune_arcane
		resists_fire		/	immune_fire
		resists_electric	/	immune_electric
		resists_poison		/	immune_poison

*/



namespace monsterdata
{

	typedef tuple<string, int, colorType> flavourdat;


	//	Monster placement data
	bool isSoloMonster(const MonsterType id);


	//	Getting monsters to emplace
	
	const vector<MonsterType> getMonstersForLevel(const int dl);
	const MonsterType rollMonsterForLevel(const int dl);
	const MonsterType rollMinibossForLevel(const int dl);
	const MonsterType rollBossForLevel(const int dl);

	const vector<MonsterType> getAltVersions(const MonsterType id, const int dl);
	const MonsterType getGroupLeaderType(const MonsterType id, const int dl);


	//	Basic monster data
	vector<string> getMonsterFlags(const MonsterType id);
	int getDefaultMonsterTier(const MonsterType id);

	flavourdat get_flavourdat_for_monster_id(MonsterType id);
	

	//	Creating monsters
	monsterPtr generate(MonsterType id, int level);

}