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
	auto pts = getBresenhamLine(ai->_pos, target->_pos);
	if (spell == "arcane_bolt")
	{
		addAnimation(gdata, anim_BulletPath(pts, getDamageTypeColor(DTYPE_ARCANE)));
		inflictEnergyDamage(gdata, target, randint(1, ai->getWeaponDamage()), DTYPE_ARCANE);
		return true;
	}
	else if (spell == "lightning")
	{
		addAnimation(gdata, anim_BulletPath(pts, getDamageTypeColor(DTYPE_ELECTRIC)));
		inflictEnergyDamage(gdata, target, dieRoll(2, ai->getWeaponDamage() / 2), DTYPE_ELECTRIC);
		return true;
	}
	else if (spell == "sludge")
	{
		addAnimation(gdata, anim_Projectile(pts, '~', TCODColor::lightSepia));
		trySetSurface(gdata, target->_pos, Surface::SLUDGE);
		return true;
	}
	return false;
}


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
		spawner->_spawnDelay = randint(6, 12);
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
			{
				return spawnMinion(gdata, ai, id, ai->_pos);
			}
		}
	}
	return false;
}

//	Returns True if we decide to cast a special ability.
bool ai::tryUseAbility(gamedataPtr gdata, monsterPtr ai)
{
	//	Chance to cast a ray spell, if we know one
	if (roll_one_in(2))
	{
		//	Ray spells
		auto spell = ai->rollRaySpellToCast();
		if (!spell.empty() && getDistanceBetweenCreatures(ai, ai->_target) <= 8 && canMonsterSeeCreature(gdata, ai, ai->_target))
			return castRaySpell(gdata, ai, ai->_target, spell);

		//	Explode automatically lol
		else if ((ai->hasFlag("poison_burst") || ai->hasFlag("fire_burst")) && getDistanceBetweenCreatures(ai, ai->_target) < 3 && roll_one_in(3))
			explode(gdata, ai);

		//	Bonetoss
		else if (ai->hasFlag("throws_bones") && getDistanceBetweenCreatures(ai, ai->_target) <= 6 && canMonsterSeeCreature(gdata, ai, ai->_target))
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
		attackWithWeapon(gdata, ai, ai->_target);
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
