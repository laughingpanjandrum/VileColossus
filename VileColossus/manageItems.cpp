#include "manageItems.h"



void openRitualAltar(gamedataPtr gdata)
{
	//	clear current ritual
	gdata->_ritualType = MaterialType::__NONE;
	gdata->_summonedViledragon = false;

	//	open the menu
	gdata->_idx = 0;
	gdata->_state = STATE_RITUAL_ALTAR;
	autodepositMaterials(gdata);
}


//	Add a material to the selected ritual.
void selectRitualMaterial(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_stashedRitualMaterials.size())
	{
		auto it = gdata->_stashedRitualMaterials[gdata->_idx];
		if (gdata->_ritualType == it->_material)
			gdata->_ritualType = MaterialType::__NONE;
		else
			gdata->_ritualType = it->_material;
	}
}


//	Completes the current ritual and opens an abyssal gate.
void openAbyssGate(gamedataPtr gdata)
{
	//	Expend materials.
	if (gdata->_ritualType != MaterialType::__NONE)
		spendMaterial(gdata, gdata->_ritualType, 1);

	//	Open the portal.
	gdata->_map->setTile(MT_ABYSSAL_GATE, 11, 14);

	//	Flavour.
	addAnimation(gdata, anim_Explosion(intpair(11, 14), 1, '*', TCODColor::purple));
	messages::add(gdata, "#* OPENED ABYSSAL GATE *", { COLOR_WHITE });
	gdata->_state = STATE_NORMAL;
}


//	Apply 'on-use' effects of a given item
void applyFlaskEffect(gamedataPtr gdata, itemPtr it)
{
	//	Instant healing
	if (it->hasProperty(PROP_HEAL_ON_USE))
		gdata->_player->healDamage(it->getProperty(PROP_HEAL_ON_USE));

	//	Heals over time
	if (it->hasEnchantment(ENCH_REGEN))
		gdata->_player->addBuff(BUFF_REGENERATION, it->getEnchantmentValue(ENCH_REGEN));

	//	Secondary effects
	if (it->hasEnchantment(ENCH_CURING))
	{
		gdata->_player->cureStatusEffect(STATUS_BURN);
		gdata->_player->cureStatusEffect(STATUS_POISON);
		gdata->_player->cureStatusEffect(STATUS_SHOCK);
	}
	if (it->hasEnchantment(ENCH_MAGIC_RESTORE))
		gdata->_player->restoreMagic(it->getEnchantmentValue(ENCH_MAGIC_RESTORE));
	if (it->hasEnchantment(ENCH_EMPOWERING))
		gdata->_player->addBuff(BUFF_EMPOWERED, it->getEnchantmentValue(ENCH_EMPOWERING));
	if (it->hasEnchantment(ENCH_HASTE))
		gdata->_player->addBuff(BUFF_HASTE, it->getEnchantmentValue(ENCH_HASTE));
	if (it->hasEnchantment(ENCH_STONESKIN))
		gdata->_player->addBuff(BUFF_STONESKIN, it->getEnchantmentValue(ENCH_STONESKIN));
	if (it->hasEnchantment(ENCH_WRATH))
		gdata->_player->addBuff(BUFF_WRATH, it->getEnchantmentValue(ENCH_WRATH));
}


