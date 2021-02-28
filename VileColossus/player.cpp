#include "player.h"

player::player()
{
	//  Flavour
	_name = "Wanderer";
	_glyph = '@';
	_color = COLOR_WHITE;

	//  Equipment
	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		_Equipped.push_back(nullptr);
		_HasNewForSlot.push_back(false);
	}
	_triggeredDeath = false;

	//  Attributes
	_level = 1;
	_magicRegenTicks = getMagicRegenDelay();
	for (unsigned i = 0; i < ATTR__MAX; i++)
		_Attributes.push_back(10);
}


//	Arcane Shield intercepts damage if we have Magic left.
void player::takeDamage(int amt)
{
	//	Apply Arcane Shield
	int per = getTotalEnchantmentBonus(ENCH_ARCANE_SHIELD);
	if (per > 0)
	{
		int pts_req = amt / per;
		pts_req = MIN(pts_req, getMagicLeft());
		expendMagic(pts_req);
		amt -= per * pts_req;
	}

	//	Actually take the damage
	if (amt > 0)
		_damageTaken += amt;
}


//	Base value of the given attribute.
int player::getBaseAttribute(const Attribute attr) const
{
	return _Attributes[attr];
}


//	This would add modifiers, but there currently aren't any available!
int player::getDerivedAttribute(const Attribute attr) const
{
	int total = getBaseAttribute(attr);
	return total;
}


//	Returns True if our equipped flask heals more damage than we have taken.
bool player::willFlaskWasteHealing() const
{
	if (_currentFlask != nullptr)
		return _currentFlask->getProperty(PROP_HEAL_ON_USE) > _damageTaken;
	else
		return true;
}


//  Indicate that we've picked up a new item that goes in the given slot.
void player::markNewForSlot(const EquipmentSlot slot)
{
	if (slot != SLOT__NONE)
	{
		//	we mark the alt ring slot if we already have a ring in the first slot
		if (slot == SLOT_LEFT_RING && _Equipped[SLOT_LEFT_RING] != nullptr)
			_HasNewForSlot[SLOT_RIGHT_RING] = true;

		//	otherwise, just mark the given slot
		else
			_HasNewForSlot[slot] = true;
	}
}

//  After we view items for this slot, no longer indicate that new ones are available.
void player::unmarkNewForSlot(const EquipmentSlot slot)
{
	if (slot != SLOT__NONE)
		_HasNewForSlot[slot] = false;
}


//	Clears the 'new item' flag from ALL equipment slots
void player::unmarkAllSlots()
{
	for (auto i = 0; i < _HasNewForSlot.size(); i++)
		_HasNewForSlot[i] = false;
}


int player::getMaxHealth() const
{
	int total = 27 + _level * 3 + getDerivedAttribute(ATTR_STRENGTH) * 2;
	total += getTotalEnchantmentBonus(ENCH_LIFE);
	total += getTotalGemBonusFromJewels(GemType::FLAMESTONE) * 20;
	return total;
}

int player::getMaxMagic() const
{
	int total = 2 + (getDerivedAttribute(ATTR_WILLPOWER) - 10) / 2;
	total += getTotalEnchantmentBonus(ENCH_MAGIC);
	total += getTotalGemBonusFromJewels(GemType::SILVERSTONE);
	return total;
}

int player::getSpellPower() const
{
	//	from items
	int total = (getDerivedAttribute(ATTR_WILLPOWER) - 10) * 5;
	total += getTotalEnchantmentBonus(ENCH_SPELLPOWER);
	total += getTotalEnchantmentBonus(ENCH_SPELLBURN);
	total += getTotalGemBonusFromJewels(GemType::BOLTSTONE) * 25;

	//	from buffs
	if (hasBuff(BUFF_EMPOWERED))
		total += (total / 2) + 50;

	return total;
}

int player::getAccuracy() const
{
	int total = getDerivedAttribute(ATTR_DEXTERITY) - 4;

	total += getEquipmentPropertySum(PROP_ACCURACY_MOD);
	total += getTotalEnchantmentBonus(ENCH_ACCURACY);
	if (usingOffhandWeapon())
		total -= 4;
	
	return total;
}

int player::getDefenceValue() const
{
	int total = getDerivedAttribute(ATTR_DEXTERITY) / 2;
	total += getEquipmentPropertySum(PROP_DEFENCE);
	return total;
}

