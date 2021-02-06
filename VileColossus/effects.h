#pragma once

#include "gamedata.h"
#include "messages.h"
#include "monster.h"
#include "progression.h"



//	Death effects

void tryDropCorpse(gamedataPtr gdata, const Surface corpse, const intpair at);
void doDeathDrops(gamedataPtr gdata, monsterPtr target);

void explodeOnDeath(gamedataPtr gdata, monsterPtr target, string flag);

void killPlayer(gamedataPtr gdata);
void killCreature(gamedataPtr gdata, creaturePtr target);


//	Status effects / damage

int adjustByPercent(int val, int per);

void inflictDamageInRadius(gamedataPtr gdata, const intpair ctr, const int r, const DamageType dt, const intpair dmg);

int getDefaultStatusEffectDuration(const StatusEffect st);
void tryInflictStatusEffect(gamedataPtr gdata, creaturePtr target, const StatusEffect st, const int odds, const int stacks = 0);

void creatureTakeDamage(gamedataPtr gdata, creaturePtr target, int dam);
void inflictEnergyDamage(gamedataPtr gdata, creaturePtr target, int dam, const DamageType dt);

void testForDamageEquipment(gamedataPtr gdata, creaturePtr attacker, creaturePtr target);



//	Tile effects

void setPlayerPosition(gamedataPtr gdata, intpair pt);

void trySetSurface(gamedataPtr gdata, const intpair pt, const Surface sf);
void fillRegionWithSurface(gamedataPtr gdata, const intpair ctr, int sz, Surface sf);

void openLootChest(gamedataPtr gdata, const intpair pt);
void standOnTile(gamedataPtr gdata, creaturePtr cr);