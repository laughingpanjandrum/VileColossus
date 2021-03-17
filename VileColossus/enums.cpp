#include "enums.h"

const string getAttributeName(const Attribute attr)
{
	switch (attr)
	{
	case(ATTR_DEXTERITY):		return "Dexterity";
	case(ATTR_STRENGTH):		return "Strength";
	case(ATTR_WILLPOWER):		return "Willpower";
	default:
		return "";
	}
}

const string getPerkName(const Perk pk)
{
	switch (pk)
	{
	case(PERK_ACCURACY):			return "Accuracy";
	case(PERK_BASE_DAMAGE):			return "Physical Damage";
	case(PERK_CRIT_DAMAGE):			return "Crit Damage";
	case(PERK_HEALTH):				return "Health";
	case(PERK_HEALTH_ON_KILL):		return "Health on Kill";
	case(PERK_MAGIC):				return "Magic";
	case(PERK_REPRISAL):			return "Reprisal Damage";
	case(PERK_RESISTANCE):			return "All Resistances";
	case(PERK_SPELL_POWER):			return "Spell Power";
	case(PERK_WRATH_DAMAGE):		return "Wrath Damage";
		return "unknown perk " + to_string(pk);
	}
}

const int getPerkBonusPerRank(const Perk pk)
{
	switch (pk)
	{
	case(PERK_ACCURACY):			return 1;
	case(PERK_BASE_DAMAGE):			return 2;
	case(PERK_CRIT_DAMAGE):			return 10;
	case(PERK_HEALTH):				return 10;
	case(PERK_HEALTH_ON_KILL):		return 3;
	case(PERK_MAGIC):				return 1;
	case(PERK_REPRISAL):			return 3;
	case(PERK_RESISTANCE):			return 5;
	case(PERK_SPELL_POWER):			return 20;
	case(PERK_WRATH_DAMAGE):		return 25;
	default:
		return 1;
	}
}

const int getPerkMaxLevel(const Perk pk)
{
	switch (pk)
	{
	case(PERK_ACCURACY):			return 10;
	case(PERK_BASE_DAMAGE):			return 10;
	case(PERK_CRIT_DAMAGE):			return 10;
	case(PERK_HEALTH):				return 10;
	case(PERK_HEALTH_ON_KILL):		return 5;
	case(PERK_MAGIC):				return 5;
	case(PERK_REPRISAL):			return 5;
	case(PERK_RESISTANCE):			return 5;
	case(PERK_SPELL_POWER):			return 10;
	case(PERK_WRATH_DAMAGE):		return 10;
	default:
		return 1;
	}
}

string getItemCategoryName(const ItemCategory cat)
{
	switch (cat)
	{
	case(ITEM_AMULET):			return "Amulet";
	case(ITEM_BOOTS):			return "Boots";
	case(ITEM_BRACERS):			return "Bracers";
	case(ITEM_CHESTPIECE):		return "Chestpiece";
	case(ITEM_CONSUMABLE):		return "Consumable";
	case(ITEM_FLASK):			return "Flask";
	case(ITEM_GEM):				return "Gem";
	case(ITEM_GLOVES):			return "Gloves";
	case(ITEM_HELMET):			return "Helmet";
	case(ITEM_MATERIAL):		return "Material";
	case(ITEM_QUIVER):			return "Quiver";
	case(ITEM_RING):			return "Ring";
	case(ITEM_SHIELD):			return "Shield";
	case(ITEM_SHOULDERS):		return "Back";
	case(ITEM_SPELLRUNE):		return "Spellrune";
	case(ITEM_WEAPON):			return "Weapon";
	default:
		return "item category " + to_string(cat);
	}
}

string getArmourCategoryName(const ArmourCategory cat)
{
	switch (cat)
	{
	case(ArmourCategory::ARMOUR_LIGHT):			return "Light";
	case(ArmourCategory::ARMOUR_HEAVY):			return "Heavy";
	default:
		return "";
	}
}

