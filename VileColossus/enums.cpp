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
	case(ITEM_SHOULDERS):		return "Shoulders";
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
	case(MaterialType::FRAGMENTS):		return "fragments";
	case(MaterialType::GLASS_SHARD):	return "glass shards";
	case(MaterialType::GLOWING_POWDER):	return "glowing powder";
	case(MaterialType::MAGIC_DUST):		return "magic dust";
	case(MaterialType::RUNE_SHARD):		return "rune shards";
	default:
		return "material";
	}
}

const colorType getMaterialTypeColor(const MaterialType mat)
{
	switch (mat)
	{
	case(MaterialType::FRAGMENTS):		return TCODColor::gold;
	case(MaterialType::GLASS_SHARD):	return TCODColor::silver;
	case(MaterialType::GLOWING_POWDER):	return TCODColor::lightYellow;
	case(MaterialType::MAGIC_DUST):		return TCODColor::lightBlue;
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
	case(SLOT_SHOULDERS):		return "Shoulder";
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
	case(ENCH_FLAMEWARD):		return "flameguard";
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
	default:
		return "enchantment_" + to_string(en);
	}
}

const string getItemEnchantmentDescription(const ItemEnchantment en)
{
	switch (en)
	{
	case(ENCH_ACCURACY):		return "Global Accuracy";
	case(ENCH_AFF_ARCANE):		return "Arcane Affinity";
	case(ENCH_AFF_ELECTRIC):	return "Electric Affinity";
	case(ENCH_AFF_FIRE):		return "Fire Affinity";
	case(ENCH_AFF_POISON):		return "Poison Affinity";
	case(ENCH_ARCANE):			return "Arcane Damage";
	case(ENCH_ARMOURING):		return "Armour Value";
	case(ENCH_BURNING):			return "Fire Damage";
	case(ENCH_CAPACITY):		return "Max Charges";
	case(ENCH_CHARGING):		return "Charges/Kill";
	case(ENCH_CURING):			return "Cure Poison";
	case(ENCH_DEFENCE):			return "Defence Value";
	case(ENCH_FLAMEWARD):		return "Fire Resistance";
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
	default:
		return "enchantment_" + to_string(en);
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
	case(BUFF_HASTE):		return "Haste";
	case(BUFF_SMITE_EVIL):	return "Smite Evil";
	case(BUFF_STONESKIN):	return "Stoneskin";
	case(BUFF_VENOMFANG):	return "Venomfang";
	case(BUFF_WRATH):		return "Wrath";
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
	case(0):	return "cracked " + name;
	case(1):	return "chipped " + name;
	case(2):	return name + " shard";
	case(3):	return name + " chunk";
	case(4):	return "whole " + name;
	case(5):	return "pure " + name;
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
