#include "game.h"



/*
TODO

	possible to somehow not die at zero health? just stay invincible?? related to self-immolation
	properly fit monster tags to screen

	refresh marked equipment slots on death
	make legendary gems much less likely at low depths
	give names to quiver tiers
*/


game::game() : _isGameOver(false)
{
	initrand();
	_ih = inputHandlerPtr(new inputHandler());
	_gdata = gamedataPtr(new gamedata());
}


void game::start()
{
	_gdata->_state = STATE_TITLE;
	messages::add(_gdata, "Press #? @to view controls.", { COLOR_LIGHT });
	menuLoop();
}


//	Start our saved game.
void game::startFromSave()
{
	//	starting character
	_gdata->_player = playerPtr(new player());


	//	Load save data.
	savegame::load_from_file(savegame::SAVE_FILE_DEFAULT_NAME, _gdata);


	//	Generate permanent maps.
	_gdata->_homeBase = mapgen::generate_HomeBase();
	_gdata->_helltemple = mapgen::generate_HellTemple();


	//	Morph starting maps if necessary.
	if (_gdata->_gameProgress._killedRotking > 0)
		mapgen::openHellPortal(_gdata->_homeBase);
	if (_gdata->_gameProgress._killedHellboss > 0)
		mapgen::openTemplePortal(_gdata->_homeBase);


	//	Add player to the starting map.
	_gdata->_depth = 0;
	_gdata->_map = _gdata->_homeBase;
	_gdata->_map->addCreature(_gdata->_player, _gdata->_map->_startPt);
	_gdata->_map->setFOVMapCentre(_gdata->_player->_pos);


	//	Set state.
	_gdata->_state = STATE_NORMAL;
}


//	We loop here prior to the game start.
void game::menuLoop()
{
	while (_gdata->_state != STATE_NORMAL && !_isGameOver)
	{
		drawScreen();
		processInput();
	}

	if (!_isGameOver)
		mainGameLoop();
}


//	Select a game mode and start the game.
void game::selectGameMode()
{
	_gdata->_state = STATE_NORMAL;
	switch (_ih->getKeypressCharacter())
	{
	case('1'):	_gdata->_mode = GameMode::CASUAL; break;
	case('2'):	_gdata->_mode = GameMode::NORMAL; break;
	case('3'):	_gdata->_mode = GameMode::PERMADEATH; break;
	default:	_gdata->_state = STATE_SELECT_MODE;		//	cancel, we didn't pick a valid option
	}

	//	If we picked a mode, setup a new game.
	if (_gdata->_state == STATE_NORMAL)
		newgame();
}


//	Starts the game from title.
void game::newgame()
{
	//	starting character
	_gdata->_player = playerPtr(new player());

	//	Starting items
	auto wp = lootgen::generateStartingWeapon();
	_gdata->_player->equipInSlot(wp, SLOT_MAIN_HAND);
	_gdata->_player->_currentFlask = lootgen::generateStartingFlask();

	//	starting map
	_gdata->_depth = 0;
	_gdata->_map = mapgen::generate_HomeBase(); //mapgen::generate(1);
	_gdata->_map->addCreature(_gdata->_player, _gdata->_map->_startPt);
	_gdata->_map->setFOVMapCentre(_gdata->_player->_pos);

	//	permanent maps
	_gdata->_homeBase = _gdata->_map;
	_gdata->_helltemple = mapgen::generate_HellTemple();

	//	test
	//addToInventory(_gdata, lootgen::generateSpellrune(3, 3));
	//addToInventory(_gdata, lootgen::generateLegendaryItem(1, ENCH_CONDUCTING));
	/*for (unsigned i = 0; i < 10; i++)
	{
		auto it = lootgen::generateSpellrune(2, lootgen::rollRarity(4));
		addToInventory(_gdata, it);
	}*/

	_gdata->_player->_actionEnergy = ENERGY_ACTION_THRESHOLD;
}


