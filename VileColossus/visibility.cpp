#include "visibility.h"



//	Returns a list of free, walkable points in the given radius
vector<intpair> getFreePointsInRadius(gamedataPtr gdata, intpair ctr, int rad)
{
	vector<intpair> pts;
	for (int x = ctr.first - rad; x <= ctr.first + rad; x++)
	{
		for (int y = ctr.second - rad; y <= ctr.second + rad; y++)
		{
			if (gdata->_map->inBounds(x, y) && gdata->_map->isPointClear(x, y))
				pts.push_back(intpair(x, y));
		}
	}
	return pts;
}


//	Returns all free points adjacent to the given point.
vector<intpair> getAdjacentFreePoints(gamedataPtr data, creaturePtr cr, intpair pos)
{
	vector<intpair> pts;
	for (int x = pos.first - 1; x <= pos.first + 1; x++)
	{
		for (int y = pos.second - 1; y <= pos.second + 1; y++)
		{
			if (data->_map->inBounds(x, y) && data->_map->isPointClear(x, y))
				pts.push_back(intpair(x, y));
		}
	}
	return pts;
}


//	Returns all walkable points adjacent to the given point.
vector<intpair> getAdjacentWalkable(gamedataPtr gdata, intpair pos)
{
	vector<intpair> pts;
	for (int x = pos.first - 1; x <= pos.first + 1; x++)
	{
		for (int y = pos.second - 1; y <= pos.second + 1; y++)
		{
			if (!(pos.first == x && pos.second == y) && gdata->_map->inBounds(x, y) && gdata->_map->isWalkable(x, y))
				pts.push_back(intpair(x, y));
		}
	}
	return pts;
}


//	Returns true if the point is considered safe for the given creature to step on.
//	A point will usually be considered 'unsafe' if stepping on it would cause us to take damage.
bool isPointSafe(gamedataPtr gdata, creaturePtr cr, const intpair pt)
{
	

	return true;
}

//	Find a random move to make for a creature with the confusion status effect.
intpair getValidConfusionMove(gamedataPtr data, creaturePtr cr, intpair ctr)
{
	//	Get a list of valid adjacent points.
	vector<intpair> pts;
	for (int x = ctr.first - 1; x <= ctr.first + 1; x++)
	{
		for (int y = ctr.second - 1; y <= ctr.second + 1; y++)
		{
			if (!(ctr.first == x && ctr.second == y) && data->_map->inBounds(x, y) && data->_map->isWalkable(x, y))
				pts.push_back(intpair(x, y));
		}
	}
	if (!pts.empty())
		return pts[randrange(pts.size())];
	return ctr;
}


//	Returns a list of enemies adjacent to the given creature.
vector<creaturePtr> getAdjacentEnemies(gamedataPtr gdata, creaturePtr t)
{
	vector<creaturePtr> targets;
	auto ctr = t->_pos;
	for (int x = ctr.first - 1; x <= ctr.first + 1; x++)
	{
		for (int y = ctr.second - 1; y <= ctr.second + 1; y++)
		{
			if (gdata->_map->inBounds(x, y))
			{
				auto here = gdata->_map->getCreature(x, y);
				if (here != nullptr && here != t)
					targets.push_back(here);
			}
		}
	}
	return targets;
}


//	Returns a list of creatures directly adjacent to the given point
vector<creaturePtr> getAdjacentCreatures(gamedataPtr gdata, intpair ctr)
{
	vector<creaturePtr> targets;
	for (int x = ctr.first - 1; x <= ctr.first + 1; x++)
	{
		for (int y = ctr.second - 1; y <= ctr.second + 1; y++)
		{
			if (gdata->_map->inBounds(x, y))
			{
				auto here = gdata->_map->getCreature(x, y);
				if (here != nullptr)
					targets.push_back(here);
			}
		}
	}
	return targets;
}


