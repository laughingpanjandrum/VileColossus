#include "creature.h"

creature::creature(string name, int glyph, colorType color) : _name(name), _glyph(glyph), _color(color), _actionEnergy(0)
{
	_damageTaken = 0;
	_actionEnergy = 0;
	_bankedAttackPoints = 0;

	_grappledBy = nullptr;

	for (unsigned i = 0; i < STATUS__MAX; i++)
		_StatusEffects.push_back(0);
	for (unsigned i = 0; i < BUFF__MAX; i++)
		_Buffs.push_back(0);
}

colorType creature::getColor() const
{
	auto col = _color;

	//	modification due to status effect
	if (hasStatusEffect(STATUS_BURN))
		col = TCODColor::lerp(col, TCODColor::flame, 0.5);
	else if (hasStatusEffect(STATUS_SHOCK))
		col = TCODColor::lerp(col, TCODColor::yellow, 0.5);
	else if (hasStatusEffect(STATUS_POISON))
		col = TCODColor::lerp(col, TCODColor::lime, 0.5);

	return col;
}


//	Stack a status effect duration.
void creature::addStatusEffect(const StatusEffect st, int stacks)
{
	if (stacks > _StatusEffects[st])
		_StatusEffects[st] = stacks;
}


//	Tick down the given status effect.
void creature::reduceStatusEffectDuration(const StatusEffect st)
{
	if (_StatusEffects[st] > 0)
		_StatusEffects[st] -= 1;
}

void creature::addBuff(const BuffType bf, int turns)
{
	if (_Buffs[bf] < turns)
		_Buffs[bf] = turns;
}

//	Tick down the given buff.
void creature::reduceBuffDuration(const BuffType bf)
{
	if (_Buffs[bf] > 0)
		_Buffs[bf] -= 1;
}


//	If we're grappled, tests to see if the grappling is still possible.
//	IF not (grappler has moved/died), clear the grapple status.
void creature::verifyGrappler()
{
	if (_grappledBy != nullptr)
	{
		if (_grappledBy->isDead() || (hypot(_pos.first - _grappledBy->_pos.first, _pos.second - _grappledBy->_pos.second) >= 2))
			_grappledBy = nullptr;
	}
}


//	Every 10 points of attack speed = 1 attack per turn.
//	Fractional points are banked and added to the next turn's count, so they will eventually contribute to an additional attack.
int creature::getAttacksThisTurn()
{
	return 1;
}


//	Called at the start of our turn.
void creature::tick()
{
	//	Reduce status effect and buff durations.	
	for (unsigned i = 0; i < STATUS__MAX; i++)
	{
		if (_StatusEffects[i] > 0 && i != STATUS_ENTANGLED && i != STATUS_PUTREFIED)
			_StatusEffects[i]--;
	}
	for (unsigned i = 0; i < BUFF__MAX; i++)
	{
		if (_Buffs[i] > 0 && i != BUFF_SMITE_EVIL && i != BUFF_CRIT_BONUS)
			_Buffs[i]--;
	}
}


//	Replenishes all health and clears all status effects.
void creature::healToMax()
{
	_damageTaken = 0;
	for (auto st : ALL_STATUS_EFFECTS)
		_StatusEffects[st] = 0;
}


void creature::healDamage(int amt)
{
	_damageTaken -= amt;
	_damageTaken = MAX(0, _damageTaken);
}