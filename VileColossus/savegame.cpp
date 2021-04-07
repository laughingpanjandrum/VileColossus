#include "savegame.h"



void savegame::load_from_file(ifstream& f, gamedataPtr gdata)
{
	cout << "LOADING FILE..." << endl;
	auto p = gdata->_player;
	bool flag;
	

	//		PLAYER DATA			//
	cout << " Loading player data..." << endl;
	p->_level = read_int(f);
	p->_PerkLevel = read_int(f);
	p->setAttributeValue(ATTR_DEXTERITY, read_int(f));
	p->setAttributeValue(ATTR_STRENGTH, read_int(f));
	p->setAttributeValue(ATTR_WILLPOWER, read_int(f));

	//	perk ranks
	for (unsigned i = 0; i < PERK__MAX; i++)
		p->setPerkRank(static_cast<Perk>(i), read_int(f));


	////		METADATA			//
	cout << " Loading metadata..." << endl;
	gdata->_xp = read_int(f);
	gdata->_attributePointsLeft = read_int(f);
	gdata->_perkPoints = read_int(f);
	gdata->_townPortalCharge = read_int(f);
	gdata->_mode = static_cast<GameMode>(read_int(f));


	////	known enchants
	cout << " Loading enchants list..." << endl;
	gdata->_knownEnchants.clear();
	auto dlen = read_size_t(f);
	for (unsigned i = 0; i < dlen; i++)
	{
		auto v = read_int(f);
		gdata->_knownEnchants.push_back(static_cast<ItemEnchantment>(v));
	}
	cout << endl;


	
	////		GAME PROGRESS		//
	gdata->_gameProgress._killedHellboss = read_int(f);
	gdata->_gameProgress._killedRotking = read_int(f);

	gdata->_gameProgress._killedAmog = read_int(f);
	gdata->_gameProgress._killedDogossa = read_int(f);
	gdata->_gameProgress._killedSallowKing = read_int(f);
	gdata->_gameProgress._killedColossus = read_int(f);

	gdata->_gameProgress._killedViledragons = read_int(f);

	gdata->_gameProgress._abyssLevel = read_int(f);


	//	Current ritual
	gdata->_ritualType = static_cast<MaterialType>(read_int(f));
	gdata->_summonedViledragon = read_bool(f);


	//	Other metadata
	gdata->_totalDeaths = read_int(f);
	gdata->_totalKills = read_int(f);
	gdata->_victory = read_bool(f);


	////		EQUIPMENT			//
	cout << " Loading equipped items..." << endl;
	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		flag = read_bool(f);
		if (flag)
		{
			auto it = deserialize_item(f);
			gdata->_player->equipInSlot(it, static_cast<EquipmentSlot>(i));
		}
	}


	////	Alt items
	flag = read_bool(f);
	if (flag)
		p->_secondaryMainHand = deserialize_item(f);
	flag = read_bool(f);
	if (flag)
		p->_secondaryOffhand = deserialize_item(f);


	////	Flask
	p->_currentFlask = deserialize_item(f);


	//	Equipped spells
	read_item_list(f, &p->_ImprintedRunes);


	////		STASHED ITEMS		//
	cout << " Loading inventory items..." << endl;
	read_item_list(f, &gdata->_carriedItems);
	cout << " Loading stashed gems.." << endl;
	read_item_list(f, &gdata->_stashedGems);
	cout << " Loading material stash..." << endl;
	read_item_list(f, &gdata->_stashedMaterials);
	read_item_list(f, &gdata->_stashedRitualMaterials);
	cout << " Loading stashed items..." << endl;
	read_item_list(f, &gdata->_stashItems);
		
		
	cout << "END OF FILE" << endl;
}


//	current test - just generates all the items saved in the test file
void savegame::load_from_file(const string name, gamedataPtr gdata)
{
	const string fname = FILE_PATH + SAVE_FILE_DEFAULT_NAME;
	auto f = ifstream(fname, ios::in | ios::binary);
	if (f.is_open())
		load_from_file(f, gdata);
	else
		cout << "ERROR: Couldn't open file " << name << endl;
}


