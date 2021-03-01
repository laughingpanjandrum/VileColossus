#include "manageMagic.h"



void openSpellList(gamedataPtr gdata)
{
	gdata->_idx = 0;
	gdata->_state = STATE_SPELL_LIST;
}


//	Move in this direction until we hit an obstacle or run out of distance.
void blinkInDirection(gamedataPtr gdata, creaturePtr caster, const intpair vec, const int dist)
{
	//	Find the final blink point
	vector<intpair> pts;
	auto at = caster->_pos;
	for (unsigned i = 0; i < dist; i++)
	{
		auto last = at;
		intpair_add(&at, &vec);
		if (gdata->_map->inBounds(at) && gdata->_map->isPointClear(at))
			pts.push_back(at);
		else
		{
			at = last;
			break;
		}
	}

	//	Move there and animate
	if (!pts.empty())
	{
		addAnimation(gdata, anim_Projectile(pts, '*', TCODColor::fuchsia));
		if (caster->isPlayer())
			setPlayerPosition(gdata, at);
	}
}


//	Damage recursively jumps to nearby targets. It's possible for it to jump back and forth between the same two
//	(though it will never just cycle across the same one, nor can it target the player)
void chainDamage(gamedataPtr gdata, creaturePtr t, const DamageType dtype, const intpair drange, int chains_left)
{
	//	Make a list of nearby candidates.
	const int r = 3;
	const intpair ctr = t->_pos;
	vector<creaturePtr> targets;
	for (int x = ctr.first - r; x <= ctr.first + r; x++)
	{
		for (int y = ctr.second - r; y <= ctr.second + r; y++)
		{
			if (gdata->_map->inBounds(x, y))
			{
				auto cr = gdata->_map->getCreature(x, y);
				if (cr != nullptr && cr != t && !cr->isPlayer())
					targets.push_back(cr);
			}
		}
	}

	//	If we found a target, they take damage, and we jump again.
	if (!targets.empty())
	{
		auto cr = targets[randrange(targets.size())];
		inflictEnergyDamage(gdata, cr, randint(drange.first, drange.second), dtype);
		addAnimation(gdata, anim_BulletPath(getBresenhamLine(ctr, cr->_pos), getDamageTypeColor(dtype)));

		//	Chain again, unless we ran out.
		if (chains_left > 1)
			chainDamage(gdata, cr, dtype, drange, chains_left - 1);
	}
}


//	Radiates a spell effect outward from the given point. All creatures within the range are affected. (The caster is not.)
void radiateSpellDamage(gamedataPtr gdata, creaturePtr caster, const intpair ctr, const int r, const Spell sp, const int lvl)
{
	for (unsigned x = ctr.first - r; x <= ctr.first + r; x++)
	{
		for (unsigned y = ctr.second - r; y <= ctr.second + r; y++)
		{
			if (gdata->_map->inBounds(x, y))
			{
				auto t = gdata->_map->getCreature(x, y);
				if (t != nullptr && t != caster)
					hitTargetWithSpell(gdata, caster, t, sp, lvl);
			}
		}
	}
}


//	Like Blink, but can pass through creatures. Cannot end on the same square as a creature, however!
void teleportInDirection(gamedataPtr gdata, creaturePtr caster, const intpair vec, const int dist)
{
	//	Find the final blink point
	vector<intpair> pts;
	auto at = caster->_pos;
	for (unsigned i = 0; i < dist; i++)
	{
		auto last = at;
		intpair_add(&at, &vec);
		if (gdata->_map->inBounds(at) && gdata->_map->isWalkable(at) && gdata->_map->isTransparent(at))
			pts.push_back(at);
		else
		{
			at = last;
			break;
		}
	}

	//	Walk backwards through the points, erasing each one that contains a creature.
	//	This way the path ACTUALLY ends at the furthest point NOT containing a creature.
	while (!pts.empty())
	{
		if (gdata->_map->getCreature(pts.back()) == nullptr)
			break;
		else
			pts.pop_back();
	}

	//	The actual teleport.
	if (!pts.empty())
	{
		addAnimation(gdata, anim_Projectile(pts, '*', TCODColor::fuchsia));
		if (caster->isPlayer())
			setPlayerPosition(gdata, at);
	}
}