//	Game runs in this loop
void game::mainGameLoop()
{
	while (!_isGameOver)
	{
		//	main game actions
		drawScreen();

		//	check for automatic behaviours
		if (_gdata->_automoveDist > 0)
			doAutomove(_gdata);

		//	saving
		else if (_gdata->_state == STATE_SAVE_GAME)
		{
			savegame::save_to_file(savegame::SAVE_FILE_DEFAULT_NAME, _gdata);
			_gdata->_state = STATE_NORMAL;
		}

		//	otherwise, fetch normal input
		else
			processInput();

		//	time passes
		while (!_gdata->_player->hasActionEnergyAboveThreshold())
			tick();
	}
}

void game::drawScreen()
{
	_disp.clear();

	//	State-dependent stuff
	switch (_gdata->_state)
	{
	case(STATE_TITLE):
		_disp.title();
		break;

	case(STATE_GAME_COMPLETED):
		_disp.drawGameCompletionScreen(_gdata);
		break;

	case(STATE_VIEW_CONTROLS):
		_disp.drawControls();
		break;
	case(STATE_HELP_SCREEN):
		_disp.drawHelpScreen(_gdata);
		break;

	case(STATE_SELECT_MODE):
		_disp.drawModeSelection();
		break;

	case(STATE_CHARACTER_SHEET):
		_disp.drawCharacterSheet(_gdata);
		break;

	case(STATE_VIEW_EQUIPMENT):
		_disp.drawEquipment(_gdata);
		break;

	case(STATE_VIEW_INVENTORY_IN_STASH):
	case(STATE_VIEW_INVENTORY):
		_disp.drawInventory(_gdata);
		break;
	case(STATE_VIEW_ITEM_DETAILS):
		_disp.drawItemInfoDetailed(_gdata, _gdata->_viewingItem);
		break;

	case(STATE_PICKING_UP_ITEMS):
		_disp.drawItemPickup(_gdata);
		break;

	case(STATE_SPEND_ATTRIBUTE_POINTS):
		_disp.drawAttributePoints(_gdata);
		break;

	case(STATE_VIEW_STASH):
		_disp.drawStash(_gdata);
		break;

	case(STATE_ANVIL):
		_disp.drawAnvil(_gdata);
		break;

	case(STATE_FORGE):
		_disp.drawEnchantedForge(_gdata);
		break;
	case(STATE_SELECT_ENCHANTMENT):
		_disp.drawEnchantmentSelect(_gdata);
		break;
	case(STATE_DEMONFORGE):
		_disp.drawDemonforge(_gdata);
		break;

	case(STATE_ALCHEMY):
		_disp.drawAlchemy(_gdata);
		break;

	case(STATE_SPELL_LIST):
		_disp.drawSpellList(_gdata);
		break;
	case(STATE_LEARN_SPELL):
		_disp.drawRuneImprinter(_gdata);
		break;

	case(STATE_GEMSTONE_PRESS):
		_disp.drawGemstonePress(_gdata);
		break;
	case(STATE_SELECT_GEM):
		_disp.drawGemstoneSelect(_gdata);
		break;
	case(STATE_FABRICATE_GEMS):
		_disp.drawGemstoneFabricator(_gdata);
		break;

	case(STATE_RITUAL_ALTAR):
		_disp.drawRitualAltar(_gdata);
		break;


		//	Highlighting/looking
	case(STATE_HIGHLIGHT_ENEMIES):
		_disp.updateVisibleMapData(_gdata);
		_disp.drawMainInterface(_gdata);
		_disp.drawMonsterHighlights(_gdata);
		break;


		//	The normal game screen
	default:
		_disp.updateVisibleMapData(_gdata);
		_disp.drawMainInterface(_gdata);
	}

	_disp.refresh();
	checkForAnimationTick();
}