//	Returns a move starting at 'from', going 1sq, that gets us closest to the target point.
//	If there are no valid moves, return the original point.
//	Will only return 'safe' points.
intpair getMoveNearerToPoint(gamedataPtr gdata, creaturePtr cr, intpair from, intpair to)
{
	float bestDist = 10000;
	intpair bestPt = from;
	for (auto pt : getAdjacentFreePoints(gdata, cr, from))
	{
		auto dist = hypot(pt.first - to.first, pt.second - to.second);
		if (dist < bestDist && isPointSafe(gdata, cr, pt))
		{
			bestDist = dist;
			bestPt = pt;
		}
	}
	return bestPt;
}


//	Returns the adjacent point that's furthest from 'awayFrom', starting from 'at'.
intpair getMoveFurtherFromPoint(gamedataPtr gdata, creaturePtr cr, intpair at, intpair awayFrom)
{
	float bestDist = 0;
	intpair bestPt = at;
	for (auto pt : getAdjacentFreePoints(gdata, cr, at))
	{
		auto dist = hypot(pt.first - awayFrom.first, pt.second - awayFrom.second);
		if (dist > bestDist)
		{
			bestDist = dist;
			bestPt = pt;
		}
	}
	return bestPt;
}


//	Returns any unoccupied point on the map.
intpair getFreePointOnMap(gamedataPtr gdata)
{
	int x, y;
	do
	{
		x = randrange(gdata->_map->_xsize);
		y = randrange(gdata->_map->_ysize);
	} while (!gdata->_map->isPointClear(x, y));
	return intpair(x, y);
}


//	Whenever a 'teleport' move is triggered, we warp to a point not too near & not too far from the starting point.
intpair getTeleportPoint(gamedataPtr gdata, const intpair ctr, const int min_dist, const int max_dist)
{
	double dist;
	intpair pt;
	do
	{
		pt = getFreePointOnMap(gdata);
		dist = hypot(pt.first - ctr.first, pt.second - ctr.second);
	} while (dist > max_dist || dist < min_dist);
	return pt;
}



