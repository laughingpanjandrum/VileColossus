#include "ai.h"


//	Move to the designated point.
void ai::executeMove(gamedataPtr gdata, monsterPtr ai, intpair to)
{
	ai->_pos = to;
	ai->spendActionEnergy(ai->getMoveEnergyCost());
}


//	Returns True if we move successfully.
bool ai::moveToPoint(gamedataPtr gdata, monsterPtr ai, intpair toPt)
{
	auto bestPt = getMoveNearerToPoint(gdata, ai, ai->_pos, toPt);
	if (bestPt != ai->_pos)
	{
		ai::executeMove(gdata, ai, bestPt);
		return true;
	}
	return false;
}


//	Returns True if we move successfully.
bool ai::moveAwayFromPoint(gamedataPtr gdata, monsterPtr ai, intpair awayFrom)
{
	auto bestPt = getMoveFurtherFromPoint(gdata, ai, ai->_pos, awayFrom);
	if (bestPt != ai->_pos)
	{
		ai::executeMove(gdata, ai, bestPt);
		return true;
	}
	return false;
}


//	Cast the given spell at the given target.
bool ai::castRaySpell(gamedataPtr gdata, monsterPtr ai, creaturePtr target, const string spell)
{
	//	test accuracy
	bool is_bullet_anim = true;
	auto anim_col = COLOR_WHITE;

	if (rollToHit(gdata, ai, target, ai->_level / 4))
	{
		if (spell == "arcane_bolt")
		{
			anim_col = getDamageTypeColor(DTYPE_ARCANE);
			inflictEnergyDamage(gdata, target, randint(1, ai->getWeaponDamage()), DTYPE_ARCANE);
		}
		else if (spell == "firebolt")
		{
			anim_col = getDamageTypeColor(DTYPE_FIRE);
			inflictEnergyDamage(gdata, target, dieRoll(3, ai->_level), DTYPE_FIRE);
		}
		else if (spell == "fireblast")
		{
			anim_col = getDamageTypeColor(DTYPE_FIRE); is_bullet_anim = false;
			inflictEnergyDamage(gdata, target, ai->getWeaponDamage(), DTYPE_FIRE);
			trySetSurface(gdata, target->_pos, Surface::FIRE);
		}
		else if (spell == "lightning")
		{
			anim_col = getDamageTypeColor(DTYPE_ELECTRIC);
			inflictEnergyDamage(gdata, target, dieRoll(2, ai->getWeaponDamage() / 2), DTYPE_ELECTRIC);
		}
		else if (spell == "poison_spit")
		{
			anim_col = getDamageTypeColor(DTYPE_POISON); is_bullet_anim = true;
			inflictEnergyDamage(gdata, target, randint(1, ai->getWeaponDamage()), DTYPE_POISON);
			trySetSurface(gdata, target->_pos, Surface::POISON_OOZE);
		}
		else if (spell == "sludge")
		{
			anim_col = TCODColor::lightSepia; is_bullet_anim = false;
			trySetSurface(gdata, target->_pos, Surface::SLUDGE);
		}
	}

	//	attack animation
	auto pts = getBresenhamLine(ai->_pos, target->_pos);
	if (is_bullet_anim)
		addAnimation(gdata, anim_BulletPath(pts, anim_col));
	else
		addAnimation(gdata, anim_Projectile(pts, '*', anim_col));

	return true;
}


//	Surround victim with ice.
void ai::castIceWall(gamedataPtr gdata, monsterPtr ai)
{
	for (auto pt : getAdjacentFreePoints(gdata, ai->_target, ai->_target->_pos))
	{
		if (roll_one_in(2))
			gdata->_map->setTile(MT_WALL_ICE, pt);
	}
}


//	Random point nearby for teleporting monsters.
intpair ai::getTeleportPoint(gamedataPtr gdata, monsterPtr ai)
{
	vector<intpair> options;
	const int r = 5;
	const intpair ctr = ai->_pos;
	for (int x = ctr.first - r; x <= ctr.first + r; x++)
	{
		for (int y = ctr.second - r; y <= ctr.second + r; y++)
		{
			if (gdata->_map->inBounds(x, y) && gdata->_map->isPointClear(x, y))
				options.push_back(intpair(x, y));
		}
	}
	if (!options.empty())
		return options[randrange(options.size())];
	else
		return ai->_pos;
}


