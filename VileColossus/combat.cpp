#include "combat.h"


//	Animated damage number. 'isGoodDamage' is True if the damage was taken by an enemy, False if it was taken a player or their allies.
//	If 'delay' is greater than 0, the animation is padded with that many empty frames at the beginning, essentially causing it to play later.
void animateHit(gamedataPtr gdata, creaturePtr target, int damage, bool wasCrit, bool isGoodDamage, int delay)
{
	//	color depends on who was hit & whether it was a crit
	colorType col;
	if (isGoodDamage)
		col = wasCrit ? TCODColor::gold : COLOR_WHITE;
	else
		col = COLOR_HIGHLIGHT_NEG;

	//	show the floating number with a random vector
	auto vec = intpair(randint(-1, 1), -1);
	auto anim = anim_MovingText(target->_pos, to_string(damage), col, vec);
	if (delay > 0)
		padAnimationFront(anim, delay);
	addAnimation(gdata, anim);
}


//	Determine damage inflicted by a weapon attack. Has some random variance applied.
int rollWeaponDamage(gamedataPtr gdata, creaturePtr attacker)
{
	auto dam = attacker->getWeaponDamage();
	auto var = attacker->getDamageVariance();
	dam += randint(-var, var);
	return dam;
}


//	Reduce weapon damage based on target's Protection stat.
int applyProtection(gamedataPtr gdata, creaturePtr target, int dam)
{
	auto prot = target->getArmourValue();
	dam = MAX(1, dam - prot);
	return dam;
}


//	Rolls attacker's Accuracy vs. target's Defence. Returns True on hit, False on miss.
bool rollToHit(gamedataPtr gdata, creaturePtr attacker, creaturePtr target)
{
	int acc = attacker->getAccuracy();
	int def = target->getDefenceValue();
	int roll = randint(1, 20);
	//messages::add(gdata, "Attack roll: " + to_string(acc) + " vs " + to_string(def) + ", rolled " + to_string(roll), COLOR_LIGHT);
	return roll == 20 || roll + acc >= def;
}


//	Damage attacker on hit in melee.
void triggerReprisal(gamedataPtr gdata, creaturePtr repriser, creaturePtr target)
{
	auto dam = repriser->getReprisalDamage();
	if (dam > 0)
		creatureTakeDamage(gdata, target, applyProtection(gdata, target, dam));
}


//	Attacker misses; defender has a chance to strike back
void triggerRiposte(gamedataPtr gdata, creaturePtr riposter, creaturePtr target)
{
	if (roll_percent(riposter->getRiposteChance()))
	{
		messages::riposte(gdata, riposter, target);
		attackWithWeapon(gdata, riposter, target, false, riposter->getRiposteDamageMult());
	}
}


