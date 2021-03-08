#pragma once

#include <string>
#include <vector>
#include <memory>

#include "color_constants.h"
#include "enums.h"

#include "spells.h"



#define GEM_MAX_TIER 5
#define MAX_ITEM_RARITY 4



class item
{
public:

	item() : item("", ITEM__MAX, 0) {}
	item(string name, ItemCategory cat, const int rarity);
	~item() {}

	void setNickname(const string name) { _nickname = name; }

	const string serialize() const;


	//	Category

	bool isArmourPiece() const;
	bool canEquipInSlot(const EquipmentSlot slot) const;

	int getSize() const;


	//	Flavour
	
	string getName() const;
	int getGlyph() const;
	colorType getColor() const;


	//	Properties
	
	void setProperty(const ItemProperty prop, int to);
	void adjustProperty(const ItemProperty prop, int adj) { _Property[prop] += adj; }

	int getProperty(const ItemProperty prop) const;
	bool hasProperty(const ItemProperty prop) const { return getProperty(prop) != 0; }

	bool isRangedWeapon() const { return getProperty(PROP_ATTACK_RANGE) > 0; }


	//	Enchantments

	void addEnchantment(const ItemEnchantment en, const int bns);
	
	bool hasEnchantment(const ItemEnchantment en) const;
	int getEnchantmentValue(const ItemEnchantment en) const;

	const vector<ItemEnchantment>* getAllEnchantments() const { return &_Enchants; }


	//	Stacking

	bool stacks() const { return _category == ITEM_CONSUMABLE || _category == ITEM_MATERIAL || _category == ITEM_GEM; }
	bool stacksWith(const shared_ptr<item> other) const;

	void adjustAmount(const int adj) { _amountLeft += adj; }


	//	Durability

	void setMaxDurability(const int to) { _maxDurability = to; }
	void reduceMaxDurability(int amt = 1);

	bool subjectToDurabilityLoss() const;
	int getRepairCost() const;

	void rollDurabilityLoss();
	void takePercentDamage(const int per);
	void repairToMax() { _damageTaken = 0; }

	bool isBroken() const;


	//	Charges

	void clearAllCharges() { _chargesLeft = 0; }
	void restoreAllCharges() { _chargesLeft = getProperty(PROP_MAX_CHARGES); }
	void regenerateCharge(int amt);

	bool hasChargesLeft() const { return _chargesLeft > 0; }
	int getChargesLeft() const { return _chargesLeft; }
	int getChargePercent() const;
	int getRegenerationPercent() const { return _chargeRegeneration; }

	void gainCharge();
	void expendCharge() { _chargesLeft -= 1; }


	//	Enhancement

	int getMaxEnhancementLevel() const;

	int getEnhanceCost() const;
	int getReinforceCost() const;
	MaterialType getEnhanceMaterial() const;


	//	Socketed gems

	int getSocketCount() const { return _socketSlots.size(); }
	void adjustMaxSockets(const int adj);

	const vector<GemType>* getAllSocketedGemTypes() const { return &_socketSlots; }
	const vector<int> getAllSocketedGemLevels() const { return _socketLevels; }
	int getSocketLevel(const int i) const { return _socketLevels[i]; }

	void socketGem(const GemType gem, const int lvl);
	void clearAllGems();
	bool hasFreeSocket() const;

	int getTotalGemTier(const GemType gem) const;



	//	Public data.
	//	All our data is public, for convenience rather than security. It can all be externally modified anyway.
	//	In particular, this makes save-game stuff a LOT easier than the huge list of getters/setters that would otherwise be necessary.
	
	int _rarity;
	int _tier;
	int _amountLeft;

	int _damageTaken;
	int _maxDurability;

	int _enhancementLevel;

	ItemCategory _category;
	ArmourCategory _armourCategory;
	MaterialType _material;
	GemType _gemType;

	Spell _containsSpell;
	int _spellLevel;

	bool _isTwoHanded;
	bool _isNewItem;	//	just picked up and not inspected yet

	bool _markedAsValuable;

	string _name;
	string _nickname;

	vector<int> _Property;

	vector<ItemEnchantment> _Enchants;
	vector<int> _EnchantLevels;

	vector<GemType> _socketSlots;
	vector<int> _socketLevels;

	int _chargesLeft;
	int _chargeRegeneration;


protected:

};
typedef shared_ptr<item> itemPtr;



struct itemContainer
{
	itemContainer() {}
	itemContainer(const intpair pos) : _pos(pos) {}

	void addItem(itemPtr it);

	vector<itemPtr> _items;
	intpair _pos;
};
typedef shared_ptr<itemContainer> itemContainerPtr;