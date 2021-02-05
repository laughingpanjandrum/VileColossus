#pragma once

#include <memory>
#include "Maptile.h"
#include "creature.h"
#include "item.h"


typedef shared_ptr<TCODMap> TCODMapPtr;



struct MapStairs
{
	MapStairs() {}
	MapStairs(string name, string id, intpair pos, int dl, int glyph) : _pos(pos), _name(name), _mapId(id), _dangerLevel(dl), _glyph(glyph) {}

	const intpair _pos;
	int _dangerLevel;
	string _name;
	int _glyph;
	string _mapId;
};
typedef shared_ptr<MapStairs> mapStairsPtr;




class gridmap
{
public:

	gridmap() : gridmap(30, 30) {}
	gridmap(const int xsize, const int ysize);
	~gridmap() {}

	string getName() const { return _name; }


	//	Bounds testing

	bool inBounds(const int x, const int y) const { return x >= 0 && x < _xsize && y >= 0 && y < _ysize; }
	bool inBounds(const intpair pt) const { return inBounds(pt.first, pt.second); }


	//	Tiles & tile properties

	void setTile(Maptile tl, int x, int y) { _tiles[x][y] = tl; }
	void setTile(Maptile tl, const intpair pt) { setTile(tl, pt.first, pt.second); }

	Maptile getTile(const int x, const int y) const { return _tiles[x][y]; }
	Maptile getTile(const intpair pt) const { return getTile(pt.first, pt.second); }

	bool isWalkable(const int x, const int y) const { return isMaptileWalkable(getTile(x, y)); }
	bool isWalkable(const intpair pt) const { return isWalkable(pt.first, pt.second); }

	bool isTransparent(const int x, const int y) const { return isMaptileTransparent(getTile(x, y)); }
	bool isTransparent(const intpair pt) const { return isTransparent(pt.first, pt.second); }

	bool isPointClear(const int x, const int y) const { return isWalkable(x, y) && getCreature(x, y) == nullptr; }
	bool isPointClear(const intpair pt) const { return isWalkable(pt) && getCreature(pt) == nullptr; }


	//	Surfaces

	void setSurface(const Surface sf, const int x, const int y) { _surfaces[x][y] = sf; }
	void setSurface(const Surface sf, const intpair pt) { setSurface(sf, pt.first, pt.second); }

	const Surface getSurface(const int x, const int y) const { return _surfaces[x][y]; }
	const Surface getSurface(const intpair pt) const { return getSurface(pt.first, pt.second); }

	bool canCoverWithSurface(const int x, const int y) const;



	//	Creatures

	void addCreature(creaturePtr cr, const intpair pt);
	void addCreature(creaturePtr cr, int x, int y) { addCreature(cr, intpair(x, y)); }

	void removeCreature(creaturePtr cr);

	const vector<creaturePtr> getAllCreatures() const { return _creatures; }

	creaturePtr getCreature(const intpair pt) const;
	creaturePtr getCreature(const int x, const int y) const { return getCreature(intpair(x, y)); }


	//	Items

	void addItem(itemPtr it, const intpair pt);
	void addItem(itemPtr it, const int x, const int y) { addItem(it, intpair(x, y)); }

	void removeItemFromContainer(const itemPtr it, itemContainerPtr icp);
	void removeItem(itemPtr it, const intpair pt);
	void removeItem(itemPtr it, const int x, const int y) { removeItem(it, intpair(x, y)); }

	itemContainerPtr getItemContainer(const intpair pt) const;
	itemContainerPtr getItemContainer(int x, int y) const { return getItemContainer(intpair(x, y)); }

	bool areItemsHere(int x, int y) const;
	const vector<itemContainerPtr>* getAllItems() const { return &_items; }



	//	Map used for fov/pathfinding

	void setFOVMapCentre(const intpair ctr);

	bool isInFov(const int x, const int y) const { return _tmap->isInFov(x, y); }
	bool isInFov(const intpair pt) const { return isInFov(pt.first, pt.second); }

	void updateTmapAtPoint(const int x, const int y);
	void updateTmap();


	//	Memory map

	bool inMemoryMap(const int x, const int y) const { return _memoryMap[x][y]; }
	void addToMemoryMap(const int x, const int y) { _memoryMap[x][y] = true; }


	//	Passage of time

	void tickSurfaces();


	//	Public data

	intpair _startPt;

	string _name;
	string _maptype;
	int _xsize, _ysize;

	int _lightLevel;


private:

	vector<vector<Maptile>> _tiles;
	vector<vector<Surface>> _surfaces;
	vector<vector<bool>> _memoryMap;

	vector<creaturePtr> _creatures;
	vector<itemContainerPtr> _items;

	TCODMapPtr _tmap;

};
typedef shared_ptr<gridmap> gridmapPtr;