const string getMaterialTypeName(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::BRIGHT_RUNE):		return "bright rune";
	case(MaterialType::FRAGMENTS):			return "fragments";
	case(MaterialType::GLASS_SHARD):		return "glass shards";
	case(MaterialType::GLOWING_POWDER):		return "luminous dust";
	case(MaterialType::MAGIC_DUST):			return "glowing goo";
	case(MaterialType::RADIANT_ASH):		return "radiant ash";
	case(MaterialType::RUNE_SHARD):			return "rune shards";

	case(MaterialType::NOTCHED_CUBE):		return "notched cube";

	case(MaterialType::SODDEN_FLESH):		return "sodden flesh";
	case(MaterialType::TOMB_IDOL):			return "tomb idol";
	case(MaterialType::VIRIDIAN_GLASS):		return "viridian glass";
	case(MaterialType::VILEDRAGON_SCALE):	return "viledragon scale";
	default:
		return "material";
	}
}

const colorType getMaterialTypeColor(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::BRIGHT_RUNE):		return TCODColor::celadon;
	case(MaterialType::FRAGMENTS):			return TCODColor::gold;
	case(MaterialType::GLASS_SHARD):		return TCODColor::silver;
	case(MaterialType::GLOWING_POWDER):		return TCODColor::lightPurple;
	case(MaterialType::MAGIC_DUST):			return TCODColor::lighterBlue;
	case(MaterialType::RADIANT_ASH):		return TCODColor::lightOrange;
	case(MaterialType::RUNE_SHARD):			return TCODColor::lightFuchsia;

	case(MaterialType::NOTCHED_CUBE):		return TCODColor::pink;

	case(MaterialType::SODDEN_FLESH):		return TCODColor::sea;
	case(MaterialType::TOMB_IDOL):			return TCODColor::crimson;
	case(MaterialType::VIRIDIAN_GLASS):		return TCODColor::cyan;
	case(MaterialType::VILEDRAGON_SCALE):	return TCODColor::flame;
	default:
		return COLOR_WHITE;
	}
}

bool isRitualMaterial(const MaterialType mat)
{
	return mat == MaterialType::SODDEN_FLESH || mat == MaterialType::TOMB_IDOL || mat == MaterialType::VIRIDIAN_GLASS || mat == MaterialType::VILEDRAGON_SCALE;
}

const string getRitualDescription(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::SODDEN_FLESH):		return "Reveals a portal to the sodden realms of Drowned Dogossa, the god of the deep. Slay Dogossa's avatar to gain power and face the Vile Colossus.";
	case(MaterialType::TOMB_IDOL):			return "Reveals a portal to the Tombs of Amog, which emanate from the infinite corpse of the dead god. Slay the avatar of Amog to gain power and face the Vile Colossus.";
	case(MaterialType::VIRIDIAN_GLASS):		return "Reveals a portal to Lost Viridia, which was taken by the abyss when its king sinned. Slay the avatar of the Viridian King to gain power and face the Vile Colossus.";
	case(MaterialType::VILEDRAGON_SCALE):	return "Use of this material attracts a Viledragon to the portal. These emissions of the Colossus are the most fearful horrors of the Abyss. Face one to grow in power.";
	case(MaterialType::__NONE):				return "Reveals a portal to the Outer Dark - the abyssal space beyond time. Slay horrors there to gain ritual materials.";
	default:
		return "unknown ritual";
	}
}

string getEquipmentSlotName(const EquipmentSlot slot)
{
	switch (slot)
	{
	case(SLOT_AMULET):			return "Neck";
	case(SLOT_BODY):			return "Body";
	case(SLOT_BOOTS):			return "Feet";
	case(SLOT_BRACERS):			return "Arms";
	case(SLOT_GLOVES):			return "Hands";
	case(SLOT_HELMET):			return "Head";
	case(SLOT_LEFT_RING):		return "L. Ring";
	case(SLOT_MAIN_HAND):		return "Main";
	case(SLOT_OFFHAND):			return "Offhand";
	case(SLOT_RIGHT_RING):		return "R. Ring";
	case(SLOT_SHOULDERS):		return "Back";
	default:
		return "";
	}
}

//	Equipment slot corresponding to the given item category.
//	Returns the primary slot for categories with multiple potential slots.
EquipmentSlot getSlotForCategory(const ItemCategory cat)
{
	switch (cat)
	{
	case(ITEM_AMULET):			return SLOT_AMULET;
	case(ITEM_BOOTS):			return SLOT_BOOTS;
	case(ITEM_BRACERS):			return SLOT_BRACERS;
	case(ITEM_CHESTPIECE):		return SLOT_BODY;
	case(ITEM_GLOVES):			return SLOT_GLOVES;
	case(ITEM_HELMET):			return SLOT_HELMET;
	case(ITEM_QUIVER):			return SLOT_OFFHAND;
	case(ITEM_RING):			return SLOT_LEFT_RING;
	case(ITEM_SHIELD):			return SLOT_OFFHAND;
	case(ITEM_SHOULDERS):		return SLOT_SHOULDERS;
	case(ITEM_WEAPON):			return SLOT_MAIN_HAND;
	default:
		return SLOT__NONE;
	}
}


