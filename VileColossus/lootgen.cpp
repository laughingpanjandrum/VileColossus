#include "lootgen.h"



string lootgen::rollSyllable(const vector<string>* clist)
{
	return clist->at(randrange(clist->size()));
}

string lootgen::generateItemName()
{
	string name = rollSyllable(&ITEM_NAME_CONSONANT_OPEN) + rollSyllable(&ITEM_NAME_VOWELS) + rollSyllable(&ITEM_NAME_CONSONANT_CLOSE);
	
	if (roll_one_in(2))
		name += "-" + rollSyllable(&ITEM_NAME_CONSONANT_OPEN) + rollSyllable(&ITEM_NAME_VOWELS) + rollSyllable(&ITEM_NAME_CONSONANT_CLOSE);
	else if (roll_one_in(2))
		name += rollSyllable(&ITEM_NAME_VOWELS) + rollSyllable(&ITEM_NAME_CONSONANT_CLOSE);

	//	capitalize first letter
	name[0] -= 32;

	return name;
}


const vector<lootgen::BaseWeaponType> lootgen::getWeaponTypesOfTier(const int tier)
{
	switch (tier)
	{
	case(1):
		return { BaseWeaponType::BATTLE_AXE, BaseWeaponType::CROSSBOW, BaseWeaponType::CLUB, BaseWeaponType::DAGGER, BaseWeaponType::SHORTBOW, BaseWeaponType::SHORTSWORD, };

	case(2):
		return { BaseWeaponType::GREAT_AXE, BaseWeaponType::GREATHAMMER, BaseWeaponType::GREATSWORD, BaseWeaponType::HEAVY_CROSSBOW, BaseWeaponType::LONGBOW,
					BaseWeaponType::LONGSWORD, BaseWeaponType::MACE, BaseWeaponType::STILETTO, BaseWeaponType::WAR_AXE, };
		
	case(3):
		return { BaseWeaponType::BROAD_AXE, BaseWeaponType::DEMON_BLADE, BaseWeaponType::DEMON_GREATBLADE, BaseWeaponType::DOUBLE_AXE, BaseWeaponType::DOUBLE_CROSSBOW,
					BaseWeaponType:: GREATBOW, BaseWeaponType:: GREATMAUL, BaseWeaponType:: LONG_KNIFE, BaseWeaponType:: WARHAMMER, };

	default:
		return {};
	}
}

//  List of enchantments it is legal to add to an item of the given category.
const vector<ItemEnchantment> lootgen::getEnchantmentsForItemCategory(const ItemCategory cat)
{
	switch (cat)
	{
	case(ITEM_AMULET):
	case(ITEM_RING):
		return { ENCH_ACCURACY, ENCH_ARMOURING, ENCH_DEFENCE, ENCH_FLAMEWARD, ENCH_LEECHING, ENCH_LIFE, ENCH_LIGHT, ENCH_MAGIC, ENCH_MANALEECH, ENCH_POISON_WARD, ENCH_RAGE,
			ENCH_SHARPNESS, ENCH_SLAYING, ENCH_SPELLPOWER, ENCH_SPELLWARD, ENCH_STORMWARD, ENCH_THORNS, ENCH_WOUNDING, };

	case(ITEM_FLASK):
		return { ENCH_CAPACITY, ENCH_CHARGING, ENCH_CURING, ENCH_HASTE, ENCH_MAGIC_RESTORE, ENCH_STONESKIN, ENCH_WRATH, };

	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_CHESTPIECE):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_SHOULDERS):
		return { ENCH_ACCURACY, ENCH_FLAMEWARD, ENCH_LIFE, ENCH_LIGHT, ENCH_MAGIC, ENCH_POISON_WARD, ENCH_RAGE, ENCH_SLAYING, 
			ENCH_SPELLPOWER, ENCH_SPELLWARD, ENCH_STORMWARD, ENCH_THORNS, ENCH_WOUNDING, };

	case(ITEM_SHIELD):
		return { ENCH_FLAMEWARD, ENCH_LIFE, ENCH_LIGHT, ENCH_MAGIC, ENCH_POISON_WARD, ENCH_SHARPNESS, ENCH_SPELLPOWER, ENCH_SPELLWARD, ENCH_STORMWARD, ENCH_THORNS, ENCH_WOUNDING, };

	case(ITEM_WEAPON):
		return { ENCH_ACCURACY, ENCH_ARCANE, ENCH_BURNING, ENCH_DEFENCE, ENCH_LEECHING, ENCH_LIGHTNING, ENCH_MANALEECH,
			ENCH_MAGIC, ENCH_RAGE, ENCH_SHARPNESS, ENCH_SLAYING, ENCH_VENOM, ENCH_WOUNDING, };
	}

	return { };
}


