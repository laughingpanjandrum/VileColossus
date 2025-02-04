#include "mapgen.h"



intpair mapgen::getRandomPoint(gridmapPtr m)
{
	int x = randint(1, m->_xsize - 2);
	int y = randint(1, m->_ysize - 2);
	return intpair(x, y);
}

intpair mapgen::getRandomWalkable(gridmapPtr m)
{
	auto pt = getRandomPoint(m);
	while (!m->isWalkable(pt))
		pt = getRandomPoint(m);
	return pt;
}


intpair mapgen::getRandomWithPad(gridmapPtr m, int pad)
{
	int x = randint(pad, m->_xsize - pad);
	int y = randint(pad, m->_ysize - pad);
	return intpair(x, y);
}

//	Random free point that's at least 'pad' points from any map edge
intpair mapgen::getFreeWithPad(gridmapPtr m, int pad)
{
	auto pt = getRandomWithPad(m, pad);
	while (!m->isPointClear(pt))
		pt = getRandomWithPad(m, pad);
	return pt;
}

intpair mapgen::getWalkableWithPad(gridmapPtr m, int pad)
{
	auto pt = getRandomWithPad(m, pad);
	while (!m->isWalkable(pt))
		pt = getRandomWithPad(m, pad);
	return pt;
}

intpair mapgen::getRandomInBounds(gridmapPtr m, int right_pad, int bottom_pad)
{
	int x = randrange(m->_xsize - right_pad);
	int y = randrange(m->_ysize - bottom_pad);
	return intpair(x, y);
}

intpair mapgen::getRandomInBounds(gridmapPtr m, int x, int y, int w, int h)
{
	int atx = randint(x, x + w);
	int aty = randint(y, y + h);
	return intpair(atx, aty);
}

intpair mapgen::getRandomFree(gridmapPtr m)
{
	auto pt = getRandomWalkable(m);
	while (m->getCreature(pt) != nullptr)
		pt = getRandomWalkable(m);
	return pt;
}



//	Random walkable square with no item on it.
intpair mapgen::getRandomForItem(gridmapPtr m)
{
	intpair pt;
	do {
		pt = getRandomSafe(m);
	} while (false);
	return pt;
}


//	Random square on which stairs can validly appear.
intpair mapgen::getRandomForStairs(gridmapPtr m)
{
	intpair pt;
	do {
		pt = getRandomSafe(m);
	} while (isMaptileStairs(m->getTile(pt)));
	return pt;
}



//	Random point in a circular region.
intpair mapgen::getRandomInCircle(gridmapPtr m, intpair ctr, int r)
{
	//	i guess we could do some silly stuff with rays and angles and whatnot
	//	but instead we just pick points in a square and reject points outside the circular region
	//	sorry. i truly apologize for doing this
	auto pt = getRandomInBounds(m, ctr.first - r, ctr.second - r, r * 2, r * 2);
	while (hypot(pt.first - ctr.first, pt.second - ctr.second) > r)
		pt = getRandomInBounds(m, ctr.first - r, ctr.second - r, r * 2, r * 2);
	return pt;
}


//	Returns true if the Maptile is considered 'safe' & thus eligible for items, stairs, etcs
bool mapgen::isSafeTile(const Maptile tl)
{
	return tl == MT_FLOOR_STONE || tl == MT_BUSH || tl == MT_FLOOR_CARPET || tl == MT_FLOOR_STONE2 || tl == MT_GRASS || tl == MT_SAND || tl == MT_FLOOR_HOT || tl == MT_FLOOR_VOID || tl == MT_BLOOD_POOL;
}

//	A random point that is some type of 'safe' floor-type tile: ie no surface, no deep water, etc
intpair mapgen::getRandomSafe(gridmapPtr m)
{
	intpair pt;
	do
	{
		pt = getRandomWalkable(m);
	} while (!isSafeTile(m->getTile(pt)));
	return pt;
}


intpair mapgen::getRandomInNode(const TCODBsp * node)
{
	int x = node->x + randint(0, node->w);
	int y = node->y + randint(0, node->h);
	return intpair(x, y);
}

intpair mapgen::getRandomWalkableInNode(gridmapPtr m, TCODBsp * node)
{
	auto pt = getRandomInNode(node);
	while (!m->isWalkable(pt))
		pt = getRandomInNode(node);
	return pt;
}


//	Returns a random point on the edge of the node, excluding the corners.
intpair mapgen::getRandomOnNodeEdge(TCODBsp * node)
{
	return getRandomPointOnEdge(node->x - 1, node->y - 1, node->w + 2, node->h + 2);
}


//	Returns a random point on an exterior edge of the given rectangle.
//	Will NOT return any of the corner points.
intpair mapgen::getRandomPointOnEdge(int x, int y, int w, int h)
{
	int fx, fy;
	if (coinflip())
	{
		fx = randint(x + 1, x + w - 2);
		if (coinflip())
			fy = y;
		else
			fy = y + h - 1;
	}
	else
	{
		fy = randint(y + 1, y + h - 2);
		if (coinflip())
			fx = x;
		else
			fx = x + w - 1;
	}
	return intpair(fx, fy);
}


//	Returns a random clear point on the map edge.
intpair mapgen::getRandomFreeOnEdge(gridmapPtr m)
{
	auto pt = getRandomPointOnEdge(0, 0, m->_xsize, m->_ysize);
	while (!m->isPointClear(pt))
		pt = getRandomPointOnEdge(0, 0, m->_xsize, m->_ysize);
	return pt;
}

intpair mapgen::getFreeINode(gridmapPtr m, const TCODBsp* node)
{
	intpair pt;
	do {
		pt = getRandomInNode(node);
	} while (!m->isPointClear(pt));
	return pt;
}


//	Returns all walkable points within the given radius of the given central point.
vector<intpair> mapgen::getGroupOfFreePoints(gridmapPtr m, const intpair ctr, const int rad)
{
	vector<intpair> pts;
	for (int x = ctr.first - rad; x <= ctr.first + rad; x++)
	{
		for (int y = ctr.second - rad; y <= ctr.second + rad; y++)
		{
			if (m->inBounds(x, y) && m->isPointClear(x, y))
				pts.push_back(intpair(x, y));
		}
	}
	return pts;
}


//	Returns every free point in the given node
vector<intpair> mapgen::getAllFreeInNode(gridmapPtr m, const TCODBsp* node)
{
	vector<intpair> pts;
	for (unsigned x = node->x; x <= node->x + node->w; x++)
	{
		for (unsigned y = node->y; y <= node->y + node->h; y++)
		{
			if (m->isPointClear(x, y))
				pts.push_back(intpair(x, y));
		}
	}
	return pts;
}


//	Returns the number of neighbours of the given cell that are set to 'true'
const int mapgen::countNeighbours(const vector<vector<bool>>* cmap, const int cx, const int cy)
{
	int count = 0;
	for (int x = cx - 1; x < cx + 1; x++)
	{
		for (int y = cy - 1; y < cy + 1; y++)
		{
			if (!(x == cx && cy == cy) && (x >= 0 && x < cmap->size() && y >= 0 && y < cmap->at(x).size()))
			{
				if ((*cmap)[x][y])
					count++;
			}
		}
	}
	return count;
}

//	Cells 'die' if they have fewer than minPop or more than maxPop neighbours.
//	A cell is 'birthed' in an empty space if it has exactly 'birthPop' neighbours.
//	'startFilled' is the chance/100 that each cell starts populated.
vector<vector<bool>> mapgen::rollCellAutomata(const int x_sz, const int y_sz, const int minPop, const int maxPop, const int birthPop, int cycles, const int startFilled)
{
	//	Create the initial cell map.
	vector<vector<bool>> cellmap;
	vector<vector<bool>> swapmap;
	for (unsigned x = 0; x < x_sz; x++)
	{
		vector<bool> xcells;
		for (unsigned y = 0; y < y_sz; y++)
			xcells.push_back(roll_percent(startFilled));
		cellmap.push_back(xcells);
		swapmap.push_back(xcells);
	}

	//	Perform some cycles
	vector<vector<bool>> *current = &cellmap;
	vector<vector<bool>> *other = &swapmap;
	while (cycles-- > 0)
	{
		//	Perform one cycle of the current map.
		for (unsigned x = 0; x < x_sz; x++)
		{
			for (unsigned y = 0; y < y_sz; y++)
			{
				auto n = countNeighbours(current, x, y);
				(*other)[x][y] = (*current)[x][y];

				//	cell is alive - check for death
				if ((*current)[x][y])
				{
					if (n < minPop || n > maxPop)
						(*other)[x][y] = false;
				}

				//	cell is not alive - check for birth
				else
				{
					if (n == birthPop)
						(*other)[x][y] = true;
				}
			}
		}

		//	Switch the current map pointer to the updated map.
		if (current == &cellmap)
		{
			current = &swapmap;
			other = &cellmap;
		}
		else
		{
			current = &swapmap;
			other = &cellmap;
		}
	}

	return *current;
}