//	Apply effects of equipped flask, if we have a charge available; use a charge
void playerQuaffCurrentFlask(gamedataPtr gdata)
{
	auto fl = gdata->_player->_currentFlask;
	if (fl != nullptr)
	{
		if (fl->hasChargesLeft())
		{
			//	Immediate effect
			applyFlaskEffect(gdata, fl);
			fl->expendCharge();
			
			//	Animations
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


//	Add an item to the given list. Attempts to stack with existing items first, appends to end otherwise.
void stackItemInList(gamedataPtr gdata, vector<itemPtr>* ilist, itemPtr it)
{
	//	Is it in the materials list already?
	for (auto other : *ilist)
	{
		if (other->stacksWith(it))
		{
			other->_amountLeft += it->_amountLeft;
			return;
		}
	}

	//	If not, add it.
	ilist->push_back(it);
}


void openStash(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_state = STATE_VIEW_STASH;
	gdata->_currentItemList = gdata->_stashItems;
	gdata->_idx = 0;
	gdata->_player->unmarkAllSlots();
}


//	Add a gemstone to our stash.
void stashGemstone(gamedataPtr gdata, itemPtr it)
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

	//	If not, add it in order with other gems of the same type.
	for (auto f = gdata->_stashedGems.begin(); f != gdata->_stashedGems.end(); f++)
	{
		//	Found gems of the same type.
		if ((*f)->_gemType == it->_gemType)
		{
			//	Find the first gem in the list that's higher level than the current one, and insert before.
			//	If none is higher level, insert afterward.
			while (f != gdata->_stashedGems.end() && (*f)->_enhancementLevel < it->_enhancementLevel && (*f)->_gemType == it->_gemType)
				f++;
			
			//	Insert.
			gdata->_stashedGems.insert(f, it);
			return;
		}
	}

	//	Push to the end.
	gdata->_stashedGems.push_back(it);
}


//	Put an item into our stash.
void addToStash(gamedataPtr gdata, itemPtr it)
{
	//	Materials go into a separate list.
	if (it->_category == ITEM_MATERIAL)
	{
		if (isRitualMaterial(it->_material))
			stackItemInList(gdata, &gdata->_stashedRitualMaterials, it);
		else
			stackItemInList(gdata, &gdata->_stashedMaterials, it);
		return;
	}

	//	As do gems.
	else if (it->_category == ITEM_GEM)
	{
		stashGemstone(gdata, it);
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


//	Toggles a mark on the item that highlights it and prevents it from being dismantled.
void toggleMarkedValuable(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];
		it->_markedAsValuable = !it->_markedAsValuable;
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

	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];

		//	Did we select a flask? If so, equip the flask
		if (it->_category == ITEM_FLASK)
			selectItemFromInventory(gdata);

		//	Otherwise, try equipping
		else if (gdata->_selectedSlot != SLOT__NONE && it->canEquipInSlot(gdata->_selectedSlot))
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


//	Returns True if we found the item in the list.
bool spendMaterialFromList(gamedataPtr gdata, vector<itemPtr>* mlist, const MaterialType mtype, const int amt)
{
	for (auto m : *mlist)
	{
		if (m->_material == mtype)
		{
			m->_amountLeft -= amt;
			if (m->_amountLeft <= 0)
				mlist->erase(find(mlist->begin(), mlist->end(), m));
			return true;
		}
	}
	return false;
}


//	Attempts to expend the given amount of stashed material.
void spendMaterial(gamedataPtr gdata, const MaterialType mtype, const int amt)
{
	bool found = spendMaterialFromList(gdata, &gdata->_stashedMaterials, mtype, amt);
	if (!found)
		spendMaterialFromList(gdata, &gdata->_stashedRitualMaterials, mtype, amt);
}


//	Dismantles all unmarked items in our inventory.
void dismantleAll(gamedataPtr gdata)
{
	//	Make a list of items to dismantle (we need a copy b/c items are removed from lists when dismantled)
	vector<itemPtr> dismantle;
	for (auto it : gdata->_carriedItems)
	{
		if (!it->_markedAsValuable)
			dismantle.push_back(it);
	}

	//	Now dismantle them.
	for (auto it : dismantle)
		dismantleItem(gdata, it);
}


//	Dismantle a particular item.
void dismantleItem(gamedataPtr gdata, itemPtr it)
{
	//	Amount/type of parts depend on the item.
	int fragments = dieRoll(3, 4 + 2 * it->_rarity);
	addToStash(gdata, lootgen::generateMaterial(MaterialType::FRAGMENTS, fragments));

	//	Higher-tier and type-based stuff
	if (it->_rarity > 1)
		addToStash(gdata, lootgen::generateMaterial(MaterialType::MAGIC_DUST, randint(4, 9)));
	if (it->_rarity > 2)
		addToStash(gdata, lootgen::generateMaterial(MaterialType::GLOWING_POWDER, randint(4, 9)));
	if (it->_rarity > 3)
		addToStash(gdata, lootgen::generateMaterial(MaterialType::RADIANT_ASH, 1));

	//	Flask components
	/*if (it->_category == ITEM_FLASK)
		addToStash(gdata, lootgen::generateMaterial(MaterialType::GLASS_SHARD, it->_rarity + 1));*/

		//	Spellrune components
	else if (it->_category == ITEM_SPELLRUNE)
	{
		addToStash(gdata, lootgen::generateMaterial(MaterialType::RUNE_SHARD, it->_rarity + it->_spellLevel / 2));
		if (it->_spellLevel >= 10 || it->_rarity > 2 || roll_one_in(20))
			addToStash(gdata, lootgen::generateMaterial(MaterialType::BRIGHT_RUNE, it->_rarity));
	}

	//	Save any gems in the item.
	auto gems = it->getAllSocketedGemTypes();
	for (unsigned i = 0; i < gems->size(); i++)
	{
		if (gems->at(i) != GemType::__NONE)
		{
			auto lvl = it->getSocketLevel(i);
			addToStash(gdata, lootgen::generateGemOfType(gems->at(i), lvl, 1));
		}
	}

	//	Learn its enchants, if any
	for (auto en : *it->getAllEnchantments())
	{
		if (!knowsEnchantmentType(gdata, en) && canLearnEnchantment(gdata, en))
		{
			messages::add(gdata, "Learned enchantment type: #" + getItemEnchantmentName(en), { COLOR_WHITE });
			learnEnchantment(gdata, en);
		}
	}

	//	Destroy the item.
	removeFromInventory(gdata, it);
	removeFromCurrentItemList(gdata, it);
}


//	Take apart an item to get parts back.
void dismantleFromInventory(gamedataPtr gdata)
{
	//	Test that we have a valid item selected.
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		auto it = gdata->_currentItemList[gdata->_idx];

		//	Don't dismantle valuables!
		if (it->_markedAsValuable)
		{
			messages::error(gdata, "This item is marked as valuable and can't be dismantled!");
			messages::add(gdata, "Remove the mark if you want to dismantle it.");
		}
		
		else
		{
			dismantleItem(gdata, it);
		}
	}
}


//	Adds an enchantment ot the list of ones we can apply.
void learnEnchantment(gamedataPtr gdata, const ItemEnchantment en)
{
	//	Attempt to add alphabetically.
	const string name = getItemEnchantmentDescription(en);
	for (auto f = gdata->_knownEnchants.begin(); f != gdata->_knownEnchants.end(); f++)
	{
		if (isFirstAlphabetically(name, getItemEnchantmentDescription(*f)))
		{
			gdata->_knownEnchants.insert(f, en);
			return;
		}
	}
	gdata->_knownEnchants.push_back(en);
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


//	Cost to generate an item of the given rarity at the demonforge
const pair<MaterialType, int> getCostToForgeItem(const int rarity)
{
	pair<MaterialType, int> cost;
	switch (rarity)
	{
	case(1):	cost.first = MaterialType::FRAGMENTS; cost.second = 500; break;
	case(2):	cost.first = MaterialType::MAGIC_DUST; cost.second = 100; break;
	case(3):	cost.first = MaterialType::GLOWING_POWDER; cost.second = 50; break;
	case(4):	cost.first = MaterialType::RADIANT_ASH; cost.second = 2; break;
	}
	return cost;
}


//	Turn one material into another; 'num' indicates which type
void transmuteMaterial(gamedataPtr gdata, const int num)
{
	//	Fragments => Magic Dist
	if (num == 1)
	{
		if (hasMaterial(gdata, MaterialType::FRAGMENTS, 10000))
		{
			spendMaterial(gdata, MaterialType::FRAGMENTS, 10000);
			addToStash(gdata, lootgen::generateMaterial(MaterialType::GLOWING_POWDER, 250));
			messages::add(gdata, "Transmutation complete!");
		}
		else
			messages::error(gdata, "Not enough materials!");
	}

	//	Magic Dust => Glowing Powder
	else if (num == 2)
	{
		if (hasMaterial(gdata, MaterialType::MAGIC_DUST, 500))
		{
			spendMaterial(gdata, MaterialType::MAGIC_DUST, 500);
			addToStash(gdata, lootgen::generateMaterial(MaterialType::GLOWING_POWDER, 100));
			messages::add(gdata, "Transmutation complete!");
		}
		else
			messages::error(gdata, "Not enough materials!");
	}

	//	Glowing Powder => Radiant Ash
	else if (num == 3)
	{
		if (hasMaterial(gdata, MaterialType::GLOWING_POWDER, 200))
		{
			spendMaterial(gdata, MaterialType::GLOWING_POWDER, 200);
			addToStash(gdata, lootgen::generateMaterial(MaterialType::RADIANT_ASH, 1));
			messages::add(gdata, "Transmutation complete!");
		}
		else
			messages::error(gdata, "Not enough materials!");
	}

	//	Glowing Powder => Notched Cube
	else if (num == 4)
	{
		if (hasMaterial(gdata, MaterialType::GLOWING_POWDER, 100))
		{
			spendMaterial(gdata, MaterialType::GLOWING_POWDER, 100);
			addToStash(gdata, lootgen::generateMaterial(MaterialType::NOTCHED_CUBE, 1));
			messages::add(gdata, "Created a Notched Cube!");
		}
		else
			messages::error(gdata, "Not enough materials!");
	}
}


//	Increases the tier of items created with the forge.
void tryUpgradeDemonforge(gamedataPtr gdata)
{
	if (gdata->_demonforgeTier == 1 && hasMaterial(gdata, MaterialType::MAGIC_DUST, 500))
	{
		gdata->_demonforgeTier++;
		spendMaterial(gdata, MaterialType::MAGIC_DUST, 500);
		messages::add(gdata, "#DEMONFORGE ENHANCED TO #TIER 2", { TCODColor::orange, TCODColor::lightBlue });
	}
	else if (gdata->_demonforgeTier == 2 && hasMaterial(gdata, MaterialType::GLOWING_POWDER, 250))
	{
		gdata->_demonforgeTier++;
		spendMaterial(gdata, MaterialType::GLOWING_POWDER, 250);
		messages::add(gdata, "#DEMONFORGE ENHANCED TO #TIER 3", { TCODColor::orange, TCODColor::yellow });
	}
	else
		messages::error(gdata, "Cannot upgrade the demonforge!");
}


//	Try to create an item of the given rarity.
void createWithDemonforge(gamedataPtr gdata, const int rarity)
{
	auto cost = getCostToForgeItem(rarity);
	if (hasMaterial(gdata, cost.first, cost.second))
	{
		spendMaterial(gdata, cost.first, cost.second);
		auto it = lootgen::rollItemDrop(gdata->_demonforgeTier, rarity, true);
		messages::add(gdata, "Forged #" + it->getName() + "@!", { it->getColor() });
		addToInventory(gdata, it);
	}
	else
		messages::error(gdata, "Not enough materials!");
}


//	Select a numbered option from the demonforge.
//	These are hardcoded to different special effects, based on the number we pass to this fn.
void selectDemonforgeOption(gamedataPtr gdata, const int num)
{
	switch (num)
	{
	case(1):	createWithDemonforge(gdata, 3); break;
	case(2):	createWithDemonforge(gdata, 4); break;
	case(3):	transmuteMaterial(gdata, 1); break;
	case(4):	transmuteMaterial(gdata, 2); break;
	case(5):	transmuteMaterial(gdata, 3); break;
	}
}


//	Tests whether we have a given no. of gems of the given type and tier stashed.
bool hasGemsOfType(gamedataPtr gdata, const GemType gtype, const int tier, const int qty)
{
	for (auto it : gdata->_stashedGems)
	{
		if (it->_gemType == gtype && it->_enhancementLevel == tier)
		{
			return it->_amountLeft >= qty;
		}
	}
	return false;
}


//	Lose the given no. of gems of the given type and tier.
void expendGems(gamedataPtr gdata, const GemType gtype, const int tier, const int qty)
{
	for (unsigned i = 0; i < gdata->_stashedGems.size(); i++)
	{
		auto it = gdata->_stashedGems[i];
		if (it->_gemType == gtype && it->_enhancementLevel == tier)
		{
			//	expend the gems
			it->_amountLeft -= qty;

			//	remove from list if none are left
			if (it->_amountLeft < 0)
				gdata->_stashedGems.erase(gdata->_stashedGems.begin() + i);
			return;
		}
	}
}


//	Type of gem required to exalt an item, and the minimum tier.
const pair<GemType, int> getGemForExalt(const itemPtr it)
{
	pair<GemType, int> cost;

	//	Type of gem determined by category
	switch (it->_category)
	{
	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_SHOULDERS):
		cost.first = GemType::BLACKSTONE;
		break;

	case(ITEM_CHESTPIECE):
		cost.first = GemType::BOLTSTONE;
		break;

	case(ITEM_AMULET):
	case(ITEM_RING):
		cost.first = GemType::SILVERSTONE;
		break;

	case(ITEM_QUIVER):
	case(ITEM_SHIELD):
		cost.first = GemType::FLAMESTONE;
		break;

	case(ITEM_WEAPON):
		cost.first = GemType::SPIDERSTONE;
		break;
	}


	//	Tier determined by exalt level
	if		(it->_exaltLevel < 2)	cost.second = 1;
	else if (it->_exaltLevel < 4)	cost.second = 2;
	else if (it->_exaltLevel < 6)	cost.second = 3;
	else							cost.second = 4;

	return cost;
}

//	No. of gems required to exalt an item.
const int getGemQuantityForExalt(const itemPtr it)
{
	switch (it->_category)
	{
	case(ITEM_AMULET):
	case(ITEM_RING):
		return 2;

	case(ITEM_BOOTS):
	case(ITEM_BRACERS):
	case(ITEM_GLOVES):
	case(ITEM_HELMET):
	case(ITEM_SHOULDERS):
		return 1;

	case(ITEM_CHESTPIECE):
		return 5;

	case(ITEM_QUIVER):
	case(ITEM_SHIELD):
		return 3;

	case(ITEM_WEAPON):
		return 3;
	}
}


//	Type and qty of materials required to exalt an item.
const pair<MaterialType, int> getMaterialForExalt(const itemPtr it)
{
	pair<MaterialType, int> cost;

	//	Material type
	if (it->_exaltLevel < 3)
	{
		cost.first = MaterialType::MAGIC_DUST;
		cost.second = (it->_exaltLevel + 1) * 50;
	}
	else if (it->_exaltLevel < 6)
	{
		cost.first = MaterialType::GLOWING_POWDER;
		cost.second = (it->_exaltLevel - 2) * 50;
	}
	else
	{
		cost.first = MaterialType::RADIANT_ASH;
		cost.second = it->_exaltLevel - 5;
	}

	return cost;
}


//	Attempt to increase the exalt level of the selected equipment item
void tryExaltItem(gamedataPtr gdata)
{
	if (gdata->_idx < SLOT__NONE)
	{
		auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
		if (it != nullptr && it->_exalted)
		{
			//	Make sure we can pay for it
			auto gcost = getGemForExalt(it);
			auto gqty = getGemQuantityForExalt(it);
			auto mcost = getMaterialForExalt(it);

			//	Do the thing
			if (hasGemsOfType(gdata, gcost.first, gcost.second, gqty) && hasMaterial(gdata, mcost.first, mcost.second))
			{
				it->addExaltLevel();
				spendMaterial(gdata, mcost.first, mcost.second);
				expendGems(gdata, gcost.first, gcost.second, gqty);
			}
		}
	}
}

void openDemonforge(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_state = STATE_DEMONFORGE;
	gdata->_idx = 0;
}



//	Returns True if the given item has 1 or more enchantments we don't already know.
bool doesItemHaveUnknownEnchants(gamedataPtr gdata, itemPtr it)
{
	//	We can't enchant flasks, so no need to harvest their enchants.
	if (it->_category == ITEM_FLASK)
		return false;

	//	Otherwise, test for enchants.
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


//	Returns True if we're allowed to learn the given enchant on dismantle.
//	We don't learn legendary enchants.
bool canLearnEnchantment(gamedataPtr gdata, const ItemEnchantment en)
{
	return find(lootgen::LEGENDARY_ENCHANTS.begin(), lootgen::LEGENDARY_ENCHANTS.end(), en) == lootgen::LEGENDARY_ENCHANTS.end();
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
				auto bns = lootgen::rollEnchantmentBonus(en) + getEnchantmentIncrement(en) * (gdata->_viewingItem->_tier - 1);
				gdata->_viewingItem->addEnchantment(en, bns);
				gdata->_viewingItem->_enhancementLevel++;

				//	return to the forge menu, but set index to selected item
				openForgeMenu(gdata);
				auto eq = gdata->_player->getAllEquippedItems();
				for (gdata->_idx = 0; gdata->_idx < eq.size(); gdata->_idx++)
				{
					if (eq[gdata->_idx] == gdata->_viewingItem)
						break;
				}
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


//	Fragment cost to increase flask healing amount.
int getFlaskEnhanceCost(gamedataPtr gdata, itemPtr it)
{
	return it->getProperty(PROP_HEAL_ON_USE) * 3 / 2;
}


void openAlchemyMenu(gamedataPtr gdata)
{
	autodepositMaterials(gdata);
	gdata->_state = STATE_ALCHEMY;
	gdata->_idx = 0;
}


//	Increase the healing potential of our current flask.
void tryEnhanceFlask(gamedataPtr gdata)
{
	auto mat_type = gdata->_player->_currentFlask->getEnhanceMaterial();
	auto mat_cost = gdata->_player->_currentFlask->getEnhanceCost();
	auto frag_cost = getFlaskEnhanceCost(gdata, gdata->_player->_currentFlask);
	if (hasMaterial(gdata, mat_type, mat_cost) && hasMaterial(gdata, MaterialType::FRAGMENTS, frag_cost))
	{
		spendMaterial(gdata, mat_type, mat_cost);
		spendMaterial(gdata, MaterialType::FRAGMENTS, frag_cost);
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
		if (it->_amountLeft >= 3 && it->_enhancementLevel < GEM_MAX_TIER && !doesGemHaveSingularEffect(it->_gemType))
		{
			auto gem = lootgen::generateGemOfType(it->_gemType, it->_enhancementLevel + 1, it->_rarity);
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
	case(2):	return 500;
	case(3):	return 2500;
	case(4):	return 10000;
	default:
		return 0;
	}
}

//	Tests whether we can add a gem slot to the given item.
//	We must have the required material, and the item must have no slots AND the ability to gain slots.
bool canAddGemSlot(gamedataPtr gdata, const itemPtr it)
{
	//	Can this item gain sockets?
	if (it->getSocketCount() == 0 && lootgen::getMaxSocketsForCategory(it->_category) > 0)
	{
		//	Do we have cubes in inventory/stash?
		if (hasMaterial(gdata, MaterialType::NOTCHED_CUBE, 1))
			return true;
	}
	return false;
}


//	If we can, add a gem slot to the selected equipment item.
void tryAddGemSlot(gamedataPtr gdata)
{
	if (gdata->_idx < SLOT__NONE)
	{
		gdata->_selectedSlot = static_cast<EquipmentSlot>(gdata->_idx);
		auto it = gdata->_player->getItemInSlot(gdata->_selectedSlot);
		if (it != nullptr && canAddGemSlot(gdata, it))
		{
			it->adjustMaxSockets(1);
			spendMaterial(gdata, MaterialType::NOTCHED_CUBE, 1);
			messages::add(gdata, "Added socket to #" + it->getName() + "@!", { it->getColor() });
		}
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
		gdata->_idx = gdata->_selectedSlot;
	}
}


//	Pick an item to socket gems into.
//	Always selected from our equipment list.
void selectItemToSocket(gamedataPtr gdata)
{
	if (gdata->_idx < SLOT__NONE)
	{
		gdata->_selectedSlot = static_cast<EquipmentSlot>(gdata->_idx);
		auto it = gdata->_player->getItemInSlot(gdata->_selectedSlot);
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
		auto cost = getGemstoneFabricateCost(gem);
		
		//	Can we afford it?
		if (hasMaterial(gdata, MaterialType::FRAGMENTS, cost))
		{
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
		else
			messages::error(gdata, "Not enough materials!");
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
	gdata->_currentItemList.clear();
	gdata->_state = STATE_VIEW_EQUIPMENT;
	gdata->_selectedSlot = SLOT__NONE;
	gdata->_idx = 0;
}


//	Insert item into inventory.
void addToInventory(gamedataPtr gdata, itemPtr it)
{
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

	//	Test that we have space
	if (getInventorySlotsAvailable(gdata) < it->getSize())
	{
		//	Drop item on the ground.
		messages::error(gdata, "Your inventory is full!");
		gdata->_map->addItem(it, gdata->_player->_pos);
		gdata->_state = STATE_NORMAL;
		return;
	}

	//	Note that we have a new item for whatever slot this goes in
	if (it->_isNewItem)
		gdata->_player->markNewForSlot(getSlotForCategory(it->_category));

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


//	View detailed information about selected inventory item.
void viewItemDetails(gamedataPtr gdata)
{
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		gdata->_viewingItem = gdata->_currentItemList[gdata->_idx];
		gdata->_state = STATE_VIEW_ITEM_DETAILS;
	}
}


//	View detailed description of selected equipment item, if any.
void viewEquipmentDetails(gamedataPtr gdata)
{
	auto items = gdata->_player->getAllEquippedItems();
	if (gdata->_idx <= items.size())
	{
		if (items[gdata->_idx] != nullptr)
		{
			gdata->_viewingItem = items[gdata->_idx];
			gdata->_state = STATE_VIEW_ITEM_DETAILS;
		}
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
			{
				messages::add(gdata, "Got #" + it->getName() + "@!", { it->getColor() });
				addAnimation(gdata, anim_FlashGlyph(gdata->_player->_pos, '$', it->getColor()));
			}

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
		if (icp->_items.size() > 0)
		{
			//	Make list of all items used.
			gdata->_currentItemList.clear();
			for (auto it : icp->_items)
				gdata->_currentItemList.push_back(it);

			//	Indicates we're picking up items
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
