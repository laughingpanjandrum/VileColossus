#pragma once

#include <string>
#include <vector>
#include <memory>

#include "color_constants.h"
#include "enums.h"
#include "utility.h"

using namespace std;

#define ENERGY_ACTION_THRESHOLD	100
#define ENERGY_COST_BASE		100


class creature
{
public:

	creature() : creature("", ' ', COLOR_WHITE) {}
	creature(string name, int glyph, colorType color);
	~creature() {}


	//	Allegiance/special state flags

	virtual bool isPlayer() const { return false; }

	virtual bool isEthereal() const { return false; }
	virtual bool isFlying() const { return false; }
	virtual bool isInvisible() const { return false; }
	virtual bool isUndead() const { return false; }


	//	Flavour

	string getName() const { return _name; }
	colorType getColor() const;
	int getGlyph() const { return _glyph; }


	//	Health

	bool isDead() const { return _damageTaken >= getMaxHealth(); }

	int getHealthLeft() const { return getMaxHealth() - _damageTaken; }
	int getHealthPercent() const { return 100.0f - 100.0f * (float)_damageTaken / (float)getMaxHealth(); }

	virtual void takeDamage(int amt) { _damageTaken += amt; }

	void healToMax();
	void healDamage(int amt);


	//	to be overriden

	virtual int getMaxHealth() const = 0;

	virtual int getAccuracy() const = 0;
	virtual int getDefenceValue() const = 0;
	virtual int getArmourValue() const = 0;

	virtual int getWeaponDamage() const = 0;
	virtual int getDamageVariance() const { return 0; }
	virtual int getCriticalChance() const { return 0; }
	virtual int getCriticalMultiplier() const { return 0; }

	virtual bool usingRangedWeapon() const = 0;
	virtual int getMaxAttackRange() const = 0;

	virtual int getSpellPower() const { return 0; }

	virtual int getResistance(const DamageType dt) const { return 0; }


	//	Special attacks

	virtual int getRiposteChance() const { return 0; }
	virtual int getRiposteDamageMult() const { return 0; }

	virtual bool cleaves() const { return false; }
	virtual int getCleaveDamageBonus() const { return 0; }

	virtual int getStaggerChance() const { return 0; }
	virtual int getStaggerAttackDuration() const { return 0; }

	virtual int getKnockbackChance() const { return 0; }

	virtual int getStatusEffectChanceOnHit(const StatusEffect st) const { return 0; }
	virtual int getWeaponDamageOfType(const DamageType dt) const { return 0; }
	virtual int getElementalAffinity(const DamageType dt) const { return 0; }

	virtual int getLeechOnKill() const { return 0; }
	virtual int getReprisalDamage() const { return 0; }
	virtual int getSmiteEvilDamage() const { return 0; }


	//	Action delay

	bool hasActionEnergyAboveThreshold() const { return _actionEnergy >= ENERGY_ACTION_THRESHOLD; }
	void spendActionEnergy(const int amt = ENERGY_COST_BASE) { _actionEnergy -= amt; }

	virtual int getMoveEnergyCost() const { return ENERGY_COST_BASE; }
	virtual int getAttackEnergyCost() const { return ENERGY_COST_BASE; }


	//	Status effects

	bool hasStatusEffect(const StatusEffect st) const { return _StatusEffects[st] > 0; }
	int getStatusEffectDuration(const StatusEffect st) const { return _StatusEffects[st]; }

	void addStatusEffect(const StatusEffect st, int stacks);
	void reduceStatusEffectDuration(const StatusEffect st);
	void cureStatusEffect(const StatusEffect st) { _StatusEffects[st] = 0; }


	//	Buffs

	bool hasBuff(const BuffType bf) const { return _Buffs[bf] > 0; }
	int getBuffDuration(const BuffType bf) const { return _Buffs[bf]; }
	
	void addBuff(const BuffType bf, int turns);
	void increaseBuff(const BuffType bf, int turns) { _Buffs[bf] += turns; }
	void setBuffDuration(const BuffType bf, int turns) { _Buffs[bf] = turns; }

	void removeBuff(const BuffType bf) { _Buffs[bf] = 0; }
	void reduceBuffDuration(const BuffType bf);


	//	Grappling

	void verifyGrappler();
	bool isGrappled() const { return _grappledBy != nullptr; }


	
	//	Number of attacks

	int getAttacksThisTurn();


	//	Turn-passage events

	virtual void tick();


	//	Public data

	shared_ptr<creature> _grappledBy;

	intpair _pos;

	int _level;
	int _damageTaken;

	int _actionEnergy;
	int _bankedAttackPoints;


protected:

	string _name;
	int _glyph;
	colorType _color;

	vector<int> _StatusEffects;
	vector<int> _Buffs;

};
typedef shared_ptr<creature> creaturePtr;