int player::getArmourValue() const
{
	int total = getEquipmentPropertySum(PROP_ARMOUR_VALUE);
	if (hasBuff(BUFF_STONESKIN))
		total += _level * 2;
	total += getTotalGemBonusFromArmour(GemType::BLACKSTONE) * 2;
	return total;
}

int player::getWeaponDamage() const
{
	int total = getEquipmentPropertySum(PROP_BASE_DAMAGE);
	
	//	stats/basic enchantments
	total += (getDerivedAttribute(ATTR_STRENGTH) - 10) * 0.3f;
	total += getTotalEnchantmentBonus(ENCH_WOUNDING);

	//	tally up total percentage bonus
	int percent_adjust = 0;
	if (_damageTaken >= (float)getMaxHealth() * 0.7)
		percent_adjust +=  getTotalEnchantmentBonus(ENCH_CUNNING);
	if (hasBuff(BUFF_WRATH))
		percent_adjust += getTotalEnchantmentBonus(ENCH_FURY);
	percent_adjust += getTotalEnchantmentBonus(ENCH_WEIGHT);

	//	Apply the percent bonus
	total = adjustByPercent(total, percent_adjust);

	//	gem bonuses
	total += getTotalGemBonusFromWeapons(GemType::BLACKSTONE) * 3;
	
	return total;
}

int player::getDamageVariance() const
{
	int total = getEquipmentPropertySum(PROP_DAMAGE_VARIANCE);
	return total;
}

int player::getCriticalChance() const
{
	int total = getEquipmentPropertySum(PROP_CRITICAL_CHANCE);
	total += getTotalEnchantmentBonus(ENCH_SHARPNESS);
	total += getTotalGemBonusFromJewels(GemType::SPIDERSTONE);
	if (getVisionRadius() <= 6)
		total += getTotalEnchantmentBonus(ENCH_SHADOWSTRIKE);
	if (hasStatusEffect(STATUS_POISON))
		total += getTotalEnchantmentBonus(ENCH_BLACKBLOOD);
	total += getBuffDuration(BUFF_CRIT_BONUS);
	return total;
}

int player::getCriticalMultiplier() const
{
	//	Base (From weapons); averaged if dualwielding.
	int total = getEquipmentPropertySum(PROP_CRITICAL_DAMAGE);
	if (usingOffhandWeapon())
		total /= 2;

	//	Other adjustments.
	total += getTotalEnchantmentBonus(ENCH_SLAYING);
	total += getTotalGemBonusFromJewels(GemType::BLACKSTONE) * 25;
	total += (getDerivedAttribute(ATTR_DEXTERITY) - 10) * 2;
	return total;
}

int player::getMoveEnergyCost() const
{
	int total = 100;
	if (hasBuff(BUFF_HASTE))				total -= 50;
	if (hasStatusEffect(STATUS_SLUDGED))	total += 100;
	total = MAX(25, total);
	return total;
}

//	Slower attacks if dual-wielding
int player::getAttackEnergyCost() const
{
	auto total = ENERGY_COST_BASE;
	if (usingOffhandWeapon() && _Equipped[SLOT_OFFHAND]->_category != ITEM_QUIVER)		
		total += 100;
	if (getTotalEnchantmentBonus(ENCH_WEIGHT) > 0)
		total += 100;
	if (hasBuff(BUFF_HASTE))		
		total -= total / 4;
	return total;
}

int player::getVisionRadius() const
{
	int total = 5 + getTotalEnchantmentBonus(ENCH_LIGHT);
	return total;
}

int player::getResistance(const DamageType dt) const
{
	int total = 0;
	total += MIN(getTotalEnchantmentBonus(ENCH_AFFINITY), getElementalAffinity(dt));
	
	switch (dt)
	{
	case(DTYPE_ARCANE):
		total += getTotalEnchantmentBonus(ENCH_SPELLWARD);
		total += MAX(20, (getDerivedAttribute(ATTR_WILLPOWER) - 10) / 2);
		break;

	case(DTYPE_FIRE):
		total += getTotalEnchantmentBonus(ENCH_FLAMEWARD);
		total += MAX(20, (getDerivedAttribute(ATTR_STRENGTH) - 10) / 2);
		break;

	case(DTYPE_ELECTRIC):
		total += getTotalEnchantmentBonus(ENCH_STORMWARD);
		total += MAX(20, (getDerivedAttribute(ATTR_DEXTERITY) - 10) / 2);
		break;

	case(DTYPE_POISON):
		total += getTotalEnchantmentBonus(ENCH_POISON_WARD);
		total += MAX(20, (getDerivedAttribute(ATTR_STRENGTH) - 10) / 2);
		break;
	}

	//  Cap.
	total = MIN(90, total);
	return total;
}

