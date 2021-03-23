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


//	Special effects when monster hits u
void monsterSpecialHitEffects(gamedataPtr gdata, monsterPtr attacker, creaturePtr target, int damage)
{
	if (attacker->hasFlag("vampiric"))
	{
		attacker->healDamage(attacker->getMaxHealth() / 4);
		attacker->adjustMaxHealth(attacker->_level * 2);
		attacker->adjustDamage(attacker->_level / 2);
	}
	if (attacker->hasFlag("grapples"))
		grapple(gdata, attacker, target);
}


//	On hit, chance to trigger spell castings.
void playerTestForSpellTriggerOnHit(gamedataPtr gdata, creaturePtr target)
{
	int lvl;

	//	MAGEBLOOD
	lvl = gdata->_player->getTotalEnchantmentBonus(ENCH_MAGEBLOOD);
	if (lvl > 0 && roll_percent(20))
		triggerSpellEffect(gdata, gdata->_player, Spell::ARCANE_EMANATION, lvl);

	//	SKYPLITTER
	lvl = gdata->_player->getTotalEnchantmentBonus(ENCH_SKYSPLITTER);
	if (lvl > 0 && roll_percent(20))
		triggerSpellEffect(gdata, gdata->_player, Spell::CALL_LIGHTNING, lvl);
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
bool rollToHit(gamedataPtr gdata, creaturePtr attacker, creaturePtr target, const int bns_acc)
{
	int acc = attacker->getAccuracy() + bns_acc;
	int def = target->getDefenceValue();
	int roll = randint(1, 20);
	return roll >= 17 || roll + acc >= def;
}


//	Damage attacker on hit in melee.
void triggerReprisal(gamedataPtr gdata, creaturePtr repriser, creaturePtr target)
{
	//	default reprisal
	auto dam = repriser->getReprisalDamage();
	if (dam > 0)
		creatureTakeDamage(gdata, target, applyProtection(gdata, target, dam));
		
	//	player might inflict special reprisal damage
	if (repriser->isPlayer())
	{
		const vector<ItemEnchantment> ops = { ENCH_FLAMESPIKE, ENCH_THUNDERSPIKE, ENCH_VENOMSPIKE };
		for (auto en : ops)
		{
			const int bns = gdata->_player->getTotalEnchantmentBonus(en);
			if (bns > 0)
			{
				//	Determine damage type of the spikes
				DamageType dt = DTYPE_NORMAL;
				switch (en)
				{
				case(ENCH_FLAMESPIKE):		dt = DTYPE_FIRE; break;
				case(ENCH_THUNDERSPIKE):	dt = DTYPE_ELECTRIC; break;
				case(ENCH_VENOMSPIKE):		dt = DTYPE_POISON; break;
				}

				//	Apply affinity bonus and inflict
				inflictEnergyDamage(gdata, target, adjustByPercent(bns, gdata->_player->getElementalAffinity(dt)), dt); break;
			}
		}
	}
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
//	if 'spendEnergy' is set, attacker expends attack energy.
void attackWithWeapon(gamedataPtr gdata, creaturePtr attacker, creaturePtr target, bool allowRiposte, int percentDamageMult, int flatDamageAdjust, bool spendEnergy)
{
	//	Determine number of attacks
	int atks = attacker->getAttacksThisTurn();

	//	Counters for message purposes after combat ends
	vector<int> damageCounts;
	vector<bool> didAttackCrit;


	//	Execute each attack until target dies or we run out
	while (atks-- > 0)
	{
		//	Must hit first
		if (rollToHit(gdata, attacker, target) || (gdata->_autokill && attacker->isPlayer()))
		{
			//	Damage
			auto dam = rollWeaponDamage(gdata, attacker);

			//	Apply damage adjustments
			dam += flatDamageAdjust;
			if (percentDamageMult != 100)
				dam = adjustByPercent(dam, percentDamageMult);

			//	Criticals
			bool crit = roll_percent(attacker->getCriticalChance());
			if (crit)
			{
				auto mult = attacker->getCriticalMultiplier();
				dam = adjustByPercent(dam, mult);
				attacker->removeBuff(BUFF_CRIT_BONUS);
			}

			//	Kinslayer buff increases the player's crit chance when they don't crit
			else if (attacker->isPlayer())
				gdata->_player->addBuff(BUFF_CRIT_BONUS, gdata->_player->getTotalEnchantmentBonus(ENCH_KINSLAYER));

			//	Apply damage resistance
			dam = applyProtection(gdata, target, dam);

			//	The SMITE EVIL buff adds massive damage vs certain targets
			if (attacker->hasBuff(BUFF_SMITE_EVIL))
			{
				if (target->isUndead())
					dam += attacker->getSmiteEvilDamage();
				attacker->reduceBuffDuration(BUFF_SMITE_EVIL);
			}

			//	Player-only special effects
			if (attacker->isPlayer())
			{
				//	Divine enchantment
				if (target->isUndead())
					dam += gdata->_player->getVisionRadius() * gdata->_player->getTotalEnchantmentBonus(ENCH_DIVINE);

				//	Spell triggers
				playerTestForSpellTriggerOnHit(gdata, target);

				//	Special crit effects
				if (crit)
				{
					//	Blackblood damage
					auto pdam = gdata->_player->getTotalEnchantmentBonus(ENCH_BLACKBLOOD);
					if (pdam > 0)
						inflictEnergyDamage(gdata, target, adjustByPercent(pdam, gdata->_player->getElementalAffinity(DTYPE_POISON)), DTYPE_POISON);

					//	Extra flask charge
					if (gdata->_player->_currentFlask->hasEnchantment(ENCH_CRUCIBLE))
						gdata->_player->chargeFlaskByFixedPercent(gdata->_player->_currentFlask->getEnchantmentValue(ENCH_CRUCIBLE));
				}
			}

			//	ETHEREAL reduces damage by 50%
			if (target->isEthereal())
				dam /= 2;

			//	Actually inflict the damage
			creatureTakeDamage(gdata, target, dam);

			//	Additional effects
			tryInflictStatusEffect(gdata, target, STATUS_STAGGER, attacker->getStaggerChance(), randint(3, 6));
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

			//	Knockback chance
			if (roll_percent(attacker->getKnockbackChance()))
				knockback(gdata, target, attacker->_pos, 2);

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

			//	monster special abilities
			if (!attacker->isPlayer())
				monsterSpecialHitEffects(gdata, static_pointer_cast<monster>(attacker), target, dam);

			//	if the AUTOKILL debug flag is set, we instantly kill whatever we hit
			if (gdata->_autokill && attacker->isPlayer())
				creatureTakeDamage(gdata, target, 1000000);
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
	if (spendEnergy)
		attacker->spendActionEnergy(attacker->getAttackEnergyCost());
}


//	Attack all adjacent enemies. Damage bonus based on no. of enemies.
void triggerCleaveAttack(gamedataPtr gdata, creaturePtr cleaver)
{
	auto targets = getAdjacentEnemies(gdata, gdata->_player);
	auto dbonus = cleaver->getCleaveDamageBonus() * (targets.size() - 1);
	for (auto t : targets)
		attackWithWeapon(gdata, cleaver, t, true, 100, dbonus, false);
	cleaver->spendActionEnergy(cleaver->getAttackEnergyCost());
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
		if (gdata->_map->inBounds(pt))
		{
			//	Test for target
			auto t = gdata->_map->getCreature(pt);
			if (t != nullptr)
			{
				attackWithWeapon(gdata, gdata->_player, t);
				break;
			}

			//	Break smashable tiles
			else if (isMaptileBreakable(gdata->_map->getTile(pt)))
			{
				addAnimation(gdata, anim_FlashGlyph(pt, '!', getMaptileColor(gdata->_map->getTile(pt))));
				gdata->_map->setTile(MT_RUBBLE, pt);
				gdata->_map->updateTmapAtPoint(pt.first, pt.second);
				gdata->_player->spendActionEnergy(gdata->_player->getAttackEnergyCost());
				break;
			}

			//	Stop if we hit a non-transparent tile
			else if (!gdata->_map->isTransparent(pt) || !gdata->_map->isWalkable(pt))
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


//	Find all monsters within attack range, and attack one. Prioritizes the monster with the lowest health remaining.
void playerTryAutoattack(gamedataPtr gdata)
{
	auto mlist = getAdjacentEnemies(gdata, gdata->_player);
	if (!mlist.empty())
	{
		//	Find the highest-tier adjacent monster.
		monsterPtr t = static_pointer_cast<monster>(mlist[0]);
		for (unsigned i = 1; i < mlist.size(); i++)
		{
			auto m = static_pointer_cast<monster>(mlist[i]);
			if (m->getHealthLeft() < t->getHealthLeft())
				t = m;
		}

		//	Attack them.
		playerTriggerMeleeAttack(gdata, t);
	}
	else
		messages::error(gdata, "No adjacent monsters to attack!");
}
