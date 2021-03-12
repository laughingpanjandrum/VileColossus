#include "savegame.h"



//	Turns a single int into 2 parts, each <256.
intpair savegame::decompose_int(const int v)
{
	auto v1 = (v & 0b1111111100000000) >> 8;
	auto v2 = (v & 0b0000000011111111);
	return intpair(v1, v2);
}

//	Re-composes two such decomposed ints.
int savegame::compose_intpair(const int v1, const int v2)
{
	auto v = (v1 << 8);
	v += v2;
	return v;
}


const string savegame::get_int_as_pair(const int v)
{
	string vstr = "";
	const intpair vpair = decompose_int(v);
	vstr += (char)vpair.first;
	vstr += (char)vpair.second;
	return vstr;
}

//	Breaks an int into two parts and pushes each part sequentially to the given filestream.
void savegame::push_int_as_pair(ofstream* f, const int v)
{
	const intpair vpair = decompose_int(v);
	*f << (char)vpair.first;
	*f << (char)vpair.second;
}


//	Re-composes the next two ints from the given filestream and returns them.
int savegame::load_composite_int(ifstream* f)
{
	auto c1 = f->get();
	auto c2 = f->get();
	return compose_intpair(c1, c2);
}



/*
SAVE GAME FORMAT:

	-> PLAYER DATA
		Level
		Perk Level
		Attribute values
		Perk values*

	-> METADATA
		xp
		attribute points
		perk points
		town portal charge
		known enchants

	-> GAME PROGRESS
		Rotking Kills
		Hellboss Kills

	-> EQUIPMENT
		Equipment
		Current Flask
		Alt Items

	-> ITEMS
		Inventory
		Stash
		Material Stash
		Gem Stash

*/
void savegame::load_from_file(ifstream& f, gamedataPtr gdata)
{
	cout << "LOADING FILE..." << endl;
	auto p = gdata->_player;
	int dlen, flag;


	//		PLAYER DATA			//
	cout << " Loading player data..." << endl;
	p->_level = f.get();
	//p->_PerkLevel = getch_safe(f);
	//p->setAttributeValue(ATTR_DEXTERITY, getch_safe(f));
	//p->setAttributeValue(ATTR_STRENGTH, getch_safe(f));
	//p->setAttributeValue(ATTR_WILLPOWER, getch_safe(f));


	////		METADATA			//
	//auto v1 = getch_safe(f);
	//auto v2 = getch_safe(f);
	//gdata->_xp = compose_intpair(v1, v2);
	//gdata->_attributePointsLeft = getch_safe(f);
	//gdata->_perkPoints = getch_safe(f);

	//v1 = getch_safe(f);
	//v2 = getch_safe(f);
	//gdata->_townPortalCharge = compose_intpair(v1, v2);


	////	known enchants
	//cout << " Loading enchants list..." << endl;
	//gdata->_knownEnchants.clear();
	//dlen = getch_safe(f);
	//for (unsigned i = 0; i < dlen; i++)
	//{
	//	auto v = getch_safe(f);
	//	gdata->_knownEnchants.push_back(static_cast<ItemEnchantment>(v));
	//}

	//
	////		GAME PROGRESS		//
	//cout << " Loading game progress flags..." << endl;
	//gdata->_gameProgress._killedRotking = getch_safe(f);
	//gdata->_gameProgress._killedHellboss = getch_safe(f);


	////		EQUIPMENT			//
	//cout << " Loading equipped items..." << endl;
	//for (unsigned i = 0; i < SLOT__NONE; i++)
	//{
	//	flag = getch_safe(f);
	//	if (flag != 0)
	//	{
	//		auto it = load_equipment_item(f);
	//		gdata->_player->equipInSlot(it, static_cast<EquipmentSlot>(i));
	//	}
	//}

	////	Flask
	//cout << " Loading current flask..." << endl;
	//getch_safe(f);
	//p->_currentFlask = load_equipment_item(f);

	////	Alt items
	//cout << " Loading alt items... " << endl;
	//flag = getch_safe(f);
	//if (flag == 1)
	//	p->_secondaryMainHand = load_equipment_item(f);
	//flag = getch_safe(f);
	//if (flag == 1)
	//	p->_secondaryOffhand = load_equipment_item(f);



	////		STASHED ITEMS		//
	//cout << " Loading inventory items..." << endl;
	//loadItemList(&gdata->_carriedItems, f);
	//loadItemList(&gdata->_stashItems, f);
	//loadItemList(&gdata->_stashedMaterials, f);
	//loadItemList(&gdata->_stashedGems, f);
		
		
	cout << "END OF FILE" << endl;
}


