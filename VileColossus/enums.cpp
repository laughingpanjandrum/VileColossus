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

string getItemCategoryName(const ItemCategory cat)
{
	switch (cat)
	{
	case(ITEM_AMULET):			return "Amulet";
	case(ITEM_BOOTS):			return "Boots";
	case(ITEM_BRACERS):			return "Bracers";
	case(ITEM_CHESTPIECE):		return "Chestpiece";
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
	case(ARMOUR_LIGHT):			return "Light";
	case(ARMOUR_MEDIUM):		return "Medium";
	case(ARMOUR_HEAVY):			return "Heavy";
	default:
		return "";
	}
}

const string getMaterialTypeName(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::BRIGHT_RUNE):	return "bright rune";
	case(MaterialType::FRAGMENTS):		return "fragments";
	case(MaterialType::GLASS_SHARD):	return "glass shards";
	case(MaterialType::GLOWING_POWDER):	return "luminous dust";
	case(MaterialType::MAGIC_DUST):		return "glowing goo";
	case(MaterialType::RADIANT_ASH):	return "radiant ash";
	case(MaterialType::RUNE_SHARD):		return "rune shards";
	default:
		return "material";
	}
}

const colorType getMaterialTypeColor(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::BRIGHT_RUNE):	return TCODColor::lightPurple;
	case(MaterialType::FRAGMENTS):		return TCODColor::gold;
	case(MaterialType::GLASS_SHARD):	return TCODColor::silver;
	case(MaterialType::GLOWING_POWDER):	return TCODColor::lightYellow;
	case(MaterialType::MAGIC_DUST):		return TCODColor::lightBlue;
	case(MaterialType::RADIANT_ASH):	return TCODColor::lightOrange;
	case(MaterialType::RUNE_SHARD):		return TCODColor::lightFuchsia;
	default:
		return COLOR_WHITE;
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
	case(PROP_ATTACK_SPEED):			return "Attack Spd";
	case(PROP_BASE_DAMAGE):				return "Base Damage";
	case(PROP_CHARGES_ON_HIT):			return "Charges per Hit";
	case(PROP_CHARGE_REGAIN_RATE):		return "Charges per Kill";
	case(PROP_CLEAVE_DAMAGE):			return "Cleave Bonus Damage";
	case(PROP_CRITICAL_CHANCE):			return "Crit Chance";
	case(PROP_CRITICAL_DAMAGE):			return "Crit Damage";
	case(PROP_DAMAGE_VARIANCE):			return "Damage Variance";
	case(PROP_DEFENCE):					return "Defence";
	case(PROP_HASTE_BUFF):				return "Haste Duration";
	case(PROP_HEAL_ON_USE):				return "Heals";
	case(PROP_KNOCKBACK_CHANCE):		return "Knockback";
	case(PROP_MAX_CHARGES):				return "Max Charges";
	case(PROP_MOVE_SPEED_ADJUST):		return "Move Speed";
	case(PROP_RIPOSTE_CHANCE):			return "Riposte Chance";
	case(PROP_RIPOSTE_DAMAGE):			return "Riposte Damage";
	case(PROP_SPELLSHIELD_FLAT):		return "Spellshield";
	case(PROP_SPELLSHIELD_PERCENT):		return "Spellshield";
	case(PROP_STAGGER_CHANCE):			return "Stagger Chance";
	case(PROP_STAGGER_DURATION):		return "Stagger Duration";
	case(PROP_STONESKIN_BUFF):			return "Stoneskin Duration";
	case(PROP_WRATH_BUFF):				return "Wrath Duration";
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
	case(PROP_SPELLSHIELD_FLAT):
		return plusminus(val);

	case(PROP_ATTACK_SPEED):
	case(PROP_MOVE_SPEED_ADJUST):
		return expressIntAsFloat(val, 1);

	case(PROP_CHARGES_ON_HIT):
	case(PROP_CHARGE_REGAIN_RATE):
	case(PROP_CRITICAL_CHANCE):
	case(PROP_CRITICAL_DAMAGE):
	case(PROP_SPELLSHIELD_PERCENT):
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