//  Chance/100 to riposte on a successful dodge.
int player::getRiposteChance() const
{
	int total = getEquipmentPropertySum(PROP_RIPOSTE_CHANCE);

	//	Average if dual-wielding.
	if (usingOffhandWeapon())
		total /= 2;

	return total;
}

//  Percent of our base damage a riposte attack inflicts.
int player::getRiposteDamageMult() const
{
	int total = getEquipmentPropertySum(PROP_RIPOSTE_DAMAGE);

	//	Average if dualwielding.
	if (usingOffhandWeapon())
		total /= 2;

	return total;
}

//  We must have a cleaving primary weapon equipped.
bool player::cleaves() const
{
	return _Equipped[SLOT_MAIN_HAND] != nullptr && _Equipped[SLOT_MAIN_HAND]->getProperty(PROP_CLEAVE_DAMAGE) > 0;
}

//  Additional damage per adjacent target when cleaving.
int player::getCleaveDamageBonus()
{
	return getEquipmentPropertySum(PROP_CLEAVE_DAMAGE);
}

//  Chance/100 to inflict Stagger on hit.
int player::getStaggerChance() const
{
	return getEquipmentPropertySum(PROP_STAGGER_CHANCE);
}

//  Number of turns the stagger effect lasts for, if applied.
int player::getStaggerAttackDuration() const
{
	return getEquipmentPropertySum(PROP_STAGGER_DURATION);
}

int player::getKnockbackChance() const
{
	return getEquipmentPropertySum(PROP_KNOCKBACK_CHANCE);
}

bool player::usingRangedWeapon() const
{
	return _Equipped[SLOT_MAIN_HAND] != nullptr && _Equipped[SLOT_MAIN_HAND]->isRangedWeapon();
}

int player::getMaxAttackRange() const
{
	if (usingRangedWeapon())
		return _Equipped[0]->getProperty(PROP_ATTACK_RANGE);
	return 0;
}


//  Special damage type applied on hit with a weapon.
int player::getWeaponDamageOfType(const DamageType dt) const
{
	int total = 0;

	switch (dt)
	{
	case(DTYPE_ARCANE):     
		total += getTotalEnchantmentBonus(ENCH_ARCANE);
		total += getTotalGemBonusFromWeapons(GemType::SILVERSTONE) * 3;
		total += MIN(getSpellPower() / 10, getTotalEnchantmentBonus(ENCH_DARK_ARCANA));
		break;
	
	case(DTYPE_ELECTRIC):   
		total += getTotalEnchantmentBonus(ENCH_LIGHTNING);
		total += getTotalEnchantmentBonus(ENCH_STORMBURST);
		total += getTotalGemBonusFromWeapons(GemType::BOLTSTONE) * 3;
		break;
	
	case(DTYPE_FIRE):       
		total += getTotalEnchantmentBonus(ENCH_BURNING); 
		total += getTotalEnchantmentBonus(ENCH_FIREBURST);
		total += getTotalGemBonusFromWeapons(GemType::FLAMESTONE) * 3;
		break;
	
	case(DTYPE_POISON):     
		total += getTotalEnchantmentBonus(ENCH_VENOM); 
		total += getTotalEnchantmentBonus(ENCH_VENOMBURST);
		total += getTotalGemBonusFromWeapons(GemType::SPIDERSTONE) * 3;
		if (hasBuff(BUFF_VENOMFANG))
			total += getVenomfangDamage();
		break;
	}

	return total;
}

int player::getLeechOnKill() const
{
	int total = getTotalEnchantmentBonus(ENCH_LEECHING);
	return total;
}

//	Magic gained on kill.
int player::getManaleech() const
{
	return 1 + getTotalEnchantmentBonus(ENCH_MANALEECH);
}

int player::getReprisalDamage() const
{
	int total = getTotalEnchantmentBonus(ENCH_THORNS);
	return total;
}


//	Bonus damage inflicted by the Smite Evil buff.
int player::getSmiteEvilDamage() const
{
	return 10 + getSpellPower() / 5;
}

//	Added to max poison damage when casting the spell.
int player::getVenomfangDamage() const
{
	return 6 + 10.0f * (float)getSpellPower() / 100.0f;
}

