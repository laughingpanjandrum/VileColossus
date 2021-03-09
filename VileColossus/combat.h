#pragma once

#include "gamedata.h"
#include "visibility.h"
#include "effects.h"
#include "manageMagic.h"
#include "messages.h"



//	Aesthetics

void animateHit(gamedataPtr gdata, creaturePtr target, int damage, bool wasCrit, bool isGoodDamage, int delay = 0);


//	Attacks & attack rolls

void monsterSpecialHitEffects(gamedataPtr gdata, monsterPtr attacker, creaturePtr target, int damage);
void playerTestForSpellTriggerOnHit(gamedataPtr gdata, creaturePtr target);

int rollWeaponDamage(gamedataPtr gdata, creaturePtr attacker);
int applyProtection(gamedataPtr gdata, creaturePtr target, int dam);
bool rollToHit(gamedataPtr gdata, creaturePtr attacker, creaturePtr target);

void triggerReprisal(gamedataPtr gdata, creaturePtr repriser, creaturePtr target);
void triggerRiposte(gamedataPtr gdata, creaturePtr riposter, creaturePtr target);

void attackWithWeapon(gamedataPtr gdata, creaturePtr attacker, creaturePtr target, bool allowRiposte = true, int percentDamageMult = 100, int flatDamageAdjust = 0, bool spendEnergy = true);


//	Player actions

void triggerCleaveAttack(gamedataPtr gdata, creaturePtr cleaver);

void playerTriggerMeleeAttack(gamedataPtr gdata, creaturePtr t);
void playerTriggerRangedAttack(gamedataPtr gdata, const intpair vec);

void playerTryRangedAttack(gamedataPtr gdata, const intpair vec);

void playerTryAutoattack(gamedataPtr gdata);