//	Also clears any surface that shouldn't be under a tile that is placed.
inline void mapgen::fillRegion(gridmapPtr m, vector<Maptile> tileOptions, int x, int y, int w, int h)
{
	for (int atx = x; atx < x + w; atx++)
	{
		for (int aty = y; aty < y + h; aty++)
		{
			if (!m->inBounds(atx, aty))
				cout << "ERROR: fillRegion out of bounds at " << atx << ',' << aty << "; parameters " << x << ',' << y << ", w/h=" << w << ',' << h << endl;
			else
			{
				auto t = tileOptions[randrange(tileOptions.size())];
				m->setTile(t, atx, aty);
			}
		}
	}
}

inline void mapgen::fillRegion(gridmapPtr m, Maptile t, int x, int y, int w, int h)
{
	fillRegion(m, vector<Maptile>({ t }), x, y, w, h);
}

inline void mapgen::fillMap(gridmapPtr m, vector<Maptile> tileOptions)
{
	fillRegion(m, tileOptions, 0, 0, m->_xsize, m->_ysize);
}


//	Also removes any surfaces that are no longer valid at the given coordinate.
inline void mapgen::fillRegionCircular(gridmapPtr m, vector<Maptile> tileOptions, int ctrx, int ctry, int rad)
{
	for (int atx = ctrx - rad; atx <= ctrx + rad; atx++)
	{
		for (int aty = ctry - rad; aty <= ctry + rad; aty++)
		{
			if (hypot(atx - ctrx, aty - ctry) <= rad && m->inBounds(atx, aty))
			{
				auto tl = tileOptions[randrange(tileOptions.size())];
				m->setTile(tl, atx, aty);
			}
		}
	}
}

inline void mapgen::fillRegionCircular(gridmapPtr m, Maptile tl, int ctrx, int ctry, int rad)
{
	fillRegionCircular(m, vector<Maptile>({ tl }), ctrx, ctry, rad);
}

//	Creates a 'room' type thing
inline void mapgen::fillRegionAndHollow(gridmapPtr m, Maptile outer, vector<Maptile> inner, int x, int y, int w, int h)
{
	fillRegion(m, outer, x, y, w, h);
	fillRegion(m, inner, x + 1, y + 1, w - 2, h - 2);
}


//	Tests adjacent points for walkability. Recursive.
void mapgen::floodfill_recursive(gridmapPtr m, vector<vector<bool>>* cells, const intpair pt)
{
	(*cells)[pt.first][pt.second] = true;
	for (int x = pt.first - 1; x <= pt.first + 1; x++)
	{
		for (int y = pt.second - 1; y <= pt.second + 1; y++)
		{
			if (m->isWalkable(x, y) && !(*cells)[x][y])
				floodfill_recursive(m, cells, intpair(x, y));
		}
	}
}


//	Starting from the given point, tests adjacent points for walkability.
//	Returns a 2d vector of cells; a cell marked 'true' means that it is both walkable AND connected to the initial point.
const vector<vector<bool>> mapgen::floodfill(gridmapPtr m, const intpair start_pt)
{
	//	Create the grid of cells
	vector<vector<bool>> pts;
	for (unsigned x = 0; x < m->_xsize; x++)
	{
		vector<bool> xpts;
		for (unsigned y = 0; y < m->_ysize; y++)
			xpts.push_back(false);
		pts.push_back(xpts);
	}

	//	Start the floodfill
	floodfill_recursive(m, &pts, start_pt);

	return pts;
}


//	Same as 'scatterTile', but won't replace non-walkable tiles like walls
void mapgen::scatterTileOnWalkable(gridmapPtr m, Maptile tl, int x, int y, int w, int h, float density)
{
	int amt = (float)(w * h) * density;
	while (amt-- >= 0)
	{
		auto pt = getRandomInBounds(m, x, y, w, h);
		if (m->isWalkable(pt))
			m->setTile(tl, pt);
	}
}


//	Scatters the given tile around in a given region.
void mapgen::scatterTile(gridmapPtr m, Maptile tl, int x, int y, int w, int h, float density)
{
	int amt = (float)(w * h) * density;
	while (amt-- >= 0)
	{
		auto pt = getRandomInBounds(m, x, y, w, h);
		if (m->inBounds(pt))
			m->setTile(tl, pt);
	}
}


//	Can appear anywhere on the map. Can over-write existing tiles.
void mapgen::scatterOnMap(gridmapPtr m, Maptile tl, float density)
{
	scatterTile(m, tl, 0, 0, m->_xsize - 1, m->_ysize - 1, density);
}


//	Randomly spread surfaces around.
void mapgen::scatterSurface(gridmapPtr m, const Surface sf, int x, int y, int w, int h, float density)
{
	auto node = new TCODBsp(x, y, w, h);
	int amt = (float)(w * h) * density;
	while (amt-- >= 0)
	{
		auto pt = getRandomWalkableInNode(m, node);
		if (m->canCoverWithSurface(x, y))
			m->setSurface(sf, pt);
	}
}


//	Assembles a list of all leaf nodes in the tree by recursively iterating over all the nodes.
void mapgen::getLeafNodesRecursive(vector<TCODBsp*>* nodes, TCODBsp * thisNode)
{
	if (thisNode->isLeaf())
		nodes->push_back(thisNode);
	else
	{
		getLeafNodesRecursive(nodes, thisNode->getLeft());
		getLeafNodesRecursive(nodes, thisNode->getRight());
	}
}



//	Creates a BSP tree; returns a list of all leaf nodes.
vector<TCODBsp*> mapgen::createNodeMap(gridmapPtr m, const int min_size, const int recursions)
{
	//	Create the tree.
	auto root = new TCODBsp(2, 2, m->_xsize - 4, m->_ysize - 4);
	root->splitRecursive(NULL, recursions, min_size, min_size, 1.5f, 1.5f);

	//	Get every leaf node.
	vector<TCODBsp*> leaves;
	getLeafNodesRecursive(&leaves, root);

	return leaves;
}


//	Dig a tunnel that connects the two given points
void mapgen::tunnelBetweenPoints(gridmapPtr m, int midx1, int midy1, int midx2, int midy2, Maptile floor)
{
	//	find tunneling directions
	int xv = 0, yv = 0;

	if (midx1 < midx2)		xv = 1;
	else if (midx1 > midx2)		xv = -1;
	if (midy1 < midy2)		yv = 1;
	else if (midy1 > midy2)		yv = -1;

	//	walk the path
	int atx = midx1, aty = midy1;
	bool vert_walk = randint(1, 2) == 1;
	while (atx != midx2 || aty != midy2)
	{
		//	decide whether to switch directions
		if (vert_walk && aty == midy2)
			vert_walk = false;
		else if (!vert_walk && atx == midx2)
			vert_walk = true;

		//	walk
		if (vert_walk && aty != midy2)
			aty += yv;
		else if (atx != midx2)
			atx += xv;
		//vert_walk = !vert_walk;

		//	set tile beneath
		m->setTile(floor, atx, aty);
	}
}


//	Digs a passage from one node to another.
void mapgen::tunnelBetweenNodes(gridmapPtr m, TCODBsp * node1, TCODBsp * node2, Maptile floor)
{
	//	find the MIDPOINTS of each room to determine where they are relative to one another
	int midx1 = node1->x + node1->w / 2 + randint(-2, 2);
	int midy1 = node1->y + node1->h / 2 + randint(-2, 2);
	int midx2 = node2->x + node2->w / 2 + randint(-2, 2);
	int midy2 = node2->y + node2->h / 2 + randint(-2, 2);
	tunnelBetweenPoints(m, midx1, midy1, midx2, midy2, floor);
}


//	Generate a building at the given coordinates with the given dimensions.
//	A 'building' is just a square with a random hole for a door.
void mapgen::addBuilding(gridmapPtr m, int x, int y, int w, int h, Maptile wall, Maptile floor, Maptile door)
{
	//	the 'building'
	fillRegionAndHollow(m, wall, { floor }, x, y, w, h);

	//	pick a random exterior point for the door
	auto door_pt = getRandomPointOnEdge(x, y, w, h);
	m->setTile(door, door_pt);
}


//	Adds a circular region, whose outer tiles all consist of the Maptile 'outer'
//	and the rest of which is filled with 'inner'.
void mapgen::addLake(gridmapPtr m, intpair ctr, int r, Maptile inner, Maptile outer)
{
	for (int x = ctr.first - r; x <= ctr.first + r; x++)
	{
		for (int y = ctr.second - r; y <= ctr.second + r; y++)
		{
			if (m->inBounds(x, y))
			{
				int d = hypot(x - ctr.first, y - ctr.second);
				if (d == r)
					m->setTile(outer, x, y);
				else if (d < r)
					m->setTile(inner, x, y);
			}
		}
	}
}


