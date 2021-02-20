#include "messages.h"


//	Generic adder
void messages::add(gamedataPtr gdata, const string txt, const vector<colorType> colors)
{
	gdata->_messages.insert(gdata->_messages.begin(), message(txt, colors));
	if (gdata->_messages.size() > MAX_MESSAGE_COUNT)
		gdata->_messages.pop_back();
}

void messages::error(gamedataPtr gdata, const string txt)
{
	messages::add(gdata, '#' + txt, { COLOR_WARNING });
}


//	Summarizes a sequence of attacks.
void messages::attack_string(gamedataPtr gdata, creaturePtr attacker, creaturePtr target, const vector<int> damage, const vector<bool> wasCrit)
{
	string txt;
	vector<colorType> colors;
	if (attacker->isPlayer())
	{
		//	indicate what we hit
		txt += "You attack " + getTargetIdentifier(gdata, target) + " (";

		//	damage per hit
		for (unsigned i = 0; i < damage.size(); i++)
		{
			if (damage[i] < 1)
			{
				txt += "#0 ";
				colors.push_back(COLOR_WARNING);
			}
			else if (wasCrit[i])
			{
				txt += "#*" + to_string(damage[i]) + "* ";
				colors.push_back(COLOR_POSITIVE);
			}
			else
			{
				txt += "#" + to_string(damage[i]) + " ";
				colors.push_back(COLOR_HIGHLIGHT_POS);
			}
		}
		txt.pop_back();
		txt += "@) #";
		
		//	monster's health remaining
		auto hp_per = target->getHealthPercent() / 10;
		if (hp_per > 0)
		{
			for (unsigned i = 0; i < hp_per; i++)
				txt += "|";
			txt += "#";
			for (unsigned i = hp_per; i < 10; i++)
				txt += "|";
			colors.push_back(COLOR_HEALTH);
			colors.push_back(TCODColor::darkGrey);
		}
		
		//	done
		add(gdata, txt, colors);
	}

	else if (target->isPlayer() && damage[0] > 0)
		add(gdata, "Took #" + to_string(damage[0]) + " damage @from #" + attacker->getName(), { COLOR_HEALTH, attacker->getColor() });
}


//	Riposte attack message
void messages::riposte(gamedataPtr gdata, creaturePtr riposter, creaturePtr target)
{
	if (riposter->isPlayer())
		add(gdata, ">> #You riposte!", { COLOR_POSITIVE });
}


//	Creature gains a status effect.
void messages::status_effect(gamedataPtr gdata, const creaturePtr target, const StatusEffect st)
{
	if (target->isPlayer())
		messages::add(gdata, "#You gain status effect: #" + getStatusEffectName(st) + "!", { COLOR_NEGATIVE, getStatusEffectColor(st) });
	else if (canPlayerSeeCreature(gdata, target) && !target->hasStatusEffect(st))
		messages::add(gdata, "#" + getTargetIdentifier(gdata, target) + " gains status effect: #" + getStatusEffectName(st) + "!", { target->getColor(), getStatusEffectColor(st) });
}

void messages::energy_damage(gamedataPtr gdata, const creaturePtr target, const DamageType dt, const int amt)
{
	if (!target->isPlayer())
		messages::add(gdata, "#" + getTargetIdentifier(gdata, target) + " takes #" + to_string(amt) + " " + getDamageTypeName(dt) + " @damage.", { target->getColor(), getDamageTypeColor(dt) });
}



//	How the player identifies a given creature, including an article; ie 'the goblin', 'Lord Baal', 'something'
string messages::getTargetIdentifier(gamedataPtr gdata, creaturePtr t)
{
	if (canPlayerSeeCreature(gdata, t))
		return "the " + t->getName();
	else
		return "something";
}
