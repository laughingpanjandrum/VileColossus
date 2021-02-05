#pragma once

#include "gamedata.h"
#include "messages.h"
#include "manageItems.h"
#include "manageMagic.h"
#include "combat.h"


//	Player movement

void playerBumpTile(gamedataPtr gdata, const intpair pt);

void playerEnterTile(gamedataPtr gdata, intpair pt);
bool doPlayerMove(gamedataPtr gdata, const intpair vec, bool allowAttacking = true);

void startAutoMoving(gamedataPtr gdata, const intpair vec);
void doAutomove(gamedataPtr gdata);