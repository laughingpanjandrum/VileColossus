#include "game.h"



/*
TODO

	Bows only take time to fire if they hit a target
	Show item comparison in inventory
	What gives a spellrune the right to be called 'major' lol
	Make generator for specific armour type [eg shield/bracer/etc] that the legendary item generator can use
	Show enchantment descriptions when adding enchants to items

	***	Why are multiple bosses spawning at depth 1???

*/



game::game() :
	_isGameOver(false)
{
}

void game::start()
{
	//	seed the RNG
	initrand();

	//	create elements
	_ih = inputHandlerPtr(new inputHandler());
	_gdata = gamedataPtr(new gamedata()); 

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
	_gdata->_homeBase = _gdata->_map;

	//	test
	//addToInventory(_gdata, lootgen::generateLegendaryItem(1, ENCH_VENOMBURST));
	/*for (unsigned i = 0; i < 10; i++)
	{
		auto it = lootgen::generateSpellrune(2, lootgen::rollRarity(4));
		addToInventory(_gdata, it);
	}*/
	/*for (unsigned i = 0; i < 30; i++)
	{
		auto it = lootgen::rollItemDrop(3, 4);
		addToInventory(_gdata, it);
	}*/

	//	begin the main game
	_gdata->_state = STATE_NORMAL;
	_gdata->_player->_actionEnergy = ENERGY_ACTION_THRESHOLD;
	mainGameLoop();
}

void game::mainGameLoop()
{
	while (!_isGameOver)
	{
		//	main game actions
		drawScreen();

		//	check for automatic behaviours
		if (_gdata->_automoveDist > 0)
			doAutomove(_gdata);

		//	otherwise, fetch normal input
		else
			processInput();

		//	progress time
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

	//	and we can always try to back out of whatever menu we're in
	else if (_ih->isKeyPressed(TCODK_ESCAPE))
		backOut();

	//	MAIN GAME STATE input
	else
	{
		switch (_gdata->_state)
		{
			//	Looking around
		case(STATE_LOOK):
			if (_ih->isKeyPressed(TCODK_TAB))
				cycleCursorTarget(_gdata);
			else if (_ih->isDirectionalKeyPressed())
				moveCursor(_gdata, _ih->getVectorFromKeypress());
			break;


			//	Inventory
		case(STATE_VIEW_INVENTORY):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				tryEquipCurrentItemInSlot(_gdata);
			else if (_ih->isKeyPressed('e'))
				selectItemFromInventory(_gdata);
			else if (_ih->isKeyPressed('d'))
				dropSelectedItem(_gdata);
			else if (_ih->isKeyPressed('v'))
				viewItemDetails(_gdata);
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
				if (_ih->getVectorFromKeypress().second != 0)
					scrollMenu(_ih->getVectorFromKeypress().second, ATTR__MAX);
				else if (_ih->getVectorFromKeypress().first == 1)
					spendAttributePoint(_gdata);
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
			break;


			//	Anvil
		case(STATE_ANVIL):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_currentItemList.size());
			else if (_ih->isKeyPressed('D'))
				dismantleFromInventory(_gdata);
			else if (_ih->isKeyPressed('r'))
				tryRepairSelected(_gdata);
			else if (_ih->isKeyPressed('x'))
				extractEnchantments(_gdata);
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
			break;
		case(STATE_SELECT_ENCHANTMENT):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, _gdata->_knownEnchants.size());
			else if (_ih->isKeyPressed(TCODK_ENTER))
				selectEnchantmentToApply(_gdata);
			break;


			//	Alchemy
		case(STATE_ALCHEMY):
			if (_ih->isKeyPressed('e'))
				tryEnhanceFlask(_gdata);
			break;


			//	Gemstone press
		case(STATE_GEMSTONE_PRESS):
			if (_ih->isDirectionalKeyPressed())
				scrollMenu(_ih->getVectorFromKeypress().second, SLOT__NONE);
			else if (_ih->isKeyPressed(TCODK_ENTER))
				selectItemToSocket(_gdata);
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
				returnToHomeBase();
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

	//	Wait one turn
	else if (_ih->isKeyPressed('z'))
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
	else if (_ih->isKeyPressed('>') || _ih->isKeyPressed('<'))
		tryUseStairs();

	//	Spell hotkeys
	else if (_ih->isNumberKey() && !_ih->isShiftKeyPressed())
		castSpellFromHotkey(_gdata, _ih->numberKeyToInt() - 1);

	//	Warp home, if allowed
	else if (_ih->isKeyPressed('T'))
		castTownPortal();

	//	View menus
	else if (_ih->isKeyPressed('@'))
		_gdata->_state = STATE_CHARACTER_SHEET;
	else if (_ih->isKeyPressed('a'))
		openAttributeMenu(_gdata);
	else if (_ih->isKeyPressed('e'))
		openEquipment(_gdata);
	else if (_ih->isKeyPressed('i'))
		openInventory(_gdata);
	else if (_ih->isKeyPressed('m'))
		openSpellList(_gdata);

	//	Debug
	else if (_ih->isKeyPressed('`'))
		awaitDebugCommand();
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
		_gdata->_state = STATE_VIEW_INVENTORY;

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
	else if (txt == "level")
		playerGainLevel(_gdata);
	else if (txt == "home")
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

	//	generate the new map
	_gdata->_depth += vec;
	if (_gdata->_depth == 0)
		_gdata->_map = _gdata->_homeBase;
	else
		_gdata->_map = mapgen::generate(_gdata->_depth, vec > 0);

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
	_gdata->_player->_triggeredDeath = false;
	_gdata->_state = STATE_NORMAL;
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


//	Events that happen to creatures at the start of their turns, after they gain action energy.
void game::doCreatureTick(creaturePtr cr)
{
	//	DOTs (all do percentage damage)
	if (cr->hasStatusEffect(STATUS_BURN))
		creatureTakeDamage(_gdata, cr, 1 + cr->getMaxHealth() / 20);
	if (cr->hasStatusEffect(STATUS_POISON))
		creatureTakeDamage(_gdata, cr, 1 + cr->getMaxHealth() / 30);

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
	_gdata->_idx += vec;
	if (_gdata->_idx < 0)
		_gdata->_idx = size - 1;
	else if (_gdata->_idx >= size)
		_gdata->_idx = 0;
}
