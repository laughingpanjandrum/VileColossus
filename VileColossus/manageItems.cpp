#include "manageItems.h"



//	Apply 'on-use' effects of a given item
void applyFlaskEffect(gamedataPtr gdata, itemPtr it)
{
	if (it->hasProperty(PROP_HEAL_ON_USE))
		gdata->_player->healDamage(it->getProperty(PROP_HEAL_ON_USE));
	if (it->hasEnchantment(ENCH_CURING))
	{
		gdata->_player->cureStatusEffect(STATUS_BURN);
		gdata->_player->cureStatusEffect(STATUS_POISON);
		gdata->_player->cureStatusEffect(STATUS_SHOCK);
	}
	if (it->hasEnchantment(ENCH_MAGIC_RESTORE))
		gdata->_player->restoreMagic(it->getEnchantmentValue(ENCH_MAGIC_RESTORE));
	if (it->hasProperty(PROP_HASTE_BUFF))
		gdata->_player->addBuff(BUFF_HASTE, it->getProperty(PROP_HASTE_BUFF));
	if (it->hasProperty(PROP_STONESKIN_BUFF))
		gdata->_player->addBuff(BUFF_STONESKIN, it->getProperty(PROP_STONESKIN_BUFF));
	if (it->hasProperty(PROP_WRATH_BUFF))
		gdata->_player->addBuff(BUFF_WRATH, it->getProperty(PROP_WRATH_BUFF));
}


//	Apply effects of equipped flask, if we have a charge available; use a charge
void playerQuaffCurrentFlask(gamedataPtr gdata)
{
	auto fl = gdata->_player->_currentFlask;
	if (fl != nullptr)
	{
		if (fl->hasChargesLeft())
		{
			applyFlaskEffect(gdata, fl);
			fl->expendCharge();
			
			addAnimation(gdata, anim_FlashGlyph(gdata->_player->_pos, fl->getGlyph(), TCODColor::red));
			messages::add(gdata, "You quaff a #" + fl->getName() + "@.", { fl->getColor() });

			gdata->_player->spendActionEnergy();
		}
		else
			messages::add(gdata, "#Your flask has no charges! Kill enemies to gain charges.", { COLOR_NEGATIVE });
	}
	else
		messages::add(gdata, "#You have no flask equipped!", { COLOR_NEGATIVE });
}


//	If we have a flask selected in inventory, attempt to equip it.
void tryEquipSelectedFlask(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		if (it->_category == ITEM_FLASK)
		{
			//	Move equipped flask back to inventory
			if (gdata->_player->_currentFlask != nullptr)
				addToInventory(gdata, gdata->_player->_currentFlask);

			//	Equip new flask. This clears its charges.
			gdata->_player->_currentFlask = it;
			it->clearAllCharges();
			removeFromInventory(gdata, it);

			//	Message
			messages::add(gdata, "You equip #" + it->getName() + "@.", { it->getColor() });

			//	Close inventory
			gdata->_state = STATE_NORMAL;
		}
	}
}


void openStash(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_state = STATE_VIEW_STASH;
	gdata->_currentItemList = gdata->_stashItems;
	gdata->_idx = 0;
	gdata->_player->unmarkAllSlots();
}


//	Put an item into our stash.
void addToStash(gamedataPtr gdata, itemPtr it)
{
	//	Materials go into a separate list.
	if (it->_category == ITEM_MATERIAL)
	{
		//	Is it in the materials list already?
		for (auto other : gdata->_stashedMaterials)
		{
			if (other->stacksWith(it))
			{
				other->_amountLeft += it->_amountLeft;
				return;
			}
		}

		//	If not, add it.
		gdata->_stashedMaterials.push_back(it);
		return;
	}

	//	As do gems.
	else if (it->_category == ITEM_GEM)
	{
		//	Is it in the gems list already?
		for (auto other : gdata->_stashedGems)
		{
			if (other->stacksWith(it))
			{
				other->_amountLeft += it->_amountLeft;
				return;
			}
		}

		//	If not, add it.
		gdata->_stashedGems.push_back(it);
		return;
	}

	//	Attempt to insert alphabetically
	for (auto f = gdata->_stashItems.begin(); f != gdata->_stashItems.end(); f++)
	{
		if (isFirstAlphabetically(it->getName(), (*f)->getName()))
		{
			gdata->_stashItems.insert(f, it);
			return;
		}
	}

	//	Push at end
	gdata->_stashItems.push_back(it);
}


