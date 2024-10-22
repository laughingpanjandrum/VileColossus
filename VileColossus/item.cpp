#include "item.h"


item::item(string name, ItemCategory cat, const int rarity)
{
	//	basic values
	_name = name;
	_category = cat;
	_rarity = rarity;
	_tier = 1;
	_amountLeft = 1;

	//	enhancement
	_enhancementLevel = 0;
	_exaltLevel = 0;

	//	flags
	_isNewItem = true;
	_isTwoHanded = false;
	_markedAsValuable = false;
	_exalted = false;

	//	list of properties
	for (unsigned i = 0; i < PROP__NONE; i++)
		_Property.push_back(0);
	_material = MaterialType::__NONE;
	_armourCategory = ArmourCategory::ARMOUR__NONE;

	//	default charges (NONE)
	_chargeRegeneration = 0;
	_chargesLeft = 0;

	//	default spell knowledge (NONE)
	_containsSpell = Spell::__NONE;
	_spellLevel = 0;

	//	for gems only
	_gemType = GemType::__NONE;

	//	default durability (N/A)
	_damageTaken = 0;
	_maxDurability = 0;
}


bool item::isArmourPiece() const
{
	return _armourCategory != ArmourCategory::ARMOUR__NONE;
}


bool item::canEquipInSlot(const EquipmentSlot slot) const
{
	switch (_category)
	{
	case(ITEM_AMULET):		return slot == SLOT_AMULET;
	case(ITEM_BOOTS):		return slot == SLOT_BOOTS;
	case(ITEM_BRACERS):		return slot == SLOT_BRACERS;
	case(ITEM_CHESTPIECE):	return slot == SLOT_BODY;
	case(ITEM_GLOVES):		return slot == SLOT_GLOVES;
	case(ITEM_HELMET):		return slot == SLOT_HELMET;
	case(ITEM_QUIVER):		return slot == SLOT_OFFHAND;
	case(ITEM_RING):		return slot == SLOT_LEFT_RING || slot == SLOT_RIGHT_RING;
	case(ITEM_SHIELD):		return slot == SLOT_OFFHAND;
	case(ITEM_SHOULDERS):	return slot == SLOT_SHOULDERS;
	case(ITEM_WEAPON):		return slot == SLOT_MAIN_HAND || (slot == SLOT_OFFHAND && !_isTwoHanded);
	default:
		return false;
	}
}


//	Depends SOLELY on category.
int item::getSize() const
{
	switch (_category)
	{
	case(ITEM_AMULET):
	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_FLASK):
	case(ITEM_GEM):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_RING):
	case(ITEM_SHOULDERS):
	case(ITEM_SPELLRUNE):
		return 1;

	case(ITEM_CHESTPIECE):
	case(ITEM_QUIVER):
	case(ITEM_SHIELD):
	case(ITEM_WEAPON):
		return 2;

	default:
		return 0;
	}
}


string item::getName() const
{
	string name = _name;

	//	Quantity, if relevant
	if (stacks())
		name = "x" + to_string(_amountLeft) + " " + name;

	//	Nickname?
	if (!_nickname.empty())
		name = name + " '" + _nickname + "'";

	//	Enchantments
	else if (!_Enchants.empty() && _category != ITEM_SPELLRUNE)
		name += " of " + getItemEnchantmentName(_Enchants[0]);
	
	return name;
}


int item::getGlyph() const
{
	switch (_category)
	{
	case(ITEM_AMULET):			return 232;
	case(ITEM_BOOTS):			return 253;
	case(ITEM_BRACERS):			return '(';
	case(ITEM_CHESTPIECE):		return '[';
	case(ITEM_FLASK):			return 235;
	case(ITEM_GEM):				return 229;
	case(ITEM_GLOVES):			return '(';
	case(ITEM_HELMET):			return 127;
	case(ITEM_MATERIAL):
		if (_material == MaterialType::NOTCHED_CUBE)
			return 249;
		else
			return '$';
	case(ITEM_QUIVER):			return '}';
	case(ITEM_RING):			return 248;
	case(ITEM_SHIELD):			return 4;
	case(ITEM_SHOULDERS):		return 127;
	case(ITEM_SPELLRUNE):		return 13;
	case(ITEM_WEAPON):			return '{';
	}
}