void game::processInput()
{

	_ih->checkForKeypress();

	//	we can quit whenever we want
	if (_ih->isKeyPressed('q') && _ih->isCtrlPressed())
		_isGameOver = true;

	//	fullscreen toggle
	else if (_ih->isKeyPressed('f') && _ih->isCtrlPressed())
		_disp.toggleFullscreen();

	//	and we can always try to back out of whatever menu we're in
	else if (_ih->isKeyPressed(TCODK_ESCAPE))
		backOut();

	//	MAIN GAME STATE input
	else
	{
		switch (_gdata->_state)
		{
			//	Looking around
		case(STATE_HIGHLIGHT_ENEMIES):
		case(STATE_LOOK):
			if (_ih->isKeyPressed(TCODK_TAB))
				cycleCursorTarget(_gdata);
			else if (_ih->isDirectionalKeyPressed())
				moveCursor(_gdata, _ih->getVectorFromKeypress());
			else if (_ih->isKeyPressed('v'))
				_gdata->_state = (_gdata->_state == STATE_LOOK) ? STATE_HIGHLIGHT_ENEMIES : STATE_LOOK;
			else if (_ih->isKeyPressed('x'))
				backOut();
			break;


			//	Inventory
		case(STATE_VIEW_INVENTORY):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				tryEquipCurrentItemInSlot(_gdata);
			else if (_ih->isKeyPressed('d'))
				dropSelectedItem(_gdata);
			else if (_ih->isKeyPressed('v'))
				viewItemDetails(_gdata);
			else if (_ih->isKeyPressed('M'))
				toggleMarkedValuable(_gdata);
			break;
		case(STATE_VIEW_EQUIPMENT):
			if (_ih->isKeyPressed('U'))
				unequipFromSelectedSlot(_gdata);
			else if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, SLOT__NONE);
			else if (_ih->isKeyPressed(TCODK_ENTER))
				selectCurrentEquipmentSlot(_gdata);
			else if (_ih->isKeyPressed('w'))
				swapToSecondaryEquipment(_gdata);
			else if (_ih->isKeyPressed('v'))
				viewEquipmentDetails(_gdata);
			break;


			//	Getting stuff
		case(STATE_PICKING_UP_ITEMS):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed('g') || _ih->isKeyPressed(TCODK_ENTER))
				pickupSelectedItem(_gdata);
			break;


			//	Leveling up
		case(STATE_SPEND_ATTRIBUTE_POINTS):
			if (_ih->isDirectionalKeyPressed())
			{
				if (_gdata->_player->canGainPerks() && _gdata->_attributePointsLeft < 1)
				{
					if (_ih->getVectorFromKeypress().second != 0)
						scrollMenu(_ih->getVectorFromKeypress().second, PERK__MAX);
					else if (_ih->getVectorFromKeypress().first == 1 || _ih->isKeyPressed(TCODK_ENTER))
						spendPerkPoint(_gdata);
				}
				else
				{
					if (_ih->getVectorFromKeypress().second != 0)
						scrollMenu(_ih->getVectorFromKeypress().second, ATTR__MAX);
					else if (_ih->getVectorFromKeypress().first == 1)
						spendAttributePoint(_gdata);
				}
			}
			else if (_ih->isKeyPressed(TCODK_ENTER))
				spendAttributePoint(_gdata);
			break;


			//	Stash
		case(STATE_VIEW_STASH):
			if (_ih->isKeyPressed(TCODK_TAB))
			{
				openInventory(_gdata);
				_gdata->_state = STATE_VIEW_INVENTORY_IN_STASH;
			}
			else if (_ih->isKeyPressed('g'))
				takeSelectedFromStash(_gdata);
			else if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed('v'))
				viewItemDetails(_gdata);
			else if (_ih->isKeyPressed('M'))
				toggleMarkedValuable(_gdata);
			break;
		case(STATE_VIEW_INVENTORY_IN_STASH):
			if (_ih->isKeyPressed(TCODK_TAB))
				openStash(_gdata);
			else if (_ih->isKeyPressed('t'))
				moveSelectedToStash(_gdata);
			else if (_ih->isKeyPressed('T'))
				transferInventoryToStash(_gdata);
			else if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed('v'))
				viewItemDetails(_gdata);
			else if (_ih->isKeyPressed('M'))
				toggleMarkedValuable(_gdata);
			break;


			//	Anvil
		case(STATE_ANVIL):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed('D'))
				dismantleFromInventory(_gdata);
			else if (_ih->isKeyPressed('A'))
				dismantleAll(_gdata);
			else if (_ih->isKeyPressed('r'))
				tryRepairSelected(_gdata);
			else if (_ih->isKeyPressed('v'))
				viewItemDetails(_gdata);
			else if (_ih->isKeyPressed('M'))
				toggleMarkedValuable(_gdata);
			break;


			//	Forge
		case(STATE_FORGE):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, SLOT__NONE);
			else if (_ih->isKeyPressed('r'))
				repairEquipmentItem(_gdata);
			else if (_ih->isKeyPressed('e'))
				enhanceSelectedItem(_gdata);
			else if (_ih->isKeyPressed('f'))
				reinforceSelectedItem(_gdata);
			else if (_ih->isKeyPressed('R'))
				repairAllEquipped(_gdata);
			else if (_ih->isKeyPressed('w'))
				swapToSecondaryEquipment(_gdata);
			else if (_ih->isKeyPressed('v'))
				viewItemDetails(_gdata);
			break;
		case(STATE_SELECT_ENCHANTMENT):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_knownEnchants.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				selectEnchantmentToApply(_gdata);
			break;


			//	Demonforge
		case(STATE_DEMONFORGE):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, SLOT__NONE);
			else if (_ih->isKeyPressed('e'))
				tryExaltItem(_gdata);
			break;


			//	Rituals
		case(STATE_RITUAL_ALTAR):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_stashedRitualMaterials.size());
			else if (_ih->isKeyPressed('a'))
				selectRitualMaterial(_gdata);
			else if (_ih->isKeyPressed(TCODK_ENTER))
				openAbyssGate(_gdata);
			break;


			//	Alchemy
		case(STATE_ALCHEMY):
			if (_ih->isNumberKey())
				transmuteMaterial(_gdata, _ih->numberKeyToInt());
			break;


			//	Gemstone press
		case(STATE_GEMSTONE_PRESS):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, SLOT__NONE);
			else if (_ih->isKeyPressed(TCODK_ENTER))
				selectItemToSocket(_gdata);
			else if (_ih->isKeyPressed('e'))
				tryAddGemSlot(_gdata);
			else if (_ih->isKeyPressed('x'))
				removeAllGemsFromItem(_gdata);
			else if (_ih->isKeyPressed('f'))
				openGemFabricator(_gdata);
			break;
		case(STATE_SELECT_GEM):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_stashedGems.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				selectGemToSocketIntoItem(_gdata);
			break;
		case(STATE_FABRICATE_GEMS):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				tryFabricateGemstone(_gdata);
			break;


			//	Magic
		case(STATE_SPELL_LIST):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_player->getAllSpellsKnown().size());
			break;
		case(STATE_AIMING_SPELL):
			if (_ih->isDirectionalKeyPressed())
				playerCastAimedSpell(_gdata, _ih->getVectorFromKeypress());
			break;
		case(STATE_LEARN_SPELL):
			if (_ih->isKeyPressed('U'))
				removeAllSpellRunes(_gdata);
			else if (_ih->isKeyPressed('u'))
				removeSelectedRune(_gdata);
			else if (_ih->isKeyPressed('e'))
				tryEnhanceRune(_gdata);
			else if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				imprintSelectedSpellRune(_gdata);
			break;


			//	Accept death and proceed
		case(STATE_ACKNOWLEDGE_DEATH):
			if (_ih->isKeyPressed(TCODK_ENTER))
				acceptDeath();
			break;
		case(STATE_GAME_COMPLETED):
			if (_ih->isKeyPressed(TCODK_ENTER))
			{
				if (_gdata->_mode == GameMode::PERMADEATH && !_gdata->_victory)
					_isGameOver = true;
				else
					_gdata->_state = STATE_NORMAL;
			}
			break;



			//	Starting the game.
		case(STATE_TITLE):
			if (_ih->isKeyPressed(TCODK_1))
				_gdata->_state = STATE_SELECT_MODE;
			else if (_ih->isKeyPressed(TCODK_2) && savegame::exists())
				startFromSave();
			break;
		case(STATE_SELECT_MODE):
			selectGameMode();
			break;


			//	Help.
		case(STATE_VIEW_CONTROLS):
			if (_ih->isKeyPressed('?') || _ih->isKeyPressed('/'))
			{
				_gdata->_idx = 0;
				_gdata->_state = STATE_HELP_SCREEN;
			}
			break;
		case(STATE_HELP_SCREEN):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, HELP_SCREEN_CATEGORIES.size());
			break;


			//	Normal input
		case(STATE_NORMAL):
			mainGameInput(); break;
		}
	}

}


