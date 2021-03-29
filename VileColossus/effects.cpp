#include "effects.h"



//	Special effects when we kill a boss.
void triggerBossKillEffects(gamedataPtr gdata, monsterPtr mon)
{
	//	turn into down stairs
	if (!mon->hasFlag("abysslord") && !mon->hasFlag("avatar"))
		gdata->_map->setTile(MT_STAIRS_DOWN, mon->_pos);

	//	count up kills
	switch (mon->getType())
	{
	case(MonsterType::BOSS_PALLID_ROTKING):
	case(MonsterType::BOSS_ROTKING_BURNED):
	case(MonsterType::BOSS_ROTKING_DESOLATE):
	case(MonsterType::BOSS_ROTKING_INFUSED):
		gdata->_gameProgress._killedRotking++;
		if (gdata->_gameProgress._killedRotking == 1)
			mapgen::openHellPortal(gdata->_homeBase);
		break;

	case(MonsterType::BOSS_DEMON_LORD):
	case(MonsterType::BOSS_WRETCHED_PRINCE):
		gdata->_gameProgress._killedHellboss++;
		if (gdata->_gameProgress._killedHellboss == 1)
			mapgen::openTemplePortal(gdata->_homeBase);
		break;

	case(MonsterType::BOSS_DROWNED_DOGOSSA):
		gdata->_gameProgress._killedDogossa++;
		gdata->_gameProgress._abyssLevel++;
		break;
	case(MonsterType::BOSS_TOMB_LORD_AMOG):
		gdata->_gameProgress._killedAmog++;
		gdata->_gameProgress._abyssLevel++;
		break;
	case(MonsterType::BOSS_VIRIDIAN_PRINCE):
		gdata->_gameProgress._killedSallowKing++;
		gdata->_gameProgress._abyssLevel++;
		break;

	case(MonsterType::BOSS_VILE_COLOSSUS):
		gdata->_gameProgress._killedColossus++;
		if (gdata->_gameProgress._killedColossus == 1)
		{
			gdata->_state = STATE_GAME_COMPLETED;
			gdata->_victory = true;
		}
		break;
	}
}



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
	int exalt_odds = 0;

	//	At higher level diffs, monsters drop loot as though they were a lower tier.
	int tier = target->_tier;
	const int ldiff = target->_level - gdata->_player->_level;
	if (ldiff < -1)
		tier = MAX(0, tier + ldiff + 1);

	//	Determine drop amounts & quality.
	switch (tier)
	{
	case(0):
		if (roll_one_in(6)) drop_amt++;
		break;

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
		drop_amt = 8 + dieRoll(4, 2);
		exalt_odds = 50;
		break;
	}

	//	does this monster qualify for exaltation?
	bool can_exalt = target->hasFlag("viledragon") || (roll_percent(exalt_odds) && target->_level >= 30);

	//	Get free points in the vicinity.
	auto pts = getAdjacentWalkable(gdata, target->_pos);
	pts.push_back(target->_pos);

	//	Roll the items to drop.
	while (drop_amt-- > 0)
	{
		//	Random point and item
		auto pt = pts[randrange(pts.size())];
		auto it = lootgen::rollItemDrop(lootgen::getLootTierForMonsterLevel(target->_level), rarity, false, (can_exalt && roll_percent(exalt_odds)));
		gdata->_map->addItem(it, pt);

		//	Animation (for special items)
		if (it->_rarity == 4)
			addAnimation(gdata, anim_Spray(pt, { '!', '!', '!', '!', }, TCODColor::orange));
	}


	//	Also drop MONEY.
	drop_amt = (target->_tier - 1) + randint(0, target->_tier);
	while (drop_amt-- > 0)
	{
		int amt = dieRoll(3, target->_level);
		amt = adjustByPercent(amt, gdata->_player->getGreedBonus());
		auto it = lootgen::generateMaterial(MaterialType::FRAGMENTS, amt);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}

	//	TIER 5 MONSTERS may drop SUPER SPECIAL ITEMS.
	if (tier == 5 && roll_one_in(3))
		gdata->_map->addItem(lootgen::generateMaterial(MaterialType::NOTCHED_CUBE, 1), pts[randrange(pts.size())]);


	//	Chance of GEM(s) at higher levels.
	if (target->_level >= 6)
	{
		//	decide how many
		int gems = 0;
		if (target->_tier == 1 && roll_one_in(50))
			gems = 1;
		else if (target->_tier == 2 && roll_one_in(10))
			gems = randint(1, 2);
		else if (target->_tier == 3 && roll_one_in(2))
			gems = randint(1, 2);
		else if (target->_tier == 4)
			gems = randint(1, 3);
		else if (target->_tier == 5)
			gems = randint(1, 4);

		//	generate the gems
		while (gems-- > 0)
		{
			auto tier = randint(1, lootgen::getGemTierForMonsterLevel(target->_level));
			if (tier >= 0)
			{
				auto it = lootgen::generateGem(tier, randint(1, rarity));
				gdata->_map->addItem(it, pts[randrange(pts.size())]);
			}
		}
	}


	//	Abyssal lords drop ritual materials.
	if (target->hasFlag("abysslord"))
	{
		auto it = lootgen::generateMaterial(lootgen::rollRitualMaterial(), 1);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}
	else if (target->hasFlag("avatar"))
	{
		auto it = lootgen::generateMaterial(MaterialType::DEAD_GODS_EYE, 1);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}
}