colorType item::getColor() const
{
	//	Materials based on material type
	if (_category == ITEM_MATERIAL)
		return getMaterialTypeColor(_material);

	//	Gem color based on gem type
	else if (_category == ITEM_GEM)
	{
		auto col = getGemTypeColor(_gemType);
		col.scaleHSV(1.0f, 0.5f + 0.2f * (float)_enhancementLevel);
		return col;
	}

	//	Broken stuff is RED for SHAME.
	else if (isBroken())
		return TCODColor::darkRed;

	//	Based on rarity
	else
	{
		colorType col;
		switch (_rarity)
		{
		case(1):	col = TCODColor::lightGrey; break;
		case(2):	col = TCODColor::lighterBlue; break;
		case(3):	col = TCODColor::lightPurple; break;
		case(4):	col = TCODColor::orange; break;
		default:
			return COLOR_WHITE;
		}
		if (_exalted)
		{
			auto f = 0.5f + (float)randint(-10, 10) * 0.01f;
			col = TCODColor::lerp(col, COLOR_WHITE, f);
		}
		return col;
	}
}


//	Set the value of a property. Obliterates its former value.
void item::setProperty(const ItemProperty prop, int to)
{
	_Property[prop] = to;
}


//	Current value of a given property.
int item::getProperty(const ItemProperty prop) const
{
	int total =_Property[prop];
	return total;
}


//	Adds a new enchantment. Also applies its modifiers to all properties.
void item::addEnchantment(const ItemEnchantment en, const int bns)
{
	//	Remember enchantment
	_Enchants.push_back(en);
	_EnchantLevels.push_back(bns);

	//	Apply stat adjustments, if any.
	switch (en)
	{
//	case(ENCH_CAPACITY):	adjustProperty(PROP_MAX_CHARGES, bns); break;
//	case(ENCH_CHARGING):	adjustProperty(PROP_CHARGE_REGAIN_RATE, bns); break;
	}
}


//	Returns True if we have the given enchantment.
bool item::hasEnchantment(const ItemEnchantment en) const
{
	return find(_Enchants.begin(), _Enchants.end(), en) != _Enchants.end();
}


//	Returns the total bonus we apply to the given enchantment, if any.
int item::getEnchantmentValue(const ItemEnchantment en) const
{
	int total = 0;

	//	search for the enchantment in our list
	auto f = find(_Enchants.begin(), _Enchants.end(), en);
	if (f != _Enchants.end())
		total += _EnchantLevels[f - _Enchants.begin()];

	return total;
}


//	Tests whether two items can stack with one another in the inventory.
bool item::stacksWith(const shared_ptr<item> other) const
{
	if (other->_category == _category)
	{
		switch (_category)
		{
		case(ITEM_CONSUMABLE):
		case(ITEM_MATERIAL):	return _material == other->_material;
		case(ITEM_GEM):			return _gemType == other->_gemType && _enhancementLevel == other->_enhancementLevel;
		default:
			return false;
		}
	}
	return false;
}


//	Exactly what it says. Will not reduce below 1.
void item::reduceMaxDurability(int amt)
{
	_maxDurability -= amt;
	_maxDurability = MAX(1, _maxDurability);
}

//	Tests whether the item can suffer durability loss.
bool item::subjectToDurabilityLoss() const
{
	return _maxDurability > 0;
}


//	Number of fragments required to repair this item.
int item::getRepairCost() const
{
	switch (_rarity)
	{
	case(1):	return _damageTaken;
	case(2):	return _damageTaken * 3;
	case(3):	return _damageTaken * 10;
	case(4):	return _damageTaken * 25;
	default:
		return 0;
	}
}


