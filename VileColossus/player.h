#pragma once

#include "creature.h"
#include "item.h"
#include "spells.h"


#define MAX_HOTKEYED_SPELLS 4


class player : public creature
{
public:

	player();
	~player() {}

	virtual bool isPlayer() const { return true; }

	virtual void takeDamage(int amt);


	//	Attributes

	int getBaseAttribute(const Attribute attr) const;
	int getDerivedAttribute(const Attribute attr) const;

	void raiseAttribute(const Attribute attr, const int amt = 1) { _Attributes[attr] += amt; }

	int getGreedBonus() const { return getTotalEnchantmentBonus(ENCH_GREED); }


	//	Flask use

	bool willFlaskWasteHealing() const;


	//	Magic points

	int getMagicPercent() const { return 100.0f - 100.0f * (float)_magicExpended / (float)getMaxMagic(); }
	int getMagicLeft() const { return getMaxMagic() - _magicExpended; }

	bool hasMagicForSpell(const Spell sp, const int lvl) const;
	void expendMagic(const int amt);
	int getMagicRegenDelay() const { return 6; }

	void restoreMagic(const int amt);
	void restoreAllMagic() { _magicExpended = 0; }


	//	Equipment slots

	int getMaxInventorySlots() const { return 30; }

	bool hasNewForSlot(const EquipmentSlot slot) const { return _HasNewForSlot[slot]; }
	void markNewForSlot(const EquipmentSlot slot);
	void unmarkNewForSlot(const EquipmentSlot slot);
	void unmarkAllSlots();


	//	Derived stats

	virtual int getMaxHealth() const;
	virtual int getMaxMagic() const;

	virtual int getSpellPower() const;

	virtual int getAccuracy() const;
	virtual int getDefenceValue() const;
	virtual int getArmourValue() const;

	virtual int getWeaponDamage() const;
	virtual int getDamageVariance() const;
	virtual int getCriticalChance() const;
	virtual int getCriticalMultiplier() const;

	virtual int getMoveEnergyCost() const;
	virtual int getAttackEnergyCost() const;
	int getVisionRadius() const;

	virtual int getResistance(const DamageType dt) const;


	//	Special attacks

	virtual int getRiposteChance() const;
	virtual int getRiposteDamageMult() const;

	virtual bool cleaves() const;
	virtual int getCleaveDamageBonus();

	virtual int getStaggerChance() const;
	virtual int getStaggerAttackDuration() const;

	virtual int getKnockbackChance() const;

	virtual bool usingRangedWeapon() const;
	virtual int getMaxAttackRange() const;

	virtual int getWeaponDamageOfType(const DamageType dt) const;
	virtual int getElementalAffinity(const DamageType dt) const;

	virtual int getLeechOnKill() const;
	virtual int getManaleech() const;
	virtual int getReprisalDamage() const;

	virtual int getArcanePulseDamage() const;
	virtual int getSmiteEvilDamage() const;
	int getVenomfangDamage() const;

	int getWrathOnKillChance() const { return getTotalEnchantmentBonus(ENCH_RAGE); }
	int getWrathDamageBonus() const { return 25 + getTotalEnchantmentBonus(ENCH_FURY); }


	//	Bonuses from socketed gems

	int getTotalGemBonusFromWeapons(const GemType gem) const;
	int getTotalGemBonusFromArmour(const GemType gem) const;
	int getTotalGemBonusFromJewels(const GemType gem) const;


	//	Spells

	vector<Spell> getAllSpellsKnown() const;

	Spell getSpellAtIndex(const int idx) const;
	int getSpellLevel(const Spell sp) const;

	void equipSpellRune(itemPtr it);
	bool isRuneEquipped(const itemPtr it) const { return find(_ImprintedRunes.begin(), _ImprintedRunes.end(), it) != _ImprintedRunes.end(); }
	vector<itemPtr> _ImprintedRunes;


	//	Equipment

	bool usingOffhandWeapon() const;
	bool isHandednessValid() const;

	int getTotalEnchantmentBonus(const ItemEnchantment en) const;
	int getEquipmentPropertySum(const ItemProperty prop) const;
	const vector<itemPtr> getAllEquippedItems() const { return _Equipped; }
	
	void equipInSlot(itemPtr it, EquipmentSlot slot);
	void unequipFromSlot(const EquipmentSlot slot);
	void unequipItem(itemPtr it);
	itemPtr getItemInSlot(const EquipmentSlot slot) const;

	void swapToSecondaryItems();

	void chargeFlaskOnKill();
	void chargeFlaskOnHit();
	itemPtr _currentFlask;


	//	Estimation

	int estimateDPS() const;


	//	Time passage events
	virtual void tick();
	bool _triggeredDeath;


protected:

	int getTotalGemBonusFromSlots(const vector<EquipmentSlot> slots, const GemType gem) const;

	vector<int> _Attributes;
	vector<itemPtr> _Equipped;
	vector<bool> _HasNewForSlot;

	itemPtr _secondaryMainHand;
	itemPtr _secondaryOffhand;

	int _magicExpended;
	int _magicRegenTicks;

};
typedef shared_ptr<player> playerPtr;