//	For spells with random targets.
creaturePtr findRandomSpellTarget(gamedataPtr gdata, creaturePtr caster)
{
	if (caster->isPlayer())
	{
		if (!gdata->_visibleMonsters.empty())
			return gdata->_visibleMonsters[randrange(gdata->_visibleMonsters.size())];
		else
			return nullptr;
	}
	else
		return gdata->_player;
}



//	Target is affected by a damage spell.
void hitTargetWithSpell(gamedataPtr gdata, creaturePtr caster, creaturePtr target, const Spell sp, const int lvl)
{
	//	Make sure a target was actually given.
	if (target != nullptr)
	{
		//	Determine damage.
		auto drange = getSpellDamage(sp, lvl);
		auto spellpower = caster->getSpellPower();
		drange.first += (float)drange.first * (float)spellpower / 100.0f;
		drange.second += (float)drange.second * (float)spellpower / 100.0f;
		auto dam = randint(drange.first, drange.second);

		//	Apply damage
		auto dtype = getSpellDamageType(sp);
		dam = adjustByPercent(dam, caster->getElementalAffinity(dtype));
		inflictEnergyDamage(gdata, target, dam, dtype);

		//	Special effects
		if (sp == Spell::FIREBALL)
			inflictDamageInRadius(gdata, target->_pos, 2, DTYPE_FIRE, intpair(1, dam));
		else if (sp == Spell::CHAIN_LIGHTNING)
			chainDamage(gdata, target, dtype, intpair(1, dam), lvl);

		//	Animation
		switch (sp)
		{
		case(Spell::ARCANE_EMANATION):
			addAnimation(gdata, anim_BulletPath(getBresenhamLine(caster->_pos, target->_pos), getDamageTypeColor(dtype)));
			break;

		case(Spell::CALL_LIGHTNING):
			addAnimation(gdata, anim_FlashGlyph(target->_pos, '!', getDamageTypeColor(dtype)));
			break;

		case(Spell::CONJURE_FLAME):
			addAnimation(gdata, anim_Projectile(getBresenhamLine(caster->_pos, target->_pos), '*', getDamageTypeColor(dtype)));
			break;

		case(Spell::CRYSTAL_SPEAR):
			addAnimation(gdata, anim_BulletPath(getBresenhamLine(caster->_pos, target->_pos), getDamageTypeColor(dtype)));
			break;

		case(Spell::FIREBALL):
			addAnimation(gdata, anim_Projectile(getBresenhamLine(caster->_pos, target->_pos), '*', getDamageTypeColor(dtype)));
			addAnimation(gdata, padAnimationFront(anim_Explosion(target->_pos, 2, '#', TCODColor::flame), 5));
			break;
		}
	}
}


//	Effects of the given spell take place.
void triggerSpellEffect(gamedataPtr gdata, creaturePtr caster, const Spell sp, const int lvl)
{
	switch (sp)
	{
	case(Spell::ARCANE_EMANATION):
	case(Spell::CALL_LIGHTNING):
		hitTargetWithSpell(gdata, caster, findRandomSpellTarget(gdata, caster), sp, lvl);
		break;

	case(Spell::ARCANE_PULSE):
		caster->setBuffDuration(BUFF_ARCANE_PULSE, getSpellDuration(sp, lvl));
		break;

	case(Spell::SMITE_EVIL):
		caster->setBuffDuration(BUFF_SMITE_EVIL, getSpellDuration(sp, lvl));
		break;

	case(Spell::STATIC_FIELD):
		caster->setBuffDuration(BUFF_STATIC_FIELD, getSpellDuration(sp, lvl));
		break;

	case(Spell::TOXIC_RADIANCE):
		caster->setBuffDuration(BUFF_TOXIC_RADIANCE, getSpellDuration(sp, lvl));
		radiateSpellDamage(gdata, caster, caster->_pos, getSpellRange(sp, lvl), sp, lvl);
		break;

	case(Spell::VENOMFANG):
		caster->addBuff(BUFF_VENOMFANG, getSpellDuration(sp, lvl));
		break;
	}
}