//	Input while in 'NORMAL' state
void game::mainGameInput()
{
	//	Movement
	if (_ih->isDirectionalKeyPressed())
	{
		if (_ih->isCtrlPressed())
			startAutoMoving(_gdata, _ih->getVectorFromKeypress());
		else if (_ih->isShiftKeyPressed())
			playerTryRangedAttack(_gdata, _ih->getVectorFromKeypress());
		else
			doPlayerMove(_gdata, _ih->getVectorFromKeypress());
	}

	//	Autoattack
	else if (_ih->isKeyPressed(TCODK_TAB))
		playerTryAutoattack(_gdata);

	//	Wait one turn
	else if (_ih->isKeyPressed('z') || _ih->isKeyPressed(TCODK_KP5))
		_gdata->_player->spendActionEnergy();

	//	Quaff
	else if (_ih->isKeyPressed('q'))
		playerQuaffCurrentFlask(_gdata);

	//	Swap readied weapons
	else if (_ih->isKeyPressed('w'))
		swapToSecondaryEquipment(_gdata);

	//	Look around
	else if (_ih->isKeyPressed('x'))
		enterLookMode(_gdata);

	//	Pick stuff up
	else if (_ih->isKeyPressed('g'))
		pickupFromFloor(_gdata);

	//	Inter-map movement
	else if (_ih->isKeyPressed(TCODK_SPACE))
		tryUseStairs();

	//	Spell hotkeys
	else if (_ih->isNumberKey() && !_ih->isShiftKeyPressed())
		castSpellFromHotkey(_gdata, _ih->numberKeyToInt() - 1);

	//	Warp home, if allowed
	else if (_ih->isKeyPressed('T'))
		castTownPortal();

	//	View menus
	else if (_ih->isKeyPressed('A'))
		_gdata->_state = STATE_CHARACTER_SHEET;
	else if (_ih->isKeyPressed('a'))
		openAttributeMenu(_gdata);
	else if (_ih->isKeyPressed('e'))
		openEquipment(_gdata);
	else if (_ih->isKeyPressed('i'))
		openInventory(_gdata);
	else if (_ih->isKeyPressed('m'))
		openSpellList(_gdata);

	else if (_ih->isKeyPressed('?') || _ih->isKeyPressed('/'))
		_gdata->_state = STATE_VIEW_CONTROLS;

	//	Debug
	else if (_ih->isKeyPressed('`'))
		awaitDebugCommand();

	//	Save/load test
	/*else if (_ih->isKeyPressed('s') && _ih->isCtrlPressed())
		savegame::save_to_file(savegame::SAVE_FILE_DEFAULT_NAME, _gdata);
	else if (_ih->isKeyPressed('p') && _ih->isCtrlPressed())
		savegame::test_load(_gdata);*/
}