//	Starting at the given tile, sets the tile, then moves to a random adjacent tile.
//	Continue for the given number of steps. It can overlap itself.
void mapgen::walkingTile(gridmapPtr m, const intpair start, int count, Maptile tl)
{
	intpair pt = start;
	intpair last;
	while (count-- > 0)
	{
		m->setTile(tl, pt);
		last = pt;
		int tries = 18;
		do {
			//	have we run out of tries?
			if (tries <= 0)
			{
				//	we can't find a legal adjacent point; give up and stop walking
				count = 0;
				break;
			}

			//	try again
			pt.first += randint(-1, 1);
			pt.second += randint(-1, 1);
			tries--;
		} while (pt == last || !m->inBounds(pt));
	}
}


//	Returns points adjacent to the given point (NOT including the central point) that match the given tile.
vector<intpair> mapgen::getAdjacentOfType(gridmapPtr m, const intpair ctr, const Maptile tl)
{
	vector<intpair> pts;
	for (int x = ctr.first - 1; x <= ctr.first + 1; x++)
	{
		for (int y = ctr.second - 1; y <= ctr.second + 1; y++)
		{
			if (m->inBounds(x, y) && !(x == ctr.first && y == ctr.second) && m->getTile(x, y) == tl)
				pts.push_back(intpair(x, y));
		}
	}
	return pts;
}



//	Rolls a gang of monsters for a given DL.
vector<monsterPtr> mapgen::rollMonsterGroup(int dl, const MonsterType id)
{
	//	Potential alternate versions
	auto alts = monsterdata::getAltVersions(id, dl);

	//	Quantity depends on type
	int count = 1;
	if (!monsterdata::isSoloMonster(id))
		count += randint(5, 8);


	//	generate the monster table
	vector<monsterPtr> mlist;
	while (count-- > 0)
	{
		//	Roll alt instead?
		auto drop = id;
		if (roll_one_in(3) && !alts.empty())
			drop = alts[randrange(alts.size())];

		//	Monster level is twice danger level with a little variance.
		int mlvl = dl * 2 + randint(-1, 0);
		mlvl = MAX(1, mlvl);

		//	Add monster to list
		mlist.push_back(monsterdata::generate(drop, mlvl));
	}


	//	add leaders to the gang, if necessary
	auto boss_id = monsterdata::getGroupLeaderType(id, dl);
	if (boss_id != MonsterType::__NONE)
	{
		count = randint(1, 3);
		while (count-- > 0)
			mlist.push_back(monsterdata::generate(boss_id, dl * 2));
	}

	return mlist;
}


//	Randomly scatter a given list of monsters on a given node.
void mapgen::addMonsterGroupToNode(gridmapPtr m, const vector<monsterPtr>* mlist, const TCODBsp* n)
{
	auto pts = getAllFreeInNode(m, n);
	for (auto mon : *mlist)
	{
		//	stop if we run of points for monsters
		if (pts.empty()) break;

		//	pick a random monster location
		auto p = randrange(pts.size());
		m->addCreature(mon, pts[p]);
		pts.erase(pts.begin() + p);
	}
}


//	Scatter groups of monsters around.
void mapgen::addMonsters(gridmapPtr m, int dl, const vector<TCODBsp*>* nodeOptions)
{
	//	Types of groups available to drop
	auto moptions = monsterdata::getMonstersForLevel(dl);

	//	Number of groups to drop
	int ncount = 1 + nodeOptions->size() / 6;
	while (ncount-- > 0)
	{
		auto n = nodeOptions->at(randrange(nodeOptions->size()));

		//	Get a set of monsters and a set of points in which to place them
		auto mlist = rollMonsterGroup(dl, moptions[randrange(moptions.size())]);
		addMonsterGroupToNode(m, &mlist, n);
	}

	//	Chance of miniboss
	if (!roll_one_in(3) && dl > 2)
	{
		auto boss_id = monsterdata::rollMinibossForLevel(dl);
		if (boss_id != MonsterType::__NONE)
			m->addCreature(monsterdata::generate(boss_id, dl * 2 + 1), getRandomFree(m));
	}

	//	Chance of BOSS
	else if (roll_one_in(3) && dl > 4)
	{
		auto boss_id = monsterdata::rollBossForLevel(dl);
		if (boss_id != MonsterType::__NONE)
			m->addCreature(monsterdata::generate(boss_id, dl * 2 + 1), getRandomFree(m));
	}
}


//	Pick some random points for up/down stairs.
//	This also determines your starting position on the map.
void mapgen::addStairsToMap(gridmapPtr m, int depth, bool descending)
{
	int count = randint(1, 3);


	//	stairs back up (only at certain depths!!)
	while (count-- > 0 && depth != 10 && depth < 12)
	{
		auto pt = getRandomForStairs(m);
		m->setTile(MT_STAIRS_UP, pt);
		if (descending)
			m->_startPt = pt;
	}


	//	at higher depths while descending, we start at a random point
	if (descending && depth >= 12)
		m->_startPt = getRandomForStairs(m);


	//	multiple down stairs
	count = randint(2, 4);
	while (count-- > 0)
	{
		auto pt = getRandomForStairs(m);
		m->setTile(MT_STAIRS_DOWN, pt);
		if (!descending)
			m->_startPt = pt;
	}


	//	LONG stairs up, making it quicker to get to the surface at deeper levels
	if (depth == 8)
		m->setTile(MT_STAIRS_UP_LONG, getRandomForStairs(m));

	else if (depth == 10)
	{
		auto spt = getRandomForStairs(m);
		m->setTile(MT_HELLPORTAL_UP, spt);
		m->_startPt = spt;
	}
}


//	Returns a string representation of non-walkable & walkable adjacent tiles (1=walkable,0=wall)
string mapgen::countAdjacentWalls(gridmapPtr m, int x, int y)
{
	string txt = "";
	for (int atx = x - 1; atx <= x + 1; atx++)
	{
		for (int aty = y - 1; aty <= y + 1; aty++)
		{
			//	what tile is here bois
			auto tl = m->getTile(atx, aty);
			if ((!m->isWalkable(atx, aty) && !m->isTransparent(atx, aty)) || (tl == MT_DOOR_STONE))
				txt += "1";
			else
				txt += "0";
		}
	}
	return txt;
}


//	Decides whether a door should go here, based on the configuration of walls surrounding the point.
bool mapgen::verifyPointOkayForDoor(gridmapPtr m, int x, int y)
{
	auto wallstr = countAdjacentWalls(m, x, y);
	return wallstr == "101101000" || wallstr == "110000110" || wallstr == "000101101" || wallstr == "011000011";
}


//	Add doors of random types to any suitable points.
void mapgen::addDoorsToMap(gridmapPtr m)
{
	for (int x = 2; x < m->_xsize - 2; x++)
	{
		for (int y = 2; y < m->_ysize - 2; y++)
		{
			if (verifyPointOkayForDoor(m, x, y))
			{
				auto tl = MT_DOOR_STONE;
				m->setTile(tl, x, y);
			}
		}
	}
}


//	Cross-shaped chapel.
void mapgen::cathedral_Chapel(gridmapPtr m, metanode* n, int dl)
{
	const int offs = 2, sz = 5;
	const int o2 = 1, s2 = 3;
	const int cx = n->x + n->w / 2;
	const int cy = n->y + n->h / 2;

	//	Chapel proper
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, cx - offs, n->y, sz, n->h);
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, n->x, cy - offs, n->w, sz);
	fillRegion(m, MT_FLOOR_CARPET, cx - o2, n->y + 2, s2, n->h - 2);

	//	Pews/other features
	m->setTile(MT_TABLE_WOODEN, cx, n->y + 1);
	m->setTile(MT_STATUE_MARBLE, cx - sz + 1, cy);
	m->setTile(MT_STATUE_MARBLE, cx + sz - 1, cy);
	for (unsigned y = cy - 2; y < cy + sz; y += 2)
	{
		m->setTile(MT_PEW_WOODEN, cx - 2, y);
		m->setTile(MT_PEW_WOODEN, cx + 2, y);
	}

	//	Chapel exits
	if (n->north) {
		m->setTile(MT_DOOR_STONE, cx, cy - sz - 1);
	}
	if (n->south) {
		m->setTile(MT_DOOR_STONE, cx, cy + sz + 1);
	}
	if (n->east) {
		m->setTile(MT_DOOR_STONE, cx + sz + 1, cy);
	}
	if (n->west) { 
		m->setTile(MT_DOOR_STONE, cx - sz - 1, cy);
	}
}


