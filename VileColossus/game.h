#pragma once

#include <chrono>

#include "ai.h"
#include "color_constants.h"
#include "display.h"
#include "inputHandler.h"
#include "gamedata.h"
#include "lootgen.h"
#include "mapgen.h"
#include "manageItems.h"
#include "manageMagic.h"
#include "movement.h"
#include "savegame.h"


#define ANIMATION_TICK_MS	25


class game
{
public:
	game();
	~game() {}

	void start();
	void startFromSave();

	void menuLoop();
	void selectGameMode();
	void newgame();

	void mainGameLoop();
	void drawScreen();
	
	void processInput();
	void mainGameInput();
	void backOut();

	void awaitDebugCommand();

	void useAbyssalGate();
	void useTemplePortal();

	void moveToNewMap(int vec);
	void tryUseStairs();
	void returnToHomeBase();
	void castTownPortal();

	void acceptDeath();

	void doCreatureTick(creaturePtr cr);
	void tick();

	void checkForAnimationTick();

	void scrollMenu(int vec, const unsigned size);


private:

	bool _isGameOver;

	display	_disp;
	gamedataPtr _gdata;
	inputHandlerPtr _ih;

	int _globalTickCounter;

	chrono::steady_clock::time_point _lastAnimationTick;

	const string getGameModeName();

};