//	Back out of whatever interface we're in, closer to the main display
void game::backOut()
{
	//	Return from selecting an item to equip in a slot
	if (_gdata->_state == STATE_VIEW_INVENTORY && _gdata->_selectedSlot != SLOT__NONE)
	{
		int old_idx = _gdata->_selectedSlot;
		openEquipment(_gdata);
		_gdata->_idx = old_idx;
	}

	//	From gem fab, return to main gem press
	else if (_gdata->_state == STATE_FABRICATE_GEMS)
		openGemstonePress(_gdata);

	//	Cancel casting
	else if (_gdata->_state == STATE_AIMING_SPELL)
	{
		messages::add(_gdata, "Okay, cancelling spell cast.");
		_gdata->_state = STATE_NORMAL;
	}

	//	From item details, return to inventory
	else if (_gdata->_state == STATE_VIEW_ITEM_DETAILS)
	{
		if (_gdata->_currentItemList.empty())
			_gdata->_state = STATE_VIEW_EQUIPMENT;
		else
			_gdata->_state = STATE_VIEW_INVENTORY;
	}

	//	Return to title.
	else if (_gdata->_state == STATE_SELECT_MODE)
		_gdata->_state = STATE_TITLE;

	//	ESC from the title screen quits.
	else if (_gdata->_state == STATE_TITLE)
		_isGameOver = true;

	//	The default: return to the normal game display
	else if (_gdata->_state != STATE_ACKNOWLEDGE_DEATH)
		_gdata->_state = STATE_NORMAL;
}