//	Read characters, one at a time, into the given string.
void savegame::read_into_string(ifstream& f, string* s, const unsigned len)
{
	for (unsigned i = 0; i < len; i++)
		s->push_back(f.get());
}


//	Reads the no. of characters first, then the string itself.
void savegame::read_into_string(ifstream& f, string* s)
{
	int dlen;
	f.read(reinterpret_cast<char*>(&dlen), sizeof(size_t));
	read_into_string(f, s, dlen);
}

uint32_t savegame::read_int(ifstream& f)
{
	uint32_t v;
	f.read(reinterpret_cast<char*>(&v), sizeof(uint32_t));
	cout << v << ",";
	return v;
}

size_t savegame::read_size_t(ifstream& f)
{
	size_t dlen;
	f.read(reinterpret_cast<char*>(&dlen), sizeof(size_t));
	return dlen;
}

bool savegame::read_bool(ifstream& f)
{
	bool b;
	f.read(reinterpret_cast<char*>(&b), sizeof(bool));
	return b;
}


itemPtr savegame::deserialize_item(ifstream& f)
{
	auto it = itemPtr(new item());
	
	read_into_string(f, &it->_name);
	read_into_string(f, &it->_nickname);

	it->_amountLeft = read_int(f);
	it->_armourCategory = static_cast<ArmourCategory>(read_int(f));
	it->_category = static_cast<ItemCategory>(read_int(f));
	it->_chargeRegeneration = read_int(f);
	it->_chargesLeft = read_int(f);
	it->_containsSpell = static_cast<Spell>(read_int(f));
	it->_damageTaken = read_int(f);
	it->_enhancementLevel = read_int(f);
	it->_exalted = read_bool(f);
	it->_exaltLevel = read_int(f);
	it->_gemType = static_cast<GemType>(read_int(f));
	it->_isNewItem = read_int(f);
	it->_isTwoHanded = read_int(f);
	it->_markedAsValuable = read_int(f);
	it->_material = static_cast<MaterialType>(read_int(f));
	it->_maxDurability = read_int(f);
	it->_rarity = read_int(f);
	it->_spellLevel = read_int(f);
	it->_tier = read_int(f);

	for (unsigned i = 0; i < PROP__NONE; i++)
		it->setProperty(static_cast<ItemProperty>(i), read_int(f));

	size_t sz = read_size_t(f);
	for (unsigned i = 0; i < sz; i++)
	{
		auto e = static_cast<ItemEnchantment>(read_int(f));
		auto v = read_int(f);
		it->addEnchantment(e, v);
	}

	sz = read_size_t(f);
	for (unsigned i = 0; i < sz; i++)
	{
		auto t = static_cast<GemType>(read_int(f));
		auto l = read_int(f);
		it->_socketSlots.push_back(t);
		it->_socketLevels.push_back(l);
	}
	
	return it;
}


//	Reads in a list of items.
void savegame::read_item_list(ifstream& f, vector<itemPtr>* ilist)
{
	ilist->clear();
	auto sz = read_size_t(f);
	cout << " >item list size: " << sz << endl;
	for (unsigned i = 0; i < sz; i++)
		ilist->push_back(deserialize_item(f));
}


//	DEBUG: attempt to load serialized items
void savegame::test_load(gamedataPtr gdata)
{
	const string fname = FILE_PATH + SAVE_FILE_DEFAULT_NAME;
	auto f = ifstream(fname, ios::in | ios::binary);
	if (f.is_open())
	{
		cout << "Reading test file..." << endl;

		//	All possible equipment items
		for (unsigned i = 0; i < SLOT__NONE; i++)
		{
			auto flag = f.get();
			cout << "Flag char was " << flag << endl;
			if (flag == 1)
			{
				cout << " Loading item...";
				auto it = deserialize_item(f);
				gdata->_map->addItem(it, gdata->_player->_pos);
			}
		}
	}
	else
		cout << "File failed to open." << endl;
	f.close();
}