//	A hallway with a carpet.
void mapgen::cathedral_Hallway(gridmapPtr m, metanode* n, int dl)
{
	const int offs = 2, sz = 5;
	const int o2 = 1, s2 = 3;
	const int cx = n->x + n->w / 2;
	const int cy = n->y + n->h / 2;

	//	halls
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, cx - offs, n->y, sz, n->h);
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, n->x, cy - offs, n->w, sz);
	fillRegion(m, MT_FLOOR_CARPET, cx - o2, cy - o2, s2, s2);

	//	connections
	if (n->north) {
		fillRegion(m, MT_FLOOR_CARPET, cx - o2, n->y - 1, s2, sz);
	} else if (roll_one_in(2)) {
		fillRegion(m, MT_WALL_STONE, cx - offs - 1, cy - offs, sz + 2, 1);
		m->setTile(MT_DOOR_STONE, cx, cy - offs);
	} else {
		m->setTile(MT_TABLE_WOODEN, cx, cy - sz + 1);
	}
	if (n->south) {
		fillRegion(m, MT_FLOOR_CARPET, cx - o2, cy + o2, s2, sz);
	} else if (roll_one_in(2)) {
		fillRegion(m, MT_WALL_STONE, cx - offs - 1, cy + offs, sz + 2, 1);
		m->setTile(MT_DOOR_STONE, cx, cy + offs);
	} else {
		m->setTile(MT_TABLE_WOODEN, cx, cy + sz - 1);
	}
	if (n->east) {
		fillRegion(m, MT_FLOOR_CARPET, cx + o2, cy - o2, sz, s2);
	} else if (roll_one_in(2)) {
		fillRegion(m, MT_WALL_STONE, cx + offs, cy - offs - 1, 1, sz + 2);
		m->setTile(MT_DOOR_STONE, cx + offs, cy);
	} else {
		m->setTile(MT_TABLE_WOODEN, cx + sz - 1, cy);
	}
	if (n->west) {
		fillRegion(m, MT_FLOOR_CARPET, n->x - 1, cy - o2, sz, s2);
	} else if (roll_one_in(2)) {
		fillRegion(m, MT_WALL_STONE, cx - offs, cy - offs - 1, 1, sz + 2);
		m->setTile(MT_DOOR_STONE, cx - offs, cy);
	} else {
		m->setTile(MT_TABLE_WOODEN, cx - sz + 1, cy);
	}

	//	goop?
	if (dl > 7 && roll_one_in(3))
	{
		scatterSurface(m, Surface::SLUDGE, n->x + 1, n->y + 1, n->w - 2, n->h - 2, 0.3);
	}
}


void mapgen::cathedral_Pool(gridmapPtr m, metanode* n, int dl)
{
	const int cx = n->x + n->w / 2;
	const int cy = n->y + n->h / 2;

	fillRegionCircular(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, cx, cy, 5);
	fillRegionCircular(m, MT_WATER, cx, cy, 3);
	m->setTile(MT_STATUE_MARBLE, cx - 2, cy - 2);
	m->setTile(MT_STATUE_MARBLE, cx + 2, cy - 2);
	m->setTile(MT_STATUE_MARBLE, cx - 2, cy + 2);
	m->setTile(MT_STATUE_MARBLE, cx + 2, cy + 2);

	//	connections
	if (n->north) {
		fillRegion(m, MT_FLOOR_STONE, cx, n->y - 1, 1, 2);
	}
	if (n->south) {
		fillRegion(m, MT_FLOOR_STONE, cx, n->y + n->h - 1, 1, 2);
	}
	if (n->east) {
		fillRegion(m, MT_FLOOR_STONE, n->x + n->w - 1, cy, 2, 1);
	}
	if (n->west) {
		fillRegion(m, MT_FLOOR_STONE, n->x - 1, cy, 2, 1);
	}

	//	goop?
	if (dl > 5)
	{
		scatterSurface(m, Surface::SLUDGE, n->x + 1, n->y + 1, n->w - 2, n->h - 2, 0.3);
	}
}


void mapgen::cathedral_Storage(gridmapPtr m, metanode* n, int dl)
{
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, n->x + 1, n->y + 1, n->w - 2, n->h - 2);
	
	m->setTile(MT_STATUE_MARBLE, n->x + 2, n->y + 2);
	m->setTile(MT_STATUE_MARBLE, n->x + 2, n->y + n->h - 2);
	m->setTile(MT_STATUE_MARBLE, n->x + n->w - 2, n->y + 2);
	m->setTile(MT_STATUE_MARBLE, n->x + n->w - 2, n->y + n->h - 2);
	
	scatterTile(m, MT_BARREL, n->x + 2, n->y + 2, n->w - 4, n->h - 4, 0.4);

	//	connections
	const int cx = n->x + n->w / 2;
	const int cy = n->y + n->h / 2;
	if (n->north) {
		fillRegion(m, MT_FLOOR_STONE, cx, n->y - 1, 1, 2);
	}
	if (n->south) {
		fillRegion(m, MT_FLOOR_STONE, cx, n->y + n->h - 1, 1, 2);
	}
	if (n->east) {
		fillRegion(m, MT_FLOOR_STONE, n->x + n->w - 1, cy, 2, 1);
	}
	if (n->west) {
		fillRegion(m, MT_FLOOR_STONE, n->x - 1, cy, 2, 1);
	}

	//	goop?
	if (dl > 5)
	{
		scatterSurface(m, Surface::SLUDGE, n->x + 1, n->y + 1, n->w - 2, n->h - 2, 0.3);
	}
}


void mapgen::cathedral_Tombs(gridmapPtr m, metanode* n, int dl)
{
	const int offs = 4, sz = 9;
	const int o2 = 1, s2 = 3;
	const int cx = n->x + n->w / 2;
	const int cy = n->y + n->h / 2;

	//	beeg room
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, n->x + 1, n->y + 1, n->w - 2, n->h - 2);
	for (unsigned x = 2; x <= n->w - 2; x += 2)
	{
		for (unsigned y = 2; y <= n->h - 2; y += 2)
		{
			m->setTile(MT_SARCOPHAGUS, n->x + x, n->y + y);
		}
	}

	//	connections
	if (n->north) {
		fillRegion(m, MT_FLOOR_STONE, cx, n->y - 1, 1, 2);
	}
	if (n->south) {
		fillRegion(m, MT_FLOOR_STONE, cx, n->y + n->h - 1, 1, 2);
	}
	if (n->east) {
		fillRegion(m, MT_FLOOR_STONE, n->x + n->w - 1, cy, 2, 1);
	}
	if (n->west) {
		fillRegion(m, MT_FLOOR_STONE, n->x - 1, cy, 2, 1);
	}


	//	goop?
	if (dl > 5)
	{
		scatterSurface(m, Surface::SLUDGE, n->x + 1, n->y + 1, n->w - 2, n->h - 2, 0.3);
	}
}


//	Recursive maze-carving algorithm.
void mapgen::mazecarve(vector<vector<metanode*>>* grid, int cellx, int celly)
{
	//	Mark this cell as visited.
	(*grid)[cellx][celly]->visited = true;

	//	Pick random directions.
	vector<intpair> drs = { intpair(1, 0), intpair(-1, 0), intpair(0, 1), intpair(0, -1) };
	while (!drs.empty())
	{
		//	Pick a random direction.
		unsigned n = randrange(drs.size());
		int newx = cellx + drs[n].first;
		int newy = celly + drs[n].second;
		drs.erase(drs.begin() + n);

		//	Test whether the selected cell is valid & unvisited.
		if (newx >= 0 && newx < grid->size() && newy >= 0 && newy < grid->at(newx).size())
		{
			//	Test whether cell is unvisited
			if (!(*grid)[newx][newy]->visited)
			{
				//	Carve through and work from this cell.
				linkNodes((*grid)[cellx][celly], (*grid)[newx][newy], intpair(cellx, celly), intpair(newx, newy));
				mazecarve(grid, newx, newy);
			}
		}
	}
}


//	Marks the two nodes as connected.
void mapgen::linkNodes(metanode* n1, metanode* n2, const intpair cell1, const intpair cell2)
{
	if (cell1.first < cell2.first)
	{
		n1->east = true;
		n2->west = true;
	}
	else if (cell1.first > cell2.first)
	{
		n1->west = true;
		n2->east = true;
	}
	if (cell1.second < cell2.second)
	{
		n1->south = true;
		n2->north = true;
	}
	else if (cell1.second > cell2.second)
	{
		n1->north = true;
		n2->south = true;
	}
}


