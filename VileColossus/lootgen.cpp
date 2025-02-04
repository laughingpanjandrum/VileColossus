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
		return { BaseWeaponType::BASTARD_SWORD, BaseWeaponType::BATTLE_AXE, BaseWeaponType::CROSSBOW, BaseWeaponType::CLUB, BaseWeaponType::DAGGER, BaseWeaponType::GREATCLUB, BaseWeaponType::GREAT_AXE,
			BaseWeaponType::LONGSPEAR, BaseWeaponType::SHORTBOW, BaseWeaponType::SPEAR, BaseWeaponType::SHORTSWORD, };

	case(2):
		return { BaseWeaponType::DOUBLE_AXE, BaseWeaponType::GREATHAMMER, BaseWeaponType::GREATSWORD, BaseWeaponType::HALBERD, BaseWeaponType::HEAVY_CROSSBOW, BaseWeaponType::LONGBOW,
					BaseWeaponType::LONGSWORD, BaseWeaponType::MACE, BaseWeaponType::STILETTO, BaseWeaponType::WAR_AXE, BaseWeaponType::WARPIKE, };
		
	case(3):
		return { BaseWeaponType::BROAD_AXE, BaseWeaponType::DEMONGLAIVE, BaseWeaponType::DEMON_BLADE, BaseWeaponType::DEMON_GREATBLADE, BaseWeaponType::DEMON_PIKE, BaseWeaponType::DOUBLE_AXE, 
			BaseWeaponType::DOUBLE_CROSSBOW, BaseWeaponType:: GREATBOW, BaseWeaponType:: GREATMAUL, BaseWeaponType:: LONG_KNIFE, BaseWeaponType::TITAN_AXE, BaseWeaponType:: WARHAMMER, };

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
		return { ENCH_ACCURACY, ENCH_AFF_ARCANE, ENCH_AFF_ELECTRIC, ENCH_AFF_FIRE, ENCH_AFF_POISON,
			ENCH_ARMOURING, ENCH_DEFENCE, ENCH_FURY, ENCH_GREED, ENCH_LEECHING, ENCH_LIFE, ENCH_LIGHT, ENCH_MAGIC, ENCH_MANALEECH, ENCH_RAGE,
			ENCH_SHARPNESS, ENCH_SLAYING, ENCH_SPELLPOWER, ENCH_THORNS, ENCH_WOUNDING, };

	case(ITEM_FLASK):
		return { ENCH_CAPACITY, ENCH_CURING, ENCH_EMPOWERING, ENCH_HASTE, ENCH_MAGIC_RESTORE, ENCH_REGEN, ENCH_STONESKIN, ENCH_WRATH, };

	case(ITEM_SPELLRUNE):
		return { ENCH_AFF_ARCANE, ENCH_AFF_ELECTRIC, ENCH_AFF_FIRE, ENCH_AFF_POISON, ENCH_MAGIC, ENCH_MANALEECH, ENCH_SPELLPOWER, };

	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_CHESTPIECE):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_SHOULDERS):
		return { ENCH_ACCURACY, ENCH_AFF_ARCANE, ENCH_AFF_ELECTRIC, ENCH_AFF_FIRE, ENCH_AFF_POISON, ENCH_FURY,  ENCH_GREED, ENCH_LEECHING, ENCH_LIFE, ENCH_LIGHT,
			ENCH_MAGIC, ENCH_RAGE, ENCH_SLAYING, ENCH_SPELLPOWER, ENCH_THORNS, ENCH_WOUNDING, };

	case(ITEM_QUIVER):
		return { ENCH_ACCURACY, ENCH_ARCANE, ENCH_BURNING, ENCH_FURY, ENCH_LEECHING, ENCH_LIGHTNING,
					ENCH_MANALEECH, ENCH_RAGE, ENCH_SHARPNESS, ENCH_SLAYING, ENCH_WOUNDING, };

	case(ITEM_SHIELD):
		return { ENCH_ARMOURING, ENCH_DEFENCE, ENCH_FURY,  ENCH_GREED, ENCH_LIFE, ENCH_MAGIC, ENCH_MANALEECH, ENCH_SHARPNESS, ENCH_SPELLPOWER, ENCH_THORNS, ENCH_WOUNDING, };

	case(ITEM_WEAPON):
		return { ENCH_ACCURACY, ENCH_ARCANE, ENCH_ARMOURING, ENCH_BURNING, ENCH_DEFENCE, ENCH_FURY,
			ENCH_LEECHING, ENCH_LIGHTNING, ENCH_MANALEECH, ENCH_MAGIC, ENCH_RAGE, ENCH_SHARPNESS, ENCH_SLAYING, ENCH_VENOM, ENCH_WOUNDING, };
	}

	return { };
}