//	Returns alternate equipment slot for the given category, if there is one.
EquipmentSlot getAltSlotForCategory(const ItemCategory cat)
{
	switch (cat)
	{
	case(ITEM_RING):			return SLOT_RIGHT_RING;
	case(ITEM_WEAPON):			return SLOT_OFFHAND;
	default:
		return SLOT__NONE;
	}
}


const string getItemPropertyName(const ItemProperty prop)
{
	switch (prop)
	{
	case(PROP_ACCURACY_MOD):			return "Accuracy";
	case(PROP_ARMOUR_VALUE):			return "Armour Value";
	case(PROP_ATTACK_RANGE):			return "Attack Range";
	case(PROP_BASE_DAMAGE):				return "Base Damage";
	case(PROP_CHARGES_ON_HIT):			return "Charges per Hit";
	case(PROP_CHARGE_REGAIN_RATE):		return "Charges per Kill";
	case(PROP_CLEAVE_DAMAGE):			return "Cleave Bonus Damage";
	case(PROP_CRITICAL_CHANCE):			return "Crit Chance";
	case(PROP_CRITICAL_DAMAGE):			return "Crit Damage";
	case(PROP_DAMAGE_VARIANCE):			return "Damage Variance";
	case(PROP_DEFENCE):					return "Defence";
	case(PROP_HEAL_ON_USE):				return "Heals";
	case(PROP_KNOCKBACK_CHANCE):		return "Knockback";
	case(PROP_MAX_CHARGES):				return "Max Charges";
	case(PROP_RESIST_ARCANE):			return "Resist Arcane";
	case(PROP_RESIST_ELECTRIC):			return "Resist Electric";
	case(PROP_RESIST_FIRE):				return "Resist Fire";
	case(PROP_RESIST_POISON):			return "Resist Poison";
	case(PROP_RIPOSTE_CHANCE):			return "Riposte Chance";
	case(PROP_RIPOSTE_DAMAGE):			return "Riposte Damage";
	case(PROP_STAGGER_CHANCE):			return "Stagger Chance";
	case(PROP_STAGGER_DURATION):		return "Stagger Duration";
	default:
		return "item property " + to_string(prop);
	}
}

const string formatItemProperty(const ItemProperty prop, int val)
{
	switch (prop)
	{
	case(PROP_ACCURACY_MOD):
	case(PROP_CLEAVE_DAMAGE):
	case(PROP_DEFENCE):
	case(PROP_ARMOUR_VALUE):
		return plusminus(val);

	case(PROP_CHARGES_ON_HIT):
	case(PROP_CHARGE_REGAIN_RATE):
	case(PROP_CRITICAL_CHANCE):
	case(PROP_CRITICAL_DAMAGE):
		return plusminus(val) + "%";

	case(PROP_KNOCKBACK_CHANCE):
	case(PROP_RIPOSTE_CHANCE):
	case(PROP_RIPOSTE_DAMAGE):
	case(PROP_STAGGER_CHANCE):
		return to_string(val) + '%';

	default:
		return to_string(val);
	}
}

