#pragma once

#include <string>
#include <vector>
#include "color_constants.h"

using namespace std;



//	Player attributes

enum Attribute
{
	ATTR_STRENGTH, ATTR_DEXTERITY, ATTR_WILLPOWER, ATTR__MAX,
};

const string getAttributeName(const Attribute attr);


//	Item categories

enum ItemCategory
{
	ITEM_WEAPON, ITEM_QUIVER, ITEM_SHIELD,
	ITEM_HELMET, ITEM_SHOULDERS, ITEM_CHESTPIECE, ITEM_BRACERS, ITEM_GLOVES, ITEM_BOOTS,
	ITEM_AMULET, ITEM_RING,
	ITEM_FLASK,
	ITEM_SPELLRUNE,
	ITEM_MATERIAL, ITEM_GEM,
	ITEM__MAX,
};

string getItemCategoryName(const ItemCategory cat);


//	Armour subcategories

enum ArmourCategory
{
	ARMOUR_LIGHT, ARMOUR_MEDIUM, ARMOUR_HEAVY,
	ARMOUR__NONE,
};

string getArmourCategoryName(const ArmourCategory cat);


//	Item material subtypes

enum class MaterialType { BRIGHT_RUNE, FRAGMENTS, GLASS_SHARD, GLOWING_POWDER, MAGIC_DUST, RADIANT_ASH, RUNE_SHARD, __NONE, };

const string getMaterialTypeName(const MaterialType mat);
const colorType getMaterialTypeColor(const MaterialType mat);


//	Equipment slots (distinct from item categories because we can have multiple items of the same type equipped at once)

enum EquipmentSlot
{
	SLOT_MAIN_HAND, SLOT_OFFHAND, SLOT_BODY,
	SLOT_HELMET, SLOT_SHOULDERS, SLOT_BRACERS, SLOT_GLOVES, SLOT_BOOTS,
	SLOT_AMULET, SLOT_LEFT_RING, SLOT_RIGHT_RING,
	SLOT__NONE,
};

string getEquipmentSlotName(const EquipmentSlot slot);

EquipmentSlot getSlotForCategory(const ItemCategory cat);
EquipmentSlot getAltSlotForCategory(const ItemCategory cat);


//	Item properties

enum ItemProperty
{
	PROP_BASE_DAMAGE, PROP_DAMAGE_VARIANCE, PROP_ACCURACY_MOD,
	PROP_CRITICAL_CHANCE, PROP_CRITICAL_DAMAGE,
	PROP_ATTACK_SPEED,
	PROP_ATTACK_RANGE,

	PROP_CLEAVE_DAMAGE, 
	PROP_RIPOSTE_CHANCE, PROP_RIPOSTE_DAMAGE,
	PROP_STAGGER_CHANCE, PROP_STAGGER_DURATION,
	PROP_KNOCKBACK_CHANCE,

	PROP_DEFENCE, PROP_ARMOUR_VALUE,
	PROP_SPELLSHIELD_PERCENT, PROP_SPELLSHIELD_FLAT,
	PROP_MOVE_SPEED_ADJUST,

	PROP_MAX_CHARGES, PROP_CHARGE_REGAIN_RATE, PROP_CHARGES_ON_HIT,
	PROP_HEAL_ON_USE, PROP_HASTE_BUFF, PROP_STONESKIN_BUFF, PROP_WRATH_BUFF,

	PROP__NONE,
};

const string getItemPropertyName(const ItemProperty prop);
const string formatItemProperty(const ItemProperty prop, int val);


//	Item quality

enum ItemQuality
{
	QUALITY_DAMAGED, QUALITY_NORMAL, QUALITY_REFINED, QUALITY_SUPERIOR, QUALITY_MASTERWORK,
};

const string getItemQualityName(const ItemQuality qual);



//	Item enchantments