itemPtr lootgen::generateStartingWeapon()
{
	//	Randomized type
	BaseWeaponType bwt;
	const int r = randint(1, 5);
	switch (r)
	{
	case(1):	bwt = BaseWeaponType::DAGGER; break;
	case(2):	bwt = BaseWeaponType::CLUB; break;
	case(3):	bwt = BaseWeaponType::SHORTSWORD; break;
	case(4):	bwt = BaseWeaponType::SPEAR; break;
	default:	bwt = BaseWeaponType::BATTLE_AXE; break;
	}
	auto it = generateWeaponOfType(bwt);

	//	Fixed durability
	it->setMaxDurability(10);
	it->_damageTaken = 0;
	return it;
}


itemPtr lootgen::generateStartingFlask()
{
	auto it = itemPtr(new item("meagre flask", ITEM_FLASK, 0));
	it->setProperty(PROP_HEAL_ON_USE, 35);
	it->setProperty(PROP_MAX_CHARGES, 2);
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
	case(ITEM_QUIVER):
	case(ITEM_SHIELD):
	case(ITEM_WEAPON):
		return 2;

	case(ITEM_AMULET):
	case(ITEM_HELMET):
	case(ITEM_RING):
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
	case(ENCH_AFF_ARCANE):
	case(ENCH_AFF_ELECTRIC):
	case(ENCH_AFF_FIRE):
	case(ENCH_AFF_POISON):		return randint(5, 10) * 5;
	case(ENCH_ARMOURING):		return randint(1, 5);
	case(ENCH_BURNING):			return randint(1, 6);
	case(ENCH_CAPACITY):		return randint(2, 3);
	case(ENCH_CHARGING):		return randint(4, 8) * 5;
	case(ENCH_CURING):			return 100;
	case(ENCH_DEFENCE):			return randint(1, 3);
	case(ENCH_EMPOWERING):		return randint(5, 15);
	case(ENCH_FLAMEWARD):		return randint(5, 15);
	case(ENCH_FURY):			return randint(5, 15) * 5;
	case(ENCH_GREED):			return randint(3, 5) * 10;
	case(ENCH_HASTE):			return randint(5, 12);
	case(ENCH_LEECHING):		return randint(1, 10);
	case(ENCH_LIFE):			return randint(5, 20);
	case(ENCH_LIGHT):			return randint(1, 3);
	case(ENCH_LIGHTNING):		return randint(1, 6);
	case(ENCH_MAGIC):			return randint(1, 3);
	case(ENCH_MAGIC_RESTORE):	return randint(5, 10);
	case(ENCH_MANALEECH):		return randint(2, 4);
	case(ENCH_POISON_WARD):		return randint(5, 15);
	case(ENCH_RAGE):			return randint(5, 10);
	case(ENCH_REGEN):			return randint(4, 8);
	case(ENCH_RESISTANCE):		return randint(8, 13);
	case(ENCH_SHARPNESS):		return randint(1, 5);
	case(ENCH_SLAYING):			return randint(3, 7) * 10;
	case(ENCH_SPELLPOWER):		return randint(1, 5) * 10;
	case(ENCH_SPELLWARD):		return randint(5, 15);
	case(ENCH_STONESKIN):		return randint(5, 12);
	case(ENCH_STORMWARD):		return randint(5, 15);
	case(ENCH_THORNS):			return randint(1, 8);
	case(ENCH_VENOM):			return randint(2, 5);
	case(ENCH_WOUNDING):		return randint(3, 6);
	case(ENCH_WRATH):			return randint(5, 12);
	default:
		return 0;
	}
}