itemPtr lootgen::generateStartingWeapon()
{
	auto it = generateWeaponOfType(BaseWeaponType::SHORTSWORD);
	it->setMaxDurability(10);
	it->_damageTaken = 0;
	it->setProperty(PROP_RIPOSTE_CHANCE, 25);
	it->setProperty(PROP_RIPOSTE_DAMAGE, 100);
	return it;
}

itemPtr lootgen::generateStartingFlask()
{
	auto it = itemPtr(new item("meagre flask", ITEM_FLASK, 0));
	it->setProperty(PROP_HEAL_ON_USE, 35);
	it->setProperty(PROP_MAX_CHARGES, 3);
	it->setProperty(PROP_CHARGES_ON_HIT, 5);
	it->setProperty(PROP_CHARGE_REGAIN_RATE, 25);
	it->restoreAllCharges();
	return it;
}


//	Maximum number of sockets an item of the given type can have.
int lootgen::getMaxSocketsForCategory(const ItemCategory cat)
{
	switch (cat)
	{
	case(ITEM_CHESTPIECE):
		return 3;

	case(ITEM_SHIELD):
	case(ITEM_WEAPON):
		return 2;

	case(ITEM_AMULET):
	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_RING):
	case(ITEM_SHOULDERS):
		return 1;

	default:
		return 0;
	}
}

//	Roll numeric component of an enchantment
int lootgen::rollEnchantmentBonus(const ItemEnchantment en)
{
	switch (en)
	{
	case(ENCH_ACCURACY):		return randint(1, 5);
	case(ENCH_ARCANE):			return randint(3, 8);
	case(ENCH_ARMOURING):		return randint(1, 5);
	case(ENCH_BURNING):			return randint(1, 6);
	case(ENCH_CAPACITY):		return randint(3, 5);
	case(ENCH_CHARGING):		return randint(4, 8) * 5;
	case(ENCH_CURING):			return 100;
	case(ENCH_DEFENCE):			return randint(1, 3);
	case(ENCH_FLAMEWARD):		return randint(5, 25);
	case(ENCH_HASTE):			return randint(5, 12);
	case(ENCH_LEECHING):		return randint(1, 10);
	case(ENCH_LIFE):			return randint(1, 10) * 4;
	case(ENCH_LIGHT):			return randint(1, 3);
	case(ENCH_LIGHTNING):		return randint(1, 6);
	case(ENCH_MAGIC):			return randint(1, 3);
	case(ENCH_MAGIC_RESTORE):	return randint(1, 6);
	case(ENCH_MANALEECH):		return randint(1, 2);
	case(ENCH_POISON_WARD):		return randint(5, 25);
	case(ENCH_RAGE):			return randint(5, 25);
	case(ENCH_SHARPNESS):		return randint(1, 5);
	case(ENCH_SLAYING):			return randint(1, 5) * 10;
	case(ENCH_SPELLPOWER):		return randint(1, 5) * 10;
	case(ENCH_SPELLWARD):		return randint(5, 25);
	case(ENCH_STONESKIN):		return randint(5, 12);
	case(ENCH_STORMWARD):		return randint(5, 25);
	case(ENCH_THORNS):			return randint(1, 8);
	case(ENCH_VENOM):			return randint(2, 5);
	case(ENCH_WOUNDING):		return randint(1, 6);
	case(ENCH_WRATH):			return randint(5, 12);
	default:
		return 0;
	}
}


//	Adds the given number of random enchantments to an item.
void lootgen::enchantItem(itemPtr it, int count)
{
	//	sockets?
	if (it->_tier > 1)
	{
		const int max_sockets = MIN(count, getMaxSocketsForCategory(it->_category));
		if (max_sockets > 0 && roll_one_in(3))
		{
			int sockets = randint(0, max_sockets);
			it->adjustMaxSockets(sockets);
			count -= sockets;
		}
	}


	//	enchants
	auto options = getEnchantmentsForItemCategory(it->_category);
	while (count-- > 0)
	{
		if (options.empty()) break;
		auto i = randrange(options.size());
		it->addEnchantment(options[i], rollEnchantmentBonus(options[i]));
		options.erase(options.begin() + i);
	}
}