//	We have a chance to lose some durability.
void item::rollDurabilityLoss()
{
	if (roll_one_in(20))
		_damageTaken++;
}

//	Reduces current durability by a percent of the maximum.
void item::takePercentDamage(const int per)
{
	_damageTaken += (float)per * (float)_maxDurability / 100.0f;
}


//	Tests whether the item is broken, ie damage exceeds max durability.
bool item::isBroken() const
{
	return _maxDurability > 0 && _damageTaken >= _maxDurability;
}


//	Progress towards regenerating charges. If it passes 100, we get an extra charge.
void item::regenerateCharge(int amt)
{
	_chargeRegeneration += amt;
	if (_chargeRegeneration >= 100)
	{
		if (_chargesLeft < getMaxCharges())
		{
			_chargeRegeneration -= 100;
			gainCharge();
		}
		else
			_chargeRegeneration = 100;
	}
}

//	Percent number of current charges.
int item::getChargePercent() const
{
	return (float)_chargesLeft * 100.0f / (float)getProperty(PROP_MAX_CHARGES);
}


//	Adds a charge, but not in excess of the maximum
void item::gainCharge()
{
	_chargesLeft = MIN(getMaxCharges(), _chargesLeft + 1);
}


//	Maximum number of enchants (for gear); maximum upgrade level (for gems)
int item::getMaxEnhancementLevel() const
{
	switch(_category)
	{
	case(ITEM_CONSUMABLE):
	case(ITEM_MATERIAL):	return 0;
	case(ITEM_FLASK):		return _rarity;
	case(ITEM_GEM):			return 5;
	case(ITEM_SPELLRUNE):	return _rarity - 1;
	default:				return _rarity + 1;
	}
}


//	Amount of material required to enhance this item.
int item::getEnhanceCost() const
{
	if (_category == ITEM_FLASK)
		return _rarity;
	else if (_category == ITEM_SPELLRUNE)
		return 1 + (_spellLevel) % 10;
	else
	{
		switch (_enhancementLevel)
		{
		case(0):	return 50;
		case(1):	return 100;
		case(2):	return 20;
		case(3):	return 20;
		case(4):	return 1;
		default:	return 0;
		}
	}
}


//	Increase max durability.
int item::getReinforceCost() const
{
	return _maxDurability * 10;
}


//	Type of material required to enhance this item.
MaterialType item::getEnhanceMaterial() const
{
	if (_category == ITEM_FLASK)
		return MaterialType::GLASS_SHARD;

	else
	{
		if (_enhancementLevel < 2)
			return MaterialType::FRAGMENTS;
		else if (_enhancementLevel < 3)
			return MaterialType::MAGIC_DUST;
		else if (_enhancementLevel < 4)
			return MaterialType::GLOWING_POWDER;
		else
			return MaterialType::RADIANT_ASH;
	}
}


//	Increases the numerical bonus of all enchantments by a portion of the step value.
void item::increaseAllEnchantLevels()
{
	for (unsigned i = 0; i < _Enchants.size(); i++)
	{
		auto v = MAX(1, getEnchantmentIncrement(static_cast<ItemEnchantment>(_Enchants[i])));
		_EnchantLevels[i] += v;
	}
}


//	Juices up an item. Effects depend on item type.
void item::addExaltLevel()
{
	_exaltLevel++;

	//	properties change
	switch (_category)
	{
	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_CHESTPIECE):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_SHOULDERS):
	case(ITEM_SHIELD):
		adjustProperty(PROP_DEFENCE, 1);
		adjustProperty(PROP_ARMOUR_VALUE, MIN(4, 1 + _Property[PROP_ARMOUR_VALUE] / 5));
		break;

	case(ITEM_AMULET):
	case(ITEM_RING):
		increaseAllEnchantLevels();
		break;

	case(ITEM_QUIVER):
		adjustProperty(PROP_ACCURACY_MOD, 1);
		adjustProperty(PROP_BASE_DAMAGE, randint(0, 2));
		break;

	case(ITEM_WEAPON):
		adjustProperty(PROP_ACCURACY_MOD, 1);
		adjustProperty(PROP_BASE_DAMAGE, _Property[PROP_BASE_DAMAGE] / 10);
		break;
	}
}


