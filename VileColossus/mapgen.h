#pragma once

#include "libtcod.hpp"
#include "utility.h"
#include "gridmap.h"
#include "lootgen.h"
#include "monsterdata.h"



namespace mapgen
{

	const int ABYSS_LEVEL_BASE = 18;


	struct game_progress
	{
		int _killedRotking = 0;
		int _killedHellboss = 0;
		
		int _killedDogossa = 0;
		int _killedAmog = 0;
		int _killedSallowKing = 0;
		int _killedColossus = 0;

		int _abyssLevel = 0;
	};

	struct metanode
	{
		metanode() {}
		metanode(int x, int y, int w, int h) :
			x(x), y(y), w(w), h(h), north(false), south(false), east(false), west(false), visited(false) {}

		bool visited;

		//	position on map
		int x, y, w, h;

		//	connections
		bool north, south, east, west;
	};



	//		Getting random points.

	intpair getRandomPoint(gridmapPtr m);
	intpair getRandomWalkable(gridmapPtr m);
	intpair getRandomInBounds(gridmapPtr m, int right_pad, int bottom_pad);
	intpair getRandomInBounds(gridmapPtr m, int x, int y, int w, int h);
	intpair getRandomFree(gridmapPtr m);
	intpair getRandomInCircle(gridmapPtr m, intpair ctr, int r);

	bool isSafeTile(const Maptile tl);
	intpair getRandomSafe(gridmapPtr m);

	intpair getRandomForItem(gridmapPtr m);
	intpair getRandomForStairs(gridmapPtr m);

	intpair getRandomWithPad(gridmapPtr m, int pad);
	intpair getFreeWithPad(gridmapPtr m, int pad);
	intpair getWalkableWithPad(gridmapPtr m, int pad);

	intpair getRandomInNode(const TCODBsp* node);
	intpair getRandomWalkableInNode(gridmapPtr m, TCODBsp* node);

	intpair getRandomOnNodeEdge(TCODBsp* node);
	intpair getRandomPointOnEdge(int x, int y, int w, int h);
	intpair getRandomFreeOnEdge(gridmapPtr m);

	intpair getFreeINode(gridmapPtr m, const TCODBsp* node);

	vector<intpair> getGroupOfFreePoints(gridmapPtr m, const intpair ctr, const int rad);
	vector<intpair> getAllFreeInNode(gridmapPtr m, const TCODBsp* node);


	//		Cellular automata

	const int countNeighbours(const vector<vector<bool>>* cmap, const int cx, const int cy);
	vector<vector<bool>> rollCellAutomata(const int x_sz, const int y_sz, const int minPop, const int maxPop, const int birth, int cycles, const int startFilled = 50);


	//		Fillers.

	void fillRegion(gridmapPtr m, vector<Maptile> tileOptions, int x, int y, int w, int h);
	void fillRegion(gridmapPtr m, Maptile t, int x, int y, int w, int h);
	void fillMap(gridmapPtr m, vector<Maptile> tileOptions);

	void fillRegionCircular(gridmapPtr m, vector<Maptile> tileOptions, int ctrx, int ctry, int rad);
	void fillRegionCircular(gridmapPtr m, Maptile tl, int ctrx, int ctry, int rad);
	void fillRegionAndHollow(gridmapPtr m, Maptile outer, vector<Maptile> inner, int x, int y, int w, int h);



	//		Scatterers.

	void scatterTileOnWalkable(gridmapPtr m, Maptile tl, int x, int y, int w, int h, float density);
	void scatterTile(gridmapPtr m, Maptile tl, int x, int y, int w, int h, float density);
	void scatterOnMap(gridmapPtr m, Maptile tl, float density);

	void scatterSurface(gridmapPtr m, const Surface sf, int x, int y, int w, int h, float density);


	//		BSP nodes.

	void getLeafNodesRecursive(vector<TCODBsp*>* nodes, TCODBsp* thisNode);
	vector<TCODBsp*> createNodeMap(gridmapPtr m, const int min_size = 10, const int recursions = 10);

	void tunnelBetweenPoints(gridmapPtr m, int midx1, int midy1, int midx2, int midy2, Maptile floor);
	void tunnelBetweenNodes(gridmapPtr m, TCODBsp* node1, TCODBsp* node2, Maptile floor);


	//		Special structures.

	void addBuilding(gridmapPtr m, int x, int y, int w, int h, Maptile wall, Maptile floor, Maptile door);
	void addLake(gridmapPtr m, intpair ctr, int r, Maptile inner, Maptile outer);
	void walkingTile(gridmapPtr m, const intpair start, int count, Maptile tl);

	vector<intpair> getAdjacentOfType(gridmapPtr m, const intpair ctr, const Maptile tl);


	//		Monsters.

	vector<monsterPtr> rollMonsterGroup(int dl, const MonsterType id);

	void addMonsterGroupToNode(gridmapPtr m, const vector<monsterPtr>* mlist, const TCODBsp* node);
	void addMonsters(gridmapPtr m, int dl, const vector<TCODBsp*>* nodeOptions);


	//		Connections.

	void addStairsToMap(gridmapPtr m, int depth, bool descending);


	//		Doors

	string countAdjacentWalls(gridmapPtr m, int x, int y);
	bool verifyPointOkayForDoor(gridmapPtr m, int x, int y);
	void addDoorsToMap(gridmapPtr m);


	//		Cathedral maps

	void cathedral_Chapel(gridmapPtr m, metanode* n, int dl);
	void cathedral_Hallway(gridmapPtr m, metanode* n, int dl);
	void cathedral_Pool(gridmapPtr m, metanode* n, int dl);
	void cathedral_Storage(gridmapPtr m, metanode* n, int dl);
	void cathedral_Tombs(gridmapPtr m, metanode* n, int dl);

	void mazecarve(vector<vector<metanode*>>* grid, int cellx, int celly);
	void linkNodes(metanode* n1, metanode* n2, const intpair cell1, const intpair cell2);
	gridmapPtr generate_Cathedral(int dl, bool descending, bool add_monsters = true);


	//		Hell maps

	gridmapPtr generate_Hellmouth(int dl, bool descending, bool add_monsters = true);
	gridmapPtr generate_HellDepths(int dl, bool descending);

	void hell_MoltenLake(gridmapPtr m, TCODBsp* n, int dl);
	void hell_SpiderNest(gridmapPtr m, TCODBsp* n, int dl);
	void hell_Tomb(gridmapPtr m, TCODBsp* n, int dl);
	void hell_VampireCourt(gridmapPtr m, TCODBsp* n, int dl);


	//		Abyss maps

	void addAbyssTreasures(gridmapPtr m);
	void addAbyssMonsters(gridmapPtr m, const vector<MonsterType> mlist, const int dl);

	gridmapPtr generate_DrownedCourt(int abyss_lvl);
	gridmapPtr generate_AmogTomb(int abyss_lvl);
	gridmapPtr generate_Viridia(int abyss_lvl);
	gridmapPtr generate_OuterDark(int dl, bool descending);


	//		Boss maps

	gridmapPtr generate_PallidRotking(int dl, bool descending, int killcount);
	gridmapPtr generate_Hellfort(int dl, bool descending, int killcount);


	//		Map generation

	int rollMapDimension();

	gridmapPtr generate_Abyssal(int abyss_lvl, MaterialType _ritualType);
	gridmapPtr generate(int dl, game_progress* progress, bool descending = true);

	void openHellPortal(gridmapPtr m);
	void openTemplePortal(gridmapPtr m);

	gridmapPtr generate_HomeBase();
	gridmapPtr generate_HellTemple();

}