//	Material types have fixed rarities.
const int lootgen::getMaterialRarity(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::FRAGMENTS):		return 1;
	case(MaterialType::GLASS_SHARD):	return 1;
	case(MaterialType::MAGIC_DUST):		return 2;
	case(MaterialType::GLOWING_POWDER):	return 3;

	default:
		return 0;
	}
}

//	Generates some material. 'qty' determines how much.
itemPtr lootgen::generateMaterial(const MaterialType mat, const int qty)
{
	auto it = itemPtr(new item(getMaterialTypeName(mat), ITEM_MATERIAL, getMaterialRarity(mat)));
	it->_material = mat;
	it->_amountLeft = qty;
	return it;
}


//	Creates a gem of the given tier.
itemPtr lootgen::generateGemOfType(const GemType gem, const int tier, const int rarity)
{
	auto it = itemPtr(new item(getGemTypeFullName(gem, tier), ITEM_GEM, rarity));
	it->_gemType = gem;
	it->_enhancementLevel = tier;
	return it;
}


//	Generate a gem of a random type & the given tier and rarity.
itemPtr lootgen::generateGem(const int tier, const int rarity)
{
	auto gem_type = BASIC_GEM_TYPES[randrange(BASIC_GEM_TYPES.size())];
	return generateGemOfType(gem_type, tier, rarity);
}


itemPtr lootgen::generateFlask(const int tier, const int rarity)
{
	//	Flask tier
	string name = "flask";
	switch (tier)
	{
	case(1):	name = "minor " + name; break;
	case(2):	name = "major " + name; break;
	case(3):	name = "great " + name; break;
	}

	//	Generate the actual item
	auto it = itemPtr(new item(name, ITEM_FLASK, rarity));
	it->setProperty(PROP_HEAL_ON_USE, randint(5, 25) + getBaseHealthForLevel(10 * tier) / 2);
	it->setProperty(PROP_MAX_CHARGES, randint(2, 5));
	it->setProperty(PROP_CHARGES_ON_HIT, randint(2, 5 + tier + rarity));
	it->setProperty(PROP_CHARGE_REGAIN_RATE, randint(10, 25));

	//	Special flask enchant
	it->_enhancementLevel = rarity;

	return it;
}


itemPtr lootgen::generateArmourPieceOfType(const ItemCategory cat, const int tier)
{
	//	Roll for light/heavy piece
	int wt = randint(1, 2);
	string name = "";
	int dv = 0, av = 0, acc = 0;

	//	Name/stats according to weight and type
	if (cat == ITEM_CHESTPIECE)
	{
		switch (wt)
		{
		case(1):
			name = "leathers";
			switch (tier)
			{
			case(2):	name = "hard " + name; break;
			case(3):	name = "demon " + name; break;
			}
			dv = randint(1, 1 + tier * 3);
			av = randint(1, tier);
			break;
		case(2):
			name = "mail";
			switch (tier)
			{
			case(2):	name = "gothic " + name; break;
			case(3):	name = "titan " + name; break;
			}
			av = 2 + randint(1, 2 * tier);
			break;
		}
	}
	else if (cat == ITEM_SHIELD)
	{
		switch (wt)
		{
		case(1):
			name = "shield";
			dv = randint(1, 3 + tier);
			break;

		case(2):
			name = "heavy shield";
			dv = randint(1, tier);
			av = randint(1, tier);
			acc = -(dv + av);
			break;
		}
	}
	else
	{
		switch (wt)
		{
		case(1):
			switch (cat)
			{
			case(ITEM_BOOTS):		name = "footwraps"; break;
			case(ITEM_BRACERS):		name = "armbands"; break;
			case(ITEM_GLOVES):		name = "gloves"; break;
			case(ITEM_HELMET):		name = "hood"; break;
			case(ITEM_SHOULDERS):	name = "cloak"; break;
			}
			dv = randint(1, tier);
			break;

		case(2):
			switch (cat)
			{
			case(ITEM_BOOTS):		name = "boots"; break;
			case(ITEM_BRACERS):		name = "bracers"; break;
			case(ITEM_GLOVES):		name = "gauntlets"; break;
			case(ITEM_HELMET):		name = "helm"; break;
			case(ITEM_SHOULDERS):	name = "pauldrons"; break;
			}
			av = randint(1, tier);
			break;
		}
		
	}

	//	Set properties.
	auto it = itemPtr(new item(name, cat, 0));
	it->setProperty(PROP_ARMOUR_VALUE, av);
	it->setProperty(PROP_DEFENCE, dv);
	it->setProperty(PROP_ACCURACY_MOD, acc);

	//	Roll durability.
	it->setMaxDurability(dieRoll(4, 6) + 10);
	it->_damageTaken = randint(0, it->_maxDurability / 2);

	return it;
}