//	Add the given number of available empty sockets.
void item::adjustMaxSockets(const int adj)
{
	for (unsigned i = 0; i < adj; i++)
	{
		_socketSlots.push_back(GemType::__NONE);
		_socketLevels.push_back(0);
	}
}

//	Insert the given gem type into the first available empty slot.
//	Really should NOT be called if we're not positive we have a free socket,
//	but if we don't, nothing will happen.
void item::socketGem(const GemType gem, const int lvl)
{
	for (unsigned i = 0; i < _socketSlots.size(); i++)
	{
		if (_socketSlots[i] == GemType::__NONE)
		{
			_socketSlots[i] = gem;
			_socketLevels[i] = lvl;
			return;
		}
	}
}


//	Erases all socketed gems.
void item::clearAllGems()
{
	for (unsigned i = 0; i < _socketSlots.size(); i++)
	{
		_socketSlots[i] = GemType::__NONE;
		_socketLevels[i] = 0;
	}
}


//	Returns True if we have at least one free socket.
bool item::hasFreeSocket() const
{
	for (unsigned i = 0; i < _socketSlots.size(); i++)
	{
		if (_socketSlots[i] == GemType::__NONE)
			return true;
	}
	return false;
}


//	Sums up the tiers of all socketed gems of the given type.
//	This determines the stat bonus they confer.
int item::getTotalGemTier(const GemType gem) const
{
	int total = 0;
	for (unsigned i = 0; i < _socketSlots.size(); i++)
	{
		if (_socketSlots[i] == gem)
			total += _socketLevels[i];
	}
	return total;
}


//	Items are prioritized based on rarity/type
void itemContainer::addItem(itemPtr it)
{
	//	Gems/currency always go on top
	if (it->_category == ITEM_GEM || it->_category == ITEM_MATERIAL)
	{
		_items.insert(_items.begin(), it);
		return;
	}

	//	Otherwise, insert
	for (auto f = _items.begin(); f != _items.end(); f++)
	{
		if (it->_rarity > (*f)->_rarity)
		{
			_items.insert(f, it);
			return;
		}
	}
	_items.push_back(it);
}



//	Amount of each marginal increase to an enchantment bonus,
int getEnchantmentIncrement(const ItemEnchantment en)
{
	switch (en)
	{
	case(ENCH_MAGIC):
	case(ENCH_SHARPNESS):
		return 1;

	case(ENCH_ACCURACY):
	case(ENCH_ARMOURING):
	case(ENCH_BURNING):
	case(ENCH_DEFENCE):
	case(ENCH_HASTE):
	case(ENCH_LIGHTNING):
	case(ENCH_RAGE):
	case(ENCH_STONESKIN):
	case(ENCH_VENOM):
	case(ENCH_WRATH):
		return 2;

	case(ENCH_ARCANE):
	case(ENCH_LEECHING):
	case(ENCH_MANALEECH):
	case(ENCH_THORNS):
	case(ENCH_WOUNDING):
		return 3;

	case(ENCH_FLAMEWARD):
	case(ENCH_FURY):
	case(ENCH_MAGIC_RESTORE):
	case(ENCH_POISON_WARD):
	case(ENCH_REGEN):
	case(ENCH_RESISTANCE):
	case(ENCH_SPELLWARD):
	case(ENCH_STORMWARD):
		return 5;

	case(ENCH_AFF_ARCANE):
	case(ENCH_AFF_ELECTRIC):
	case(ENCH_AFF_FIRE):
	case(ENCH_AFF_POISON):
	case(ENCH_LIFE):
		return 10;

	case(ENCH_CHARGING):
	case(ENCH_EMPOWERING):
		return 20;

	case(ENCH_GREED):
	case(ENCH_SLAYING):
	case(ENCH_SPELLPOWER):
		return 25;

	default:
		return 0;
	}
}