//	Test for a debug command & then execute it.
void game::awaitDebugCommand()
{
	auto txt = _disp.getstr(3, 3, "> ");
	if (txt == "vis")
		_gdata->_omniscient = !_gdata->_omniscient;
	else if (txt == "god")
		_gdata->_invincible = !_gdata->_invincible;
	else if (txt == "autokill")
		_gdata->_autokill = !_gdata->_autokill;
	else if (txt == "level")
		playerGainLevel(_gdata);
	else if (txt == "level10")
	{
		for (unsigned i = 0; i < 10; i++)
			playerGainLevel(_gdata);
	}
	else if (txt == "level30")
	{
		for (unsigned i = 0; i < 30; i++)
			playerGainLevel(_gdata);
	}
	else if (txt == "home")
		returnToHomeBase();
	else if (txt == "down")
		moveToNewMap(1);
	else if (txt == "drop")
		_gdata->_map->addItem(lootgen::rollItemDrop(3, 4), _gdata->_player->_pos);
	else if (txt == "drop10")
	{
		for (unsigned i = 0; i < 10; i++)
			_gdata->_map->addItem(lootgen::rollItemDrop(3, 4, false, roll_one_in(4)), _gdata->_player->_pos);
	}
	else if (txt == "droprune")
	{
		for (unsigned i = 0; i < 10; i++)
			_gdata->_map->addItem(lootgen::generateSpellrune(randint(1, 3), randint(1, 3)), _gdata->_player->_pos);
	}
	else if (txt == "dropleg")
		_gdata->_map->addItem(lootgen::rollItemDrop(3, 4, true), _gdata->_player->_pos);
	else if (txt == "gems10")
	{
		for (unsigned i = 0; i < 10; i++)
			_gdata->_map->addItem(lootgen::generateGem(randint(1, 4), 1), _gdata->_player->_pos);
	}
	else if (txt == "sgems")
	{
		_gdata->_map->addItem(lootgen::generateGemOfType(GemType::ABYSSAL_SPIKEGEM, 1, 4), _gdata->_player->_pos);
		_gdata->_map->addItem(lootgen::generateGemOfType(GemType::BLOODY_FLESHGEM, 1, 4), _gdata->_player->_pos); 
		_gdata->_map->addItem(lootgen::generateGemOfType(GemType::DOGOSSAN_BONEGEM, 1, 4), _gdata->_player->_pos);
		_gdata->_map->addItem(lootgen::generateGemOfType(GemType::LUMINOUS_GEM, 1, 4), _gdata->_player->_pos);
		_gdata->_map->addItem(lootgen::generateGemOfType(GemType::VIRIDIAN_PALEGEM, 1, 4), _gdata->_player->_pos);
	}
	else if (txt == "materials")
	{
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::FRAGMENTS, 5000));
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::MAGIC_DUST, 500));
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::GLOWING_POWDER, 250));
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::RADIANT_ASH, 1));
	}
	else if (txt == "ritual")
	{
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::SODDEN_FLESH, 1));
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::TOMB_IDOL, 1));
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::VILEDRAGON_SCALE, 1));
		addToInventory(_gdata, lootgen::generateMaterial(MaterialType::VIRIDIAN_GLASS, 1));
	}
	else if (txt == "cube")
		_gdata->_map->addItem(lootgen::generateMaterial(MaterialType::NOTCHED_CUBE, 1), _gdata->_player->_pos);
	else if (txt == "hellportal")
		mapgen::openHellPortal(_gdata->_homeBase);
	else if (txt == "helltemple")
		mapgen::openTemplePortal(_gdata->_homeBase);
	else if (txt == "killall")
	{
		for (auto m : _gdata->_visibleMonsters)
			killCreature(_gdata, m);
	}
}


