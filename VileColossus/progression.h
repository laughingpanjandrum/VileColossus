#pragma once

#include "gamedata.h"
#include "messages.h"
#include "monster.h"

#define XP_PER_LEVEL 10000
#define XP_PER_PERK 2000
#define TOWN_PORTAL_CHARGE_REQ 5000


const int getXPForLevel(gamedataPtr gdata);

void playerGainLevel(gamedataPtr gdata);
void addKillXP(gamedataPtr gdata, monsterPtr target);

void spendPerkPoint(gamedataPtr gdata);
void spendAttributePoint(gamedataPtr gdata);
void openAttributeMenu(gamedataPtr gdata);



//	Portal ability

bool isTownPortalCharged(gamedataPtr gdata);