const string getItemEnchantmentName(const ItemEnchantment en)
{
	switch (en)
	{
		//	Standard enchantments
	case(ENCH_ACCURACY):		return "accuracy";
	case(ENCH_AFF_ARCANE):		return "sigils";
	case(ENCH_AFF_ELECTRIC):	return "the griffin";
	case(ENCH_AFF_FIRE):		return "the dragon";
	case(ENCH_AFF_POISON):		return "the serpent";
	case(ENCH_ARCANE):			return "arcane";
	case(ENCH_ARMOURING):		return "armour";
	case(ENCH_BURNING):			return "burning";
	case(ENCH_CAPACITY):		return "capacity";
	case(ENCH_CHARGING):		return "charging";
	case(ENCH_CURING):			return "curing";
	case(ENCH_DEFENCE):			return "defence";
	case(ENCH_EMPOWERING):		return "empowering";
	case(ENCH_FLAMEWARD):		return "flameguard";
	case(ENCH_FURY):			return "fury";
	case(ENCH_GREED):			return "greed";
	case(ENCH_HASTE):			return "haste";
	case(ENCH_LEECHING):		return "leeching";
	case(ENCH_LIFE):			return "life";
	case(ENCH_LIGHT):			return "light";
	case(ENCH_LIGHTNING):		return "lightning";
	case(ENCH_MAGIC):			return "magic";
	case(ENCH_MAGIC_RESTORE):	return "restore magic";
	case(ENCH_MANALEECH):		return "manaleech";
	case(ENCH_POISON_WARD):		return "venomguard";
	case(ENCH_RAGE):			return "rage";
	case(ENCH_REGEN):			return "regeneration";
	case(ENCH_RESISTANCE):		return "resistance";
	case(ENCH_SHARPNESS):		return "sharpness";
	case(ENCH_SLAYING):			return "slaying";
	case(ENCH_SPELLPOWER):		return "spellpower";
	case(ENCH_SPELLWARD):		return "spellguard";
	case(ENCH_STONESKIN):		return "stoneskin";
	case(ENCH_STORMWARD):		return "stormguard";
	case(ENCH_THORNS):			return "thorns";
	case(ENCH_VENOM):			return "venom";
	case(ENCH_WOUNDING):		return "wounding";
	case(ENCH_WRATH):			return "wrath";

		//	Unique enchantments
	case(ENCH_AFFINITY):		return "Affinity";
	case(ENCH_ARCANE_SHIELD):	return "Arcane Mirror";
	case(ENCH_AVARICE):			return "Avarice";
	case(ENCH_BLACKBLOOD):		return "Blackblood";
	case(ENCH_CONDUCTING):		return "Fierce Conductor";
	case(ENCH_CRUCIBLE):		return "Crucible";
	case(ENCH_DERVISH):			return "Dervish";
	case(ENCH_CUNNING):			return "Rat's Cunning";
	case(ENCH_DARK_ARCANA):		return "Dark Arcana";
	case(ENCH_DIVINE):			return "Divinity";
	case(ENCH_FIREBURST):		return "Fireburst";
	case(ENCH_FLAMESPIKE):		return "Flamespike";
	case(ENCH_KINSLAYER):		return "Kinslayer";
	case(ENCH_LEAGUE):			return "League";
	case(ENCH_MAGEBLOOD):		return "Mageblood";
	case(ENCH_PETRIFYING):		return "Petrify";
	case(ENCH_SHADOWSTRIKE):	return "Shade";
	case(ENCH_SKYSPLITTER):		return "Skysplitter";
	case(ENCH_SPELLBURN):		return "Spellburner";
	case(ENCH_STORMBURST):		return "Stormburst";
	case(ENCH_THUNDERSPIKE):	return "Thunderspike";
	case(ENCH_VENOMBURST):		return "Venomburst";
	case(ENCH_VENOMSPIKE):		return "Venomspike";
	case(ENCH_WEIGHT):			return "Dragonbone";

	default:
		return "enchantment_" + to_string(en);
	}
}

