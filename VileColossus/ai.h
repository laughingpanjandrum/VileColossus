#pragma once

#include "gamedata.h"
#include "monster.h"
#include "visibility.h"
#include "combat.h"
#include "manageMagic.h"
#include "movement.h"
#include "monsterdata.h"


#define AI_MAX_DETECTION_RADIUS 14



namespace ai
{


	//		Movement.

	void executeMove(gamedataPtr gdata, monsterPtr ai, intpair to);

	bool moveToPoint(gamedataPtr data, monsterPtr ai, intpair toPt);
	bool moveAwayFromPoint(gamedataPtr gdata, monsterPtr ai, intpair awayFrom);


	//		Special abilities.

	bool castRaySpell(gamedataPtr gdata, monsterPtr ai, creaturePtr target, const string spell);

	intpair getTeleportPoint(gamedataPtr gdata, monsterPtr ai);


	//		Combat basics.
	
	bool spawnMinion(gamedataPtr gdata, monsterPtr spawner, const MonsterType spawn_id, const intpair pt);

	bool trySpawn(gamedataPtr gdata, monsterPtr ai);
	bool tryUseAbility(gamedataPtr gdata, monsterPtr ai);
	bool tryRangedAttack(gamedataPtr gdata, monsterPtr ai);

	void explode(gamedataPtr gdata, monsterPtr ai);

	void doCombatAction(gamedataPtr gdata, monsterPtr ai);


	//		Target finding.

	bool canTargetCreature(gamedataPtr gdata, monsterPtr ai, creaturePtr t);
	void checkTargetValidity(gamedataPtr gdata, monsterPtr ai);

	void setTarget(gamedataPtr gdata, monsterPtr ai, creaturePtr t);
	void findMonsterTarget(gamedataPtr gdata, monsterPtr ai);
	void findTarget(gamedataPtr gdata, monsterPtr ai);



	//	Turns all start here.
	void takeTurn(gamedataPtr gdata, monsterPtr ai);

}