//	Adds the given number of random enchantments to an item.
void lootgen::enchantItem(itemPtr it, int count)
{
	bool is_2h = it->_isTwoHanded && !it->isRangedWeapon();

	//	sockets?
	if (it->_tier > 1 || roll_one_in(10))
	{
		const int max_sockets = getMaxSocketsForCategory(it->_category);
		if (max_sockets > 0 && roll_one_in(2))
		{
			int sockets = randint(0, max_sockets);
			if (sockets > 0 && is_2h) sockets++;
			it->adjustMaxSockets(sockets);
		}
	}

	//	enchants
	auto options = getEnchantmentsForItemCategory(it->_category);
	while (count-- > 0)
	{
		//	Pick a random enchant
		if (options.empty()) break;
		auto i = randrange(options.size());

		//	Roll a bonus
		auto bns = rollEnchantmentBonus(options[i]);
		bns += (it->_tier - 1) * getEnchantmentIncrement(options[i]);

		//	2h weapons have an increased bonus
		if (is_2h) bns += bns / 2;

		//	add the enchant
		it->addEnchantment(options[i], bns);
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
	if (doesGemHaveSingularEffect(gem))
		it->_enhancementLevel = 4;
	return it;
}


//	Generate a gem of a random type & the given tier and rarity.
itemPtr lootgen::generateGem(const int tier, const int rarity)
{
	GemType gem_type;
	
	//	chance of special gem, if allowed
	if (rarity >= 3 && roll_one_in(2))
		gem_type = SPECIAL_GEM_TYPES[randrange(SPECIAL_GEM_TYPES.size())];
	else
		gem_type = BASIC_GEM_TYPES[randrange(BASIC_GEM_TYPES.size())];

	return generateGemOfType(gem_type, tier, rarity);
}


//	Create a random flask.
itemPtr lootgen::generateFlask(const int tier, const int rarity)
{
	//	Flask tier
	string name = "flask";
	switch (tier)
	{
	case(1):	name = "minor " + name; break;
	case(2):	name = "major " + name; break;
	case(3):	name = "grand " + name; break;
	}

	//	Generate the actual item
	auto it = itemPtr(new item(name, ITEM_FLASK, rarity));
	it->setProperty(PROP_HEAL_ON_USE, randint(4, 6) * tier * 25);
	it->setProperty(PROP_MAX_CHARGES, randint(2, 3));
	it->setProperty(PROP_CHARGES_ON_HIT, randint(2, 5 + tier + rarity));
	it->setProperty(PROP_CHARGE_REGAIN_RATE, randint(10, 25));

	//	Enchantment level. Always has the maximum amount.
	it->_enhancementLevel = rarity;
	it->_tier = tier;

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
			dv = tier * 3;
			av = tier;
			break;
		case(2):
			name = "mail";
			switch (tier)
			{
			case(2):	name = "gothic " + name; break;
			case(3):	name = "titan " + name; break;
			}
			av = 3 + 3 * tier;
			break;
		}
	}
	else if (cat == ITEM_SHIELD)
	{
		switch (wt)
		{
		case(1):
			switch (tier)
			{
			case(1):	name = "buckler"; break;
			case(2):	name = "kite shield"; break;
			case(3):	name = "demon shield"; break;
			}
			dv = 1 + tier * 3;
			break;

		case(2):
			switch (tier)
			{
			case(1):	name = "heavy shield"; break;
			case(2):	name = "tower shield"; break;
			case(3):	name = "titan shield"; break;
			}
			dv = tier;
			av = tier * 3;
			acc = -tier;
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
			dv = tier + MAX(tier - 1, 0);
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
			av = tier + MAX(tier - 1, 0);
			break;
		}

		switch (tier)
		{
		case(2):	name = "great " + name; break;
		case(3):	name = "grand " + name; break;
		}
		
	}


	//	Set properties.
	auto it = itemPtr(new item(name, cat, 0));
	it->setProperty(PROP_ARMOUR_VALUE, av);
	it->setProperty(PROP_DEFENCE, dv);
	it->setProperty(PROP_ACCURACY_MOD, acc);


	//	Resistances.
	for (auto res : RESIST_PROPS)
	{
		int val = 2 * tier * wt;
		switch (cat)
		{
		case(ITEM_CHESTPIECE):
			it->setProperty(res, randint(val, val + val / 2));
			break;

		default:
			it->setProperty(res, randint(tier * 2, val));
			break;
		}
	}

	//	Roll durability.
	it->setMaxDurability(dieRoll(4, 6) + 10);
	it->_damageTaken = randint(0, it->_maxDurability / 2);
	it->_armourCategory = static_cast<ArmourCategory>(wt - 1);

	return it;
}