const string getItemEnchantmentDescription(const ItemEnchantment en)
{
	switch (en)
	{
	case(ENCH_ACCURACY):		return "Global Accuracy";
	case(ENCH_AFF_ARCANE):		return "Arcane Bonus";
	case(ENCH_AFF_ELECTRIC):	return "Electric Bonus";
	case(ENCH_AFF_FIRE):		return "Fire Bonus";
	case(ENCH_AFF_POISON):		return "Poison Bonus";
	case(ENCH_ARCANE):			return "Arcane Damage";
	case(ENCH_ARMOURING):		return "Armour Value";
	case(ENCH_BURNING):			return "Fire Damage";
	case(ENCH_CAPACITY):		return "Max Charges";
	case(ENCH_CHARGING):		return "Charges/Kill";
	case(ENCH_CURING):			return "Cure Afflictions";
	case(ENCH_DEFENCE):			return "Defence Value";
	case(ENCH_EMPOWERING):		return "Empower Duration";
	case(ENCH_FLAMEWARD):		return "Fire Resistance";
	case(ENCH_FURY):			return "Wrath Damage";
	case(ENCH_GREED):			return "Fragments Dropped";
	case(ENCH_HASTE):			return "Haste";
	case(ENCH_LEECHING):		return "Health on Kill";
	case(ENCH_LIFE):			return "Health";
	case(ENCH_LIGHT):			return "Light Radius";
	case(ENCH_LIGHTNING):		return "Electric Damage";
	case(ENCH_MAGIC):			return "Max Magic";
	case(ENCH_MAGIC_RESTORE):	return "Regain Magic";
	case(ENCH_MANALEECH):		return "Mana on Kill";
	case(ENCH_POISON_WARD):		return "Poison Resistance";
	case(ENCH_RAGE):			return "Wrath on Kill Chance";
	case(ENCH_REGEN):			return "Regeneration";
	case(ENCH_RESISTANCE):		return "All Resistances";
	case(ENCH_SHARPNESS):		return "Critical Chance";
	case(ENCH_SLAYING):			return "Critical Damage";
	case(ENCH_SPELLPOWER):		return "Spell Power";
	case(ENCH_SPELLWARD):		return "Arcane Resistance";
	case(ENCH_STONESKIN):		return "Stoneskin";
	case(ENCH_STORMWARD):		return "Electric Resistance";
	case(ENCH_THORNS):			return "Reprisal Damage";
	case(ENCH_VENOM):			return "Poison Damage";
	case(ENCH_WOUNDING):		return "Weapon Damage";
	case(ENCH_WRATH):			return "Wrath";

		//	Legendary enchants
	case(ENCH_AFFINITY):		return "Elemental Affinity";
	case(ENCH_ARCANE_SHIELD):	return "Arcane Shield";
	case(ENCH_AVARICE):			return "Avarice";
	case(ENCH_BLACKBLOOD):		return "Blackblood";
	case(ENCH_CONDUCTING):		return "Conduction";
	case(ENCH_CRUCIBLE):		return "Charge on Crit";
	case(ENCH_CUNNING):			return "Cunning";
	case(ENCH_DARK_ARCANA):		return "Dark Arcana";
	case(ENCH_DERVISH):			return "Wrath Crit Chance";
	case(ENCH_DIVINE):			return "Divine Damage";
	case(ENCH_FIREBURST):		return "Fireburst";
	case(ENCH_FLAMESPIKE):		return "Flamespike";
	case(ENCH_KINSLAYER):		return "Kinslay";
	case(ENCH_LEAGUE):			return "Move Speed";
	case(ENCH_MAGEBLOOD):		return "Arcane Bolt";
	case(ENCH_PETRIFYING):		return "Petrifying";
	case(ENCH_SHADOWSTRIKE):	return "Shadowstrike";
	case(ENCH_SKYSPLITTER):		return "Lightning Strike";
	case(ENCH_SPELLBURN):		return "Spellburn";
	case(ENCH_STORMBURST):		return "Stormburst";
	case(ENCH_THUNDERSPIKE):	return "Thunderspike";
	case(ENCH_VENOMBURST):		return "Venomburst";
	case(ENCH_VENOMSPIKE):		return "Venomspike";
	case(ENCH_WEIGHT):			return "Massive";

	default:
		return "enchantment_" + to_string(en);
	}
}