//	THE CATHEDRAL MAP
//	This generates a new Cathedral map.
gridmapPtr mapgen::generate_Cathedral(int dl, bool descending, bool add_monsters)
{
	int grid_size = 12;
	int gridx = randint(3, 4), gridy = randint(3, 4);
	auto m = gridmapPtr(new gridmap(2 + grid_size * gridx, 2 + grid_size * gridy));
	fillMap(m, { MT_WALL_STONE });
	m->_name = "Cathedral [Depth " + to_string(dl) + "]";

	//	Make a 2d node map.
	vector<vector<metanode*>> nodes;
	vector<metanode*> nodes_linear;
	for (unsigned x = 1; x < m->_xsize - 2; x += grid_size)
	{
		vector<metanode*> xnodes;
		for (unsigned y = 1; y < m->_ysize - 2; y += grid_size)
		{
			//	Generate the node
			auto n = new metanode(x, y, grid_size - 1, grid_size - 1);
			xnodes.push_back(n);
			nodes_linear.push_back(n);
		}
		nodes.push_back(xnodes);
	}

	//	Generate a maze from the nodes
	mazecarve(&nodes, 0, 0);

	//	Create the nodes and their connections
	vector<TCODBsp*> tcod_nodes;
	for (auto n : nodes_linear)
	{
		int r = randint(1, 10);
		if (r <= 1)
			cathedral_Chapel(m, n, dl);
		else if (r <= 2)
			cathedral_Tombs(m, n, dl);
		else if (r <= 3)
			cathedral_Pool(m, n, dl);
		else if (r <= 4)
			cathedral_Storage(m, n, dl);
		else
			cathedral_Hallway(m, n, dl);
		tcod_nodes.push_back(new TCODBsp(n->x, n->y, n->w, n->h));
	}

	//	Bigger messes at lower depth
	if (dl >= 6)
	{
		int lcount = randint(1, 4);
		while (lcount-- > 0)
		{
			int r = randint(4, 8);
			addLake(m, getRandomWithPad(m, r + 1), r, MT_WATER, MT_FLOOR_STONE);
		}
	}

	//	Chaos
	float density = 0.05;
	if		(dl >= 6)	density = 0.20f;
	else if (dl >= 4)	density = 0.15f;
	else if (dl >= 2)	density = 0.10f;
	scatterOnMap(m, MT_BUSH, density);
	scatterOnMap(m, MT_GRASS, density);
	scatterOnMap(m, MT_WATER, density);

	//	Treasures
	int chest_count = randint(1, 5);
	while (chest_count-- > 0)
	{
		//	roll type
		auto ctype = MT_CHEST_SMALL;
		int r = randint(1, 100);
		if (r <= 30 && dl > 4)	ctype = MT_CHEST_GLOWING;

		//	emplace it
		m->setTile(ctype, getRandomWalkable(m));
	}

	//	Monsters, exits, entrances
	if (add_monsters)
	{
		addMonsters(m, dl, &tcod_nodes);
		addStairsToMap(m, dl, descending);
	}

	return m;
}


//	The SECOND TIER of horror levels
gridmapPtr mapgen::generate_Hellmouth(int dl, bool descending, bool add_monsters)
{
	const int MIN_NODE_SIZE = 10;
	auto m = gridmapPtr(new gridmap(2 + MIN_NODE_SIZE * randint(4, 6), 2 + MIN_NODE_SIZE * randint(4, 6)));
	fillMap(m, { MT_FLOOR_HOT, MT_FLOOR_HOT, MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_SAND, });
	m->_name = "Hellmouth [Depth " + to_string(dl) + "]";

	
	//	AUTOMATA
	auto cells = rollCellAutomata(m->_xsize, m->_ysize, 2, 5, 3, 8);
	for (unsigned x = 0; x < m->_xsize; x++)
	{
		for (unsigned y = 0; y < m->_ysize; y++)
		{
			if (cells[x][y])
				m->setTile(MT_WALL_BLOODY, x, y);
		}
	}


	//	Lakes of FIRE
	auto count = randint(1, 3);
	while (count-- > 0)
	{
		const int r = randint(3, 8);
		auto ctr = getRandomWithPad(m, r + 1);
		addLake(m, ctr, r, MT_LAVA, MT_FLOOR_HOT);
	}


	//	Chaos
	scatterOnMap(m, MT_TOMBSTONE, 0.05);
	scatterSurface(m, Surface::CORPSE, 2, 2, m->_xsize - 4, m->_ysize - 4, 0.05);
	scatterSurface(m, Surface::BONES, 2, 2, m->_xsize - 4, m->_ysize - 4, 0.05);


	//	More chaos
	if (roll_one_in(6))
		scatterOnMap(m, MT_BUSH, 0.05);
	if (roll_one_in(6))
		scatterOnMap(m, MT_WATER, 0.05);


	//	Treasures
	int chest_count = randint(1, 5);
	while (chest_count-- > 0)
	{
		//	roll type
		auto ctype = MT_CHEST_SMALL;
		int r = randint(1, 100);
		if		(r <= 50)	ctype = MT_CHEST_GLOWING;
		else if (r <= 75)	ctype = MT_CHEST_LUMINOUS;

		//	emplace it
		m->setTile(ctype, getRandomWalkable(m));
	}

	//	Divide into nodes for monster-placing purposes
	auto tcod_nodes = createNodeMap(m, MIN_NODE_SIZE, 10);

	//	Monsters, exits, entrances
	if (add_monsters)
	{
		addMonsters(m, dl, &tcod_nodes);
		addStairsToMap(m, dl, descending);
	}

	return m;
}


//	A more complex hellmap. We start with the basic template, and then add more stuff.
gridmapPtr mapgen::generate_HellDepths(int dl, bool descending)
{
	const int MIN_NODE_SIZE = 15;
	auto m = gridmapPtr(new gridmap(2 + MIN_NODE_SIZE * randint(2, 4), 2 + MIN_NODE_SIZE * randint(2, 4)));
	fillMap(m, { MT_FLOOR_HOT, MT_FLOOR_HOT, MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_SAND, });
	m->_name = "Deep Hells [Depth " + to_string(dl) + "]";


	//	AUTOMATA
	auto cells = rollCellAutomata(m->_xsize, m->_ysize, 2, 5, 3, 8);
	for (unsigned x = 0; x < m->_xsize; x++)
	{
		for (unsigned y = 0; y < m->_ysize; y++)
		{
			if (cells[x][y])
				m->setTile(MT_WALL_BLOODY, x, y);
		}
	}


	//	ADDITIONAL FEATURES
	auto nodes = createNodeMap(m, MIN_NODE_SIZE, 5);
	vector<monsterPtr> mgroup;
	for (auto n : nodes)
	{
		int r = randint(1, 6);
		switch (r)
		{
		case(1):	hell_MoltenLake(m, n, dl); break;
		case(2):	hell_SpiderNest(m, n, dl); break;
		case(3):	hell_Tomb(m, n, dl); break;
		case(4):	hell_VampireCourt(m, n, dl); break;
		default:
			//	add wretches to existing map
			mgroup = rollMonsterGroup(dl, monsterdata::rollMonsterForLevel(11));
			addMonsterGroupToNode(m, &mgroup, n);
			break;
		}
	}

	//	Treasures
	int chest_count = randint(1, 5);
	while (chest_count-- > 0)
	{
		//	roll type
		auto ctype = MT_CHEST_SMALL;
		int r = randint(1, 100);
		if (r <= 50)	ctype = MT_CHEST_GLOWING;
		else if (r <= 75)	ctype = MT_CHEST_LUMINOUS;

		//	emplace it
		m->setTile(ctype, getRandomWalkable(m));
	}

	addStairsToMap(m, dl, descending);
	return m;
}


void mapgen::hell_MoltenLake(gridmapPtr m, TCODBsp* n, int dl)
{
	//	Flames.
	fillRegion(m, { MT_LAVA, MT_LAVA, MT_LAVA, MT_SAND }, n->x, n->y, n->w, n->h);

	//	Generate islands.
	int isles = randint(1, 3);
	while (isles-- > 0)
	{
		const int r = randint(4, 9);
		const intpair ctr = getRandomInBounds(m, n->x + 1, n->y + 1, n->w - 2, n->h - 2);
		addLake(m, ctr, r, MT_SAND, MT_FLOOR_HOT);
		scatterTile(m, MT_TOMBSTONE, ctr.first - r / 2, ctr.second - r / 2, r, r, 0.1);
	}

	//	Scatter some chunks of floor.
	isles = randint(2, 4);
	while (isles-- > 0)
		walkingTile(m, getRandomInBounds(m, n->x + 1, n->y + 1, n->w - 2, n->h - 2), randint(30, 50), MT_FLOOR_HOT);

	//	Monsters
	auto mlist = rollMonsterGroup(dl, MonsterType::IMP);
	addMonsterGroupToNode(m, &mlist, n);
	if (roll_one_in(2))
		m->addCreature(monsterdata::generate(MonsterType::DEMON_PRINCE, dl * 2 + 1), getFreeINode(m, n));
}