//	Closes the portal and warps us to a special map type.
void game::useAbyssalGate()
{
	//	Clear the portal
	_gdata->_map->setTile(MT_DRAINED_SHRINE, _gdata->_player->_pos);

	//	Generate the map
	if (_gdata->_summonedViledragon)
		_gdata->_map = mapgen::generate_ViledragonLair(&_gdata->_gameProgress, _gdata->_ritualType);
	else
		_gdata->_map = mapgen::generate_Abyssal(_gdata->_gameProgress._abyssLevel, &_gdata->_gameProgress, _gdata->_ritualType);

	//	Put us on the new map
	messages::add(_gdata, "#You enter the Abyss!", { TCODColor::lightPurple });
	auto pt = _gdata->_map->_startPt;
	_gdata->_map->addCreature(_gdata->_player, pt);
	setPlayerPosition(_gdata, pt);
}


//	Moves us between the home base and the hell temple.
void game::useTemplePortal()
{
	//	descend to hell temple
	if (_gdata->_map == _gdata->_homeBase)
		moveToNewMap(16);

	//	return home
	else
		returnToHomeBase();
}


//	Move to a map with the given difference in depth to our current map.
void game::moveToNewMap(int vec)
{
	//	message about it
	if (vec > 0)
		messages::add(_gdata, "You descend...");
	else
		messages::add(_gdata, "You ascend...");
	drawScreen();

	//	generate the new map, or use a permanent map (at certain depths)
	_gdata->_depth += vec;
	if (_gdata->_depth == 0)
		_gdata->_map = _gdata->_homeBase;
	else if (_gdata->_depth == 16)
		_gdata->_map = _gdata->_helltemple;
	else
		_gdata->_map = mapgen::generate(_gdata->_depth, &_gdata->_gameProgress, vec > 0);

	//	add us to it
	auto pt = _gdata->_map->_startPt;
	_gdata->_map->addCreature(_gdata->_player, pt);
	setPlayerPosition(_gdata, pt);
}


//	Attempt to go up or down stairs.
void game::tryUseStairs()
{
	auto tl = _gdata->_map->getTile(_gdata->_player->_pos);
	if (tl == MT_STAIRS_DOWN)
		moveToNewMap(1);
	else if (tl == MT_STAIRS_UP)
		moveToNewMap(-1);
	else if (tl == MT_STAIRS_UP_LONG)
		moveToNewMap(-8);
	else if (tl == MT_STAIRS_DOWN_LONG)
		moveToNewMap(8);
	else if (tl == MT_HELLPORTAL_DOWN)
		moveToNewMap(10);
	else if (tl == MT_HELLPORTAL_UP)
		moveToNewMap(-10);
	else if (tl == MT_TEMPLE_PORTAL)
		useTemplePortal();
	else if (tl == MT_ABYSSAL_GATE)
		useAbyssalGate();
	else
		messages::error(_gdata, "There are no stairs here!");
}


//	(After dying, or casting town portal) teleport us back to the surface.
void game::returnToHomeBase()
{
	//	Put us on the proper map
	_gdata->_map = _gdata->_homeBase;
	_gdata->_map->addCreature(_gdata->_player, _gdata->_map->_startPt);
	setPlayerPosition(_gdata, _gdata->_map->_startPt);

	//	Set depth and automatically replenish health
	_gdata->_depth = 0;
	_gdata->_player->healToMax();
	_gdata->_state = STATE_NORMAL;

	//	Clear current ritual, if any
	_gdata->_ritualType = MaterialType::__NONE;
	_gdata->_summonedViledragon = false;

	//	Autosave
	savegame::save_to_file(savegame::SAVE_FILE_DEFAULT_NAME, _gdata);
}