const string getItemEnchantmentVerbose(const ItemEnchantment en, const int val)
{
	switch (en)
	{
	case(ENCH_ACCURACY):		return "Adjusts the accuracy of all attacks by " + plusminus(val) + ". Each point increases your chance to hit by about 5%.";
	case(ENCH_AFF_ARCANE):		return "Inflict " + to_string(val) + "% more Arcane damage with spells and weapon attacks.";
	case(ENCH_AFF_ELECTRIC):	return "Inflict " + to_string(val) + "% more Electric damage with spells and weapon attacks.";
	case(ENCH_AFF_FIRE):		return "Inflict " + to_string(val) + "% more Fire damage with spells and weapon attacks.";
	case(ENCH_AFF_POISON):		return "Inflict " + to_string(val) + "% more Poison damage with spells and weapon attacks.";
	case(ENCH_ARCANE):			return "Weapon attacks inflict up to " + to_string(val) + " bonus Arcane damage.";
	case(ENCH_ARMOURING):		return "Adjusts your Armour Value by " + plusminus(val) + ". This directly reduces physical damage taken.";
	case(ENCH_BURNING):			return "Weapon attacks inflict up to " + to_string(val) + " bonus Fire damage.";
	case(ENCH_CAPACITY):		return "The flask has " + to_string(val) + " additional charges.";
	case(ENCH_CHARGING):		return "Kills add " + to_string(val) + "% more charge to the flask.";
	case(ENCH_CURING):			return "Immediately removes Burn, Poison, and Shock status effects when quaffed.";
	case(ENCH_DEFENCE):			return "Adjusts your Defence Value by " + plusminus(val) + ", which reduces the chance that enemy weapon attacks will hit you.";
	case(ENCH_EMPOWERING):		return "Buffs your Spell Power by 50%, plus half of its base value, for " + to_string(val) + " turns.";
	case(ENCH_FLAMEWARD):		return "Reduces Fire damage taken by " + to_string(val) + "%.";
	case(ENCH_FURY):			return "Inflict +" + to_string(val) + "% damage when buffed with Wrath.";
	case(ENCH_GREED):			return "Monsters drop +" + to_string(val) + "% more fragments.";
	case(ENCH_HASTE):			return "Temporarily increases your movement and attack speed.";
	case(ENCH_LEECHING):		return "Heal " + to_string(val) + " damage when you kill something.";
	case(ENCH_LIFE):			return "Adjusts your maximum health by " + plusminus(val) + ".";
	case(ENCH_LIGHT):			return "Adjusts your maximum vision radius by " + plusminus(val) + ".";
	case(ENCH_LIGHTNING):		return "Weapon attacks inflict up to " + to_string(val) + " bonus Electric damage.";
	case(ENCH_MAGIC):			return "Adjusts your maximum Magic by " + plusminus(val) + ". You expend this to cast spells.";
	case(ENCH_MAGIC_RESTORE):	return "Replenishes " + to_string(val) + " Magic when quaffed.";
	case(ENCH_MANALEECH):		return "Recover " + to_string(val) + " Magic when you kill something.";
	case(ENCH_POISON_WARD):		return "Reduces Poison damage taken by " + to_string(val) + "%.";
	case(ENCH_RAGE):			return "Increases chance to gain the Wrath buff when you kill something by " + to_string(val) + "%. Wrath temporarily increases weapon damage you inflict.";
	case(ENCH_REGEN):			return "In addition to its immediate effect, heals 5% of our max health per turn for " + to_string(val) + " turns.";
	case(ENCH_RESISTANCE):		return "Increases Arcane, Electric, Fire, and Poison resistances by " + plusminus(val) + "%.";
	case(ENCH_SHARPNESS):		return "Weapon attacks are " + to_string(val) + "% more likely to be critical hits.";
	case(ENCH_SLAYING):			return "Critical hits inflict " + plusminus(val) + "% extra damage.";
	case(ENCH_SPELLPOWER):		return "Adjusts damage inflicted by spells by " + plusminus(val) + ".";
	case(ENCH_SPELLWARD):		return "Reduces Arcane damage taken by " + to_string(val) + "%.";;
	case(ENCH_STONESKIN):		return "Grants Stoneskin when quaffed for " + to_string(val) + " rounds, which increases your Armour Value by 2 points per level.";
	case(ENCH_STORMWARD):		return "Reduces Electric damage taken by " + to_string(val) + "%.";;
	case(ENCH_THORNS):			return "A melee attacker that damages you takes " + to_string(val) + " damage in return.";
	case(ENCH_VENOM):			return "Weapon attacks inflict up to " + to_string(val) + " bonus Poison damage.";
	case(ENCH_WOUNDING):		return "Weapon attacks inflict up to " + to_string(val) + " bonus physical damage.";
	case(ENCH_WRATH):			return "When quaffed, gain Wrath for " + to_string(val) + " rounds. Wrath increases weapon damage dealt.";


		//	Legendary enchants

	case(ENCH_AFFINITY):		return "Elemental bonuses are increased by the value of the associated Resistance.";
	case(ENCH_ARCANE_SHIELD):	return "Each point of Magic in your pool absorbs " + to_string(val) + " points of elemental damage when you are attacked (Arcane/Electric/Fire/Poison).";
	case(ENCH_AVARICE):			return "Your Fragments Gained bonus is also added to your critical damage.";
	case(ENCH_BLACKBLOOD):		return "Critical hits also inflict " + to_string(val) + " poison damage, scaling with your Poison Bonus.";
	case(ENCH_CONDUCTING):		return "Taking electric damage temporarily boosts your Electric Bonus by " + to_string(val) + "%.";
	case(ENCH_CRUCIBLE):		return "Regains " + to_string(val) + "% charge when you score a critical hit.";
	case(ENCH_CUNNING):			return "Inflict " + to_string(val) + "% more damage when your health is below 30%.";
	case(ENCH_DARK_ARCANA):		return "Weapon attacks inflict bonus Arcane damage equal to 1/10 of your Spell Power, up to " + plusminus(val) + ".";
	case(ENCH_DERVISH):			return "While affected by Wrath, your critical hit chance is increased by " + plusminus(val) + "%.";
	case(ENCH_DIVINE):			return "Inflict +" + to_string(val) + " damage to undead creatures, multiplied by your Light Radius.";
	case(ENCH_FIREBURST):		return "Weapon attacks inflict up to " + to_string(val) + " bonus Fire damage.";
	case(ENCH_FLAMESPIKE):		return "You inflict " + plusminus(val) + " Reprisal damage as Fire.";
	case(ENCH_KINSLAYER):		return "Each time you fail to crit, you gain a cumulative " + plusminus(val) + "% bonus to crit chance. This bonus is cleared on crit.";
	case(ENCH_LEAGUE):			return "Reduces your move delay by " + to_string(val) + ".";
	case(ENCH_MAGEBLOOD):		return "On hit, has a 20% chance to cast Arcane Bolt at level " + to_string(val) + ".";
	case(ENCH_PETRIFYING):		return "The Stoneskin buff increases your Armour Value by an additional +" + to_string(val) + ".";
	case(ENCH_SHADOWSTRIKE):	return "You critical chance increases by " + to_string(val) + "% if your vision radius is 6 or less.";
	case(ENCH_SKYSPLITTER):		return "On hit, has a 20% chance to cast Lightning Strike at level " + to_string(val) + ".";
	case(ENCH_SPELLBURN):		return "Raises spell power by " + to_string(val) + "%. You take damage when casting spells equal to twice their Magic cost.";
	case(ENCH_STORMBURST):		return "Weapon attacks inflict up to " + to_string(val) + " bonus Electric damage.";
	case(ENCH_THUNDERSPIKE):	return "You inflict " + plusminus(val) + " Reprisal damage as Electric.";
	case(ENCH_VENOMBURST):		return "Weapon attacks inflict up to " + to_string(val) + " bonus Poison damage.";
	case(ENCH_VENOMSPIKE):		return "You inflict " + plusminus(val) + " Reprisal damage as Poison.";
	case(ENCH_WEIGHT):			return "Inflicts " + plusminus(val) + "% damage, but attack speed is slowed.";

	default:
		return "enchantment_" + to_string(en);
	}
}


