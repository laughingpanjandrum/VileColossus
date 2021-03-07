#include "item.h"


item::item(string name, ItemCategory cat, const int rarity)
{
	//	basic values
	_name = name;
	_category = cat;
	_isNewItem = true;
	_isTwoHanded = false;
	_rarity = rarity;
	_amountLeft = 1;
	_enhancementLevel = 0;
	_markedAsValuable = false;

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

	//	default durability (N/A)
	_damageTaken = 0;
	_maxDurability = -1;
}



//	Turns item into a string.
const string item::serialize() const
{
	//	name, in quotes
	string t = "\"" + _name + "\"";
	t += "\"" + _nickname + "\"";

	//	enchants
	for (unsigned i = 0; i < _Enchants.size(); i++)
	{
		t += (char)_Enchants[i];
		t += (char)_EnchantLevels[i];
	}

	//	individual int properties
	t += (char)_amountLeft;
	t += (char)_category;
	t += (char)_chargesLeft;
	t += (char)_chargeRegeneration;
	t += (char)_containsSpell;
	t += (char)_damageTaken;
	t += (char)_enhancementLevel;
	t += (char)_gemType;
	t += (char)_material;
	t += (char)_maxDurability;
	t += (char)_rarity;
	t += (char)_spellLevel;
	t += (char)_tier;

	//	properties from the table
	for (unsigned i = 0; i < PROP__NONE; i++)
		t += (char)_Property[i];

	return t;
}


bool item::isArmourPiece() const
{
	return _armourCategory != ARMOUR__NONE;
	//return _category == ITEM_BOOTS || _category == ITEM_BRACERS || _category == ITEM_CHESTPIECE || _category == ITEM_GLOVES || _category == ITEM_HELMET || _category == ITEM_SHOULDERS;
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
	case(ITEM_MATERIAL):		return '$';
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
		col.scaleHSV(1.0f, 0.5f + 0.15f * (float)_enhancementLevel);
		return col;
	}

	//	Broken stuff is RED for SHAME.
	else if (isBroken())
		return TCODColor::darkRed;

	//	Based on rarity
	else
	{
		switch (_rarity)
		{
		case(1):	return TCODColor::lightGrey;
		case(2):	return TCODColor::lighterBlue;
		case(3):	return TCODColor::lightPurple;
		case(4):	return TCODColor::orange;
		default:
			return COLOR_WHITE;
		}
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
	case(ENCH_CAPACITY):	adjustProperty(PROP_MAX_CHARGES, bns); break;
	case(ENCH_CHARGING):	adjustProperty(PROP_CHARGE_REGAIN_RATE, bns); break;
	case(ENCH_HASTE):		adjustProperty(PROP_HASTE_BUFF, bns); break;
	case(ENCH_STONESKIN):	adjustProperty(PROP_STONESKIN_BUFF, bns); break;
	case(ENCH_WRATH):		adjustProperty(PROP_WRATH_BUFF, bns); break;
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
	return _maxDurability > -1;
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
	return _maxDurability > -1 && _damageTaken >= _maxDurability;
}


//	Progress towards regenerating charges. If it passes 100, we get an extra charge.
void item::regenerateCharge(int amt)
{
	_chargeRegeneration += amt;
	if (_chargeRegeneration >= 100)
	{
		if (_chargesLeft < getProperty(PROP_MAX_CHARGES))
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
	_chargesLeft = MIN(getProperty(PROP_MAX_CHARGES), _chargesLeft + 1);
}


//	Maximum number of enchants (for gear); maximum upgrade level (for gems)
int item::getMaxEnhancementLevel() const
{
	switch(_category)
	{
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