void removeFromStash(gamedataPtr gdata, itemPtr it)
{
	auto f = find(gdata->_stashItems.begin(), gdata->_stashItems.end(), it);
	if (f != gdata->_stashItems.end())
		gdata->_stashItems.erase(f);
}



//	Transfer all materials and gems from inventory to stash.
void autodepositMaterials(gamedataPtr gdata)
{
	//	Put in stash
	vector<itemPtr> toRemove;
	for (auto it : gdata->_carriedItems)
	{
		if (it->_category == ITEM_MATERIAL || it->_category == ITEM_GEM)
		{
			addToStash(gdata, it);
			toRemove.push_back(it);
		}
	}

	//	Remove from inventory
	for (auto it : toRemove)
		removeFromInventory(gdata, it);
}


//	Transfers entire inventory to our stash.
void transferInventoryToStash(gamedataPtr gdata)
{
	for (auto it : gdata->_carriedItems)
		addToStash(gdata, it);
	gdata->_carriedItems.clear();
	gdata->_currentItemList.clear();
}


//	Transfer a single selected item from inventory to stash.
void moveSelectedToStash(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	Transfer the item
		auto it = gdata->_currentItemList[gdata->_idx];
		addToStash(gdata, it);
		removeFromInventory(gdata, it);
		removeFromCurrentItemList(gdata, it);
	}
}


//	Transfer a single selected item from stash to inventory.
void takeSelectedFromStash(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		addToInventory(gdata, it);
		removeFromStash(gdata, it);
		removeFromCurrentItemList(gdata, it);
	}
}


//	Open inventory menu
void openInventory(gamedataPtr gdata)
{
	gdata->_state = STATE_VIEW_INVENTORY;
	gdata->_selectedSlot = SLOT__NONE;

	//	View all carried items
	gdata->_idx = 0;
	gdata->_currentItemList.clear();
	for (auto it : gdata->_carriedItems)
		gdata->_currentItemList.push_back(it);
}


//	Switch between equipment & inventory menus.
void swapInventoryAndEquipment(gamedataPtr gdata)
{
	if (gdata->_state == STATE_VIEW_EQUIPMENT)
	{
		gdata->_state = STATE_VIEW_INVENTORY;
		if (gdata->_idx >= gdata->_carriedItems.size())
			gdata->_idx = gdata->_carriedItems.size() - 1;
	}
	else
	{
		gdata->_state = STATE_VIEW_EQUIPMENT;
		gdata->_selectedSlot = SLOT__NONE;
		if (gdata->_idx >= SLOT__NONE)
			gdata->_idx = SLOT__NONE - 1;
	}
}


//	Switches to the player's secondary equipment set.
void swapToSecondaryEquipment(gamedataPtr gdata)
{
	gdata->_player->swapToSecondaryItems();
	messages::add(gdata, "You swap to your secondary set of weapons.");
	gdata->_player->spendActionEnergy();
}


//	Attempt to equip the currently selected item in the currently selected slot.
void tryEquipCurrentItemInSlot(gamedataPtr gdata)
{
	if (gdata->_selectedSlot != SLOT__NONE && gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		if (it->canEquipInSlot(gdata->_selectedSlot))
		{
			//	restore previous item to inventory, if possible
			auto here = gdata->_player->getItemInSlot(gdata->_selectedSlot);
			if (here != nullptr)
				addToInventory(gdata, here);

			//	equip item
			auto old_idx = gdata->_selectedSlot;
			gdata->_player->equipInSlot(it, gdata->_selectedSlot);
			openEquipment(gdata);
			gdata->_idx = old_idx;

			//	erase from inventory
			removeFromInventory(gdata, it);

			//	if we equipped a 2h weapon, we have to unequip our offhand item
			if (old_idx == SLOT_MAIN_HAND && !gdata->_player->isHandednessValid())
				unequipFromSlot(gdata, SLOT_OFFHAND);

			//	if we equip an item in the offhand, unequip a 2-handed item in main hand
			else if (old_idx == SLOT_OFFHAND && !gdata->_player->isHandednessValid())
				unequipFromSlot(gdata, SLOT_MAIN_HAND);

			//	takes time
			gdata->_player->spendActionEnergy();
		}
	}
}