//	The verbose name displayed in the item's description. Adds little formatting niceties.
const string formatItemEnchantment(const ItemEnchantment en, const int val)
{
	auto name = getItemEnchantmentDescription(en);
	switch (en)
	{
		//	Numeric value is irrelevant
	case(ENCH_AFFINITY):
	case(ENCH_AVARICE):
	case(ENCH_CURING):
		return name;

		//	Plus percent
	case(ENCH_AFF_ARCANE):
	case(ENCH_AFF_ELECTRIC):
	case(ENCH_AFF_FIRE):
	case(ENCH_AFF_POISON):
	case(ENCH_CHARGING):
	case(ENCH_CRUCIBLE):
	case(ENCH_CUNNING):
	case(ENCH_DERVISH):
	case(ENCH_FLAMEWARD):
	case(ENCH_FURY):
	case(ENCH_GREED):
	case(ENCH_KINSLAYER):
	case(ENCH_POISON_WARD):
	case(ENCH_RAGE):
	case(ENCH_RESISTANCE):
	case(ENCH_SHADOWSTRIKE):
	case(ENCH_SHARPNESS):
	case(ENCH_SLAYING):
	case(ENCH_SPELLBURN):
	case(ENCH_SPELLPOWER):
	case(ENCH_SPELLWARD):
	case(ENCH_STORMWARD):
	case(ENCH_WEIGHT):
		return name + " #" + plusminus(val) + "%";

		//	Duration
	case(ENCH_EMPOWERING):
	case(ENCH_HASTE):
	case(ENCH_REGEN):
	case(ENCH_STONESKIN):
	case(ENCH_WRATH):
		return name + " #" + to_string(val) + " turns";

		//	Level (for spell trigger effects)
	case(ENCH_MAGEBLOOD):
	case(ENCH_SKYSPLITTER):
		return name + " #Lv" + to_string(val);
		
		//	Just the +bonus
	default:
		return name + " #" + plusminus(val);
	}
}


