#include "effects.h"



//	Attempt to drop a corpse at the given point.
//	Certain points will not accept corpses.
void tryDropCorpse(gamedataPtr gdata, const Surface corpse, const intpair at)
{
	if (corpse != Surface::__NONE)
	{
		if (gdata->_map->canCoverWithSurface(at.first, at.second))
			gdata->_map->setSurface(corpse, at);
	}
}



//	Monster drops loot on death. Quantity & quality determined by the monster's level and tier.
void doDeathDrops(gamedataPtr gdata, monsterPtr target)
{
	//	Determine number of drops & their best possible quality
	int drop_amt = 0;
	int rarity = target->_tier;
	switch (target->_tier)
	{
	case(1):
		if (roll_one_in(3)) drop_amt++;
		if (roll_one_in(12)) rarity++;
		break;

	case(2):
		drop_amt = 1;
		if (roll_one_in(3)) drop_amt++;
		if (roll_one_in(20)) rarity++;
		break;

	case(3):
		drop_amt = randint(3, 5);
		if (roll_one_in(20)) rarity++;
		break;

	case(4):
		drop_amt = randint(3, 6);
		break;

	case(5):
		drop_amt = randint(8, 12);
		break;
	}

	//	Get free points in the vicinity.
	auto pts = getAdjacentWalkable(gdata, target->_pos);

	//	Roll the items to drop.
	while (drop_amt-- > 0)
	{
		//	Random point and item
		auto pt = pts[randrange(pts.size())];
		auto it = lootgen::rollItemDrop(lootgen::getLootTierForMonsterLevel(target->_level), rarity);
		gdata->_map->addItem(it, pt);

		//	Animation
		addAnimation(gdata, anim_FlashGlyph(pt, it->getGlyph(), it->getColor()));
	}

	//	Also drop MONEY.
	drop_amt = (target->_tier - 1) + randint(0, target->_tier);
	while (drop_amt-- > 0)
	{
		auto it = lootgen::generateMaterial(MaterialType::FRAGMENTS, dieRoll(3, target->_level));
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}

	//	Chance of GEM(s) at higher levels.
	if (target->_level >= 12)
	{
		int gems = 0;
		if (target->_tier == 1 && roll_one_in(50))
			gems = 1;
		else if (target->_tier == 2)
			gems = randint(1, 2);
		else if (target->_tier == 3)
			gems = randint(2, 5);
		while (gems-- > 0)
		{
			auto it = lootgen::generateGem(lootgen::getGemTierForMonsterLevel(target->_level), 1);
			gdata->_map->addItem(it, pts[randrange(pts.size())]);
		}
	}
}


//	For monsters that have death effects.
void explodeOnDeath(gamedataPtr gdata, monsterPtr target, string flag)
{
	if (flag == "poison_burst")
		fillRegionWithSurface(gdata, target->_pos, 2, Surface::POISON_OOZE);
	else if (flag == "fire_burst")
		inflictDamageInRadius(gdata, target->_pos, 1, DTYPE_FIRE, intpair(1, target->_level));
}



//	Dead monster spawns several of its minion-type monsters on death.
void spawnOnDeath(gamedataPtr gdata, monsterPtr target)
{
	auto ctr = target->_pos;
	for (int x = ctr.first - 1; x <= ctr.first + 1; x++)
	{
		for (int y = ctr.second - 1; y <= ctr.second + 1; y++)
		{
			if (gdata->_map->inBounds(x, y) && gdata->_map->isPointClear(x, y) && roll_one_in(2))
			{
				auto mon = monsterdata::generate(target->rollMonsterToSpawn(), target->_level);
				mon->addFlag("minion");
				gdata->_map->addCreature(mon, x, y);
			}
		}
	}
}


void trySetSurface(gamedataPtr gdata, const intpair pt, const Surface sf)
{
	if (gdata->_map->inBounds(pt) && gdata->_map->canCoverWithSurface(pt.first, pt.second))
		gdata->_map->setSurface(sf, pt);
}


//	A square-shaped region.
void fillRegionWithSurface(gamedataPtr gdata, const intpair ctr, int sz, Surface sf)
{
	for (int x = ctr.first - sz; x <= ctr.first + sz; x++)
	{
		for (int y = ctr.second - sz; y <= ctr.second + sz; y++)
		{
			trySetSurface(gdata, intpair(x, y), sf);
		}
	}
}