//	We first push string length, then each character of the string individually.
void savegame::serialize_string(ofstream& f, const string s)
{
	auto sz = s.size();
	f.write((const char*)&sz, sizeof(size_t));
	for (unsigned i = 0; i < sz; i++)
		f.put(s[i]);
}


//	Serializing different data types	//

void savegame::serialize_bool(ofstream& f, const bool b)
{
	f.write((const char*)&b, sizeof(b));
}
void savegame::serialize_int(ofstream& f, const uint32_t i)
{
	f.write((const char*)&i, sizeof(int));
}
void savegame::serialize_size_t(ofstream& f, const size_t sz)
{
	f.write((const char*)&sz, sizeof(size_t));
}


//	Push an entire item to file.
void savegame::serialize_item(ofstream& f, const itemPtr it)
{
	serialize_string(f, it->_name);
	serialize_string(f, it->_nickname);
	cout << "Serializing " << it->_name << " '" << it->_nickname << "' ";

	serialize_int(f, it->_amountLeft);
	serialize_int(f, static_cast<uint32_t>(it->_armourCategory));
	serialize_int(f, it->_category);
	serialize_int(f, it->_chargeRegeneration);
	serialize_int(f, it->_chargesLeft);
	serialize_int(f, static_cast<uint32_t>(it->_containsSpell));
	serialize_int(f, it->_damageTaken);
	serialize_int(f, it->_enhancementLevel);
	serialize_bool(f, it->_exalted);
	serialize_int(f, it->_exaltLevel);
	serialize_int(f, static_cast<uint32_t>(it->_gemType));
	serialize_int(f, it->_isNewItem);
	serialize_int(f, it->_isTwoHanded);
	serialize_int(f, it->_markedAsValuable);
	serialize_int(f, static_cast<uint32_t>(it->_material));
	serialize_int(f, it->_maxDurability);
	serialize_int(f, it->_rarity);
	serialize_int(f, it->_spellLevel);
	serialize_int(f, it->_tier);

	for (unsigned i = 0; i < PROP__NONE; i++)
	{
		auto p = static_cast<ItemProperty>(i);
		auto v = it->getProperty(p);
		serialize_int(f, v);
	}
	
	serialize_size_t(f, it->_Enchants.size());
	for (unsigned i = 0; i < it->_Enchants.size(); i++)
	{
		serialize_int(f, it->_Enchants[i]);
		serialize_int(f, it->_EnchantLevels[i]);
	}

	serialize_size_t(f, it->_socketSlots.size());
	for (unsigned i = 0; i < it->_socketSlots.size(); i++)
	{
		serialize_int(f, static_cast<uint32_t>(it->_socketSlots[i]));
		serialize_int(f, it->_socketLevels[i]);
	}

	cout << endl;
}