//	Cast the spell we're aiming in the given direction.
void playerCastAimedSpell(gamedataPtr gdata, const intpair vec)
{
	//	spell data
	auto sp = gdata->_player->getAllSpellsKnown()[gdata->_aimingSpellIdx];
	auto lvl = gdata->_player->getSpellLevel(sp);

	//	fire the spell in the chosen direction
	auto at = gdata->_player->_pos;
	int max_range = MIN(getSpellRange(sp, lvl), gdata->_player->getVisionRadius());

	//	Special spell effects
	if (sp == Spell::BLINK)
		blinkInDirection(gdata, gdata->_player, vec, max_range);
	else if (sp == Spell::TELEPORT)
		teleportInDirection(gdata, gdata->_player, vec, max_range);

	//	Standard spell effects
	else
	{
		while (max_range-- > 0)
		{
			intpair_add(&at, &vec);
			if (gdata->_map->inBounds(at))
			{
				//	did we hit someone?
				auto t = gdata->_map->getCreature(at);
				if (t != nullptr)
				{
					hitTargetWithSpell(gdata, gdata->_player, t, sp, lvl);
					if (sp != Spell::CRYSTAL_SPEAR)
						break;
				}

				//	if not, did we hit an obstacle?
				else if (!gdata->_map->isTransparent(at) || !gdata->_map->isWalkable(at))
					break;
			}
		}
	}

	//	pay spell costs
	playerSpendSpellCost(gdata, sp, lvl);
	gdata->_state = STATE_NORMAL;
}



//	Cast the given spell. Untargeted spells take effect instantly; other spells might require targeting.
void playerCastSpell(gamedataPtr gdata, const int sp_idx)
{
	//	Get spell details.
	auto sp = gdata->_player->getAllSpellsKnown()[sp_idx];
	auto lvl = gdata->_player->getSpellLevel(sp);

	//	Do we have enough mana?
	if (gdata->_player->hasMagicForSpell(sp, lvl))
	{
		//	Trigger spell automatically
		if (!isSpellTargeted(sp))
		{
			triggerSpellEffect(gdata, gdata->_player, sp, lvl);
			playerSpendSpellCost(gdata, sp, lvl);
		}

		//	We need to aim the spell
		else
		{
			gdata->_state = STATE_AIMING_SPELL;
			gdata->_aimingSpellIdx = sp_idx;
			messages::add(gdata, "Enter a direction to cast #" + getSpellName(sp), { getSpellColor(sp) });
		}
	}
	else
		messages::error(gdata, "Not enough magic to cast this spell!");
}


//	Spend the MP and time required to cast a spell, plus any other costs.
void playerSpendSpellCost(gamedataPtr gdata, const Spell sp, const int lvl)
{
	//	usual costs
	auto mp = getSpellMPCost(sp, lvl);
	gdata->_player->expendMagic(mp);
	gdata->_player->spendActionEnergy();

	//	spellburn, if any
	if (gdata->_player->getTotalEnchantmentBonus(ENCH_SPELLBURN) > 0)
		creatureTakeDamage(gdata, gdata->_player, mp * 2);
}



//	Attempt to cast the spell at the given hotkey index.
//	If no spell is equipped there, or an invalid hotkey is given, nothing happens.
void castSpellFromHotkey(gamedataPtr gdata, const int hotkey)
{
	if (hotkey < gdata->_player->_ImprintedRunes.size())
		playerCastSpell(gdata, hotkey);
}


//	Level up runes.
void openRuneEnhancer(gamedataPtr gdata)
{
	populateCurrentItemListWithRunes(gdata);
	gdata->_state = STATE_RUNE_ENHANCER;
}