void mapgen::hell_SpiderNest(gridmapPtr m, TCODBsp* n, int dl)
{
	//	Area
	scatterSurface(m, Surface::WEB, n->x, n->y, n->w, n->h, 0.1);
	scatterTile(m, MT_BUSH, n->x, n->y, n->w, n->h, 0.1);

	//	Monsters
	auto mlist = rollMonsterGroup(dl, MonsterType::SPIDER);
	addMonsterGroupToNode(m, &mlist, n);
	if (roll_one_in(2))
		m->addCreature(monsterdata::generate(MonsterType::SPIDER_TITAN, dl * 2 + 1), getFreeINode(m, n));
}


void mapgen::hell_Tomb(gridmapPtr m, TCODBsp* n, int dl)
{
	//	TONS of sarcophagi
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, n->x + 1, n->y + 1, n->w - 3, n->h - 3);
	for (unsigned x = n->x + 2; x <= n->x + n->w - 4; x += 2)
	{
		for (unsigned y = n->y + 2; y <= n->y + n->h - 4; y += 2)
			m->setTile(MT_SARCOPHAGUS, x, y);
	}

	//	some chaos
	scatterTile(m, MT_TOMBSTONE, n->x + 1, n->y + 1, n->w - 3, n->h - 3, 0.05);
	scatterTile(m, MT_BUSH, n->x + 1, n->y + 1, n->w - 3, n->h - 3, 0.1);

	//	Monsters
	auto mlist = rollMonsterGroup(dl, MonsterType::WRAITH);
	addMonsterGroupToNode(m, &mlist, n);
}

void mapgen::hell_VampireCourt(gridmapPtr m, TCODBsp* n, int dl)
{
	//	main setup
	const int sz = 10;
	vector<TCODBsp*> nodes;
	for (unsigned x = n->x + 1; x < n->x + n->w - sz; x += sz)
	{
		for (unsigned y = n->y + 1; y < n->y + n->h - sz; y += sz)
		{
			//	walls
			fillRegion(m, MT_WALL_STONE, x, y, sz, sz);

			//	interior
			fillRegion(m, MT_FLOOR_STONE, x + 1, y + 1, sz - 2, sz - 2);
			fillRegion(m, MT_FLOOR_CARPET, x + 2, y + 2, sz - 4, sz - 4);
			m->setTile(MT_STATUE_MARBLE, x + 2, y + sz - 3);
			m->setTile(MT_STATUE_MARBLE, x + 2, y + 2);
			m->setTile(MT_STATUE_MARBLE, x + sz - 3, y + sz - 3);
			m->setTile(MT_STATUE_MARBLE, x + sz - 3, y + 2);

			//	connections
			fillRegion(m, MT_FLOOR_STONE, x + sz / 2 - 1, y, 2, 1);
			fillRegion(m, MT_FLOOR_STONE, x + sz / 2 - 1, y + sz - 1, 2, 1);
			fillRegion(m, MT_FLOOR_STONE, x, y + sz / 2 - 1, 1, 2);
			fillRegion(m, MT_FLOOR_STONE, x + sz - 1, y + sz / 2 - 1, 1, 2);

			//	for monsters
			nodes.push_back(new TCODBsp(x + 1, y + 1, sz - 2, sz - 2));
		}
	}

	//	Monsters
	auto mlist = rollMonsterGroup(dl, MonsterType::VAMPIRE_SPAWN);
	addMonsterGroupToNode(m, &mlist, n);
	if (roll_one_in(2))
		m->addCreature(monsterdata::generate(MonsterType::VAMPIRE_PRINCE, dl * 2 + 1), getFreeINode(m, n));
}




//	Add treasure to an abyssal map. These have the best types.
void mapgen::addAbyssTreasures(gridmapPtr m)
{
	int count = randint(1, 3);
	while (count-- > 0)
		m->setTile(MT_CHEST_LUMINOUS, getRandomWalkable(m));
	if (roll_one_in(2))
		m->setTile(MT_CHEST_RADIANT, getRandomWalkable(m));
}

//	Creates a node map and spews monsters.
void mapgen::addAbyssMonsters(gridmapPtr m, const vector<MonsterType> mtypes, const int dl)
{
	auto nodes = createNodeMap(m);
	auto count = nodes.size() / 2 + 1;
	while (count-- > 0)
	{
		unsigned n = randrange(nodes.size());
		auto mlist = rollMonsterGroup(dl, mtypes[randrange(mtypes.size())]);
		addMonsterGroupToNode(m, &mlist, nodes[n]);
		nodes.erase(nodes.begin() + n);
	}
}

//	COURT OF THE DROWNED GOD
gridmapPtr mapgen::generate_DrownedCourt(int abyss_lvl)
{
	auto m = gridmapPtr(new gridmap(randint(30, 60), randint(30, 60)));
	fillMap(m, { MT_WATER, MT_WATER, MT_BUSH, MT_GRASS, MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_FLOOR_CARPET });
	m->_name = "The Drowned Deep [Level " + to_string(abyss_lvl) + "]";

	const int dlvl = ABYSS_LEVEL_BASE + abyss_lvl;

	for (unsigned x = 3; x < m->_xsize - 4; x += 3)
	{
		for (unsigned y = 3; y <= m->_ysize - 4; y += 3)
		{
			const int r = randint(1, 3);
			if		(r == 1)	m->setTile(MT_WALL_STONE, x, y);
			else if (r == 2)	m->setTile(MT_WALL_BLOODY, x, y);
		}
	}
	scatterSurface(m, Surface::SLUDGE, 0, 0, m->_xsize - 1, m->_ysize - 1, 0.1);

	addAbyssMonsters(m, { MonsterType::CULTIST_DOGGOSAN, MonsterType::TENTACLE }, dlvl);
	auto boss = monsterdata::generate(MonsterType::BOSS_DROWNED_DOGOSSA, dlvl * 2 + 1);
	m->addCreature(boss, getRandomFree(m));

	addAbyssTreasures(m);
	return m;
}


//	FLESH OF AMOG
gridmapPtr mapgen::generate_AmogTomb(int abyss_lvl)
{
	auto m = gridmapPtr(new gridmap(randint(30, 60), randint(30, 60)));
	fillMap(m, { MT_WALL_FLESH });
	m->_name = "Flesh of Amog [Level " + to_string(abyss_lvl) + "]";
	const int dlvl = ABYSS_LEVEL_BASE + abyss_lvl;

	const vector<Maptile> tiles = { MT_BLOOD_POOL, MT_BLOOD_POOL, MT_FLOOR_STONE, MT_FLOOR_STONE2 };

	//	carve out floors
	vector<intpair> pts;
	auto at = intpair(m->_xsize / 2, m->_ysize / 2);
	m->setTile(MT_BLOOD_POOL, at);
	pts.push_back(at);
	int count = 1000;
	while (count-- > 0 && !pts.empty())
	{
		//	get adjacent points that are walls
		auto adj = getAdjacentOfType(m, at, MT_WALL_FLESH);

		//	if there are no adjacent walls, back up to the previous point.
		if (adj.empty())
		{
			at = pts.back();
			pts.pop_back();
		}

		//	otherwise, pick one at random and clear it.
		else
		{
			at = adj[randrange(adj.size())];
			m->setTile(tiles[randrange(tiles.size())], at);
			pts.push_back(at);
		}
	}

	scatterTileOnWalkable(m, MT_TOMBSTONE, 0, 0, m->_xsize - 1, m->_ysize - 1, 0.05);

	addAbyssMonsters(m, { MonsterType::BLOOD_BLOB, MonsterType::BONES_BLOODY, MonsterType::DEMON_PUTRESCENT, }, dlvl);
	auto boss = monsterdata::generate(MonsterType::BOSS_TOMB_LORD_AMOG, dlvl * 2 + 1);
	m->addCreature(boss, getRandomFree(m));

	addAbyssTreasures(m);
	return m;
}


