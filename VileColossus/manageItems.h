#pragma once

#include "effects.h"
#include "gamedata.h"



//	Flasks

void applyFlaskEffect(gamedataPtr gdata, itemPtr it);
void playerQuaffCurrentFlask(gamedataPtr gdata);

void tryEquipSelectedFlask(gamedataPtr gdata);


//	Stash management

void openStash(gamedataPtr gdata);

void addToStash(gamedataPtr gdata, itemPtr it);
void removeFromStash(gamedataPtr gdata, itemPtr it);

void autodepositMaterials(gamedataPtr gdata);
void transferInventoryToStash(gamedataPtr gdata);
void moveSelectedToStash(gamedataPtr gdata);
void takeSelectedFromStash(gamedataPtr gdata);


//	The ANVIL

int getCostToRepairEquipment(gamedataPtr gdata);

bool hasMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt);
void spendMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt);

void dismantleFromInventory(gamedataPtr gdata);
void extractEnchantments(gamedataPtr gdata);
bool doesItemHaveUnknownEnchants(gamedataPtr gdata, itemPtr it);

void repairItem(gamedataPtr gdata, itemPtr it);
void tryRepairSelected(gamedataPtr gdata);
void repairAllEquipped(gamedataPtr gdata);

void openAnvilMenu(gamedataPtr gdata);


//	The FORGE

void openForgeMenu(gamedataPtr gdata);

bool knowsEnchantmentType(gamedataPtr gdata, const ItemEnchantment en);
void enhanceItem(gamedataPtr gdata, itemPtr it);

void reinforceSelectedItem(gamedataPtr gdata);

void selectEnchantmentToApply(gamedataPtr gdata);
void enhanceSelectedItem(gamedataPtr gdata);
void repairEquipmentItem(gamedataPtr gdata);


//	ALCHEMY

void openAlchemyMenu(gamedataPtr gdata);
void tryEnhanceFlask(gamedataPtr gdata);


//	GEMSTONES

const vector<itemPtr> getGemstonesWeCanFabricate(gamedataPtr gdata);
const int getGemstoneFabricateCost(itemPtr it);

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