//	Picks a random type of armour piece.
itemPtr lootgen::generateArmourPiece(const int tier, const int rarity)
{
	auto it = generateArmourPieceOfType(ARMOUR_CATEGORIES[randrange(ARMOUR_CATEGORIES.size())], tier);
	it->_rarity = rarity;
	it->_enhancementLevel = rollEnhancementLevel(it->_rarity);
	return it;
}


//	Base weapon of the given type.
itemPtr lootgen::generateWeaponOfType(const BaseWeaponType bwt)
{
	string name = "";
	int dam = 1, var = 0, crit = 0, mult = 25, acc = 0;
	switch (bwt)
	{
	case(BaseWeaponType::BASTARD_SWORD):
		name = "bastard sword";
		dam = 12; var = 4; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::BATTLE_AXE):
		name = "battle-axe";
		dam = 4; var = 2; crit = 3; mult = 150; acc = -2;
		break;
	case(BaseWeaponType::BROAD_AXE):
		name = "demon axe";
		dam = 24; var = 6; crit = 3; mult = 150; acc = -4;
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
		dam = 24; var = 7; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::DEMONGLAIVE):
		name = "demon glaive";
		dam = 70; var = 15; crit = 5; mult = 100; acc = -1;
		break;
	case(BaseWeaponType::DEMON_GREATBLADE):
		name = "ultrablade";
		dam = 60; var = 18; crit = 5; mult = 100; acc = -1;
		break;
	case(BaseWeaponType::DEMON_PIKE):
		name = "demon pike";
		dam = 28; var = 5; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::DOUBLE_AXE):
		name = "double axe";
		dam = 30; var = 5; crit = 3; mult = 150; acc - -5;
		break;
	case(BaseWeaponType::DOUBLE_CROSSBOW):
		name = "ultra crossbow";
		dam = 36; var = 10; crit = 3; mult = 100; acc = -4;
		break;
	case(BaseWeaponType::GREAT_AXE):
		name = "greataxe";
		dam = 12; var = 3; crit = 3; mult = 150; acc = -2;
		break;
	case(BaseWeaponType::GREATBOW):
		name = "demon bow";
		dam = 24; var = 7; crit = 5; mult = 100; acc = -3;
		break;
	case(BaseWeaponType::GREATCLUB):
		name = "greatclub";
		dam = 14; var = 4; crit = 1; mult = 50; acc = -3;
		break;
	case(BaseWeaponType::GREATHAMMER):
		name = "greathammer";
		dam = 30; var = 6; crit = 1; mult = 50; acc = -4;
		break;
	case(BaseWeaponType::GREATMAUL):
		name = "titan maul";
		dam = 75; var = 14; crit = 1; mult = 50; acc = -4;
		break;
	case(BaseWeaponType::GREATSWORD):
		name = "greatsword";
		dam = 30; var = 7; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::HALBERD):
		name = "halberd";
		dam = 35; var = 5; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::HEAVY_CROSSBOW):
		name = "heavy crossbow";
		dam = 16; var = 3; crit = 3; mult = 100; acc = -2;
		break;
	case(BaseWeaponType::LONGBOW):
		name = "longbow";
		dam = 10; var = 4; crit = 5; mult = 100; acc = -1;
		break;
	case(BaseWeaponType::LONGSPEAR):
		name = "longspear";
		dam = 15; var = 2; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::LONGSWORD):
		name = "longsword";
		dam = 14; var = 6; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::SPEAR):
		name = "spear";
		dam = 8; var = 1; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::LONG_KNIFE):
		name = "starknife";
		dam = 12; var = 3; crit = 15; mult = randint(30, 40) * 10; acc = 2;
		break;
	case(BaseWeaponType::MACE):
		name = "mace";
		dam = 16; var = 3; crit = 1; mult = 50;
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
		dam = 6; var = 2; crit = 15; mult = randint(25, 35) * 10; acc = 2;
		break;
	case(BaseWeaponType::TITAN_AXE):
		name = "titan axe";
		dam = 45; var = 6; crit = 3; mult = 150; acc = -4;
		break;
	case(BaseWeaponType::WARHAMMER):
		name = "demon mace";
		dam = 22; var = 4; crit = 1; mult = 50; acc = -1;
		break;
	case(BaseWeaponType::WARPIKE):
		name = "war pike";
		dam = 18; var = 4; crit = 5; mult = 100;
		break;
	case(BaseWeaponType::WAR_AXE):
		name = "war axe";
		dam = 14; var = 3; crit = 3; mult = 150; acc = -2;
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
	case(BaseWeaponType::TITAN_AXE):
	case(BaseWeaponType::WAR_AXE):
		it->setProperty(PROP_CLEAVE_DAMAGE, 5 + randint(dam / 3, dam / 2));
		break;

	case(BaseWeaponType::BASTARD_SWORD):
	case(BaseWeaponType::DEMON_BLADE):
	case(BaseWeaponType::DEMON_GREATBLADE):
	case(BaseWeaponType::GREATSWORD):
	case(BaseWeaponType::LONGSWORD):
	case(BaseWeaponType::SHORTSWORD):
		it->setProperty(PROP_RIPOSTE_CHANCE, randint(70, 95));
		it->setProperty(PROP_RIPOSTE_DAMAGE, 150);
		break;

	case(BaseWeaponType::CLUB):
	case(BaseWeaponType::GREATCLUB):
	case(BaseWeaponType::GREATHAMMER):
	case(BaseWeaponType::GREATMAUL):
	case(BaseWeaponType::MACE):
	case(BaseWeaponType::WARHAMMER):
		it->setProperty(PROP_STAGGER_CHANCE, randint(25, 75));
		break;

	case(BaseWeaponType::SPEAR):
	case(BaseWeaponType::WARPIKE):
	case(BaseWeaponType::DEMON_PIKE):
	case(BaseWeaponType::LONGSPEAR):
	case(BaseWeaponType::HALBERD):
	case(BaseWeaponType::DEMONGLAIVE):
		it->setProperty(PROP_PENETRATION_DAMAGE, 100 + 20 * randint(3, 5));
		break;

	case(BaseWeaponType::SHORTBOW):
	case(BaseWeaponType::LONGBOW):
	case(BaseWeaponType::GREATBOW):
		it->setProperty(PROP_ATTACK_RANGE, randint(12, 15));
		break;

	case(BaseWeaponType::CROSSBOW):
	case(BaseWeaponType::HEAVY_CROSSBOW):
	case(BaseWeaponType::DOUBLE_CROSSBOW):
		it->setProperty(PROP_ATTACK_RANGE, randint(4, 8));
		it->setProperty(PROP_KNOCKBACK_CHANCE, randint(30, 75));
		break;
	}

	//	Set 2h flag
	switch (bwt)
	{
	case(BaseWeaponType::BASTARD_SWORD):
	case(BaseWeaponType::DOUBLE_AXE):
	case(BaseWeaponType::GREAT_AXE):
	case(BaseWeaponType::GREATCLUB):
	case(BaseWeaponType::DEMON_GREATBLADE):
	case(BaseWeaponType::GREATSWORD):
	case(BaseWeaponType::GREATHAMMER):
	case(BaseWeaponType::GREATMAUL):
	case(BaseWeaponType::TITAN_AXE):
	case(BaseWeaponType::LONGSPEAR):
	case(BaseWeaponType::HALBERD):
	case(BaseWeaponType::DEMONGLAIVE):

	case(BaseWeaponType::SHORTBOW):
	case(BaseWeaponType::LONGBOW):
	case(BaseWeaponType::GREATBOW):
	case(BaseWeaponType::CROSSBOW):
	case(BaseWeaponType::HEAVY_CROSSBOW):
	case(BaseWeaponType::DOUBLE_CROSSBOW):

		it->_isTwoHanded = true;
		break;
	}
	return it;
}