//	LOST VIRIDIA
gridmapPtr mapgen::generate_Viridia(int abyss_lvl)
{
	auto m = gridmapPtr(new gridmap(randint(30, 60), randint(30, 60)));
	fillMap(m, { MT_WATER, MT_WATER, MT_WATER, MT_FLOOR_VOID });
	m->_name = "Lost Viridia [Level " + to_string(abyss_lvl) + "]";
	const int dlvl = ABYSS_LEVEL_BASE + abyss_lvl;

	auto nodes = createNodeMap(m);
	for (auto n : nodes)
	{
		const int r = randint(1, 4);
		if (r == 1)
		{
			scatterTile(m, MT_TREE_DEAD, n->x, n->y, n->w, n->h, 0.05);
			scatterTile(m, MT_GRASS, n->x, n->y, n->w, n->h, 0.05);
			scatterTile(m, MT_STATUE_MARBLE, n->x, n->y, n->w, n->h, 0.03);
		}
		else if (r == 2)
		{
			fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_BUSH, MT_GRASS }, n->x + 2, n->y + 2, n->w - 4, n->h - 4);
		}
		else if (r != 4)
		{
			unsigned cx = n->x + n->w / 2;
			unsigned cy = n->y + n->h / 2;
			
			fillRegion(m, MT_WALL_ICE, n->x, n->y, n->w, n->h);
			fillRegion(m, MT_FLOOR_VOID, n->x + 2, n->y + 2, n->w - 3, n->h - 3);
			
			const int w = 3;
			fillRegion(m, MT_FLOOR_CARPET, cx - 1, n->y, w, n->h);
			fillRegion(m, MT_FLOOR_CARPET, n->x, cy - 1, n->w, w);
			fillRegion(m, MT_FLOOR_CARPET, cx - 2, cy - 2, 5, 5);
		}
	}

	addAbyssMonsters(m, { MonsterType::CRAB_TITAN, MonsterType::PALE_KNIGHT, MonsterType::PALE_SCHOLAR, }, dlvl);
	auto boss = monsterdata::generate(MonsterType::BOSS_VIRIDIAN_PRINCE, dlvl * 2 + 1);
	m->addCreature(boss, getRandomFree(m));

	addAbyssTreasures(m);
	return m;
}


//	Weird void maps
gridmapPtr mapgen::generate_OuterDark(int abyss_lvl, bool descending)
{
	auto m = gridmapPtr(new gridmap(randint(30, 60), randint(30, 60)));
	fillMap(m, { MT_FLOOR_VOID });
	scatterOnMap(m, MT_WALL_ICE, 0.05);
	scatterOnMap(m, MT_WATER, 0.05);
	m->_name = "The Outer Dark [Level " + to_string(abyss_lvl) + "]";

	const int dlvl = ABYSS_LEVEL_BASE + abyss_lvl;

	//	Monsters
	addAbyssMonsters(m, { MonsterType::ABYSSAL_WRAITH, MonsterType::NIGHTGAUNT, MonsterType::STARSPAWN, MonsterType::STAR_VAMPIRE, }, dlvl);

	auto boss = monsterdata::generate_AbyssLord(dlvl * 2 + 1);
	m->addCreature(boss, getRandomFree(m));

	//	special treasures
	addAbyssTreasures(m);
	return m;
}


//	Final zone, with the final boss
gridmapPtr mapgen::generate_AbyssHeart(game_progress* progress)
{
	auto m = gridmapPtr(new gridmap(30, 30));
	fillMap(m, { MT_FLOOR_VOID });
	scatterOnMap(m, MT_WALL_ICE, 0.05);
	scatterOnMap(m, MT_BUSH, 0.05);
	scatterOnMap(m, MT_LAVA, 0.05);
	m->_name = "HEART OF THE ABYSS";

	auto boss = monsterdata::generate(MonsterType::BOSS_VILE_COLOSSUS, 40 + 5 * progress->_killedColossus);
	m->addCreature(boss, 15, 15);

	auto n = new TCODBsp(10, 10, 10, 10);
	vector<MonsterType> mtypes = { MonsterType::KNIGHT_DREAD, MonsterType::VILESPAWN };
	for (auto mt : mtypes)
	{
		auto mlist = rollMonsterGroup(20, mt);
		addMonsterGroupToNode(m, &mlist, n);
	}

	return m;
}


//	Special lair of a super-boss.
gridmapPtr mapgen::generate_ViledragonLair(game_progress* progress)
{
	auto m = gridmapPtr(new gridmap(31, 31));
	m->_name = "Viledragon Lair [Level " + to_string(progress->_killedViledragons) + "]";

	//	basic map layout
	fillMap(m, { MT_WALL_BLOODY, MT_WALL_STONE });
	fillRegionCircular(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_FLOOR_HOT }, 15, 15, 13);
	for (unsigned x = 5; x <= 25; x += 2)
	{
		for (unsigned y = 5; y <= 25; y+= 2)
		{
			if (roll_one_in(2))
				m->setTile(MT_WALL_STONE, x, y);
		}
	}

	//	additional stuff
	const int r = randint(1, 3);
	switch (r)
	{
	case(1):
		scatterTile(m, MT_GRASS, 5, 5, 20, 20, 0.1);
		scatterTile(m, MT_BUSH, 5, 5, 20, 20, 0.1);
		scatterTile(m, MT_WATER, 5, 5, 20, 20, 0.1);
		scatterSurface(m, Surface::SLUDGE, 5, 5, 20, 20, 0.05);
		break;

	case(2):
		m->_lightLevel = -2;
		scatterTile(m, MT_TOMBSTONE, 5, 5, 20, 20, 0.05);
		scatterTile(m, MT_BUSH, 5, 5, 20, 20, 0.1);
		break;

	case(3):
		scatterTile(m, MT_WALL_ICE, 5, 5, 20, 20, 0.05);
		scatterTile(m, MT_WATER, 5, 5, 20, 20, 0.1);
		break;
	}


	//	boss
	auto mlist = monsterdata::generate_ViledragonGang(40 + progress->_killedViledragons * 2);
	auto n = new TCODBsp(10, 10, 10, 10);
	addMonsterGroupToNode(m, &mlist, n);


	//	finalization
	m->_startPt = getRandomSafe(m);
	m->updateTmap();
	return m;
}



//	BOSS MAP: The Pallid Rotking
gridmapPtr mapgen::generate_PallidRotking(int dl, bool descending, int killcount)
{
	auto m = generate_Cathedral(1, descending, false);
	m->_name = "THE ROTTEN THRONE";
	const int r = MIN(m->_xsize / 3, m->_ysize / 3);
	const intpair ctr = intpair(m->_xsize / 2, m->_ysize / 2);

	//	Mess up the map.
	addLake(m, ctr, r, MT_WATER, MT_FLOOR_CARPET);
	scatterOnMap(m, MT_BUSH, 0.3);
	scatterOnMap(m, MT_BARREL, 0.05);
	scatterSurface(m, Surface::SLUDGE, 1, 1, m->_xsize - 2, m->_ysize - 2, 0.3);

	//	Pick a boss.
	vector<MonsterType> options;
	if (killcount == 0)
		options = { MonsterType::BOSS_PALLID_ROTKING };
	else
	{
		options = { MonsterType::BOSS_ROTKING_BURNED, MonsterType::BOSS_ROTKING_INFUSED };
		if (killcount > 2)
			options.push_back(MonsterType::BOSS_ROTKING_DESOLATE);
	}
	m->addCreature(monsterdata::generate(options[randrange(options.size())], dl * 2 + MIN(5, killcount)), ctr);

	//	Add additional monsters.
	auto node = new TCODBsp(ctr.first - 3, ctr.second - 3, 6, 6);
	auto mtable = rollMonsterGroup(dl, MonsterType::CULTIST);
	addMonsterGroupToNode(m, &mtable, node);

	//	Add downstairs.
	auto spt = getRandomForStairs(m);
	m->setTile(MT_STAIRS_UP, spt);
	m->_startPt = spt;

	return m;
}


gridmapPtr mapgen::generate_Hellfort(int dl, bool descending, int killcount)
{
	auto m = gridmapPtr(new gridmap(30, 30));
	fillMap(m, { MT_FLOOR_HOT, MT_FLOOR_HOT, MT_FLOOR_HOT, MT_FLOOR_STONE, MT_FLOOR_STONE2 });
	scatterTile(m, MT_LAVA, 1, 1, m->_xsize - 2, m->_ysize - 2, 0.05);
	m->_name = "MOUTH OF THE ABYSS";

	scatterOnMap(m, MT_WALL_BLOODY, 0.05);
	
	//	centre of map
	auto ctr = intpair(m->_xsize / 2, m->_ysize / 2);

	//	Pick a boss.
	vector<MonsterType> options = { MonsterType::BOSS_DEMON_LORD, MonsterType::BOSS_WRETCHED_PRINCE, };
	m->addCreature(monsterdata::generate(options[randrange(options.size())], dl * 2 + MIN(5, killcount)), ctr);

	//	Add additional monsters.
	auto node = new TCODBsp(ctr.first - 3, ctr.second - 3, 6, 6);
	auto mtable = rollMonsterGroup(dl, MonsterType::CULTIST);
	addMonsterGroupToNode(m, &mtable, node);

	//	Random start position.
	m->_startPt = getRandomFree(m);

	return m;
}