int player::getArcanePulseDamage() const
{
	return 10 + getSpellPower() / 5;
}


//	Sums up gem levels from equipped weapons.
int player::getTotalGemBonusFromWeapons(const GemType gem) const
{
	return getTotalGemBonusFromSlots({ SLOT_MAIN_HAND, SLOT_OFFHAND }, gem);
}

//	Sums up gem levels from equipped armour pieces.
int player::getTotalGemBonusFromArmour(const GemType gem) const
{
	return getTotalGemBonusFromSlots({ SLOT_BODY, SLOT_BOOTS, SLOT_BRACERS, SLOT_GLOVES, SLOT_HELMET, SLOT_SHOULDERS }, gem);
}

//	Sums up gem levels from equipped rings/amulets.
int player::getTotalGemBonusFromJewels(const GemType gem) const
{
	return getTotalGemBonusFromSlots({ SLOT_AMULET, SLOT_LEFT_RING, SLOT_RIGHT_RING  }, gem);
}


//	Returns a list of all spells we know (from gems+items)
vector<Spell> player::getAllSpellsKnown() const
{
	vector<Spell> spells;
	for (auto it : _ImprintedRunes)
		spells.push_back(it->_containsSpell);
	return spells;
}


//	If we have a spell at this index, return it; otherwise returns Spell::__NONE
Spell player::getSpellAtIndex(const int idx) const
{
	if (idx < _ImprintedRunes.size())
		return _ImprintedRunes[idx]->_containsSpell;
	else
		return Spell::__NONE;
}


//	Returns the level at which we cast the given spell.
int player::getSpellLevel(const Spell sp) const
{
	int total = 0;

	//	From rune imprints
	for (auto it : _ImprintedRunes)
	{
		if (it->_containsSpell == sp)
		{
			total += it->_spellLevel;
			break;
		}
	}

	//	From items
	for (auto it : getAllEquippedItems())
	{
		if (it != nullptr && it->_containsSpell == sp)
			total += it->_spellLevel;
	}

	return total;
}


//	Put spell rune in the first available slot.
void player::equipSpellRune(itemPtr it)
{
	if (_ImprintedRunes.size() < MAX_HOTKEYED_SPELLS)
		_ImprintedRunes.push_back(it);
}


//	Percent bonus to spell and weapon damage of the given type that we inflict.
int player::getElementalAffinity(const DamageType dt) const
{
	int total = 0;
	switch (dt)
	{
	case(DTYPE_ARCANE):
		total += getTotalGemBonusFromArmour(GemType::SILVERSTONE) * 20;
		total += getTotalEnchantmentBonus(ENCH_AFF_ARCANE);
		break;

	case(DTYPE_ELECTRIC):
		total += getTotalGemBonusFromArmour(GemType::BOLTSTONE) * 20;
		total += getTotalEnchantmentBonus(ENCH_AFF_ELECTRIC);
		if (hasBuff(BUFF_CONDUCTION))
			total += getTotalEnchantmentBonus(ENCH_CONDUCTING);
		break;

	case(DTYPE_FIRE):
		total += getTotalGemBonusFromArmour(GemType::FLAMESTONE) * 20;
		total += getTotalEnchantmentBonus(ENCH_AFF_FIRE);
		break;

	case(DTYPE_POISON):
		total += getTotalGemBonusFromArmour(GemType::SPIDERSTONE) * 20;
		total += getTotalEnchantmentBonus(ENCH_AFF_POISON);
		if (hasBuff(BUFF_TOXIC_RADIANCE))
			total += 25 + getSpellPower() / 5;
		break;
	}
	return total;
}



//	Returns True if we have enough MP left to cast the given spell/
bool player::hasMagicForSpell(const Spell sp, const int lvl) const
{
	return getMagicLeft() >= getSpellMPCost(sp, lvl);
}


//	Expend the given number of magic points.
void player::expendMagic(const int amt)
{
	_magicExpended += amt;
}


//	Replenish magic.
void player::restoreMagic(const int amt)
{
	_magicExpended -= amt;
	_magicExpended = MAX(0, _magicExpended);
}


//	Tests whether we have a weapon equipped in our offhand.
//	Shields don't count.
bool player::usingOffhandWeapon() const
{
	return _Equipped[SLOT_OFFHAND] != nullptr && _Equipped[SLOT_OFFHAND]->_category == ITEM_WEAPON;
}