//	current test - just generates all the items saved in the test file
void savegame::load_from_file(const string name, gamedataPtr gdata)
{
	auto f = ifstream(FILE_PATH + name);
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

int savegame::read_int(ifstream& f)
{
	int dlen;
	f.read(reinterpret_cast<char*>(&dlen), sizeof(size_t));
	return dlen;
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

	size_t sz;
	f.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
	for (unsigned i = 0; i < sz; i++)
	{
		auto e = static_cast<ItemEnchantment>(read_int(f));
		auto v = read_int(f);
		it->addEnchantment(e, v);
	}

	f.read(reinterpret_cast<char*>(&sz), sizeof(size_t));
	for (unsigned i = 0; i < sz; i++)
	{
		auto t = static_cast<GemType>(read_int(f));
		auto l = read_int(f);
		it->_socketSlots.push_back(t);
		it->_socketLevels.push_back(l);
	}
	
	return it;
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


//	Serializes an integer to file.
void savegame::serialize_int(ofstream& f, const int i)
{
	f.write((const char*)&i, sizeof(int));
}

void savegame::serialize_size_t(ofstream& f, const size_t sz)
{
	f.write((const char*)&sz, sizeof(size_t));
}


//	Push an entire item to file.
void savegame::serialize_to_file(ofstream& f, const itemPtr it)
{
	int sz;

	serialize_string(f, it->_name);
	serialize_string(f, it->_nickname);

	serialize_int(f, it->_amountLeft);
	serialize_int(f, (int)it->_armourCategory);
	serialize_int(f, it->_category);
	serialize_int(f, it->_chargeRegeneration);
	serialize_int(f, it->_chargesLeft);
	serialize_int(f, (int)it->_containsSpell);
	serialize_int(f, it->_damageTaken);
	serialize_int(f, it->_enhancementLevel);
	serialize_int(f, (int)it->_gemType);
	serialize_int(f, it->_isNewItem);
	serialize_int(f, it->_isTwoHanded);
	serialize_int(f, it->_markedAsValuable);
	serialize_int(f, (int)it->_material);
	serialize_int(f, it->_maxDurability);
	serialize_int(f, it->_rarity);
	serialize_int(f, it->_spellLevel);
	serialize_int(f, it->_tier);

	for (unsigned i = 0; i < PROP__NONE; i++)
		serialize_int(f, it->_Property[i]);
	
	serialize_size_t(f, it->_Enchants.size());
	for (unsigned i = 0; i < it->_Enchants.size(); i++)
	{
		serialize_int(f, it->_Enchants[i]);
		serialize_int(f, it->_EnchantLevels[i]);
	}

	serialize_size_t(f, it->_socketSlots.size());
	for (unsigned i = 0; i < it->_socketSlots.size(); i++)
	{
		serialize_int(f, (int)it->_socketSlots[i]);
		serialize_int(f, it->_socketLevels[i]);
	}
}


/*
SAVE GAME FORMAT:
	
	-> PLAYER DATA
		Level
		Perk Level
		Attribute values
		Perk values*

	-> METADATA
		xp
		attribute points
		perk points
		town portal charge
		known enchants

	-> GAME PROGRESS
		Rotking Kills
		Hellboss Kills

	-> EQUIPMENT
		Equipment
		Current Flask
		Alt Items

	-> ITEMS
		Inventory
		Stash
		Material Stash
		Gem Stash

*/
void savegame::save_to_file(ofstream& f, gamedataPtr gdata)
{
	auto p = gdata->_player;

	for (auto it : p->getAllEquippedItems())
	{
		if (it == nullptr)
			f.put(0);
		else
		{
			f.put(1);
			serialize_to_file(f, it);
		}
	}

	//		PLAYER DATA		//
	//*f << (char)p->_level;
	//*f << p->_PerkLevel;
	//*f << p->getBaseAttribute(ATTR_DEXTERITY);
	//*f << p->getBaseAttribute(ATTR_STRENGTH);
	//*f << p->getBaseAttribute(ATTR_WILLPOWER);



	////		METADATA		//
	//*f << gdata->_xp;
	//*f << gdata->_attributePointsLeft;
	//*f << gdata->_perkPoints;
	//*f << gdata->_townPortalCharge;

	/*
	//	list of known enchants
	*f << (char)gdata->_knownEnchants.size();
	for (unsigned i = 0; i < gdata->_knownEnchants.size(); i++)
		*f << (char)gdata->_knownEnchants[i];


	//		GAME PROGRESS	//
	*f << (char)gdata->_gameProgress._killedRotking;
	*f << (char)gdata->_gameProgress._killedHellboss;



	//		EQUIPMENT		//

	//	Currently equipped
	for (auto it : gdata->_player->getAllEquippedItems())
	{
		//	Test for existence of item in this slot. If none, push a 0; otherwise, push a 1.
		if (it == nullptr)
			*f << (char)0;
		else
		{
			*f << (char)1;
			*f << serialize_item(it);
		}
	}

	//	Flask
	*f << char(1);
	*f << serialize_item(p->_currentFlask);

	//	Alt items
	auto alts = { p->_secondaryMainHand, p->_secondaryOffhand };
	for (auto it : alts)
	{
		if (it == nullptr)
			*f << (char)0;
		else
		{
			*f << (char)1;
			*f << serialize_item(it);
		}
	}


	//		STASHED ITEMS	//
	
	*f << (char)gdata->_carriedItems.size();
	for (auto it : gdata->_carriedItems)
		*f << serialize_item(it);

	*f << (char)gdata->_stashItems.size();
	for (auto it : gdata->_stashItems)
		*f << serialize_item(it);

	*f << (char)gdata->_stashedMaterials.size();
	for (auto it : gdata->_stashedMaterials)
		*f << serialize_item(it);

	*f << (char)gdata->_stashedGems.size();
	for (auto it : gdata->_stashedGems)
		*f << serialize_item(it);
	
	*/


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
