#include "movement.h"



//	Player tries to walk into a non-walkable tile.
void playerBumpTile(gamedataPtr gdata, const intpair pt)
{
	auto tl = gdata->_map->getTile(pt);

	switch (tl)
	{
		//	Treasure?
	case(MT_CHEST_GLOWING):
	case(MT_CHEST_LUMINOUS):
	case(MT_CHEST_RADIANT):
	case(MT_CHEST_SMALL):
		openLootChest(gdata, pt);
		break;

		//	Destructible?
	case(MT_BARREL):
	case(MT_PEW_WOODEN):
	case(MT_TABLE_WOODEN):
	case(MT_TREE_DEAD):
	case(MT_TOMBSTONE):
		messages::add(gdata, "You smash the #" + getMaptileName(tl) + "@!", { getMaptileColor(tl) });
		addAnimation(gdata, anim_Spray(pt, { '%', '*', '#', }, getMaptileColor(tl)));
		gdata->_map->setTile(MT_RUBBLE, pt);
		gdata->_map->updateTmapAtPoint(pt.first, pt.second);
		gdata->_player->spendActionEnergy();
		break;

		//	Destructible (alternate)
	case(MT_WALL_ICE):
		messages::add(gdata, "You shatter the block of ice!");
		addAnimation(gdata, anim_Spray(pt, { '~', '~', '~', }, getMaptileColor(tl)));
		gdata->_map->setTile(MT_WATER, pt);
		gdata->_map->updateTmapAtPoint(pt.first, pt.second);
		gdata->_player->spendActionEnergy();
		break;

		//	Magical healing fountain
	case(MT_HEALING_FOUNTAIN):
		messages::add(gdata, "#You touch the healing waters and are replenished!", { COLOR_POSITIVE });
		gdata->_player->healToMax();
		gdata->_player->restoreAllMagic();
		if (gdata->_player->_currentFlask != nullptr)
			gdata->_player->_currentFlask->restoreAllCharges();
		break;

		//	Learn spells
	case(MT_RUNE_IMPRINTER):
		openRuneImprinter(gdata);
		break;

		//	Equipment menus
	case(MT_STASH):
		openStash(gdata);
		break;
	case(MT_ANVIL):
		openAnvilMenu(gdata);
		break;
	case(MT_FORGE):
		openForgeMenu(gdata);
		break;
	case(MT_ALCHEMY):
		openAlchemyMenu(gdata);
		break;
	case(MT_GEMSTONE_PRESS):
		openGemstonePress(gdata);
		break;
	case(MT_DEMONFORGE):
		openDemonforge(gdata);
		break;
	case(MT_ABYSSAL_ALTAR):
		openRitualAltar(gdata);
		break;

		//	Saving
	case(MT_MEDITATION_SHRINE):
		gdata->_state = STATE_SAVE_GAME;
		break;

		//	Just prevented from moving
	default:
		messages::add(gdata, "Blocked by a #" + getMaptileName(tl) + "@.", { getMaptileColor(tl) });
	}
}


//	Happens when we step onto a tile (but does not recur if we linger there)
void playerEnterTile(gamedataPtr gdata, intpair pt)
{
	////	Check for stuff to pick up
	tryAutopickup(gdata, pt);


	//	Alert if there are items here
	auto icp = gdata->_map->getItemContainer(pt);
	if (icp != nullptr && !icp->_items.empty())
	{
		auto it = icp->_items.front();
		string txt = "You see here: #" + it->getName() + "@";
		if (icp->_items.size() > 2)
			txt += " and " + to_string(icp->_items.size() - 1) + " other items.";
		else if (icp->_items.size() == 2)
			txt += " and one other item.";
		else
			txt += ".";
		messages::add(gdata, txt, { it->getColor() });
	}

	//	Alert if there are stairs here.
	auto tl = gdata->_map->getTile(pt);
	if (isMaptileStairs(tl))
		messages::add(gdata, "You see here: #" + getMaptileName(tl) + "@! #(Press SPACE to use)", { getMaptileColor(tl), COLOR_DARK });

	//	Set our position
	setPlayerPosition(gdata, pt);
}


//	Attempts to move the player along the given vector.
//	Returns True if we actually moved.
bool doPlayerMove(gamedataPtr gdata, const intpair vec, bool allowAttacking)
{
	//	Get the point we'll be moving to.
	intpair pt = gdata->_player->_pos;
	intpair_add(&pt, &vec);


	//	Actually attempt the move.
	if (gdata->_map->inBounds(pt))
	{
		//	Is there a creature here?
		auto cr = gdata->_map->getCreature(pt);

		//	Melee attack
		if (cr != nullptr)
		{
			playerTriggerMeleeAttack(gdata, cr);
			return false;
		}

		//	Is the point walkable?
		else if (gdata->_map->isWalkable(pt))
		{
			//	we struggle if entangled.
			if (gdata->_player->hasStatusEffect(STATUS_ENTANGLED))
			{
				messages::add(gdata, "#You struggle against the entangling webs!", { COLOR_WHITE });
				gdata->_player->reduceStatusEffectDuration(STATUS_ENTANGLED);
			}

			//	can't move if grappled
			else if (gdata->_player->isGrappled())
			{
				messages::error(gdata, "You are grappled and can't move!");
				gdata->_player->verifyGrappler();
			}

			//	movement
			else
			{
				playerEnterTile(gdata, pt);
				if (gdata->_player->getTotalEnchantmentBonus(ENCH_DANCER) > 0)
					gdata->_player->setBuffDuration(BUFF_DANCER, 2);
			}

			//	lose energy either way
			gdata->_player->spendActionEnergy(gdata->_player->getMoveEnergyCost());
			return true;
		}

		//	Can't move here!
		else
			playerBumpTile(gdata, pt);
	}

	return false;
}

void startAutoMoving(gamedataPtr gdata, const intpair vec)
{
	gdata->_automoveVec = vec;
	gdata->_automoveDist = 3;
}


void doAutomove(gamedataPtr gdata)
{
	//	Try to move.
	if (doPlayerMove(gdata, gdata->_automoveVec, false))
	{
		gdata->_automoveDist--;
		if (!gdata->_visibleMonsters.empty())
			gdata->_automoveDist = 0;
	}

	//	If we fail to move, cancel the automove.
	else
	{
		gdata->_automoveDist = 0;
	}
}