//	Picks a random type of armour piece.
itemPtr lootgen::generateArmourPiece(const int tier, const int rarity)
{
	auto it = generateArmourPieceOfType(ARMOUR_CATEGORIES[randrange(ARMOUR_CATEGORIES.size())], tier);
	it->_rarity = rarity;
	it->_enhancementLevel = 1 + randint(0, it->_rarity);
	return it;
}


//	Base weapon of the given type.
itemPtr lootgen::generateWeaponOfType(const BaseWeaponType bwt)
{
	string name = "";
	int dam = 1, var = 0, crit = 0, mult = 25, acc = 0;
	switch (bwt)
	{
	case(BaseWeaponType::BATTLE_AXE):
		name = "battle-axe";
		dam = 4; var = 2; crit = 3; mult = 150; acc = -2;
		break;
	case(BaseWeaponType::BROAD_AXE):
		name = "broad axe";
		dam = 8; var = 4; crit = 3; mult = 150; acc = -4;
		break;
	case(BaseWeaponType::CLUB):
		name = "club";
		dam = 7; var = 3; crit = 1; mult = 50; acc = -1;
		break;
	case(BaseWeaponType::CROSSBOW):
		name = "crossbow";
		dam = 5; var = 2; crit = 3; mult = 100; acc = -1;
		break;
	case(BaseWeaponType::DAGGER):
		name = "dagger";
		dam = 2; var = 1; crit = 10; mult = randint(25, 35) * 10; acc = 2;
		break;
	case(BaseWeaponType::DEMON_BLADE):
		name = "demon blade";
		dam = 8; var = 7; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::DEMON_GREATBLADE):
		name = "ultrablade";
		dam = 14; var = 9; crit = 5; mult = 100; acc = -1;
		break;
	case(BaseWeaponType::DOUBLE_AXE):
		name = "double axe";
		dam = 7; var = 3; crit = 3; mult = 150; acc = -4;
		break;
	case(BaseWeaponType::DOUBLE_CROSSBOW):
		name = "double crossbow";
		dam = 12; var = 5; crit = 3; mult = 100; acc = -4;
		break;
	case(BaseWeaponType::GREAT_AXE):
		name = "great-axe";
		dam = 10; var = 5; crit = 3; mult = 150; acc - -5;
		break;
	case(BaseWeaponType::GREATBOW):
		name = "greatbow";
		dam = 8; var = 7; crit = 5; mult = 100; acc = -3;
		break;
	case(BaseWeaponType::GREATHAMMER):
		name = "greathammer";
		dam = 10; var = 3; crit = 1; mult = 50; acc = -4;
		break;
	case(BaseWeaponType::GREATMAUL):
		name = "greatmaul";
		dam = 17; var = 7; crit = 1; mult = 50; acc = -4;
		break;
	case(BaseWeaponType::GREATSWORD):
		name = "greatsword";
		dam = 10; var = 7; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::HEAVY_CROSSBOW):
		name = "heavy crossbow";
		dam = 8; var = 3; crit = 3; mult = 100; acc = -2;
		break;
	case(BaseWeaponType::LONGBOW):
		name = "longbow";
		dam = 5; var = 4; crit = 5; mult = 100; acc = -1;
		break;
	case(BaseWeaponType::LONGSWORD):
		name = "longsword";
		dam = 7; var = 6; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::LONG_KNIFE):
		name = "long knife";
		dam = 4; var = 3; crit = 15; mult = randint(30, 40) * 10; acc = 2;
		break;
	case(BaseWeaponType::MACE):
		name = "mace";
		dam = 8; var = 3; crit = 1; mult = 50;
		break;
	case(BaseWeaponType::SHORTBOW):
		name = "shortbow";
		dam = 3; var = 2; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::SHORTSWORD):
		name = "short sword";
		dam = 5; var = 3; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::STILETTO):
		name = "stiletto";
		dam = 3; var = 2; crit = 15; mult = randint(25, 35) * 10; acc = 2;
		break;
	case(BaseWeaponType::WARHAMMER):
		name = "warhammer";
		dam = 11; var = 4; crit = 1; mult = 50; acc = -1;
		break;
	case(BaseWeaponType::WAR_AXE):
		name = "war axe";
		dam = 7; var = 3; crit = 3; mult = 150; acc = -2;
		break;
	}


	//	Set basic attributes
	auto it = itemPtr(new item(name, ITEM_WEAPON, 0));
	it->setProperty(PROP_ACCURACY_MOD, acc);
	it->setProperty(PROP_BASE_DAMAGE, dam);
	it->setProperty(PROP_DAMAGE_VARIANCE, var);
	it->setProperty(PROP_CRITICAL_CHANCE, crit);
	it->setProperty(PROP_CRITICAL_DAMAGE, mult);


	//	Additional properties by weapon type
	switch (bwt)
	{
	case(BaseWeaponType::BATTLE_AXE):
	case(BaseWeaponType::BROAD_AXE):
	case(BaseWeaponType::DOUBLE_AXE):
	case(BaseWeaponType::GREAT_AXE):
	case(BaseWeaponType::WAR_AXE):
		it->setProperty(PROP_CLEAVE_DAMAGE, randint(3, 6));
		break;

	case(BaseWeaponType::DEMON_BLADE):
	case(BaseWeaponType::DEMON_GREATBLADE):
	case(BaseWeaponType::GREATSWORD):
	case(BaseWeaponType::LONGSWORD):
	case(BaseWeaponType::SHORTSWORD):
		it->setProperty(PROP_RIPOSTE_CHANCE, randint(70, 95));
		it->setProperty(PROP_RIPOSTE_DAMAGE, randint(100, 250));
		break;

	case(BaseWeaponType::CLUB):
	case(BaseWeaponType::GREATHAMMER):
	case(BaseWeaponType::GREATMAUL):
	case(BaseWeaponType::MACE):
	case(BaseWeaponType::WARHAMMER):
		it->setProperty(PROP_STAGGER_CHANCE, randint(25, 75));
		it->setProperty(PROP_STAGGER_DURATION, randint(3, 6));
		break;

	case(BaseWeaponType::SHORTBOW):
	case(BaseWeaponType::LONGBOW):
	case(BaseWeaponType::GREATBOW):
		it->setProperty(PROP_ATTACK_RANGE, randint(12, 15));
		break;

	case(BaseWeaponType::CROSSBOW):
	case(BaseWeaponType::HEAVY_CROSSBOW):
	case(BaseWeaponType::DOUBLE_CROSSBOW):
		it->setProperty(PROP_ATTACK_RANGE, randint(5, 9));
		break;
	}

	//	Set 2h flag
	switch (bwt)
	{
	case(BaseWeaponType::DOUBLE_AXE):
	case(BaseWeaponType::GREAT_AXE):
	case(BaseWeaponType::DEMON_GREATBLADE):
	case(BaseWeaponType::GREATSWORD):
	case(BaseWeaponType::GREATHAMMER):
	case(BaseWeaponType::GREATMAUL):
	case(BaseWeaponType::SHORTBOW):
	case(BaseWeaponType::LONGBOW):
	case(BaseWeaponType::GREATBOW):
	case(BaseWeaponType::CROSSBOW):
	case(BaseWeaponType::HEAVY_CROSSBOW):
	case(BaseWeaponType::DOUBLE_CROSSBOW):
		it->_isTwoHanded = true;
		break;
	}


	//	Roll durability.
	it->setMaxDurability(dieRoll(4, 6) + 10);
	it->_damageTaken = randint(0, it->_maxDurability / 2);

	return it;
}