//	Select the current equipment slot; we now choose an item to put there
void selectCurrentEquipmentSlot(gamedataPtr gdata)
{
	gdata->_selectedSlot = static_cast<EquipmentSlot>(gdata->_idx);
	gdata->_player->unmarkNewForSlot(gdata->_selectedSlot);
	gdata->_state = STATE_VIEW_INVENTORY;

	//	Show only items available for the current equipment slot
	gdata->_idx = 0;
	gdata->_currentItemList.clear();
	for (auto it : gdata->_carriedItems)
	{
		if (it->canEquipInSlot(gdata->_selectedSlot))
			gdata->_currentItemList.push_back(it);
	}
}


//	Unequip item in the given slot & return it to inventory.
void unequipFromSlot(gamedataPtr gdata, const EquipmentSlot slot)
{
	auto it = gdata->_player->getItemInSlot(slot);
	if (it != nullptr)
	{
		gdata->_player->unequipFromSlot(slot);
		addToInventory(gdata, it);
		gdata->_player->spendActionEnergy();
	}
}

//	Unequip the selected item & return it to inventory.
//	Figures out which slot to unequip based on the current _idx in gamedata.
void unequipFromSelectedSlot(gamedataPtr gdata)
{
	auto slot = static_cast<EquipmentSlot>(gdata->_idx);
	unequipFromSlot(gdata, slot);
}


//	Sum up the total cost of repairing all equipped items.
int getCostToRepairEquipment(gamedataPtr gdata)
{
	int cost = 0;
	for (auto it : gdata->_player->getAllEquippedItems())
	{
		if (it != nullptr)
			cost += it->getRepairCost();
	}
	return cost;
}

//	Returns True if we have the given amount of material stashed.
bool hasMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt)
{
	for (auto m : gdata->_stashedMaterials)
	{
		if (m->_material == mtype)
			return m->_amountLeft >= amt;
	}
	return false;
}


//	Attempts to expend the given amount of stashed material.
void spendMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt)
{
	for (auto m : gdata->_stashedMaterials)
	{
		if (m->_material == mtype)
		{
			m->_amountLeft -= amt;
			return;
		}
	}
}


//	Take apart an item to get parts back.
void dismantleFromInventory(gamedataPtr gdata)
{
	//	Test that we have a valid item selected.
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];

		//	Amount/type of parts depend on the item.
		int fragments = dieRoll(3, 4 + 2 * it->_rarity);
		addToStash(gdata, lootgen::generateMaterial(MaterialType::FRAGMENTS, fragments));

		//	Higher-tier and type-based stuff
		if (it->_rarity > 1)
			addToStash(gdata, lootgen::generateMaterial(MaterialType::MAGIC_DUST, randint(4, 9)));
		if (it->_rarity > 2)
			addToStash(gdata, lootgen::generateMaterial(MaterialType::GLOWING_POWDER, randint(4, 9)));
		if (it->_category == ITEM_FLASK)
			addToStash(gdata, lootgen::generateMaterial(MaterialType::GLASS_SHARD, it->_rarity));
		else if (it->_category == ITEM_SPELLRUNE)
			addToStash(gdata, lootgen::generateMaterial(MaterialType::RUNE_SHARD, dieRoll(it->_rarity, 3)));

		//	Destroy the item.
		removeFromInventory(gdata, it);
		removeFromCurrentItemList(gdata, it);
	}
}


void repairItem(gamedataPtr gdata, itemPtr it)
{
	auto cost = it->getRepairCost();
	if (cost > 0 && hasMaterial(gdata, MaterialType::FRAGMENTS, cost))
	{
		spendMaterial(gdata, MaterialType::FRAGMENTS, cost);
		it->repairToMax();
	}
}

