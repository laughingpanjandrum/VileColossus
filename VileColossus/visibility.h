#pragma once

#include "gamedata.h"
#include "messages.h"


#define MONSTER_MAX_VISION_RANGE 15



//	Getting coordinates for special effects

bool isPointSafe(gamedataPtr gdata, creaturePtr cr, const intpair pt);

vector<intpair> getFreePointsInRadius(gamedataPtr gdata, intpair ctr, int rad);
vector<intpair> getAdjacentFreePoints(gamedataPtr gdata, creaturePtr cr, intpair pos);
vector<intpair> getAdjacentWalkable(gamedataPtr gdata, intpair pos);
vector<creaturePtr> getAdjacentEnemies(gamedataPtr gdata, creaturePtr t);
vector<creaturePtr> getAdjacentCreatures(gamedataPtr gdata, intpair ctr);

intpair getValidConfusionMove(gamedataPtr data, creaturePtr cr, intpair ctr);
intpair getMoveNearerToPoint(gamedataPtr gdata, creaturePtr cr, intpair from, intpair to);
intpair getMoveFurtherFromPoint(gamedataPtr gdata, creaturePtr cr, intpair at, intpair awayFrom);

intpair getFreePointOnMap(gamedataPtr gdata);
intpair getTeleportPoint(gamedataPtr gdata, const intpair ctr, const int min_dist, const int max_dist);


//	Line drawing

vector<intpair> getBresenhamLine(intpair p1, intpair p2);
vector<intpair> getLineToTarget(gamedataPtr gdata, intpair p1, creaturePtr t, bool stopAtNonWalkable = false);

double getDistanceBetweenCreatures(creaturePtr c1, creaturePtr c2);


//	Cursor control

intpair autotargetCursor(gamedataPtr gdata);
void cycleCursorTarget(gamedataPtr gdata);

void moveCursor(gamedataPtr gdata, const intpair vec);
void enterLookMode(gamedataPtr gdata);

bool inCursorState(gamedataPtr gdata);


//	Sight lines

bool canCreatureSeeCreature(gamedataPtr gdata, creaturePtr c1, creaturePtr c2);

bool canMonsterSeeCreature(gamedataPtr gdata, creaturePtr mon, creaturePtr target);
bool canPlayerSeeCreature(gamedataPtr gdata, creaturePtr target);

int getActualVisionRadius(gamedataPtr gdata);


//	Highlight enemies/tiles

void startHighlightingEnemies(gamedataPtr gdata);