//	Create a random weapon of the given rarity and tier.
itemPtr lootgen::generateWeapon(const int tier, const int rarity)
{
	auto wtable = getWeaponTypesOfTier(tier);
	auto it = generateWeaponOfType(wtable[randrange(wtable.size())]);
	it->_rarity = rarity;
	it->_enhancementLevel = 1 + randint(0, it->_rarity);
	return it;
}


//	Either an amulet or ring (rings are more common)
itemPtr lootgen::generateJewel(const int tier, const int rarity)
{
	//	base type
	string name = "ring";
	auto cat = ITEM_RING;
	if (roll_one_in(8))
	{
		cat = ITEM_AMULET;
		name = "amulet";
	}

	//	generate the item
	auto it = itemPtr(new item(name, cat, rarity));

	//	decide how many enchantments to add
	it->_enhancementLevel = 1 + rarity;
	it->_maxDurability = dieRoll(6, 2);

	return it;
}


//	Random spell rune (raises spell level when memorized)
itemPtr lootgen::generateSpellrune(const int tier, const int rarity)
{
	//	Pick a spell (based on tier)
	const vector<Spell>* options = &SPELLS_TIER_1;
	auto sp = options->at(randrange(options->size()));

	//	Spell level (determined by rarity)
	int lvl = 1;
	switch (rarity)
	{
	case(1):	lvl = randint(1, 3); break;
	case(2):	lvl = randint(4, 6); break;
	case(3):	lvl = randint(7, 10); break;
	case(4):	lvl = randint(11, 14); break;
	}

	//	name depends on tier
	string name = " rune of " + getSpellName(sp);
	if		(tier == 1)	name = "minor" + name;
	else if (tier == 2)	name = "major" + name;
	else				name = "grand" + name;

	//	Generate the item.
	auto it = itemPtr(new item(name, ITEM_SPELLRUNE, rarity));
	it->_containsSpell = sp;
	it->_spellLevel = lvl;

	return it;
}