//	Spawn something adjacent to us.
bool ai::spawnMinion(gamedataPtr gdata, monsterPtr spawner, const MonsterType spawn_id, const intpair pt)
{
	//	Find an adjacent point to spawn.
	auto pts = getAdjacentFreePoints(gdata, spawner, pt);
	if (!pts.empty())
	{
		//	Generate and place monster.
		auto mon = monsterdata::generate(spawn_id, spawner->_level);
		mon->addFlag("minion");
		gdata->_map->addCreature(mon, pts[randrange(pts.size())]);

		//	AI spends costs.
		spawner->spendActionEnergy();
		spawner->_spawnDelay = mon->hasFlag("megaspawner") ? randint(3, 6) : randint(6, 12);
		return true;
	}
	return false;
}


//	Checks to see if we CAN spawn something, & if we WANT to.
bool ai::trySpawn(gamedataPtr gdata, monsterPtr ai)
{
	if (ai->hasFlag("spawner"))
	{
		//	We can only spawn on a timer.
		if (ai->_spawnDelay > 0)
			ai->_spawnDelay--;

		else
		{
			auto id = ai->rollMonsterToSpawn();
			if (id != MonsterType::__NONE)
				return spawnMinion(gdata, ai, id, ai->_pos);
		}
	}
	return false;
}


//	Returns True if we decide to cast a special ability.
bool ai::tryUseAbility(gamedataPtr gdata, monsterPtr ai)
{
	//	Use abilities 50% of the time
	if (roll_one_in(2))
	{
		//	Visibility tests
		auto dist = getDistanceBetweenCreatures(ai, ai->_target);
		auto vis = canMonsterSeeCreature(gdata, ai, ai->_target);

		//	Ray spells
		auto spell = ai->rollRaySpellToCast();
		if (!spell.empty() && vis && dist <= 8)
			return castRaySpell(gdata, ai, ai->_target, spell);

		//	Explode automatically lol
		else if ((ai->hasFlag("poison_burst") || ai->hasFlag("fire_burst")) && dist < 3 && roll_one_in(3))
			explode(gdata, ai);

		//	FIRESTORM
		else if (ai->hasFlag("casts_firestorm"))
		{
			radiateSpellDamage(gdata, ai, ai->_pos, 8, Spell::FIRESTORM, ai->_level / 2);
			return true;
		}

		//	Bonetoss
		else if (ai->hasFlag("throws_bones") && dist <= 6 && vis)
		{
			if (rollToHit(gdata, ai, ai->_target))
			{
				addAnimation(gdata, anim_Projectile(getBresenhamLine(ai->_pos, ai->_target->_pos), '*', TCODColor::lightYellow));
				creatureTakeDamage(gdata, ai->_target, applyProtection(gdata, ai->_target, randint(1, ai->_level)));
			}
			spawnMinion(gdata, ai, MonsterType::SKULL_FLOATING, ai->_target->_pos);
			ai->spendActionEnergy();
			return true;
		}

		//	Spray webs
		else if (ai->hasFlag("webs") && dist <= 6 && vis && !ai->_target->hasStatusEffect(STATUS_ENTANGLED))
		{
			addAnimation(gdata, anim_Projectile(getBresenhamLine(ai->_pos, ai->_target->_pos), '%', TCODColor::white));
			for (auto pt : getAdjacentWalkable(gdata, ai->_target->_pos))
			{
				if (roll_one_in(2))
					trySetSurface(gdata, pt, Surface::WEB);
			}
			ai->spendActionEnergy();
			return true;
		}

		//	Random teleport
		else if (ai->hasFlag("teleports") && roll_one_in(4))
		{
			auto pt = getTeleportPoint(gdata, ai);
			addAnimation(gdata, anim_FlashGlyph(ai->_pos, '*', TCODColor::purple));
			addAnimation(gdata, anim_FlashGlyph(pt, '*', TCODColor::purple));
			ai->_pos = pt;
			ai->spendActionEnergy();
			return true;
		}

		//	Ice wall
		else if (ai->hasFlag("casts_ice_wall") && roll_one_in(10))
		{
			castIceWall(gdata, ai);
			return true;
		}
	}
	return false;
}


