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
		BASTARD_SWORD, BATTLE_AXE, BROAD_AXE, CROSSBOW, CLUB, DAGGER, DEMON_BLADE, DEMON_GREATBLADE, DOUBLE_AXE, DOUBLE_CROSSBOW, GREATCLUB, GREAT_AXE, GREATBOW, GREATHAMMER, GREATMAUL, GREATSWORD, HEAVY_CROSSBOW,
		LONG_KNIFE, LONGBOW, LONGSWORD, MACE, SHORTBOW, SHORTSWORD, STILETTO, TITAN_AXE, WAR_AXE, WARHAMMER,
	};


	//	Tables

	const vector<ItemCategory> ARMOUR_CATEGORIES = { ITEM_BOOTS, ITEM_BRACERS, ITEM_CHESTPIECE, ITEM_GLOVES, ITEM_HELMET, ITEM_SHOULDERS, ITEM_SHIELD, };

	const vector<ItemProperty> RESIST_PROPS = { PROP_RESIST_ARCANE, PROP_RESIST_ELECTRIC, PROP_RESIST_FIRE, PROP_RESIST_POISON };

	const vector<ItemEnchantment> LEGENDARY_ENCHANTS = { 
		ENCH_AFFINITY, ENCH_ARCANE_SHIELD, ENCH_AVARICE, ENCH_BLACKBLOOD, ENCH_CONDUCTING, ENCH_CRUCIBLE, ENCH_CUNNING, 
		ENCH_DARK_ARCANA, ENCH_DERVISH, ENCH_DIVINE, ENCH_FIREBURST, ENCH_FLAMESPIKE, ENCH_KINSLAYER, ENCH_LEAGUE, ENCH_MAGEBLOOD, ENCH_PETRIFYING,
		ENCH_SHADOWSTRIKE, ENCH_SKYSPLITTER, ENCH_SPELLBURN, ENCH_STORMBURST, ENCH_THUNDERSPIKE, ENCH_VENOMBURST, ENCH_VENOMSPIKE, ENCH_WEIGHT, 
	};

	const vector<GemType> BASIC_GEM_TYPES = { GemType::BLACKSTONE, GemType::BOLTSTONE, GemType::FLAMESTONE, GemType::SILVERSTONE, GemType::SPIDERSTONE, };
	const vector<GemType> SPECIAL_GEM_TYPES = { GemType::ABYSSAL_SPIKEGEM, GemType::BLOODY_FLESHGEM, GemType::DOGOSSAN_BONEGEM, GemType::LUMINOUS_GEM, GemType::VIRIDIAN_PALEGEM };

	const vector<Spell> SPELLS_TIER_1 = { Spell::ARCANE_EMANATION, Spell::BLINK, Spell::CALL_LIGHTNING, Spell::CONJURE_FLAME, Spell::SMITE_EVIL, Spell::VENOMFANG, };
	const vector<Spell> SPELLS_TIER_2 = { Spell::ARCANE_PULSE, Spell::CHAIN_LIGHTNING, Spell::FIREBALL, Spell::TELEPORT, Spell::TOXIC_RADIANCE, };
	const vector<Spell> SPELLS_TIER_3 = { Spell::CRYSTAL_SPEAR, Spell::FIRESTORM, Spell::PUTRESCENCE, Spell::STATIC_FIELD, };

	const vector<BaseWeaponType> getWeaponTypesOfTier(const int tier);
	const vector<ItemEnchantment> getEnchantmentsForItemCategory(const ItemCategory cat);


	//	Value for various attributes by level.
	inline int getBaseAccuracyForLevel(const int lvl) { return (5 + lvl) * 2 / 3; }
	inline int getBaseDamageForLevel(const int lvl) { return 4 + 2 * lvl; }
	inline int getBaseDefenceForLevel(const int lvl) { return 7 + lvl; }
	inline int getBaseHealthForLevel(const int lvl) { return 8 + 8 * lvl; }
	inline int getBaseProtectionForLevel(const int lvl) { return lvl; }
	inline int getHeavyProtectionForLevel(const int lvl) { return lvl * 2; }


	//	Starting equipment.

	itemPtr generateStartingWeapon();
	itemPtr generateStartingFlask();


	//	Enhancements

	int getMaxSocketsForCategory(const ItemCategory cat);
	int rollEnchantmentBonus(const ItemEnchantment en);
	int getEnchantmentIncrement(const ItemEnchantment en);
	void enchantItem(itemPtr it, int count);


	//	Some misc items

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

	const int rollEnhancementLevel(const int rarity);
	const int rollRarity(const int maxRarity);
	const int rollTier(const int maxTier);
	itemPtr rollItemDrop(const int maxTier, const int bestRarityAllowed, bool forceRarity = false, bool exalt = false);

	MaterialType rollRitualMaterial();

}