//	Default weapon attack against a given target.
//	if 'allowRiposte' is set, target has a chance to riposte on miss.
//	'percentDamageMult' is a multiplier, so 100% means no change to damage inflicted.
//	'flatDamageAdjust' is added directly to damage BEFORE 'percentDamageMult' is applied.
void attackWithWeapon(gamedataPtr gdata, creaturePtr attacker, creaturePtr target, bool allowRiposte, int percentDamageMult, int flatDamageAdjust)
{
	//	Determine number of attacks
	int atks = attacker->getAttacksThisTurn();

	//	Counters for message purposes after combat ends
	vector<int> damageCounts;
	vector<bool> didAttackCrit;


	//	Electrified victims take more damage
	if (target->hasStatusEffect(STATUS_SHOCK))
		percentDamageMult += 20;


	//	Execute each attack until target dies or we run out
	while (atks-- > 0)
	{
		//	Must hit first
		if (rollToHit(gdata, attacker, target))
		{
			//	Damage
			auto dam = rollWeaponDamage(gdata, attacker);

			//	Apply damage adjustments
			dam += flatDamageAdjust;
			if (percentDamageMult != 100)
				dam = (float)dam * (float)percentDamageMult / 100.0f;

			//	Criticals
			bool crit = roll_percent(attacker->getCriticalChance());
			if (crit)
			{
				auto mult = attacker->getCriticalMultiplier();
				dam = adjustByPercent(dam, mult);
			}

			//	Apply damage resistance
			dam = applyProtection(gdata, target, dam);

			//	The SMITE EVIL buff adds massive damage vs certain targets
			if (attacker->hasBuff(BUFF_SMITE_EVIL))
			{
				if (target->isUndead())
					dam += attacker->getSmiteEvilDamage();
				attacker->reduceBuffDuration(BUFF_SMITE_EVIL);
			}

			//	Actually inflict the damage
			creatureTakeDamage(gdata, target, dam);

			//	Additional effects
			tryInflictStatusEffect(gdata, target, STATUS_STAGGER, attacker->getStaggerChance(), attacker->getStaggerAttackDuration());
			for (auto dt : SPECIAL_DAMAGE_TYPES)
			{
				auto amt = attacker->getWeaponDamageOfType(dt);
				amt = adjustByPercent(amt, attacker->getElementalAffinity(dt));
				if (amt > 0)
					inflictEnergyDamage(gdata, target, randint(1, amt), dt);
			}

			//	Player can regain flask charges on hit
			if (attacker->isPlayer())
				gdata->_player->chargeFlaskOnHit();

			//	attack animations
			addAnimation(gdata, anim_FlashGlyph(target->_pos, '/', TCODColor::crimson));
			if (!target->isPlayer())
				animateHit(gdata, target, dam, crit, !target->isPlayer(), atks);
			damageCounts.push_back(dam);
			didAttackCrit.push_back(crit);

			//	reprisal attacks
			if (allowRiposte)
				triggerReprisal(gdata, target, attacker);

			//	player's gear might get damaged
			testForDamageEquipment(gdata, attacker, target);
		}

		//	Missed!
		else
		{
			damageCounts.push_back(-1);
			didAttackCrit.push_back(false);
			if (allowRiposte)
				triggerRiposte(gdata, target, attacker);
		}

		//	Drop out if target dies
		if (target->isDead())
			break;
	}

	//	Attacker expends energy regardless
	messages::attack_string(gdata, attacker, target, damageCounts, didAttackCrit);
	attacker->spendActionEnergy(attacker->getAttackEnergyCost());
}


//	Attack all adjacent enemies. Damage bonus based on no. of enemies.
void triggerCleaveAttack(gamedataPtr gdata, creaturePtr cleaver)
{
	auto targets = getAdjacentEnemies(gdata, gdata->_player);
	auto dbonus = cleaver->getCleaveDamageBonus() * targets.size();
	for (auto t : targets)
		attackWithWeapon(gdata, cleaver, t, true, 100, dbonus);
}


//	Player attempts to attack the given creature in melee.
void playerTriggerMeleeAttack(gamedataPtr gdata, creaturePtr t)
{
	if (gdata->_player->cleaves())
		triggerCleaveAttack(gdata, gdata->_player);
	else
		attackWithWeapon(gdata, gdata->_player, t);
}


//	Make a ranged attack in the given direction.
void playerTriggerRangedAttack(gamedataPtr gdata, const intpair vec)
{
	//	Origin position and point being tested
	auto o = gdata->_player->_pos;
	auto pt = gdata->_player->_pos;
	vector<intpair> pts;
	const int maxdist = MIN(gdata->_player->getMaxAttackRange(), gdata->_player->getVisionRadius());

	//	Trace the path
	while (hypot(pt.first - o.first, pt.second - o.second) <= maxdist)
	{
		//	Walk along the line
		intpair_add(&pt, &vec);
		pts.push_back(pt);
		if (gdata->_map->inBounds(pt) && gdata->_map->isWalkable(pt))
		{
			//	Test for target
			auto t = gdata->_map->getCreature(pt);
			if (t != nullptr)
			{
				attackWithWeapon(gdata, gdata->_player, t);
				break;
			}

			//	Stop if we hit a non-transparent tile
			if (!gdata->_map->isTransparent(pt))
				break;
		}

		//	Stop if we leave the map or hit a non-walkable point
		else
			break;
	}

	//	Animate the shot
	addAnimation(gdata, anim_BulletPath(pts, TCODColor::white));
}


//	Tests that we have a ranged weapon equipped.
void playerTryRangedAttack(gamedataPtr gdata, const intpair vec)
{
	if (gdata->_player->usingRangedWeapon())
		playerTriggerRangedAttack(gdata, vec);
	else
		messages::error(gdata, "You don't have a ranged weapon equipped!");
}