//	Create a random weapon of the given rarity and tier.
itemPtr lootgen::generateWeapon(const int tier, const int rarity)
{
	//	chance of quiver instead
	itemPtr it;
	if (roll_one_in(8))
		it = generateQuiver(tier, rarity);
	else
	{
		auto wtable = getWeaponTypesOfTier(tier);
		it = generateWeaponOfType(wtable[randrange(wtable.size())]);
	}

	//	Roll durability.
	it->setMaxDurability(dieRoll(4, 6) + 10);
	it->_damageTaken = randint(0, it->_maxDurability / 2);

	//	complete item
	it->_rarity = rarity;
	it->_enhancementLevel = rollEnhancementLevel(it->_rarity);

	return it;
}


//	Secondary shield-like item to go alongside bows.
itemPtr lootgen::generateQuiver(const int tier, const int rarity)
{
	//	name depends on tier
	string name = "quiver";
	if		(tier == 3)	name = "grand quiver";
	else if (tier == 2)	name = "great quiver";

	auto it = itemPtr(new item(name, ITEM_QUIVER, rarity));
	it->setProperty(PROP_ACCURACY_MOD, randint(tier, tier * 2));
	it->setProperty(PROP_BASE_DAMAGE, randint(tier, tier * 2));
	return it;
}


