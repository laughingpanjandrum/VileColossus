#pragma once

#include "gamedata.h"
#include "visibility.h"


#define MAX_MESSAGE_COUNT 11


namespace messages
{

	void add(gamedataPtr gdata, const string txt, const vector<colorType> colors = {});
	void error(gamedataPtr gdata, const string txt);


	//	Combat
	void attack_string(gamedataPtr gdata, creaturePtr attacker, creaturePtr target, const vector<int> damage, const vector<bool> wasCrit);
	void riposte(gamedataPtr gdata, creaturePtr riposter, creaturePtr target);
	void status_effect(gamedataPtr gdata, const creaturePtr target, const StatusEffect st);
	void energy_damage(gamedataPtr gdata, const creaturePtr target, const DamageType dt, const int amt);


	//	Utility
	string getTargetIdentifier(gamedataPtr gdata, creaturePtr t);

}