//	Get loot from a location
void openLootChest(gamedataPtr gdata, const intpair pt)
{
	//	Visual pleasures
	auto tl = gdata->_map->getTile(pt);
	addAnimation(gdata, anim_FlashGlyph(pt, '!', getMaptileColor(tl)));
	messages::add(gdata, "You smash open the #" + getMaptileName(tl) + "@!", { getMaptileColor(tl) });

	//	Quality determined by type of chest
	int quality = 1;
	switch (tl)
	{
	case(MT_CHEST_GLOWING):		quality = 2; break;
	case(MT_CHEST_RADIANT):		quality = 3; break;
	}

	//	Delete the chest
	gdata->_map->setTile(MT_FLOOR_STONE, pt);
	gdata->_map->updateTmapAtPoint(pt.first, pt.second);


	//	Drop items in random nearby spots
	auto pts = getAdjacentWalkable(gdata, pt);
	int amt = randint(1, 2 + quality);
	while (amt-- > 0)
	{
		auto it = lootgen::rollItemDrop(quality, quality);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}


	//	And MONEY
	amt = (quality - 1) + randint(0, quality);
	while (amt-- > 0)
	{
		//	determine type
		MaterialType mt = MaterialType::FRAGMENTS;
		int r = randint(1, 100);
		if (r <= 50 && quality > 1)	mt = MaterialType::MAGIC_DUST;

		//	generate the item
		auto it = lootgen::generateMaterial(mt, dieRoll(3, 6 + quality));
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}


	//	And GEM(s)?
	if (quality > 1)
	{
		amt = randint(1, quality);
		auto it = lootgen::generateGem(lootgen::getGemTierForMonsterLevel(quality * 12), 1);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}
}


//	Oops, you died.
void killPlayer(gamedataPtr gdata)
{
	//	Are we cheating?
	if (gdata->_invincible)
	{
		gdata->_player->healToMax();
		return;
	}

	//	SET STATE
	else if (!gdata->_player->_triggeredDeath)
	{
		gdata->_player->_triggeredDeath = true;
		gdata->_state = STATE_ACKNOWLEDGE_DEATH;
		messages::add(gdata, "# *** YOU DIED ***", { COLOR_NEGATIVE });
		messages::add(gdata, "#Equipment durability reduced!", { COLOR_WARNING });
		messages::add(gdata, "Press ENTER to continue...");

		//	Damage equipment
		for (auto it : gdata->_player->getAllEquippedItems())
		{
			if (it != nullptr)
			{
				it->reduceMaxDurability(1 + it->_maxDurability / 10);
				it->takePercentDamage(10);
			}
		}

		//	Lose inventory
		gdata->_carriedItems.clear();
	}
}



//	Kill & remove an NPC from the map.
void killCreature(gamedataPtr gdata, creaturePtr target)
{
	//	player death (ignore for now)
	if (target->isPlayer())
		killPlayer(gdata);

	//	NPC death
	else
	{
		//	Remove from map
		gdata->_map->removeCreature(target);

		//	Kill rewards
		auto mon = static_pointer_cast<monster>(target);
		if (!mon->_doneDeathDrops)
		{
			//	drops
			mon->_doneDeathDrops = true;
			if (!mon->hasFlag("minion"))
			{
				doDeathDrops(gdata, mon);
				addKillXP(gdata, mon);
			}

			//	corpse?
			if (mon->hasFlag("poison_burst"))
				explodeOnDeath(gdata, mon, "poison_burst");
			else if (mon->hasFlag("fire_burst"))
				explodeOnDeath(gdata, mon, "fire_burst");
			else
				tryDropCorpse(gdata, mon->getCorpseType(), mon->_pos);
			if (mon->hasFlag("infested"))
				spawnOnDeath(gdata, mon);

			//	effects on the player
			gdata->_player->healDamage(gdata->_player->getLeechOnKill());
			gdata->_player->restoreMagic(gdata->_player->getManaleech());
			gdata->_player->chargeFlaskOnKill();
			if (roll_percent(gdata->_player->getWrathOnKillChance()))
				gdata->_player->addBuff(BUFF_WRATH, 1 + mon->_tier * 2);
		}
	}
}


//	It says 'radius', but it's actually a square-shaped region.
//	If ignore_centre is set, the central point (ctr) is unaffected; this is for, eg, spells that 'burst out' from a caster
void inflictDamageInRadius(gamedataPtr gdata, const intpair ctr, const int r, const DamageType dt, const intpair dmg, bool ignore_centre)
{
	for (unsigned x = ctr.first - r; x <= ctr.first + r; x++)
	{
		for (unsigned y = ctr.second - r; y <= ctr.second + r; y++)
		{
			if (gdata->_map->inBounds(x, y) && (!ignore_centre || !(x == ctr.first && y == ctr.second)))
			{
				auto t = gdata->_map->getCreature(x, y);
				if (t != nullptr)
					inflictEnergyDamage(gdata, t, randint(dmg.first, dmg.second), dt);
			}
		}
	}
	addAnimation(gdata, anim_Explosion(ctr, r, '#', getDamageTypeColor(dt)));
}



//	Default number of rounds a given status effect lasts for.
int getDefaultStatusEffectDuration(const StatusEffect st)
{
	switch (st)
	{
	case(STATUS_AGONY):		return dieRoll(3, 6);
	case(STATUS_BURN):		return randint(2, 5);
	case(STATUS_POISON):	return 10;
	case(STATUS_SLUDGED):	return 10;
	case(STATUS_SHOCK):		return 10;
	case(STATUS_STAGGER):	return randint(2, 5);
	default:
		return 1;
	}
}