//	If we can afford it, repair the currently selected item.
void tryRepairSelected(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
		repairItem(gdata, gdata->_currentItemList[gdata->_idx]);
}


//	Try to repair every item we have equipped.
void repairAllEquipped(gamedataPtr gdata)
{
	for (auto it : gdata->_player->getAllEquippedItems())
	{
		if (it != nullptr)
			repairItem(gdata, it);
	}
}


void openAnvilMenu(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_player->unmarkAllSlots();
	gdata->_currentItemList = gdata->_carriedItems;
	gdata->_idx = 0;
	gdata->_state = STATE_ANVIL;
}

void openForgeMenu(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_state = STATE_FORGE;
	gdata->_idx = 0;
}


//	Destroys an item. We learn any enchantments it contained.
void extractEnchantments(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	Destroy the item
		auto it = gdata->_currentItemList[gdata->_idx];
		removeFromInventory(gdata, it);
		removeFromCurrentItemList(gdata, it);

		//	Learn its enchants, if any
		for (auto en : *it->getAllEnchantments())
		{
			if (!knowsEnchantmentType(gdata, en))
			{
				messages::add(gdata, "Learned enchantment type: #" + getItemEnchantmentName(en), { COLOR_WHITE });
				gdata->_knownEnchants.push_back(en);
			}
		}
	}
}


//	Returns True if the given item has 1 or more enchantments we don't already know.
bool doesItemHaveUnknownEnchants(gamedataPtr gdata, itemPtr it)
{
	for (auto en : *it->getAllEnchantments())
	{
		if (!knowsEnchantmentType(gdata, en))
			return true;
	}
	return false;
}


//	Increases the selected item's max durability.
void reinforceSelectedItem(gamedataPtr gdata)
{
	auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
	if (it != nullptr)
	{
		int cost = it->getReinforceCost();
		if (hasMaterial(gdata, MaterialType::FRAGMENTS, cost))
		{
			it->_maxDurability += 5;
			spendMaterial(gdata, MaterialType::FRAGMENTS, cost);
			messages::add(gdata, "Reinforced #" + it->getName() + "@!", { it->getColor() });
		}
		else
			messages::error(gdata, "Insufficient materials to reinforce!");
	}
}


//	Returns True if we have extracted the given enchantment type already.
bool knowsEnchantmentType(gamedataPtr gdata, const ItemEnchantment en)
{
	return find(gdata->_knownEnchants.begin(), gdata->_knownEnchants.end(), en) != gdata->_knownEnchants.end();
}


//	If possible, try to add another enchantment to the item, from available options.
void enhanceItem(gamedataPtr gdata, itemPtr it)
{
	//	Is it enchantable?
	if (it->_enhancementLevel < it->getMaxEnhancementLevel())
	{
		//	Can we afford it?
		auto mat_cost = it->getEnhanceCost();
		auto mat_type = it->getEnhanceMaterial();
		if (hasMaterial(gdata, mat_type, mat_cost))
		{
			gdata->_idx = 0;
			gdata->_viewingItem = it;
			gdata->_state = STATE_SELECT_ENCHANTMENT;
		}
		else
			messages::error(gdata, "Not enough materials!");
	}
	else
		messages::error(gdata, "This item is maximally enchanted already!");
}


//	Apply the selected enchantment to the selected item, if possible.
void selectEnchantmentToApply(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_knownEnchants.size())
	{
		auto en = gdata->_knownEnchants[gdata->_idx];
		if (!gdata->_viewingItem->hasEnchantment(en))
		{
			//	test validity
			auto ops = lootgen::getEnchantmentsForItemCategory(gdata->_viewingItem->_category);
			if (find(ops.begin(), ops.end(), en) != ops.end())
			{
				//	spend materials
				spendMaterial(gdata, gdata->_viewingItem->getEnhanceMaterial(), gdata->_viewingItem->getEnhanceCost());

				//	add enchantment
				gdata->_viewingItem->addEnchantment(en, lootgen::rollEnchantmentBonus(en));
				gdata->_viewingItem->_enhancementLevel++;
				openForgeMenu(gdata);
			}
			else
				messages::error(gdata, "This item can't have that enchantment.");
		}
		else
			messages::error(gdata, "Item already has that enchantment!");
	}
}


