#pragma once

#include "item.h"
#include "spells.h"


namespace lootgen
{

	//	Random names for special items

	const vector<string> ITEM_NAME_CONSONANT_OPEN = { "b", "br", "by", "bl", "c", "cr", "dr", "ch", "fr", "fl", "gr", "gl", "g", "h", "j", "kr", "kl", "k", "l", "m", "n", "p", "pr", "pl", "qu",
														"q", "r", "s", "sh", "t", "th", "tr", "tl", "v", "vr", "w", "wr", "x", "y", "z" };
	const vector<string> ITEM_NAME_VOWELS = { "a", "e", "i", "o", "u", "y", "ae", "au", "ai", "ay", "ei", "ea", "eo", "ia", "io", "iu", "ie", "oo", "oe", "oi", "oa", "ua", "ue", "ui", "uu", };

	const vector<string> ITEM_NAME_CONSONANT_CLOSE = { "b", "ch", "ck", "c", "d", "dd", "f", "ff", "g", "gh", "h", "k", "ll", "l",  "lt", "lk", "lch", "ld", "lm", "lf",
							"m", "n", "p", "r", "rr", "rb", "rd", "rf", "rg", "rk", "rl", "rm",
							"rt", "rs", "rv", "s", "sh", "st", "ss", "t", "v", "lv", "w", "x", "y", "z" };

	string rollSyllable(const vector<string>* clist);
	string generateItemName();


	//	Item identifiers
	enum class BaseWeaponType { 
		BATTLE_AXE, BROAD_AXE, CROSSBOW, CLUB, DAGGER, DEMON_BLADE, DEMON_GREATBLADE, DOUBLE_AXE, DOUBLE_CROSSBOW, GREAT_AXE, GREATBOW, GREATHAMMER, GREATMAUL, GREATSWORD, HEAVY_CROSSBOW,
		LONG_KNIFE, LONGBOW, LONGSWORD, MACE, SHORTBOW, SHORTSWORD, STILETTO, WAR_AXE, WARHAMMER,
	};


	//	Tables

	const vector<ItemCategory> ARMOUR_CATEGORIES = { ITEM_BOOTS, ITEM_BRACERS, ITEM_CHESTPIECE, ITEM_GLOVES, ITEM_HELMET, ITEM_SHOULDERS, ITEM_SHIELD, };

	const vector<ItemEnchantment> LEGENDARY_ENCHANTS = { ENCH_AFFINITY, ENCH_ARCANE_SHIELD, ENCH_BLACKBLOOD, ENCH_CUNNING, ENCH_SHADOWSTRIKE, ENCH_WEIGHT, };

	const vector<GemType> BASIC_GEM_TYPES = { GemType::BLACKSTONE, GemType::BOLTSTONE, GemType::FLAMESTONE, GemType::SILVERSTONE, GemType::SPIDERSTONE };

	const vector<Spell> SPELLS_TIER_1 = { Spell::ARCANE_EMANATION, Spell::BLINK, Spell::CALL_LIGHTNING, Spell::CONJURE_FLAME, Spell::SMITE_EVIL, Spell::VENOMFANG, };

	const vector<Spell> SPELLS_TIER_2 = { Spell::ARCANE_PULSE, Spell::CHAIN_LIGHTNING, Spell::FIREBALL, Spell::TELEPORT, Spell::TOXIC_RADIANCE, };

	const vector<BaseWeaponType> getWeaponTypesOfTier(const int tier);
	const vector<ItemEnchantment> getEnchantmentsForItemCategory(const ItemCategory cat);


	//	Value for various attributes by level.
	inline int getBaseAccuracyForLevel(const int lvl) { return 5 + lvl; }
	inline int getBaseDamageForLevel(const int lvl) { return 4 + 2 * lvl; }
	inline int getBaseDefenceForLevel(const int lvl) { return 7 + lvl; }
	inline int getBaseHealthForLevel(const int lvl) { return 8 + 8 * lvl; }
	inline int getBaseProtectionForLevel(const int lvl) { return 1 + lvl / 2; }
	inline int getHeavyProtectionForLevel(const int lvl) { return lvl; }


	//	Starting equipment.

	itemPtr generateStartingWeapon();
	itemPtr generateStartingFlask();


	//	Enhancements

	int getMaxSocketsForCategory(const ItemCategory cat);
	int rollEnchantmentBonus(const ItemEnchantment en);
	void enchantItem(itemPtr it, int count);


	//	Materials [currency] and gems

	const int getMaterialRarity(const MaterialType mat);
	itemPtr generateMaterial(const MaterialType mat, const int qty);

	itemPtr generateGemOfType(const GemType gem, const int tier, const int rarity);
	itemPtr generateGem(const int tier, const int rarity);


	//	Flask generation

	itemPtr generateFlask(const int tier, const int rarity);


	//	Armour generation

	itemPtr generateArmourPieceOfType(const ItemCategory cat, const int tier);
	itemPtr generateArmourPiece(const int tier, const int rarity);


	//	Weapon generation

	itemPtr generateWeaponOfType(const BaseWeaponType bwt);
	itemPtr generateWeapon(const int tier, const int rarity);

	itemPtr generateQuiver(const int tier, const int rarity);


	//	Jewelry

	itemPtr generateJewel(const int tier, const int rarity);


	//	Spells

	itemPtr generateSpellrune(const int tier, const int rarity);
	const vector<Spell> rollSpellsToLearn(const int tier);



	//	General drops

	itemPtr generateLegendaryItem(const int maxTier, ItemEnchantment en = ENCH__NONE);

	int getLootTierForMonsterLevel(const int dl);
	int getGemTierForMonsterLevel(const int dl);

	const int rollRarity(const int maxRarity);
	itemPtr rollItemDrop(const int maxTier, const int bestRarityAllowed);

}