string getDamageTypeName(const DamageType dt)
{
	switch (dt)
	{
	case(DTYPE_ARCANE):		return "Arcane";
	case(DTYPE_ELECTRIC):	return "Electric";
	case(DTYPE_FIRE):		return "Fire";
	case(DTYPE_NORMAL):		return "Physical";
	case(DTYPE_POISON):		return "Poison";
	default:
		return "dtype_" + to_string(dt);
	}
}

colorType getDamageTypeColor(const DamageType dt)
{
	switch (dt)
	{
	case(DTYPE_ARCANE):		return TCODColor::fuchsia;
	case(DTYPE_ELECTRIC):	return TCODColor::darkYellow;
	case(DTYPE_FIRE):		return TCODColor::flame;
	case(DTYPE_NORMAL):		return TCODColor::lightGrey;
	case(DTYPE_POISON):		return TCODColor::lime;
	default:
		return COLOR_WHITE;
	}
}

string getStatusEffectName(const StatusEffect st)
{
	switch (st)
	{
	case(STATUS_AGONY):		return "Agony";
	case(STATUS_BURN):		return "Burn";
	case(STATUS_ENTANGLED):	return "Entangled";
	case(STATUS_POISON):	return "Poison";
	case(STATUS_SHOCK):		return "Shock";
	case(STATUS_SLUDGED):	return "Sludged";
	case(STATUS_STAGGER):	return "Stagger";
	default:
		return "status_effect";
	}
}

colorType getStatusEffectColor(const StatusEffect st)
{
	switch (st)
	{
	case(STATUS_AGONY):		return TCODColor::crimson;
	case(STATUS_BURN):		return TCODColor::flame;
	case(STATUS_ENTANGLED):	return TCODColor::white;
	case(STATUS_POISON):	return TCODColor::lime;
	case(STATUS_SHOCK):		return TCODColor::darkYellow;
	case(STATUS_SLUDGED):	return TCODColor::sepia;
	case(STATUS_STAGGER):	return TCODColor::lightBlue;
	default:
		return COLOR_WHITE;
	}
}

string getBuffName(const BuffType bf)
{
	switch (bf)
	{
	case(BUFF_ARCANE_PULSE):	return "Arcane Pulse";
	case(BUFF_CONDUCTION):		return "Conduction";
	case(BUFF_CRIT_BONUS):		return "Crit Chance";
	case(BUFF_EMPOWERED):		return "Empowered";
	case(BUFF_HASTE):			return "Haste";
	case(BUFF_REGENERATION):	return "Regeneration";
	case(BUFF_SMITE_EVIL):		return "Smite Undead";
	case(BUFF_STATIC_FIELD):	return "Static Field";
	case(BUFF_STONESKIN):		return "Stoneskin";
	case(BUFF_TOXIC_RADIANCE):	return "Toxic Radiance";
	case(BUFF_VENOMFANG):		return "Venomfang";
	case(BUFF_WRATH):			return "Wrath";
	default:
		return "buff_" + to_string(bf);
	}
}

string getGemTypeName(const GemType gem)
{
	switch (gem)
	{
	case(GemType::BLACKSTONE):		return "blackstone";
	case(GemType::BOLTSTONE):		return "boltstone";
	case(GemType::FLAMESTONE):		return "flamestone";
	case(GemType::SILVERSTONE):		return "silverstone";
	case(GemType::SPIDERSTONE):		return "spiderstone";
	default:
		return "gemstone??";
	}
}

string getGemTypeFullName(const GemType gem, const int tier)
{
	string name = getGemTypeName(gem);
	switch (tier)
	{
	case(1):	return "cracked " + name;
	case(2):	return "chipped " + name;
	case(3):	return name + " shard";
	case(4):	return name + " chunk";
	case(5):	return "whole " + name;
	case(6):	return "pure " + name;
	default:
		return "error_tier " + name;
	}
}

colorType getGemTypeColor(const GemType gem)
{
	switch (gem)
	{
	case(GemType::BLACKSTONE):		return TCODColor::purple;
	case(GemType::BOLTSTONE):		return getDamageTypeColor(DTYPE_ELECTRIC);
	case(GemType::FLAMESTONE):		return getDamageTypeColor(DTYPE_FIRE);
	case(GemType::SILVERSTONE):		return getDamageTypeColor(DTYPE_ARCANE);
	case(GemType::SPIDERSTONE):		return getDamageTypeColor(DTYPE_POISON);
	default:
		return TCODColor::darkGrey;
	}
}
