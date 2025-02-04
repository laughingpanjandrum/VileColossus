#pragma once

#include <string>
#include <vector>
#include "color_constants.h"

using namespace std;



//	Player attributes

enum Attribute { ATTR_STRENGTH, ATTR_DEXTERITY, ATTR_WILLPOWER, ATTR__MAX, };
const string getAttributeName(const Attribute attr);


enum Perk {
	PERK_ACCURACY,
	PERK_ARMOUR,
	PERK_BASE_DAMAGE,
	PERK_CRIT_DAMAGE, 
	PERK_DEFENCE,
	PERK_FRAGMENTS_GAINED,
	PERK_HEALTH,
	PERK_HEALTH_ON_KILL,
	PERK_MAGIC,
	PERK_REPRISAL,
	PERK_SPELL_POWER,
	PERK_WRATH_DAMAGE,
	PERK__MAX,
};
const string getPerkName(const Perk pk);
const int getPerkBonusPerRank(const Perk pk);
const int getPerkMaxLevel(const Perk pk);


//	Item categories

enum ItemCategory
{
	ITEM_WEAPON, ITEM_QUIVER, ITEM_SHIELD,
	ITEM_HELMET, ITEM_SHOULDERS, ITEM_CHESTPIECE, ITEM_BRACERS, ITEM_GLOVES, ITEM_BOOTS,
	ITEM_AMULET, ITEM_RING,
	ITEM_FLASK, ITEM_SPELLRUNE,
	ITEM_MATERIAL, ITEM_GEM, ITEM_CONSUMABLE,
	ITEM__MAX,
};

string getItemCategoryName(const ItemCategory cat);


//	Armour subcategories

enum class ArmourCategory { ARMOUR_LIGHT, ARMOUR_HEAVY, ARMOUR__NONE, };

string getArmourCategoryName(const ArmourCategory cat);



//	Item material subtypes

enum class MaterialType { 
	//	Basic materials
	BRIGHT_RUNE, FRAGMENTS, GLASS_SHARD, GLOWING_POWDER, MAGIC_DUST, RADIANT_ASH, RUNE_SHARD, 

	//	Consumable types
	NOTCHED_CUBE, 

	//	Abyssal materials
	SODDEN_FLESH, TOMB_IDOL, VIRIDIAN_GLASS,
	VILEDRAGON_SCALE,
	DEAD_GODS_EYE,

	__NONE, };

const string getMaterialTypeName(const MaterialType mat);
const colorType getMaterialTypeColor(const MaterialType mat);

bool isRitualMaterial(const MaterialType mat);
const string getRitualDescription(const MaterialType mat);


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
	PROP_ATTACK_RANGE,

	PROP_CLEAVE_DAMAGE, 
	PROP_RIPOSTE_CHANCE, PROP_RIPOSTE_DAMAGE,
	PROP_STAGGER_CHANCE, PROP_STAGGER_DURATION,
	PROP_KNOCKBACK_CHANCE,
	PROP_PENETRATION_DAMAGE,

	PROP_DEFENCE, PROP_ARMOUR_VALUE,
	PROP_RESIST_ARCANE, PROP_RESIST_ELECTRIC, PROP_RESIST_FIRE, PROP_RESIST_POISON,

	PROP_MAX_CHARGES, PROP_CHARGE_REGAIN_RATE, PROP_CHARGES_ON_HIT, PROP_HEAL_ON_USE,

	PROP__NONE,
};

const string getItemPropertyName(const ItemProperty prop);
const string formatItemProperty(const ItemProperty prop, int val);



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
	ENCH_RAGE, ENCH_REGEN, ENCH_RESISTANCE,
	ENCH_SHARPNESS, ENCH_SLAYING, ENCH_SPELLPOWER, ENCH_SPELLWARD, ENCH_STONESKIN, ENCH_STORMWARD,
	ENCH_THORNS,
	ENCH_VENOM,
	ENCH_WOUNDING, ENCH_WRATH,


	//	Legendary enchants
	ENCH_AFFINITY, ENCH_ARCANE_SHIELD, ENCH_AVARICE,
	ENCH_BERSERKER, ENCH_BLACKBLOOD,
	ENCH_CONDUCTING, ENCH_CRUCIBLE, ENCH_CUNNING,
	ENCH_DANCER, ENCH_DARK_ARCANA, ENCH_DERVISH, ENCH_DIVINE,
	ENCH_FIREBURST, ENCH_FLAMESPIKE,
	ENCH_KINSLAYER,
	ENCH_LEAGUE,
	ENCH_MAGEBLOOD,
	ENCH_PETRIFYING,
	ENCH_SADIST, ENCH_SHADOWSTRIKE, ENCH_SKYSPLITTER, ENCH_SPELLBURN, ENCH_STORMBURST,
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

enum StatusEffect { STATUS_AGONY, STATUS_BURN, STATUS_ENTANGLED, STATUS_POISON, STATUS_PUTREFIED, STATUS_SHOCK, STATUS_SLUDGED, STATUS_STAGGER, STATUS__MAX, };

const vector<StatusEffect> ALL_STATUS_EFFECTS = { STATUS_AGONY, STATUS_BURN, STATUS_ENTANGLED, STATUS_POISON, STATUS_PUTREFIED, STATUS_SHOCK, STATUS_SLUDGED, STATUS_STAGGER };

string getStatusEffectName(const StatusEffect st);
colorType getStatusEffectColor(const StatusEffect st);


//	Buffs

enum BuffType { 
	BUFF_ARCANE_PULSE, BUFF_CONDUCT_ELECTRIC, BUFF_CONDUCT_FIRE, BUFF_CONDUCT_POISON, BUFF_CRIT_BONUS, BUFF_DANCER, BUFF_EMPOWERED, BUFF_HASTE, 
	BUFF_REGENERATION, BUFF_SMITE_EVIL, BUFF_STATIC_FIELD, BUFF_STONESKIN, BUFF_TOXIC_RADIANCE, BUFF_VENOMFANG, BUFF_WRATH, 
	BUFF__MAX, };

string getBuffName(const BuffType bf);


//	Socketables

enum class GemType { 
	BLACKSTONE, BOLTSTONE, FLAMESTONE, SILVERSTONE, SPIDERSTONE, 
	ABYSSAL_SPIKEGEM, BLOODY_FLESHGEM, DOGOSSAN_BONEGEM, LUMINOUS_GEM, VIRIDIAN_PALEGEM,
	__NONE, };

string getGemTypeName(const GemType gem);
string getGemTypeFullName(const GemType gem, const int tier);
colorType getGemTypeColor(const GemType gem);

bool doesGemHaveSingularEffect(const GemType gem);