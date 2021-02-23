#include "gridmap.h"



gridmap::gridmap(const int xsize, const int ysize) :
	_xsize(xsize), _ysize(ysize)
{
	_lightLevel = 0;	//	adjustment to vision radius

	//	empty lists of tiles/surfaces/etc
	for (unsigned x = 0; x < _xsize; x++)
	{
		vector<Maptile> xtiles;
		vector<Surface> xsurf;
		vector<bool> xmem;
		for (unsigned y = 0; y < _ysize; y++)
		{
			xtiles.push_back(MT_FLOOR_STONE);
			xsurf.push_back(Surface::__NONE);
			xmem.push_back(false);
		}
		_tiles.push_back(xtiles);
		_surfaces.push_back(xsurf);
		_memoryMap.push_back(xmem);
	}

	//	this map is used for fov/pathfinding
	_tmap = TCODMapPtr(new TCODMap(_xsize, _ysize));
}


//	Returns True if it is valid to put a surface here.
//	We don't put surfaces on stairs, doors, or non-walkable points.
bool gridmap::canCoverWithSurface(const int x, const int y) const
{
	return isTransparent(x, y) && isWalkable(x, y) && !isMaptileStairs(_tiles[x][y]) && _tiles[x][y] != MT_LAVA;
}


void gridmap::addCreature(creaturePtr cr, const intpair pt)
{
	_creatures.push_back(cr);
	cr->_pos = pt;
}

void gridmap::removeCreature(creaturePtr cr)
{
	auto f = find(_creatures.begin(), _creatures.end(), cr);
	if (f != _creatures.end())
		_creatures.erase(f);
}

//	Returns the creature at the given point, if any.
//	May have to cycle through EVERY creature, so don't call it a lot!
creaturePtr gridmap::getCreature(const intpair pt) const
{
	for (auto cr : _creatures)
	{
		if (cr->_pos == pt)
			return cr;
	}
	return nullptr;
}


//	Drop an item on the given point. Adds to an existing container, or creates a container if there isn't one.
void gridmap::addItem(itemPtr it, const intpair pt)
{
	auto icp = getItemContainer(pt);
	if (icp == nullptr)
	{
		icp = itemContainerPtr(new itemContainer(pt));
		_items.push_back(icp);
	}
	icp->addItem(it);
}


//	Delete an item from the given container. If the container is empty, also deletes the container from the map.
void gridmap::removeItemFromContainer(const itemPtr it, itemContainerPtr icp)
{
	//	delete item from container
	auto f = find(icp->_items.begin(), icp->_items.end(), it);
	if (f != icp->_items.end())
	{
		icp->_items.erase(f);

		//	if the container is empty, delete it
		if (icp->_items.empty())
		{
			auto f2 = find(_items.begin(), _items.end(), icp);
			if (f2 != _items.end())
				_items.erase(f2);
		}
	}
}


//	Delete the given item from the given point.
//	If no such item exists at the given point, does nothing.
void gridmap::removeItem(itemPtr it, const intpair pt)
{
	auto icp = getItemContainer(pt);
	if (icp != nullptr)
		removeItemFromContainer(it, icp);
}


//	Returns the item container at the given coordinate, if any.
itemContainerPtr gridmap::getItemContainer(const intpair pt) const
{
	for (auto icp : _items)
	{
		if (icp->_pos == pt)
			return icp;
	}
	return nullptr;
}


//	Returns True if there is an item container here.
bool gridmap::areItemsHere(int x, int y) const
{
	return getItemContainer(x, y) != nullptr;
}


void gridmap::setFOVMapCentre(const intpair ctr)
{
	_tmap->computeFov(ctr.first, ctr.second, 20);
}

void gridmap::updateTmapAtPoint(const int x, const int y)
{
	_tmap->setProperties(x, y, isTransparent(x, y), isWalkable(x, y));
}

void gridmap::updateTmap()
{
	for (unsigned x = 0; x < _xsize; x++)
	{
		for (unsigned y = 0; y < _ysize; y++)
			updateTmapAtPoint(x, y);
	}
}


//	Removes certain surfaces as time passes.
void gridmap::tickSurfaces()
{
	for (unsigned x = 0; x < _xsize; x++)
	{
		for (unsigned y = 0; y < _ysize; y++)
		{
			auto sf = getSurface(x, y);
			switch (sf)
			{
			case(Surface::FIRE):
			case(Surface::POISON_OOZE):
				if (roll_one_in(4))
					setSurface(Surface::__NONE, x, y);
				break;
			}
		}
	}
}