//	Tests to see if we have a ranged attack & want to use it.
//	IF we do make a ranged attack, returns True.
bool ai::tryRangedAttack(gamedataPtr gdata, monsterPtr ai)
{
	if (ai->usingRangedWeapon() && canMonsterSeeCreature(gdata, ai, ai->_target))
	{
		auto dist = getDistanceBetweenCreatures(ai, ai->_target);
		if (dist > 1 && dist < ai->getMaxAttackRange() && roll_one_in(2))
		{
			addAnimation(gdata, anim_BulletPath(getBresenhamLine(ai->_pos, ai->_target->_pos), TCODColor::yellow));
			attackWithWeapon(gdata, ai, ai->_target, false);
			return true;
		}
	}
	return false;
}


//	Self-destruct; has some kind of special effect, you'd expect.
void ai::explode(gamedataPtr gdata, monsterPtr ai)
{
	//	Die.
	killCreature(gdata, ai);

	//	Effect.
	if (ai->hasFlag("poison_burst"))
		explodeOnDeath(gdata, ai, "poison_burst");
	else if (ai->hasFlag("fire_burst"))
		explodeOnDeath(gdata, ai, "fire_burst");
}


//	Controller function for when we are fighting something.
void ai::doCombatAction(gamedataPtr gdata, monsterPtr ai)
{
	auto dist = getDistanceBetweenCreatures(ai, ai->_target);

	//	If we can see our target, we mark their position.
	if (canMonsterSeeCreature(gdata, ai, ai->_target))
		ai->_targetPt = ai->_target->_pos;


	//	Spawn something?
	if (trySpawn(gdata, ai))
		return;


	//	Cast a spell?
	if (tryUseAbility(gdata, ai))
	{
		ai->spendActionEnergy();
		return;
	}

	//	Make a ranged attack?
	if (tryRangedAttack(gdata, ai))
		return;


	//	Chance to flit, if that's our type
	if (!ai->hasFlag("immobile") && (ai->hasFlag("flits") || ai->hasStatusEffect(STATUS_STAGGER)) && roll_one_in(3))
	{
		moveToPoint(gdata, ai, getValidConfusionMove(gdata, ai, ai->_pos));
		return;
	}


	//	Can we melee our target?
	auto melee_ran = 1.5;
	
	//	test distance (we also need a straight line)
	if (dist < melee_ran && !ai->hasFlag("no_attack"))
	{
		//	Perform an attack.
		attackWithWeapon(gdata, ai, ai->_target);

		//	BERSERKERS gain damage every time they attack.
		if (ai->hasFlag("berserker"))
			ai->adjustDamage(ai->_level / 2);

		return;
	}


	//	Can we move?
	if (!ai->hasFlag("immobile"))
		moveToPoint(gdata, ai, ai->_targetPt);
}


//	Returns true if we can target the given creature (we have to be able to see them)
bool ai::canTargetCreature(gamedataPtr gdata, monsterPtr ai, creaturePtr t)
{
	return hypot(ai->_pos.first - t->_pos.first, ai->_pos.second - t->_pos.second) <= AI_MAX_DETECTION_RADIUS;
}


//	Checks whether we want to continue targeting our current target; clears target if so.
void ai::checkTargetValidity(gamedataPtr gdata, monsterPtr ai)
{
	if (ai->_target != nullptr)
	{
		if ((ai->_target->isDead()))
		{
			ai->_target = nullptr;
			ai->_targetPt.first = -1;
		}
	}
}


//	AI will now try to kill this creature.
void ai::setTarget(gamedataPtr gdata, monsterPtr ai, creaturePtr t)
{
	ai->_target = t;
	ai->_targetPt = t->_pos;
}



//	Looks for a target that is NOT the player.
void ai::findMonsterTarget(gamedataPtr gdata, monsterPtr ai)
{
	for (auto cr : gdata->_map->getAllCreatures())
	{
		if (ai::canTargetCreature(gdata, ai, cr))
		{
			setTarget(gdata, ai, cr);
			return;
		}
	}
}


//	Look for something we would like to kill.
void ai::findTarget(gamedataPtr gdata, monsterPtr ai)
{
	//	if we can see the player, we target the player.
	if (canMonsterSeeCreature(gdata, ai, gdata->_player))
		setTarget(gdata, ai, gdata->_player);
}



//	AI turns loop through here.
void ai::takeTurn(gamedataPtr gdata, monsterPtr ai)
{
	//	Do we need a target?
	if (!ai->hasTarget())
		findTarget(gdata, ai);

	//	Did we get a target?
	if (ai->hasTarget())
		ai::doCombatAction(gdata, ai);

	//	If we still have extra energy, pass our turn
	while (ai->hasActionEnergyAboveThreshold())
		ai->spendActionEnergy();
}