//	Rolls 3 random spells we can gain a level in.
const vector<Spell> lootgen::rollSpellsToLearn(const int tier)
{
	const vector<Spell>* options = &SPELLS_TIER_1;
	vector<Spell> spells;
	for (unsigned i = 0; i < 3; i++)
	{
		spells.push_back(options->at(randrange(options->size())));
	}
	return spells;
}


//	3 tiers of items; better ones drop at higher levels
int lootgen::getLootTierForMonsterLevel(const int lvl)
{
	if		(lvl < 12)	return 1;
	else if (lvl < 24)	return 2;
	else				return 3;
}


//	Gems have more tiers than normal items, so they use a different table
int lootgen::getGemTierForMonsterLevel(const int lvl)
{
	if		(lvl < 6)	return 1;
	else if (lvl < 12)	return 2;
	else if (lvl < 18)	return 3;
	else if (lvl < 24)	return 4;
	else				return 5;
}


//	Higher rarities are increasingly less likely.
const int lootgen::rollRarity(const int maxRarity)
{
	int r = randint(1, 100);
	if (r <= 4 && maxRarity > 3)
		return 4;
	else if (r <= 20 && maxRarity > 2)
		return 3;
	else if (r <= 50 && maxRarity > 1)
		return 2;
	else
		return 1;
}


//	Generic item roller. Higher tiers/rarities are somewhat less likely.
itemPtr lootgen::rollItemDrop(const int maxTier, const int bestRarityAllowed)
{
	//	Roll actual tier and rarity
	int tier = randint(1, maxTier);
	int rarity = rollRarity(bestRarityAllowed);


	//	Generate the base item
	itemPtr it;
	int r = randint(1, 100);
	if		(r <= 40)	it = generateArmourPiece(tier, rarity);
	else if (r <= 80)	it = generateWeapon(tier, rarity);
	else if (r <= 90)	it = generateSpellrune(tier, rarity);
	else if (r <= 95)	it = generateJewel(tier, rarity);
	else				it = generateFlask(tier, rarity);


	//	Random enchantments.
	it->_tier = tier;
	if (it->_enhancementLevel > 0)
		enchantItem(it, it->_enhancementLevel);	

	//	Nickname for rares!!
	if (rarity == 3 && it->_category != ITEM_SPELLRUNE)
		it->setNickname(generateItemName());

	return it;
}
