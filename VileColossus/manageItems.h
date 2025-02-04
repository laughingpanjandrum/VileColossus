#pragma once

#include "effects.h"
#include "gamedata.h"
#include "mapgen.h"



//	Rituals

void openRitualAltar(gamedataPtr gdata);
void selectRitualMaterial(gamedataPtr gdata);

void openAbyssGate(gamedataPtr gdata);



//	Flasks

void applyFlaskEffect(gamedataPtr gdata, itemPtr it);
void playerQuaffCurrentFlask(gamedataPtr gdata);

void tryEquipSelectedFlask(gamedataPtr gdata);


//	Stash management

void stackItemInList(gamedataPtr gdata, vector<itemPtr>* ilist, itemPtr it);

void openStash(gamedataPtr gdata);

void stashGemstone(gamedataPtr gdata, itemPtr it);
void addToStash(gamedataPtr gdata, itemPtr it);
void removeFromStash(gamedataPtr gdata, itemPtr it);

void autodepositMaterials(gamedataPtr gdata);
void transferInventoryToStash(gamedataPtr gdata);
void moveSelectedToStash(gamedataPtr gdata);
void takeSelectedFromStash(gamedataPtr gdata);

void toggleMarkedValuable(gamedataPtr gdata);


//	The ANVIL

int getCostToRepairEquipment(gamedataPtr gdata);

bool hasMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt);
bool spendMaterialFromList(gamedataPtr gdata, vector<itemPtr>* mlist, const MaterialType mtype, const int amt);
void spendMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt);

void dismantleAll(gamedataPtr gdata);
void dismantleItem(gamedataPtr gdata, itemPtr it);
void dismantleFromInventory(gamedataPtr gdata);

void learnEnchantment(gamedataPtr gdata, const ItemEnchantment en);
bool doesItemHaveUnknownEnchants(gamedataPtr gdata, itemPtr it);

void repairItem(gamedataPtr gdata, itemPtr it);
void tryRepairSelected(gamedataPtr gdata);
void repairAllEquipped(gamedataPtr gdata);

void openAnvilMenu(gamedataPtr gdata);


//	The FORGE

void openForgeMenu(gamedataPtr gdata);

bool canLearnEnchantment(gamedataPtr gdata, const ItemEnchantment en);
bool knowsEnchantmentType(gamedataPtr gdata, const ItemEnchantment en);
void enhanceItem(gamedataPtr gdata, itemPtr it);

void reinforceSelectedItem(gamedataPtr gdata);

void selectEnchantmentToApply(gamedataPtr gdata);
void enhanceSelectedItem(gamedataPtr gdata);
void repairEquipmentItem(gamedataPtr gdata);



//	The DEMONFORGE

//	(no longer used)	----
const pair<MaterialType, int> getCostToForgeItem(const int rarity);

void transmuteMaterial(gamedataPtr gdata, const int num);
void tryUpgradeDemonforge(gamedataPtr gdata);
void createWithDemonforge(gamedataPtr gdata, const int rarity);

void selectDemonforgeOption(gamedataPtr gdata, const int num);
//						----


bool hasGemsOfType(gamedataPtr gdata, const GemType gtype, const int tier, const int qty);
void expendGems(gamedataPtr gdata, const GemType gtype, const int tier, const int qty);


const pair<GemType, int> getGemForExalt(const itemPtr it);
const int getGemQuantityForExalt(const itemPtr it);
const pair<MaterialType, int> getMaterialForExalt(const itemPtr it);

void tryExaltItem(gamedataPtr gdata);

void openDemonforge(gamedataPtr gdata);


//	ALCHEMY

int getFlaskEnhanceCost(gamedataPtr gdata, itemPtr it);
void openAlchemyMenu(gamedataPtr gdata);
void tryEnhanceFlask(gamedataPtr gdata);


//	GEMSTONES

const vector<itemPtr> getGemstonesWeCanFabricate(gamedataPtr gdata);
const int getGemstoneFabricateCost(itemPtr it);

bool canAddGemSlot(gamedataPtr gdata, const itemPtr it);
void tryAddGemSlot(gamedataPtr gdata);

void removeAllGemsFromItem(gamedataPtr gdata);
void selectGemToSocketIntoItem(gamedataPtr gdata);
void selectItemToSocket(gamedataPtr gdata);
void tryFabricateGemstone(gamedataPtr gdata);

void openGemFabricator(gamedataPtr gdata);
void openGemstonePress(gamedataPtr gdata);


//	Inventory management

void openEquipment(gamedataPtr gdata);
void openInventory(gamedataPtr gdata);
void swapInventoryAndEquipment(gamedataPtr gdata);

void swapToSecondaryEquipment(gamedataPtr gdata);

void tryEquipCurrentItemInSlot(gamedataPtr gdata);
void selectCurrentEquipmentSlot(gamedataPtr gdata);

void unequipFromSlot(gamedataPtr gdata, const EquipmentSlot slot);
void unequipFromSelectedSlot(gamedataPtr gdata);

void addToInventory(gamedataPtr gdata, itemPtr it);
void removeFromInventory(gamedataPtr gdata, itemPtr it);

void selectItemFromInventory(gamedataPtr gdata);
void viewItemDetails(gamedataPtr gdata);
void viewEquipmentDetails(gamedataPtr gdata);

void removeFromCurrentItemList(gamedataPtr gdata, itemPtr it);


//	Picking stuff up/dropping stuff

void dropSelectedItem(gamedataPtr gdata);

void tryAutopickup(gamedataPtr gdata, const intpair pt);
void pickupSelectedItem(gamedataPtr gdata);
void pickupFromFloor(gamedataPtr gdata);

int getInventorySlotsUsed(gamedataPtr gdata);
int getInventorySlotsAvailable(gamedataPtr gdata);