//	Raise level of selected rune.
void tryEnhanceRune(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		auto cost = it->getEnhanceCost();
		auto mat = it->_spellLevel >= 10 ? MaterialType::BRIGHT_RUNE : MaterialType::RUNE_SHARD;
		if (hasMaterial(gdata, mat, cost))
		{
			spendMaterial(gdata, mat, cost);
			it->_spellLevel++;
			messages::add(gdata, "Enhanced #" + it->getName() + " to level #" + to_string(it->_spellLevel) + "@!", { it->getColor(), COLOR_POSITIVE });
		}
		else
			messages::error(gdata, "Insufficient materials!");
	}
}


//	Gives us a random selection of new spells we can learn.
void openMeditationShrine(gamedataPtr gdata, int dl)
{
	gdata->_state = STATE_LEARN_SPELL;
	gdata->_spellsToLearn = lootgen::rollSpellsToLearn(1);
	gdata->_idx = 0;
}



//	Adds all spellrunes from stash AND inventory to the current item list.
void populateCurrentItemListWithRunes(gamedataPtr gdata)
{
	gdata->_currentItemList.clear();
	for (auto it : gdata->_carriedItems)
	{
		if (it->_category == ITEM_SPELLRUNE)
			gdata->_currentItemList.push_back(it);
	}
	for (auto it : gdata->_stashItems)
	{
		if (it->_category == ITEM_SPELLRUNE)
			gdata->_currentItemList.push_back(it);
	}
	for (auto it : gdata->_player->_ImprintedRunes)
		gdata->_currentItemList.push_back(it);
}


//	Allows us to equip runes.
void openRuneImprinter(gamedataPtr gdata)
{
	//	Assemble list of runes in stash & inventory.
	populateCurrentItemListWithRunes(gdata);

	//	Other setup
	gdata->_idx = 0;
	gdata->_state = STATE_LEARN_SPELL;
}



//	Remove all memorized spell runes, so we can equip new ones.
void removeAllSpellRunes(gamedataPtr gdata)
{
	//	Place in stash
	for (auto it : gdata->_player->_ImprintedRunes)
		addToStash(gdata, it);

	//	Forget
	gdata->_player->_ImprintedRunes.clear();

	//	Re-populate the visible list
	populateCurrentItemListWithRunes(gdata);
}



//	Memorize this rune, if we have room.
//	If we don't have room, returns False.
bool playerImprintSpellRune(gamedataPtr gdata, itemPtr it)
{
	//	We can't equip the exact same rune twice!
	if (gdata->_player->isRuneEquipped(it))
		return false;

	//	If we have a different version of this spell equipped already, replace it.
	for (unsigned i = 0; i < gdata->_player->_ImprintedRunes.size(); i++)
	{
		if (gdata->_player->_ImprintedRunes[i]->_containsSpell == it->_containsSpell)
		{
			//	replace!
			addToStash(gdata, gdata->_player->_ImprintedRunes[i]);
			gdata->_player->_ImprintedRunes[i] = it;
			return true;
		}
	}

	//	otherwise, append.
	if (gdata->_player->_ImprintedRunes.size() < gdata->_player->getMaxSpellsKnown())
	{
		gdata->_player->_ImprintedRunes.push_back(it);
		return true;
	}
	else
	{
		messages::error(gdata, "No free spell slots!");
		return false;
	}
}


//	Try to memorize the select spell rune.
void imprintSelectedSpellRune(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	Equip it.
		auto it = gdata->_currentItemList[gdata->_idx];
		if (playerImprintSpellRune(gdata, it))
		{
			//	Remove from inventory or stash, whichever it's in.
			removeFromInventory(gdata, it);
			removeFromStash(gdata, it);
			
			//	Re-populate the list of runes, as we've changed which ones are available.
			populateCurrentItemListWithRunes(gdata);
		}
	}
}


//	If the selected rune is equipped, we un-equip it.
void removeSelectedRune(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		for (unsigned i = 0; i < gdata->_player->_ImprintedRunes.size(); i++)
		{
			if (gdata->_player->_ImprintedRunes[i] == it)
			{
				gdata->_player->_ImprintedRunes.erase(gdata->_player->_ImprintedRunes.begin() + i);
				addToStash(gdata, it);
				break;
			}
		}
	}
	populateCurrentItemListWithRunes(gdata);
}