//	Either an amulet or ring (rings are more common)
itemPtr lootgen::generateJewel(const int tier, const int rarity)
{
	//	base type
	string name = "ring";
	auto cat = ITEM_RING;
	if (roll_one_in(4))
	{
		cat = ITEM_AMULET;
		name = "amulet";
	}

	//	tier adjusts name
	if		(tier == 3)	name = "jeweled " + name;
	else if (tier == 2)	name = "studded " + name;

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
	const vector<Spell>* options;
	switch (tier)
	{
	case(3):	options = &SPELLS_TIER_3; break;
	case(2):	options = &SPELLS_TIER_2; break;
	default:	options = &SPELLS_TIER_1; break;
	}
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

	//	name
	string name = "rune of " + getSpellName(sp);

	//	Generate the item.
	auto it = itemPtr(new item(name, ITEM_SPELLRUNE, rarity));
	it->_containsSpell = sp;
	it->_spellLevel = lvl;

	//	Enchantments.
	it->_enhancementLevel = it->getMaxEnhancementLevel();

	return it;
}


//	Rolls 3 random spells we can gain a level in.   ***	NO LONGER USED, DELETE ***
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


//	These are special items from a distinct pool of enchants, which are otherwise generated like normal songs.
itemPtr lootgen::generateLegendaryItem(const int maxTier, ItemEnchantment en)
{
	//	pick an enchantment if none is given
	itemPtr it;
	if(en == ENCH__NONE)
		en = LEGENDARY_ENCHANTS[randrange(LEGENDARY_ENCHANTS.size())];

	//	generate the associated item
	switch (en)
	{
	case(ENCH_AFFINITY):
		it = generateArmourPiece(maxTier, 4);
		it->addEnchantment(ENCH_AFFINITY, 100);
		break;

	case(ENCH_ARCANE_SHIELD):
		it = generateArmourPieceOfType(ITEM_SHIELD, maxTier);
		it->setProperty(PROP_DEFENCE, 3);
		it->addEnchantment(ENCH_ARCANE_SHIELD, randint(5, 15));
		break;

	case(ENCH_AVARICE):
		it = generateJewel(maxTier, 4);
		it->addEnchantment(ENCH_AVARICE, 1);
		break;

	case(ENCH_BERSERKER):
		it = generateArmourPiece(maxTier, 4);
		it->addEnchantment(ENCH_BERSERKER, randint(5, 10));
		break;

	case(ENCH_BLACKBLOOD):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_BLACKBLOOD, randint(30, 50));
		break;

	case(ENCH_CONDUCTING):
		it = generateArmourPiece(maxTier, 4);
		it->addEnchantment(ENCH_CONDUCTING, randint(20, 25) * 10);
		break;

	case(ENCH_CRUCIBLE):
		it = generateFlask(maxTier, 4);
		it->addEnchantment(ENCH_CRUCIBLE, randint(50, 70));
		break;

	case(ENCH_CUNNING):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_CUNNING, randint(200, 300));
		break;

	case(ENCH_DANCER):
		it = generateArmourPiece(maxTier, 4);
		it->addEnchantment(ENCH_DANCER, 25);
		break;

	case(ENCH_DARK_ARCANA):
		it = generateJewel(maxTier, 4);
		it->addEnchantment(ENCH_DARK_ARCANA, randint(10, 20));
		break;

	case(ENCH_DERVISH):
		it = generateJewel(maxTier, 4);
		it->addEnchantment(ENCH_DERVISH, randint(5, 10));
		break;

	case(ENCH_DIVINE):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_DIVINE, randint(6, 9));
		break;

	case(ENCH_KINSLAYER):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_KINSLAYER, randint(5, 10));
		break;

	case(ENCH_LEAGUE):
		it = generateArmourPieceOfType(ITEM_BOOTS, maxTier);
		it->_rarity = 4;
		it->addEnchantment(ENCH_LEAGUE, 25);
		break;

	case(ENCH_MAGEBLOOD):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_MAGEBLOOD, randint(4, 9));
		break;

	case(ENCH_PETRIFYING):
		it = generateArmourPiece(maxTier, 4);
		it->addEnchantment(ENCH_PETRIFYING, randint(4, 8));
		break;

	case(ENCH_FIREBURST):
	case(ENCH_STORMBURST):
	case(ENCH_VENOMBURST):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(en, randint(7, 10) + 5 * maxTier);
		break;

	case(ENCH_FLAMESPIKE):
	case(ENCH_THUNDERSPIKE):
	case(ENCH_VENOMSPIKE):
		it = generateArmourPiece(maxTier, 4);
		it->addEnchantment(en, randint(5, 8) * maxTier);
		break;

	case(ENCH_SADIST):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(en, randint(7, 9) * 10);
		break;

	case(ENCH_SHADOWSTRIKE):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_SHADOWSTRIKE, randint(5, 10));
		break;

	case(ENCH_SKYSPLITTER):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_SKYSPLITTER, randint(4, 6) + maxTier);
		break;

	case(ENCH_SPELLBURN):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_SPELLBURN, randint(20, 25) * 10);
		break;

	case(ENCH_WEIGHT):
		it = generateWeapon(maxTier, 4);
		it->addEnchantment(ENCH_WEIGHT, randint(20, 25) * 10);
		break;


		//	THIS IS BAD, DON'T LET IT HAPPEN
	default:
		cout << "ERROR: Generated bad unique item enchantment " << en << endl;
		it = itemPtr(new item());
		break;
	}

	//	remaining enchants
	it->setNickname(getItemEnchantmentName(en));
	it->_rarity = 4;
	it->_enhancementLevel = randint(3, 4);
	return it;
}