//	If an equipment item is selected, test if we can enhance it.
void enhanceSelectedItem(gamedataPtr gdata)
{
	auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
	if (it != nullptr)
		enhanceItem(gdata, it);
}


//	Attempt to repair the selected equipment item.
void repairEquipmentItem(gamedataPtr gdata)
{
	auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
	if (it != nullptr)
		repairItem(gdata, it);
}

void openAlchemyMenu(gamedataPtr gdata)
{
	gdata->_state = STATE_ALCHEMY;
	gdata->_idx = 0;
}


//	Increase the healing potential of our current flask.
void tryEnhanceFlask(gamedataPtr gdata)
{
	auto mat_type = gdata->_player->_currentFlask->getEnhanceMaterial();
	auto mat_cost = gdata->_player->_currentFlask->getEnhanceCost();
	if (hasMaterial(gdata, mat_type, mat_cost))
	{
		spendMaterial(gdata, mat_type, mat_cost);
		int heal = gdata->_player->_currentFlask->getProperty(PROP_HEAL_ON_USE);
		gdata->_player->_currentFlask->adjustProperty(PROP_HEAL_ON_USE, (float)heal * 0.15f);
		messages::add(gdata, "Enhanced #" + gdata->_player->_currentFlask->getName() + "@!", { gdata->_player->_currentFlask->getColor() });
	}
	else
		messages::error(gdata, "Insufficient materials!");
}


//	Creates a list of gemstones we can fabricate, given our current supply of stones.
const vector<itemPtr> getGemstonesWeCanFabricate(gamedataPtr gdata)
{
	vector<itemPtr> gems;
	for (auto it : gdata->_stashedGems)
	{
		if (it->_amountLeft >= 3 && it->_enhancementLevel < GEM_MAX_TIER)
		{
			auto gem = lootgen::generateGemOfType(it->_gemType, it->_enhancementLevel + 1, it->_rarity);
			if (hasMaterial(gdata, MaterialType::FRAGMENTS, getGemstoneFabricateCost(gem)))
				gems.push_back(gem);
		}
	}
	return gems;
}


//	Fragments required to combine
const int getGemstoneFabricateCost(itemPtr it)
{
	switch (it->_enhancementLevel)
	{
	case(2):	return 100;
	case(3):	return 300;
	case(4):	return 900;
	case(5):	return 2700;
	default:
		return 0;
	}
}


//	Clears all gems from the currently selected equipment item, returning them to our stash.
void removeAllGemsFromItem(gamedataPtr gdata)
{
	if (gdata->_idx < SLOT__NONE)
	{
		auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
		if (it != nullptr)
		{
			//	Add all the gems to our stash
			auto gems = it->getAllSocketedGemTypes();
			for (unsigned i = 0; i < gems->size(); i++)
			{
				if (gems->at(i) != GemType::__NONE)
					addToStash(gdata, lootgen::generateGemOfType(gems->at(i), it->getSocketLevel(i), 1));
			}

			//	Remove them from the item
			it->clearAllGems();
		}
	}
}



//	Select a gem to socket into the selected item.
void selectGemToSocketIntoItem(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_stashedGems.size())
	{
		//	socket this gem
		auto gem = gdata->_stashedGems[gdata->_idx];
		gdata->_viewingItem->socketGem(gem->_gemType, gem->_enhancementLevel);

		//	remove the gem
		gem->adjustAmount(-1);
		if (gem->_amountLeft < 1)
			gdata->_stashedGems.erase(gdata->_stashedGems.begin() + gdata->_idx);

		//	return to previous menu
		openGemstonePress(gdata);
	}
}


//	Pick an item to socket gems into.
//	Always selected from our equipment list.
void selectItemToSocket(gamedataPtr gdata)
{
	if (gdata->_idx < SLOT__NONE)
	{
		auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
		if (it != nullptr && it->hasFreeSocket())
		{
			gdata->_idx = 0;
			gdata->_state = STATE_SELECT_GEM;
			gdata->_viewingItem = it;
		}
	}
}