//	Randomly rolls one dimensional axis for a map. Larger maps are increasingly unlikely.
int mapgen::rollMapDimension()
{
	int sz = 40;
	while (roll_percent(130 - sz))
		sz += 5;
	return sz;
}


//	Creates an abyssal map. Different types of maps are created depending on the ritual type selected.
gridmapPtr mapgen::generate_Abyssal(int abyss_lvl, game_progress* progress, MaterialType _ritualType)
{
	gridmapPtr m;

	//	Ritual type determines map generator.
	switch (_ritualType)
	{
	case(MaterialType::SODDEN_FLESH):	m = generate_DrownedCourt(abyss_lvl); break;
	case(MaterialType::TOMB_IDOL):		m = generate_AmogTomb(abyss_lvl); break;
	case(MaterialType::VIRIDIAN_GLASS):	m = generate_Viridia(abyss_lvl); break;

	case(MaterialType::DEAD_GODS_EYE):	m = generate_AbyssHeart(progress); break;

	default:
		m = generate_OuterDark(abyss_lvl, true);
	}

	m->_startPt = getRandomForStairs(m);
	m->updateTmap();
	return m;
}


//	Generate a random map for the given danger level.
//	The 'descending' flag determines whether start position is set to the upstairs or downstairs.
gridmapPtr mapgen::generate(int dl, game_progress* progress, bool descending)
{
	gridmapPtr m;
	
	//	BOSS MAPS
	if (dl == 9)
		m = generate_PallidRotking(dl, descending, progress->_killedRotking);
	else if (dl == 15)
		m = generate_Hellfort(dl, descending, progress->_killedRotking);

	//	ABYSS
	else if (dl > 15)
		m = generate_OuterDark(dl, descending);

	//	HELL
	else if (dl >= 12)
		m = generate_HellDepths(dl, descending);

	//	Entrance to HELL
	else if (dl > 9)
		m = generate_Hellmouth(dl, descending);

	//	Cathedral maps from the early game
	else
		//m = generate_Abyssal(0, MaterialType::VIRIDIAN_GLASS);
		m = generate_Cathedral(dl, descending);


	//	Map exterior is always filled with indestructible walls.
	for (unsigned x = 0; x < m->_xsize; x++)
	{
		m->setTile(MT_WALL_SUPER, x, 0);
		m->setTile(MT_WALL_SUPER, x, m->_ysize - 1);
	}
	for (unsigned y = 0; y < m->_ysize; y++)
	{
		m->setTile(MT_WALL_SUPER, 0, y);
		m->setTile(MT_WALL_SUPER, m->_xsize - 1, y);
	}

	m->updateTmap();
	return m;
}


//	Adds the hell-portal to the home map.
void mapgen::openHellPortal(gridmapPtr m)
{
	scatterSurface(m, Surface::BONES, 16, 16, 4, 4, 0.8);
	scatterSurface(m, Surface::CORPSE, 16, 16, 4, 4, 0.8);
	m->setSurface(Surface::__NONE, 18, 18);
	m->setTile(MT_HELLPORTAL_DOWN, 18, 18);
}


//	Second portal, connecting us to the bottom of hell.
void mapgen::openTemplePortal(gridmapPtr m)
{
	fillRegionCircular(m, { MT_FLOOR_HOT, MT_FLOOR_HOT, MT_FLOOR_HOT, MT_RUBBLE, MT_FLOOR_STONE, MT_FLOOR_STONE2 }, 24, 27, 4);
	fillRegionCircular(m, MT_LAVA, 32, 35, 2);
	fillRegionCircular(m, MT_LAVA, 22, 33, 3);
	fillRegionCircular(m, MT_LAVA, 31, 24, 2);

	m->setTile(MT_DEMONFORGE, 25, 24);
	m->setTile(MT_ALCHEMY, 27, 26);

	m->updateTmap();
}


//	Our MAIN MAP with all our stuff.
gridmapPtr mapgen::generate_HomeBase()
{
	auto m = gridmapPtr(new gridmap(40, 45));
	m->_name = "Ruined Chapel, Aboveground";
	fillMap(m, { MT_GRASS, MT_GRASS, MT_GRASS, MT_GRASS, MT_GRASS, MT_BUSH });
	scatterOnMap(m, MT_TREE_DEAD, 0.05);

	addLake(m, intpair(33, 5), 4, MT_WATER, MT_WATER);

	fillRegion(m, MT_WALL_STONE, 9, 4, 16, 24);
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, 10, 5, 14, 22);
	fillRegion(m, MT_WALL_STONE, 14, 27, 6, 6);
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2 }, 15, 28, 4, 4);

	fillRegion(m, MT_FLOOR_CARPET, 16, 8, 2, 24);
	scatterTile(m, MT_RUBBLE, 10, 5, 13, 21, 0.05);

	m->setTile(MT_DOOR_STONE, 16, 32);
	m->setTile(MT_DOOR_STONE, 17, 32);
	m->setTile(MT_DOOR_STONE, 24, 14);

	for (unsigned y = 11; y <= 25; y += 2)
	{
		for (unsigned x = 11; x <= 13; x++)
			m->setTile(MT_PEW_WOODEN, x, y);
		for (unsigned x = 20; x <= 22; x++)
			m->setTile(MT_PEW_WOODEN, x, y);
	}

	scatterTile(m, MT_BUSH, 10, 5, 14, 22, 0.05);

	fillRegion(m, MT_SAND, 16, 33, 2, 12);
	fillRegionCircular(m, MT_WATER, 12, 9, 1);

	//	features in main structure
	m->setTile(MT_HEALING_FOUNTAIN, 12, 9);
	m->setTile(MT_STASH, 13, 5);
	m->setTile(MT_ANVIL, 16, 6);
	m->setTile(MT_FORGE, 17, 7);
	//m->setTile(MT_ALCHEMY, 20, 9);
	m->setTile(MT_GEMSTONE_PRESS, 21, 7);
	m->setTile(MT_RUNE_IMPRINTER, 19, 5);

	m->setTile(MT_STAIRS_DOWN, 15, 15);
	m->_startPt = intpair(15, 15);

	//	save point
	m->setTile(MT_MEDITATION_SHRINE, 15, 10);

	//	additional stairs down
	fillRegion(m, MT_WALL_STONE, 28, 11, 7, 8);
	fillRegion(m, { MT_FLOOR_STONE, MT_FLOOR_STONE2, MT_BUSH }, 29, 12, 5, 6);
	m->setTile(MT_FLOOR_HOT, 28, 15);
	m->setTile(MT_STAIRS_DOWN_LONG, 31, 15);

	//	the hell-temple portal appears here later
	fillRegion(m, MT_WALL_BLOODY, 26, 29, 9, 9);
	fillRegion(m, MT_FLOOR_HOT, 27, 30, 7, 7);
	m->setTile(MT_TEMPLE_PORTAL, 29, 32);

	m->_lightLevel = 10;
	m->updateTmap();
	return m;
}

gridmapPtr mapgen::generate_HellTemple()
{
	auto m = gridmapPtr(new gridmap(22, 22));
	fillMap(m, { MT_FLOOR_HOT });
	m->_name = "Hell Temple";

	const vector<string> pattern = {
		".##............##.",
		"######..##..######",
		"####.........~####",
		".##~~.........~##.",
		".#~..........~~~#.",
		".#...B......B..~#.",
		"..................",
		"..................",
		".#..............#.",
		".#..............#.",
		"..................",
		"..................",
		".#...B......B...#.",
		".#~~..........~~#.",
		".##~~........~~##.",
		"####~........~####",
		"######..##..######",
		".##............##.",
	};
	for (unsigned px = 0; px < pattern.size(); px++)
	{
		for (unsigned py = 0; py < pattern[px].size(); py++)
		{
			const unsigned x = px + 2, y = py + 2;
			switch (pattern[px][py])
			{
			case('.'):	m->setTile(MT_FLOOR_HOT, x, y); break;
			case('#'):	m->setTile(MT_WALL_BLOODY, x, y); break;
			case('~'):	m->setTile(MT_LAVA, x, y); break;
			case('B'):	m->setTile(MT_STATUE_BLOODY, x, y); break;
			}
		}
	}

	//	Portal home
	m->setTile(MT_TEMPLE_PORTAL, 9, 12);
	m->_startPt = intpair(9, 12);


	//	DOWNWARD
	//m->setTile(MT_ABYSSAL_GATE, 11, 14);


	//	Other stuff
	m->setTile(MT_STASH, 8, 10);
	m->setTile(MT_DEMONFORGE, 10, 9);
	m->setTile(MT_ABYSSAL_ALTAR, 12, 16);


	//	Finish
	m->updateTmap();
	m->_lightLevel = 5;
	return m;
}