const string getItemQualityName(const ItemQuality qual)
{
	switch (qual)
	{
	case(QUALITY_DAMAGED):		return "damaged";
	case(QUALITY_MASTERWORK):	return "mwk";
	case(QUALITY_NORMAL):		return "normal";
	case(QUALITY_REFINED):		return "fine";
	case(QUALITY_SUPERIOR):		return "superb";
	default:
		return "Error_quality_" + to_string(qual);
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
	case(ENCH_BLACKBLOOD):		return "Blackblood";
	case(ENCH_CONDUCTING):		return "Fierce Conductor";
	case(ENCH_CUNNING):			return "Rat's Cunning";
	case(ENCH_DARK_ARCANA):		return "Dark Arcana";
	case(ENCH_DIVINE):			return "Divinity";
	case(ENCH_FIREBURST):		return "Fireburst";
	case(ENCH_FLAMESPIKE):		return "Flamespike";
	case(ENCH_KINSLAYER):		return "Kinslayer";
	case(ENCH_SHADOWSTRIKE):	return "Shade";
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
	case(ENCH_SHARPNESS):		return "Critical Chance";
	case(ENCH_SLAYING):			return "Critical Damage";
	case(ENCH_SPELLPOWER):		return "Spell Power";
	case(ENCH_SPELLWARD):		return "Arcane Resistance";
	case(ENCH_STONESKIN):		return "Stoneskin Duration";
	case(ENCH_STORMWARD):		return "Electric Resistance";
	case(ENCH_THORNS):			return "Reprisal Damage";
	case(ENCH_VENOM):			return "Poison Damage";
	case(ENCH_WOUNDING):		return "Weapon Damage";
	case(ENCH_WRATH):			return "Wrath Duration";

		//	Legendary enchants
	case(ENCH_AFFINITY):		return "Bonus Resist";
	case(ENCH_ARCANE_SHIELD):	return "Arcane Shield";
	case(ENCH_BLACKBLOOD):		return "Blackblood";
	case(ENCH_CONDUCTING):		return "Conduction";
	case(ENCH_CUNNING):			return "Cunning";
	case(ENCH_DARK_ARCANA):		return "Dark Arcana";
	case(ENCH_DIVINE):			return "Divine Damage";
	case(ENCH_FIREBURST):		return "Fireburst";
	case(ENCH_FLAMESPIKE):		return "Flamespike";
	case(ENCH_KINSLAYER):		return "Kinslay";
	case(ENCH_SHADOWSTRIKE):	return "Shadowstrike";
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

	case(ENCH_AFFINITY):		return "Your Resistances are increased by up to " + to_string(val) + "% based on the value of the associated bonus.";
	case(ENCH_ARCANE_SHIELD):	return "Each point of Magic in your pool absorbs " + to_string(val) + " points of damage when you are attacked.";
	case(ENCH_BLACKBLOOD):		return "When you are poisoned, your critical hit chance is increased by " + to_string(val) + "%.";
	case(ENCH_CONDUCTING):		return "Taking electric damage temporarily boosts your Electric Bonus by " + to_string(val) + "%.";
	case(ENCH_CUNNING):			return "Inflict " + to_string(val) + "% more damage when your health is below 30%.";
	case(ENCH_DARK_ARCANA):		return "Weapon attacks inflict bonus Arcane damage equal to 1/10 of your Spell Power, up to " + plusminus(val) + ".";
	case(ENCH_DIVINE):			return "Inflict +" + to_string(val) + " damage to undead creatures, multiplied by your Light Radius.";
	case(ENCH_FIREBURST):		return "Increases maximum fire damage by " + plusminus(val) + ".";
	case(ENCH_FLAMESPIKE):		return "You inflict " + plusminus(val) + " Reprisal damage as Fire.";
	case(ENCH_KINSLAYER):		return "Each time you fail to crit, you gain a cumulative " + plusminus(val) + "% bonus to crit chance. This bonus is cleared on crit.";
	case(ENCH_SHADOWSTRIKE):	return "You critical chance increases by " + to_string(val) + "% if your vision radius is 6 or less.";
	case(ENCH_SPELLBURN):		return "Raises spell power by " + to_string(val) + "%. You take damage when casting spells equal to twice their Magic cost.";
	case(ENCH_STORMBURST):		return "Increases maximum electric damage by " + plusminus(val) + ".";
	case(ENCH_THUNDERSPIKE):	return "You inflict " + plusminus(val) + " Reprisal damage as Electric.";
	case(ENCH_VENOMBURST):		return "Increases maximum poison damage by " + plusminus(val) + ".";
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
	case(ENCH_CURING):
		return name;

		//	Plus percent
	case(ENCH_AFF_ARCANE):
	case(ENCH_AFF_ELECTRIC):
	case(ENCH_AFF_FIRE):
	case(ENCH_AFF_POISON):
	case(ENCH_BLACKBLOOD):
	case(ENCH_CHARGING):
	case(ENCH_CUNNING):
	case(ENCH_FLAMEWARD):
	case(ENCH_FURY):
	case(ENCH_GREED):
	case(ENCH_KINSLAYER):
	case(ENCH_POISON_WARD):
	case(ENCH_RAGE):
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
	case(ENCH_STONESKIN):
	case(ENCH_WRATH):
		return name + " #" + to_string(val) + " turns";
		
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
	case(BUFF_SMITE_EVIL):		return "Smite Evil";
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