//	3 tiers of items; better ones drop at higher levels
int lootgen::getLootTierForMonsterLevel(const int lvl)
{
	if		(lvl < 20)	return 1;
	else if (lvl < 30)	return 2;
	else				return 3;
}


//	Gems have more tiers than normal items, so they use a different table
int lootgen::getGemTierForMonsterLevel(const int lvl)
{
	if		(lvl <= 16)	return 1;
	else if (lvl <= 32)	return 2;
	else				return 3;
}


//	Rolls a no. of active enchants for an item of a given rarity.
const int lootgen::rollEnhancementLevel(const int rarity)
{
	const int r = randint(1, 100);
	if		(r <= 25)	return rarity - 1;
	else if (r <= 75)	return rarity;
	else				return rarity + 1;
}


//	Higher rarities are increasingly less likely.
const int lootgen::rollRarity(const int maxRarity)
{
	int r = randint(1, 100);
	if (r <= 3 && maxRarity > 3)
		return 4;
	else if (r <= 15 && maxRarity > 2)
		return 3;
	else if (r <= 50 && maxRarity > 1)
		return 2;
	else
		return 1;
}


//	Most likely to drop an item of the given tier, but a lower tier is still possible.
//	If a value greater than 3 is given, always generates tier 3.
const int lootgen::rollTier(const int maxTier)
{
	const int r = randint(1, 100);
	switch (maxTier)
	{
	case(1):			return 1;
	case(2):
		if (r <= 20)	return 1;
		else			return 2;
	case(3):
		if (r <= 20)	return 2;
		else			return 3;
	default:
		return 3;
	}
}