enum ItemEnchantment
{
	//	Standard enchantments
	ENCH_ACCURACY, ENCH_ARCANE, ENCH_ARMOURING,
	ENCH_AFF_ARCANE, ENCH_AFF_ELECTRIC, ENCH_AFF_FIRE, ENCH_AFF_POISON,
	ENCH_BURNING,
	ENCH_CAPACITY, ENCH_CHARGING, ENCH_CURING,
	ENCH_DEFENCE,
	ENCH_EMPOWERING,
	ENCH_FLAMEWARD, ENCH_FURY,
	ENCH_GREED,
	ENCH_HASTE,
	ENCH_LEECHING, ENCH_LIGHT, ENCH_LIFE, ENCH_LIGHTNING,
	ENCH_MAGIC, ENCH_MAGIC_RESTORE, ENCH_MANALEECH,
	ENCH_POISON_WARD,
	ENCH_RAGE, ENCH_REGEN,
	ENCH_SHARPNESS, ENCH_SLAYING, ENCH_SPELLPOWER, ENCH_SPELLWARD, ENCH_STONESKIN, ENCH_STORMWARD,
	ENCH_THORNS,
	ENCH_VENOM,
	ENCH_WOUNDING, ENCH_WRATH,


	//	Legendary enchants
	ENCH_AFFINITY, ENCH_ARCANE_SHIELD, ENCH_AVARICE,
	ENCH_BLACKBLOOD,
	ENCH_CONDUCTING, ENCH_CUNNING,
	ENCH_DARK_ARCANA, ENCH_DIVINE,
	ENCH_FIREBURST, ENCH_FLAMESPIKE,
	ENCH_KINSLAYER,
	ENCH_MAGEBLOOD,
	ENCH_PETRIFYING,
	ENCH_SHADOWSTRIKE, ENCH_SKYSPLITTER, ENCH_SPELLBURN, ENCH_STORMBURST,
	ENCH_THUNDERSPIKE,
	ENCH_VENOMBURST, ENCH_VENOMSPIKE,
	ENCH_WEIGHT,

	ENCH__NONE,
};

const string getItemEnchantmentName(const ItemEnchantment en);
const string getItemEnchantmentDescription(const ItemEnchantment en);
const string getItemEnchantmentVerbose(const ItemEnchantment en, const int val);
const string formatItemEnchantment(const ItemEnchantment en, const int val);



//	Damage types

enum DamageType { DTYPE_NORMAL, DTYPE_ARCANE, DTYPE_ELECTRIC, DTYPE_FIRE, DTYPE_POISON, };

const vector<DamageType> SPECIAL_DAMAGE_TYPES = { DTYPE_ARCANE, DTYPE_ELECTRIC, DTYPE_FIRE, DTYPE_POISON };

string getDamageTypeName(const DamageType dt);
colorType getDamageTypeColor(const DamageType dt);


//	Status effects

enum StatusEffect { STATUS_AGONY, STATUS_BURN, STATUS_ENTANGLED, STATUS_POISON, STATUS_SHOCK, STATUS_SLUDGED, STATUS_STAGGER, STATUS__MAX, };

const vector<StatusEffect> ALL_STATUS_EFFECTS = { STATUS_AGONY, STATUS_BURN, STATUS_ENTANGLED, STATUS_POISON, STATUS_SHOCK, STATUS_SLUDGED, STATUS_STAGGER };

string getStatusEffectName(const StatusEffect st);
colorType getStatusEffectColor(const StatusEffect st);


//	Buffs

enum BuffType { BUFF_ARCANE_PULSE, BUFF_CONDUCTION, BUFF_CRIT_BONUS, BUFF_EMPOWERED, BUFF_HASTE, BUFF_REGENERATION, BUFF_SMITE_EVIL, BUFF_STONESKIN, BUFF_TOXIC_RADIANCE, BUFF_VENOMFANG, BUFF_WRATH, BUFF__MAX, };

string getBuffName(const BuffType bf);


//	Socketables

enum class GemType { BLACKSTONE, BOLTSTONE, FLAMESTONE, SILVERSTONE, SPIDERSTONE, __NONE, };

string getGemTypeName(const GemType gem);
string getGemTypeFullName(const GemType gem, const int tier);
colorType getGemTypeColor(const GemType gem);