//	Attempt to fabricate the selected gemstone.
void tryFabricateGemstone(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	what we're trying to make
		auto gem = gdata->_currentItemList[gdata->_idx];
		
		//	pay the PRICE
		spendMaterial(gdata, MaterialType::FRAGMENTS, getGemstoneFabricateCost(gem));
		for (unsigned i = 0; i < gdata->_stashedGems.size(); i++)
		{
			//	identify the type of gem required to create this
			if (gdata->_stashedGems[i]->_gemType == gem->_gemType && gdata->_stashedGems[i]->_enhancementLevel == gem->_enhancementLevel - 1)
			{
				gdata->_stashedGems[i]->_amountLeft -= 3;
				if (gdata->_stashedGems[i]->_amountLeft < 1)
					gdata->_stashedGems.erase(gdata->_stashedGems.begin() + i);
				break;
			}
		}

		//	get the gem
		addToStash(gdata, gem);

		//	re-create the list of forgeable gems
		gdata->_currentItemList = getGemstonesWeCanFabricate(gdata);
		if (gdata->_idx >= gdata->_currentItemList.size())
			gdata->_idx = gdata->_currentItemList.size() - 1;
	}
}


//	Allows us to combine gems to make bigger gems.
void openGemFabricator(gamedataPtr gdata)
{
	gdata->_idx = 0;
	gdata->_currentItemList = getGemstonesWeCanFabricate(gdata);
	gdata->_state = STATE_FABRICATE_GEMS;
}


//	Allows us to socket/unsocket gems.
void openGemstonePress(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_idx = 0;
	gdata->_state = STATE_GEMSTONE_PRESS;
}


//	Open equipment menu
void openEquipment(gamedataPtr gdata)
{
	gdata->_state = STATE_VIEW_EQUIPMENT;
	gdata->_selectedSlot = SLOT__NONE;
	gdata->_idx = 0;
}


//	Insert item into inventory.
void addToInventory(gamedataPtr gdata, itemPtr it)
{
	//	Test that we have space
	if (getInventorySlotsAvailable(gdata) < it->getSize())
	{
		//	Drop item on the ground.
		messages::error(gdata, "Your inventory is full!");
		gdata->_map->addItem(it, gdata->_player->_pos);
		return;
	}

	//	Note that we have a new item for whatever slot this goes in
	if (it->_isNewItem)
		gdata->_player->markNewForSlot(getSlotForCategory(it->_category));

	//	Attempt to stack
	if (it->stacks())
	{
		for (auto other : gdata->_carriedItems)
		{
			if (other->stacksWith(it))
			{
				other->adjustAmount(it->_amountLeft);
				return;
			}
		}
	}

	//	Attempt to insert alphabetically
	for (auto f = gdata->_carriedItems.begin(); f != gdata->_carriedItems.end(); f++)
	{
		if (isFirstAlphabetically(it->getName(), (*f)->getName()))
		{
			gdata->_carriedItems.insert(f, it);
			return;
		}
	}

	//	Push at end
	gdata->_carriedItems.push_back(it);
}


//	Deletes the given item from our inventory (because we have equipped it/dropped it/whatever)
void removeFromInventory(gamedataPtr gdata, itemPtr it)
{
	auto f = find(gdata->_carriedItems.begin(), gdata->_carriedItems.end(), it);
	if (f != gdata->_carriedItems.end())
		gdata->_carriedItems.erase(f);
}


//	Does something contextual with the selected inventory item.
void selectItemFromInventory(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		if (it->_category == ITEM_FLASK)
			tryEquipSelectedFlask(gdata);
		else
			openEquipment(gdata);
	}
}

void viewItemDetails(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		gdata->_viewingItem = gdata->_currentItemList[gdata->_idx];
		gdata->_state = STATE_VIEW_ITEM_DETAILS;
	}
}