//	Tests whether our main and offhand items can be equipped together.
//	Returns True if valid, False otherwise. The calling function decides which to unequip.
bool player::isHandednessValid() const
{
	auto mh = _Equipped[SLOT_MAIN_HAND];
	auto oh = _Equipped[SLOT_OFFHAND];

	if (mh != nullptr && oh != nullptr)
	{
		//	Quivers can ONLY be equipped alongside bows.
		if (oh->_category == ITEM_QUIVER)
			return mh->isRangedWeapon();

		//	2h weapons CAN NEVER be equipped alongside another item, EXCEPT in the case of bow+quiver.
		else if (mh->_isTwoHanded)
			return (mh->isRangedWeapon() && oh->_category == ITEM_QUIVER);
	}
	return true;
}

//  Total value of all enchantment bonuses of the given type.
//  We only need this for enchantments that do not directly adjust an item's statistics.
int player::getTotalEnchantmentBonus(const ItemEnchantment en) const
{
	int total = 0;
	for (auto it : getAllEquippedItems())
	{
		if (it != nullptr)
			total += it->getEnchantmentValue(en);
	}
	for (auto it : _ImprintedRunes)
	{
		total += it->getEnchantmentValue(en);
	}
	return total;
}


//  Sums up the total value of the given item property across ALL of our equipped items.
int player::getEquipmentPropertySum(const ItemProperty prop) const
{
	int total = 0;
	for (auto it : _Equipped)
	{
		if (it != nullptr)
			total += it->getProperty(prop);
	}
	return total;
}


//  Place item in the given slot
void player::equipInSlot(itemPtr it, EquipmentSlot slot)
{
	it->_isNewItem = false;
	_Equipped[slot] = it;
}

//	Sets this slot to empty.
void player::unequipFromSlot(const EquipmentSlot slot)
{
	_Equipped[slot] = nullptr;
}

//	Finds the item in our equipment & removes it, if possible
void player::unequipItem(itemPtr it)
{
	for (unsigned i = 0; i < _Equipped.size(); i++)
	{
		if (_Equipped[i] == it)
		{
			_Equipped[i] = nullptr;
			break;
		}
	}
}

itemPtr player::getItemInSlot(const EquipmentSlot slot) const
{
	if (slot == SLOT__NONE)
		return nullptr;
	else
		return _Equipped[slot];
}


//	Swaps our main-hand items for our secondary items.
void player::swapToSecondaryItems()
{
	swap(_Equipped[SLOT_MAIN_HAND], _secondaryMainHand);
	swap(_Equipped[SLOT_OFFHAND], _secondaryOffhand);
}


//  If we have a flask equipped, it gains some charge.
void player::chargeFlaskOnKill()
{
	if (_currentFlask != nullptr)
		_currentFlask->regenerateCharge(_currentFlask->getProperty(PROP_CHARGE_REGAIN_RATE));
}

void player::chargeFlaskOnHit()
{
	if (_currentFlask != nullptr)
		_currentFlask->regenerateCharge(_currentFlask->getProperty(PROP_CHARGES_ON_HIT));
}


//	Figures out our average damage (summing every damage type) and divides by attack delay.
int player::estimateDPS() const
{
	//	base damage
	int total = getWeaponDamage();

	//	factor in critical chance
	const int cc = getCriticalChance();
	const int cd = getCriticalMultiplier();
	total = (float)(cc * adjustByPercent(total, cd) + (100 - cc) * total) / 10.0f;

	//	elemental damage (unaffected by crits)
	for (auto dt : SPECIAL_DAMAGE_TYPES)
	{
		auto dam = getWeaponDamageOfType(dt) / 2;
		if (dam > 0)
		{
			auto aff = getElementalAffinity(dt);
			dam = adjustByPercent(dam, aff);
		}
		total += dam * 10;
	}

	//	Adjust by attack delay
	total = (float)total / ((float)getAttackEnergyCost() / 100.0f);

	return total;
}


//	Special time-passage events for the player only
void player::tick()
{
	creature::tick();
}


//	Total tier of gems of the given type socketed in items equipped in the given slots.
int player::getTotalGemBonusFromSlots(const vector<EquipmentSlot> slots, const GemType gem) const
{
	int total = 0;
	for (auto slot : slots)
	{
		auto it = getItemInSlot(slot);
		if (it != nullptr)
			total += it->getTotalGemTier(gem);
	}
	return total;
}