void savegame::save_to_file(ofstream& f, gamedataPtr gdata)
{
	auto p = gdata->_player;
	messages::add(gdata, "Saving...");
	f.clear();


	//		PLAYER DATA		//
	serialize_int(f, p->_level);
	serialize_int(f, p->_PerkLevel);
	serialize_int(f, p->getBaseAttribute(ATTR_DEXTERITY));
	serialize_int(f, p->getBaseAttribute(ATTR_STRENGTH));
	serialize_int(f, p->getBaseAttribute(ATTR_WILLPOWER));

	//	perk levels
	for (unsigned i = 0; i < PERK__MAX; i++)
		serialize_int(f, p->getPerkRank(static_cast<Perk>(i)));



	////		METADATA		//
	serialize_int(f, gdata->_xp);
	serialize_int(f, gdata->_attributePointsLeft);
	serialize_int(f, gdata->_perkPoints);
	serialize_int(f, gdata->_townPortalCharge);
	serialize_int(f, static_cast<uint32_t>(gdata->_mode));

	//	all known enchants
	serialize_size_t(f, gdata->_knownEnchants.size());
	for (unsigned i = 0; i < gdata->_knownEnchants.size(); i++)
		serialize_int(f, gdata->_knownEnchants[i]);


	//		GAME PROGRESS	//

	serialize_int(f, gdata->_gameProgress._killedHellboss);
	serialize_int(f, gdata->_gameProgress._killedRotking);

	serialize_int(f, gdata->_gameProgress._killedAmog);
	serialize_int(f, gdata->_gameProgress._killedDogossa);
	serialize_int(f, gdata->_gameProgress._killedSallowKing);
	serialize_int(f, gdata->_gameProgress._killedColossus);

	serialize_int(f, gdata->_gameProgress._killedViledragons);

	serialize_int(f, gdata->_gameProgress._abyssLevel);


	//	Current ritual
	serialize_int(f, static_cast<int>(gdata->_ritualType));
	serialize_bool(f, gdata->_summonedViledragon);


	//	Other metadata
	serialize_int(f, gdata->_totalDeaths);
	serialize_int(f, gdata->_totalKills);
	serialize_bool(f, gdata->_victory);



	//		EQUIPMENT		//

	//	Currently equipped
	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		auto it = p->getItemInSlot(static_cast<EquipmentSlot>(i));

		//	Test for existence of item in this slot. If none, push a 0; otherwise, push a 1.
		if (it == nullptr)
			serialize_bool(f, false);
		else
		{
			serialize_bool(f, true);
			serialize_item(f, it);
		}
	}

	//	Alt items
	auto alts = { p->_secondaryMainHand, p->_secondaryOffhand };
	for (auto it : alts)
	{
		if (it == nullptr)
			serialize_bool(f, false);
		else
		{
			serialize_bool(f, true);
			serialize_item(f, it);
		}
	}

	//	Flask
	serialize_item(f, p->_currentFlask);


	//	Equipped spells
	serialize_size_t(f, p->_ImprintedRunes.size());
	for (auto sp : p->_ImprintedRunes)
		serialize_item(f, sp);



	//		STASHED ITEMS	//

	serialize_size_t(f, gdata->_carriedItems.size());
	cout << " Carried items count: " << gdata->_carriedItems.size() << endl;
	for (auto it : gdata->_carriedItems)
		serialize_item(f, it);

	serialize_size_t(f, gdata->_stashedGems.size());
	cout << " Stashed gems count: " << gdata->_stashedGems.size() << endl;
	for (auto it : gdata->_stashedGems)
		serialize_item(f, it);

	serialize_size_t(f, gdata->_stashedMaterials.size());
	cout << " Stashed materials: " << gdata->_stashedMaterials.size() << endl;
	for (auto it : gdata->_stashedMaterials)
		serialize_item(f, it);

	serialize_size_t(f, gdata->_stashedRitualMaterials.size());
	cout << " Stashed ritual materials: " << gdata->_stashedRitualMaterials.size() << endl;
	for (auto it : gdata->_stashedRitualMaterials)
		serialize_item(f, it);

	serialize_size_t(f, gdata->_stashItems.size());
	cout << " Stashed items count: " << gdata->_stashItems.size() << endl;
	for (auto it : gdata->_stashItems)
		serialize_item(f, it);


	//	Finished.
	messages::add(gdata, "Saved game.");
}



//	Given a name.
void savegame::save_to_file(const string name, gamedataPtr gdata)
{
	const string fname = FILE_PATH + name;
	ofstream f(fname, ios::out | ios::binary);
	if (f.is_open())
		save_to_file(f, gdata);
	else
		cout << "ERROR: Couldn't open file " << name << endl;
	
	f.close();
}

bool savegame::exists()
{
	const string fname = FILE_PATH + SAVE_FILE_DEFAULT_NAME;
	ifstream f(fname, ios::in | ios::binary);
	if (f.is_open())
	{
		f.close();
		return true;
	}
	else
		return false;
}

void savegame::delete_save_file()
{
	const string fname = FILE_PATH + SAVE_FILE_DEFAULT_NAME;
	remove(fname.c_str());
}