//	Generic item roller. Higher tiers/rarities are somewhat less likely.
//	If 'forceRarity' is set, the item is guaranteed to have the maximum possible rarity.
itemPtr lootgen::rollItemDrop(const int maxTier, const int bestRarityAllowed, bool forceRarity, bool exalt)
{
	//	Roll actual tier and rarity
	int tier = rollTier(maxTier);
	int rarity = forceRarity ? bestRarityAllowed : rollRarity(bestRarityAllowed);


	//	Legendary items use a special generator
	itemPtr it;
	if (rarity == 4)
		it = generateLegendaryItem(maxTier);


	//	Generate the base item
	else
	{
		int r = randint(1, 100);
		if		(r <= 40)	it = generateArmourPiece(tier, rarity);
		else if (r <= 80)	it = generateWeapon(tier, rarity);
		else if (r <= 90)	it = generateSpellrune(tier, rarity);
		else if (r <= 95)	it = generateJewel(tier, rarity);
		else				it = generateFlask(tier, rarity);
	}


	//	Certain item categories can't be exalted
	if (it->_category != ITEM_FLASK && it->_category != ITEM_SPELLRUNE)
		it->_exalted = exalt;


	//	Random enchantments.
	it->_tier = tier;
	if (it->_enhancementLevel > 0)
		enchantItem(it, it->_enhancementLevel);	


	//	Nickname for rares!!
	if (rarity == 3 && it->_category != ITEM_SPELLRUNE)
		it->setNickname(generateItemName());

	return it;
}


//	Returns a random ritual material.
MaterialType lootgen::rollRitualMaterial()
{
	const int r = randint(1, 3);
	switch (r)
	{
	case(1):	return MaterialType::SODDEN_FLESH;
	case(2):	return MaterialType::TOMB_IDOL;
	case(3):	return MaterialType::VIRIDIAN_GLASS;
	default:
		return MaterialType::__NONE;
	}
}