//	Attempts to inflict a status effect with the given % chance.
void tryInflictStatusEffect(gamedataPtr gdata, creaturePtr target, const StatusEffect st, const int odds, const int stacks)
{
	if (roll_percent(odds))
	{
		messages::status_effect(gdata, target, st);
		target->addStatusEffect(st, (stacks == 0) ? getDefaultStatusEffectDuration(st) : stacks);
	}
}


//	Reduce health. Die if health runs out.
void creatureTakeDamage(gamedataPtr gdata, creaturePtr target, int dam)
{
	if (dam > 0)
	{
		//	damage indicator
		target->takeDamage(dam);
		if (target->isDead())
			killCreature(gdata, target);

		//	warning if player is dying
		else if (target->isPlayer() && target->getHealthPercent() <= 20)
			messages::add(gdata, "# *** LOW HEALTH WARNING! ***", { COLOR_NEGATIVE });
	}
}


//	Inflict nonphysical damage. Applies Resistance to the damage taken.
void inflictEnergyDamage(gamedataPtr gdata, creaturePtr target, int dam, const DamageType dt)
{
	//	Apply resistance
	auto res = target->getResistance(dt);
	if (res > 0)
		dam -= (float)dam * (float)res / 100.0f;

	//	Apply remaining damage
	if (dam > 0)
	{
		//	message/animation
		messages::energy_damage(gdata, target, dt, dam);
		addAnimation(gdata, padAnimationFront(anim_FlashGlyph(target->_pos, '%', getDamageTypeColor(dt)), 2));
		addAnimation(gdata, anim_MovingText(target->_pos, to_string(dam), getDamageTypeColor(dt), intpair(randint(-1, 1), randint(-1, 1))));
		
		//	the actual damage
		creatureTakeDamage(gdata, target, dam);

		//	some energy types also confer a status effect
		switch (dt)
		{
		case(DTYPE_ELECTRIC):	tryInflictStatusEffect(gdata, target, STATUS_SHOCK, 5 * dam, dam * 2); break;
		case(DTYPE_FIRE):		tryInflictStatusEffect(gdata, target, STATUS_BURN, 5 * dam, dam); break;
		case(DTYPE_POISON):		tryInflictStatusEffect(gdata, target, STATUS_POISON, 5 * dam, dam); break;
		}
	}
}


//	If the player attacks, their weapon degrades; if they ARE attacked, their armour degrades
void testForDamageEquipment(gamedataPtr gdata, creaturePtr attacker, creaturePtr target)
{
	//	Determine which items might be damaged
	vector<EquipmentSlot> toTest;
	if (attacker->isPlayer())
		toTest = { SLOT_MAIN_HAND, SLOT_OFFHAND };
	else if (target->isPlayer())
		toTest = { SLOT_BODY, SLOT_BOOTS, SLOT_BRACERS, SLOT_GLOVES, SLOT_HELMET, SLOT_OFFHAND, SLOT_SHOULDERS };

	//	Test each one
	for (auto slot : toTest)
	{
		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr && !it->isBroken())
		{
			it->rollDurabilityLoss();
			if (it->isBroken())
			{
				messages::add(gdata, "#" + it->getName() + " breaks!", { COLOR_NEGATIVE });
			}
		}
	}
}


//	Push target away from the given coordinate.
void knockback(gamedataPtr gdata, creaturePtr t, const intpair awayFrom, int dist)
{
	auto vec = get2dVector(awayFrom, t->_pos);
	auto at = t->_pos;
	while (dist-- > 0)
	{
		intpair_add(&at, &vec);
		if (gdata->_map->inBounds(at) && gdata->_map->isPointClear(at))
			t->_pos = at;
		else
			break;
	}
	if (t->isPlayer())
		setPlayerPosition(gdata, at);
}


//	Effects of standing on a tile
void standOnTile(gamedataPtr gdata, creaturePtr cr)
{
	if (!cr->isFlying())
	{
		//	Tile effects
		switch (gdata->_map->getTile(cr->_pos))
		{
		case(MT_WATER):
			cr->cureStatusEffect(STATUS_BURN);
			break;
		}

		//	Surface effects
		auto sf = gdata->_map->getSurface(cr->_pos);
		switch (sf)
		{
		case(Surface::POISON_OOZE):
			inflictEnergyDamage(gdata, cr, randint(2, 5), DTYPE_POISON);
			break;

		case(Surface::SLUDGE):
			tryInflictStatusEffect(gdata, cr, STATUS_SLUDGED, 75, 10);
			break;
		}
	}
}


//	Updates the player's position on the map (moving the FOV, etc)
void setPlayerPosition(gamedataPtr gdata, intpair pt)
{
	gdata->_player->_pos = pt;
	gdata->_map->setFOVMapCentre(pt);
}