vector<intpair> getBresenhamLine(intpair p1, intpair p2)
{
	float x1 = p1.first;
	float x2 = p2.first;
	float y1 = p1.second;
	float y2 = p2.second;

	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if (steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	const bool swapped = x1 > x2;
	if (swapped)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	vector<intpair> pts;
	const int maxX = (int)x2;
	for (int x = (int)x1; x <= maxX; x++)
	{
		auto npt = intpair(x, y);
		if (steep) npt = intpair(y, x);
		pts.push_back(npt);

		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}

	//	check whether we need to reverse the list (so that the point of origin is first)
	if (pts.front() != p1)
		reverse(pts.begin(), pts.end());

	//	exclude the first point
	pts.erase(pts.begin());
	return pts;
}


//	Creates a Bresenham line passing from the given point to the target creature.
//	If 'stopAtNonWalkable' is set, the line will terminate early if it reaches a non-walkable point (& thus may not actually reach the creature)
vector<intpair> getLineToTarget(gamedataPtr gdata, intpair p1, creaturePtr t, bool stopAtNonWalkable)
{
	auto pts = getBresenhamLine(p1, t->_pos);
	vector<intpair> pts_final;
	
	for (auto pt : pts)
	{
		if (stopAtNonWalkable && !gdata->_map->isWalkable(pt))
			break;

		pts_final.push_back(pt);

		if (gdata->_map->getCreature(pt) == t)
			break;
	}

	return pts_final;
}

//	just uses pythagoras, but it's convenient to have this in a single function
double getDistanceBetweenCreatures(creaturePtr c1, creaturePtr c2)
{
	return hypot(c1->_pos.first - c2->_pos.first, c1->_pos.second - c2->_pos.second);
}


//	When we toggle the cursor on, returns a point to auto-target.
//	This defaults to the closest enemy - if no enemy is visible, defaults to player's position instead.
intpair autotargetCursor(gamedataPtr gdata)
{
	if (gdata->_visibleMonsters.empty())
		return gdata->_player->_pos;

	else
	{
		//	find closest monster
		auto bestDist = getDistanceBetweenCreatures(gdata->_player, gdata->_visibleMonsters[0]);
		auto bestPt = gdata->_visibleMonsters[0]->_pos;
		for (unsigned i = 1; i < gdata->_visibleMonsters.size(); i++)
		{
			auto dist = getDistanceBetweenCreatures(gdata->_player, gdata->_visibleMonsters[i]);
			if (dist < bestDist)
			{
				bestDist = dist;
				bestPt = gdata->_visibleMonsters[i]->_pos;
			}
		}
		return bestPt;
	}
	return gdata->_player->_pos;
}


//	Moves the cursor to the 'next' visible monster.
void cycleCursorTarget(gamedataPtr gdata)
{
	//	If there are no visible monsters, nothing happens.
	if (!gdata->_visibleMonsters.empty())
	{
		//	What are we currently targeting?
		auto t = gdata->_map->getCreature(gdata->_cursorPt);

		//	If we're not targeting anything, use autotarget.
		if (t == nullptr)
			gdata->_cursorPt = autotargetCursor(gdata);

		//	Target the 'next' creature
		else
		{
			//	figure out which monster we're highlighting
			auto f = find(gdata->_visibleMonsters.begin(), gdata->_visibleMonsters.end(), t);
			if (f != gdata->_visibleMonsters.end())
			{
				f++;
				if (f == gdata->_visibleMonsters.end())
					f = gdata->_visibleMonsters.begin();
				gdata->_cursorPt = (*f)->_pos;
			}

			//	(this should only happen if the player character is highlighted, I think?)
			else
				gdata->_cursorPt = gdata->_visibleMonsters[0]->_pos;
		}
	}
}


//	Move the cursor. Can only look at points in FOV.
void moveCursor(gamedataPtr gdata, const intpair vec)
{
	auto pt = gdata->_cursorPt;
	intpair_add(&pt, &vec);
	if (gdata->_map->inBounds(pt) && gdata->_map->isInFov(pt))
		gdata->_cursorPt = pt;
}


//	Start looking around.
void enterLookMode(gamedataPtr gdata)
{
	gdata->_state = STATE_LOOK;
	gdata->_cursorPt = gdata->_player->_pos;
}


//	If True, we display the cursor position on screen
bool inCursorState(gamedataPtr gdata)
{
	return gdata->_state == STATE_LOOK;
}


//	Universal seeing-test, ignores anything unique to players/monsters
bool canCreatureSeeCreature(gamedataPtr gdata, creaturePtr c1, creaturePtr c2)
{
	//	can always see if we're adjacent
	if (getDistanceBetweenCreatures(c1, c2) < 2)
		return true;

	//	default to true
	return true;
}

//	Checks whether a monster can see a given character.
//	Monsters can't see anything unless they're in FOV.
bool canMonsterSeeCreature(gamedataPtr gdata, creaturePtr mon, creaturePtr target)
{
	return gdata->_map->isInFov(mon->_pos) && canCreatureSeeCreature(gdata, mon, target);
}

//	Tests whether the player can see a given creature.
bool canPlayerSeeCreature(gamedataPtr gdata, creaturePtr target)
{
	return gdata->_map->isInFov(target->_pos) && 
		getDistanceBetweenCreatures(gdata->_player, target) <= gdata->_player->getVisionRadius() && 
		canCreatureSeeCreature(gdata, gdata->_player, target);
}


//	Combines player's torch level and the map's inherent light adjustment
int getActualVisionRadius(gamedataPtr gdata)
{
	int total = gdata->_player->getVisionRadius() + gdata->_map->_lightLevel;
	if		(total < 3)		total = 3;
	else if (total > 15)	total = 15;
	return total;
}


//	A special overlay that highlights visible enemies.
void startHighlightingEnemies(gamedataPtr gdata)
{
	if (!gdata->_visibleMonsters.empty())
	{
		gdata->_state = STATE_HIGHLIGHT_ENEMIES;

	}
}