//	Warp back to the surface, if we can.
void game::castTownPortal()
{
	if (isTownPortalCharged(_gdata))
	{
		_gdata->_townPortalCharge = 0;
		returnToHomeBase();
		addAnimation(_gdata, anim_FlashGlyph(_gdata->_player->_pos, '*', TCODColor::lighterBlue));
		messages::add(_gdata, "#You warp back to the surface!", { TCODColor::lighterBlue });
	}
	else
		messages::error(_gdata, "Your portal isn't charged yet! Kill monsters to charge it.");
}


//	Normally this returns us home; in Permadeath mode, it ends the game instead.
void game::acceptDeath()
{
	if (_gdata->_mode == GameMode::PERMADEATH)
		_gdata->_state = STATE_GAME_COMPLETED;
	else
	{
		_gdata->_player->_triggeredDeath = false;
		returnToHomeBase();
	}
}


//	Events that happen to creatures at the start of their turns, after they gain action energy.
void game::doCreatureTick(creaturePtr cr)
{
	//	DOTs (all do percentage damage)
	if (cr->hasStatusEffect(STATUS_BURN))
		creatureTakeDamage(_gdata, cr, 1 + cr->getMaxHealth() / 20);
	if (cr->hasStatusEffect(STATUS_POISON))
		creatureTakeDamage(_gdata, cr, 1 + cr->getMaxHealth() / 30);
	if (cr->hasBuff(BUFF_REGENERATION))
		cr->healDamage(1  +cr->getMaxHealth() / 20);

	//	Test that grapple is still valid, if applicable
	cr->verifyGrappler();

	//	PULSE
	if (cr->isPlayer() && cr->hasBuff(BUFF_ARCANE_PULSE))
	{
		auto dam = _gdata->_player->getArcanePulseDamage();
		for (auto t : getAdjacentCreatures(_gdata, cr->_pos))
		{
			if (t != cr)
				inflictEnergyDamage(_gdata, t, randint(1, dam), DTYPE_ARCANE);
		}
		addAnimation(_gdata, anim_Explosion(cr->_pos, 1, '#', getDamageTypeColor(DTYPE_ARCANE)));
	}

	//	CHAIN
	if (cr->isPlayer() && cr->hasBuff(BUFF_STATIC_FIELD))
	{
		auto t = findRandomSpellTarget(_gdata, _gdata->_player);
		if (t != nullptr)
		{
			const int dbase = _gdata->_player->getStaticFieldDamage();
			auto dam = intpair(dbase / 2, dbase);
			chainDamage(_gdata, t, DTYPE_ELECTRIC, dam, 3);
		}
	}

	//	Progress creature state
	cr->tick();

	//	Tile effects
	standOnTile(_gdata, cr);
}


//	Passage of time. NPCs get actions.
void game::tick()
{
	//	progress all creatures
	for (auto cr : _gdata->_map->getAllCreatures())
	{
		cr->_actionEnergy += ENERGY_COST_BASE;
		doCreatureTick(cr);
		if (cr->hasActionEnergyAboveThreshold())
		{
			//	creature gets a turn
			if (!cr->isPlayer())
				ai::takeTurn(_gdata, static_pointer_cast<monster>(cr));
		}
	}
	_gdata->_map->tickSurfaces();
}


//	If enough time has passed, advance animations.
void game::checkForAnimationTick()
{
	auto tdiff = chrono::steady_clock::now() - _lastAnimationTick;
	if (chrono::duration_cast<chrono::milliseconds>(tdiff).count() >= ANIMATION_TICK_MS)
	{
		_disp.tickAnimations(_gdata);
		_lastAnimationTick = chrono::steady_clock::now();
	}
}

void game::scrollMenu(int vec, const unsigned size)
{
	if (_ih->isCtrlPressed())
		vec *= 3;
	_gdata->_idx += vec;
	if (_gdata->_idx < 0)
		_gdata->_idx = size - 1;
	else if (_gdata->_idx >= size)
		_gdata->_idx = 0;
}

const string game::getGameModeName()
{
	switch (_gdata->_mode)
	{
	case(GameMode::CASUAL):		return "Casual";
	case(GameMode::NORMAL):		return "Normal";
	case(GameMode::PERMADEATH):	return "Permadeath";
	default:					return "Not Set";
	}
}