//	When we drop an item from inventory/transfer it from stash/etc, we need to remove it from the list of visible items.
//	We also need to update the current item index (to make sure it stays valid).
//	This handy function does both those things.
void removeFromCurrentItemList(gamedataPtr gdata, itemPtr it)
{
	auto f = find(gdata->_currentItemList.begin(), gdata->_currentItemList.end(), it);
	if (f != gdata->_currentItemList.end())
	{
		gdata->_currentItemList.erase(f);
		if (gdata->_idx >= gdata->_currentItemList.size())
			gdata->_idx--;
	}
}


//	Discard the selected inventory item.
void dropSelectedItem(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	Remove the item from inventory & drop it on the ground.
		auto it = gdata->_currentItemList[gdata->_idx];
		removeFromInventory(gdata, it);
		gdata->_map->addItem(it, gdata->_player->_pos);

		//	Remove the item from the list and make sure our index is okay.
		removeFromCurrentItemList(gdata, it);
	}
}


//	IF there's anything at the given point we want to get automatically, we do.
void tryAutopickup(gamedataPtr gdata, const intpair pt)
{
	int anim_delay = 0;
	auto icp = gdata->_map->getItemContainer(pt);
	if (icp != nullptr)
	{
		//	make a list of items to get
		vector<itemPtr> toPickup;
		for (auto it : icp->_items)
		{
			if (it->_category == ITEM_MATERIAL)
				toPickup.push_back(it);
		}
		
		//	Pick up all those items
		//	(We make a list first so that the items can be safely removed from the container as we get them)
		for (auto it : toPickup)
		{
			//	get the item
			addToInventory(gdata, it);
			gdata->_map->removeItemFromContainer(it, icp);

			//	we play a little animation when we pick up money
			if (it->_category == ITEM_MATERIAL)
				addAnimation(gdata, anim_FlashGlyph(gdata->_player->_pos, '$', it->getColor()));
				//addAnimation(gdata, padAnimationFront(anim_MovingText(gdata->_player->_pos, "$+" + to_string(it->_amountLeft), it->getColor(), intpair(1, -1)), anim_delay++));
		}
	}
}


//	Pick up the selected menu item
void pickupSelectedItem(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	Get the item
		auto it = gdata->_currentItemList[gdata->_idx];
		addToInventory(gdata, it);
		gdata->_map->removeItem(it, gdata->_player->_pos);

		//	Remove it from the current list
		gdata->_currentItemList.erase(gdata->_currentItemList.begin() + gdata->_idx);

		//	If list is now empty, close menu
		if (gdata->_currentItemList.empty())
			gdata->_state = STATE_NORMAL;

		//	If list is still open, make sure our selected index is still valid
		else
		{
			while (gdata->_idx >= gdata->_currentItemList.size())
				gdata->_idx -= 1;
		}
	}
}


//	Pick up an item from directly beneath us.
void pickupFromFloor(gamedataPtr gdata)
{
	auto icp = gdata->_map->getItemContainer(gdata->_player->_pos);
	if (icp != nullptr)
	{
		//	if there's just one item, pick it up immediately
		//if (icp->_items.size() == 1)
		//{
		//	auto it = icp->_items.front();
		//	addToInventory(gdata, it);
		//	gdata->_map->removeItemFromContainer(it, icp);
		//	messages::add(gdata, "Picked up #" + it->getName() + "@!", { it->getColor() });
		//}

		////	otherwise, give us a choice of what to get
		//else 
		if (icp->_items.size() > 0)
		{
			gdata->_currentItemList.clear();
			for (auto it : icp->_items)
				gdata->_currentItemList.push_back(it);

			gdata->_state = STATE_PICKING_UP_ITEMS;
			gdata->_idx = 0;
		}
	}
}


//	Sums up the total number of inventory slots we are using.
int getInventorySlotsUsed(gamedataPtr gdata)
{
	int total = 0;
	for (auto it : gdata->_carriedItems)
		total += it->getSize();
	return total;
}


//	Free slots in our inventory.
int getInventorySlotsAvailable(gamedataPtr gdata)
{
	return gdata->_player->getMaxInventorySlots() - getInventorySlotsUsed(gdata);
}