//	For monsters that have death effects.
void explodeOnDeath(gamedataPtr gdata, monsterPtr target, string flag)
{
	if (flag == "poison_burst")
	{
		fillRegionWithSurface(gdata, target->_pos, 1, Surface::POISON_OOZE);
		addAnimation(gdata, anim_Explosion(target->_pos, 1, '#', TCODColor::lime));
	}
	else if (flag == "fire_burst")
	{
		inflictDamageInRadius(gdata, target->_pos, 2, DTYPE_FIRE, intpair(1, target->_level));
		addAnimation(gdata, anim_Explosion(target->_pos, 2, '#', TCODColor::flame));
	}
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


//	Places a surface in the given place, if that would be valid.
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


	//	Rarity determined by type of chest
	int rarity = 1;
	switch (tl)
	{
	case(MT_CHEST_GLOWING):		rarity = 2; break;
	case(MT_CHEST_LUMINOUS):	rarity = 3; break;
	case(MT_CHEST_RADIANT):		rarity = 4; break;
	}


	//	Chance to get higher-rarity items
	if (roll_one_in(6)) rarity++;


	//	Tier of item
	int quality;
	if		(gdata->_depth < 10)	quality = 1;
	else if (gdata->_depth < 20)	quality = 2;
	else							quality = 3;


	//	Delete the chest
	gdata->_map->setTile(MT_FLOOR_STONE, pt);
	gdata->_map->updateTmapAtPoint(pt.first, pt.second);


	//	Drop items in random nearby spots
	auto pts = getAdjacentWalkable(gdata, pt);
	int amt = randint(quality, 2 * quality);
	while (amt-- > 0)
	{
		auto it = lootgen::rollItemDrop(quality, rarity);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}


	//	And MONEY
	amt = (quality - 1) + randint(0, quality);
	while (amt-- > 0)
	{
		//	determine type
		MaterialType mt = MaterialType::FRAGMENTS;
		int r = randint(1, 100);
		if		(r <= 50 && quality > 1)	mt = MaterialType::MAGIC_DUST;
		else if (r <= 75 && quality > 2)	mt = MaterialType::GLOWING_POWDER;
		else if (r <= 90 && quality > 3)	mt = MaterialType::GLOWING_POWDER;	//	two chances for glowing powder at higher qualities

		//	increase fragment gain
		auto amt = dieRoll(3, 6 + quality);
		if (mt == MaterialType::FRAGMENTS)
			amt = adjustByPercent(amt, gdata->_player->getGreedBonus());

		//	generate the item
		auto it = lootgen::generateMaterial(mt, amt);
		gdata->_map->addItem(it, pts[randrange(pts.size())]);
	}


	//	And GEM(s)?
	if (quality > 1 && roll_one_in(4))
	{
		amt = randint(1, quality);
		const int gtier = randint(1, lootgen::getGemTierForMonsterLevel(6 + quality * 6));
		auto it = lootgen::generateGem(gtier, 1);
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
		//	Death screen.
		gdata->_player->_triggeredDeath = true;
		gdata->_state = STATE_ACKNOWLEDGE_DEATH;
		messages::add(gdata, "# *** YOU DIED ***", { COLOR_NEGATIVE });
		messages::add(gdata, "#Equipment durability reduced!", { COLOR_WARNING });
		messages::add(gdata, "Press ENTER to continue...");

		//	Penalties, if not in permadeath mode
		if (gdata->_mode != GameMode::PERMADEATH)
		{
			//	Damage equipment
			for (auto it : gdata->_player->getAllEquippedItems())
			{
				if (it != nullptr)
				{
					it->reduceMaxDurability(1 + it->_maxDurability / 10);
					it->takePercentDamage(10);
				}
				gdata->_totalDeaths++;
			}

			//	Lose inventory (on normal mode)
			if (gdata->_mode == GameMode::NORMAL)
				gdata->_carriedItems.clear();
		}
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

			//	corpse/death effects
			if (mon->hasFlag("poison_burst"))
				explodeOnDeath(gdata, mon, "poison_burst");
			else if (mon->hasFlag("fire_burst"))
				explodeOnDeath(gdata, mon, "fire_burst");
			else
				tryDropCorpse(gdata, mon->getCorpseType(), mon->_pos);
			if (mon->hasFlag("infested"))
				spawnOnDeath(gdata, mon);

			//	from the putrefaction effect
			if (mon->hasStatusEffect(STATUS_PUTREFIED))
				inflictDamageInRadius(gdata, mon->_pos, 1, DTYPE_POISON, intpair(1, mon->getStatusEffectDuration(STATUS_POISON) * 10), true);

			//	effects on the player
			gdata->_player->healDamage(gdata->_player->getLeechOnKill());
			gdata->_player->restoreMagic(gdata->_player->getManaleech());
			gdata->_player->chargeFlaskOnKill();
			if (roll_percent(gdata->_player->getWrathOnKillChance()))
				gdata->_player->increaseBuff(BUFF_WRATH, 1 + mon->_tier * 2);

			//	special events when we kill a boss
			if (mon->_tier == 5)
				triggerBossKillEffects(gdata, mon);
			gdata->_totalKills++;
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
	case(STATUS_ENTANGLED):	return 3;
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
		//	shock increases damage taken
		if (target->hasStatusEffect(STATUS_SHOCK))
			dam = adjustByPercent(dam, 30);

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
		const int odds = 50 - target->getResistance(dt) / 2;
		switch (dt)
		{
		case(DTYPE_ELECTRIC):	tryInflictStatusEffect(gdata, target, STATUS_SHOCK, odds); break;
		case(DTYPE_FIRE):		tryInflictStatusEffect(gdata, target, STATUS_BURN, odds); break;
		case(DTYPE_POISON):		tryInflictStatusEffect(gdata, target, STATUS_POISON, odds); break;
		}

		//	other special effects
		if (target->isPlayer() && gdata->_player->getTotalEnchantmentBonus(ENCH_CONDUCTING) > 0)
		{
			switch (dt)
			{
			case(DTYPE_ELECTRIC):	gdata->_player->setBuffDuration(BUFF_CONDUCT_ELECTRIC, 10); break;
			case(DTYPE_FIRE):		gdata->_player->setBuffDuration(BUFF_CONDUCT_FIRE, 10); break;
			case(DTYPE_POISON):		gdata->_player->setBuffDuration(BUFF_CONDUCT_POISON, 10); break;
			}
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
		toTest = { SLOT_BODY, SLOT_BOOTS, SLOT_BRACERS, SLOT_GLOVES, SLOT_HELMET, SLOT_SHOULDERS };

	//	Test each one
	for (auto slot : toTest)
	{
		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr && !it->isBroken())
		{
			it->rollDurabilityLoss();
			if (it->isBroken())
				messages::add(gdata, "#" + it->getName() + " breaks!", { COLOR_NEGATIVE });
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
		setPlayerPosition(gdata, t->_pos);
}


//	One creature tries to grapple another.
void grapple(gamedataPtr gdata, creaturePtr grappler, creaturePtr target)
{
	target->verifyGrappler();
	if (!target->isGrappled())
	{
		target->_grappledBy = grappler;
		if (target->isPlayer())
			messages::add(gdata, "#" + grappler->getName() + " @grabs you!", { grappler->getColor() });
	}
}


//	Effects of standing on a tile
void standOnTile(gamedataPtr gdata, creaturePtr cr)
{
	if (!cr->isFlying())
	{
		//	Tile effects
		switch (gdata->_map->getTile(cr->_pos))
		{
		case(MT_LAVA):
			if (cr->isPlayer())
				inflictEnergyDamage(gdata, cr, randint(1, 10), DTYPE_FIRE);
			break;

		case(MT_WATER):
			cr->cureStatusEffect(STATUS_BURN);
			break;
		}

		//	Surface effects
		auto sf = gdata->_map->getSurface(cr->_pos);
		switch (sf)
		{
		case(Surface::FIRE):
			inflictEnergyDamage(gdata, cr, randint(2, 5), DTYPE_FIRE);
			break;

		case(Surface::POISON_OOZE):
			inflictEnergyDamage(gdata, cr, randint(2, 5), DTYPE_POISON);
			break;

		case(Surface::SLUDGE):
			tryInflictStatusEffect(gdata, cr, STATUS_SLUDGED, 75, 10);
			break;

		case(Surface::WEB):
			if (cr->isPlayer())
			{
				tryInflictStatusEffect(gdata, cr, STATUS_ENTANGLED, 100, 2);
				gdata->_map->setSurface(Surface::__NONE, cr->_pos);
			}
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