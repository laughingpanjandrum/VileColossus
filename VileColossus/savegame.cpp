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

//	Generates an item from the filestream. Stops at the end of the item's data.
itemPtr savegame::load_equipment_item(ifstream* f)
{
	auto it = itemPtr(new item());

	//	Item name.
	string dstr = "";
	int dlen = f->get();
	for (unsigned i = 0; i < dlen; i++)
		dstr += f->get();
	it->_name = dstr;

	//	Item nickname.
	dlen = f->get();
	dstr = "";
	for (unsigned i = 0; i < dlen; i++)
		dstr += f->get();
	it->_nickname = dstr;


	//	Static properties.
	it->_amountLeft = f->get();
	it->_armourCategory = static_cast<ArmourCategory>(f->get());
	it->_category = static_cast<ItemCategory>(f->get());
	it->_chargesLeft = f->get();
	it->_chargeRegeneration = f->get();
	it->_containsSpell = static_cast<Spell>(f->get());
	it->_damageTaken = f->get();
	it->_enhancementLevel = f->get();
	it->_gemType = static_cast<GemType>(f->get());
	it->_isNewItem = f->get();
	it->_isTwoHanded = f->get();
	it->_markedAsValuable = f->get();
	it->_material = static_cast<MaterialType>(f->get());
	it->_maxDurability = f->get();
	it->_rarity = f->get();
	it->_spellLevel = f->get();
	it->_tier = f->get();

	//	Dynamic properties; recomposed from a pair of chars.
	for (unsigned i = 0; i < PROP__NONE; i++)
	{
		auto c1 = f->get();
		auto c2 = f->get();
		auto v = compose_intpair(c1, c2) - PROP_VALUE_OFFSET;
		//cout << "Recomposed " << v + PROP_VALUE_OFFSET << " from " << c1 << "," << c2 << endl;
		it->setProperty(static_cast<ItemProperty>(i), v);
	}
	
	//	Enchants.
	dlen = f->get();
	for (unsigned i = 0; i < dlen; i++)
	{
		auto t = f->get();
		auto v = f->get();
		it->addEnchantment(static_cast<ItemEnchantment>(t), v);
	}

	//	Gem slots.
	dlen = f->get();
	for (unsigned i = 0; i < dlen; i++)
	{
		auto t = f->get();
		auto v = f->get();
		it->_socketSlots.push_back(static_cast<GemType>(t));
		it->_socketLevels.push_back(v);
	}
	
	return it;
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
void savegame::load_from_file(ifstream* f, gamedataPtr gdata)
{
	cout << "LOADING FILE..." << endl;
	auto p = gdata->_player;
	int dlen, flag;


	//		PLAYER DATA			//
	p->_level = f->get();
	p->_PerkLevel = f->get();
	p->setAttributeValue(ATTR_DEXTERITY, f->get());
	p->setAttributeValue(ATTR_STRENGTH, f->get());
	p->setAttributeValue(ATTR_WILLPOWER, f->get());


	//		METADATA			//
	auto v1 = f->get();
	auto v2 = f->get();
	gdata->_xp = compose_intpair(v1, v2);
	gdata->_attributePointsLeft = f->get();
	gdata->_perkPoints = f->get();
	gdata->_townPortalCharge = f->get();


	//	known enchants
	gdata->_knownEnchants.clear();
	dlen = f->get();
	for (unsigned i = 0; i < dlen; i++)
	{
		auto v = f->get();
		gdata->_knownEnchants.push_back(static_cast<ItemEnchantment>(v));
	}

	
	//		GAME PROGRESS		//
	gdata->_gameProgress._killedRotking = f->get();
	gdata->_gameProgress._killedHellboss = f->get();


	//		EQUIPMENT
	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		flag = f->get();
		if (flag != 0)
		{
			auto it = load_equipment_item(f);
			gdata->_player->equipInSlot(it, static_cast<EquipmentSlot>(i));
		}
	}

	//	Flask
	f->get();
	p->_currentFlask = load_equipment_item(f);

	//	Alt items
	flag = f->get();
	if (flag == 1)
		p->_secondaryMainHand = load_equipment_item(f);
	flag = f->get();
	if (flag == 1)
		p->_secondaryOffhand = load_equipment_item(f);
		
		
	cout << "END OF FILE" << endl;
}


//	current test - just generates all the items saved in the test file
void savegame::load_from_file(const string name, gamedataPtr gdata)
{
	auto f = ifstream(FILE_PATH + name);
	if (f.is_open())
		load_from_file(&f, gdata);
	else
		cout << "ERROR: Couldn't open file " << name << endl;
}


//	Returns a string representation of an item.
const string savegame::serialize_item(const itemPtr it)
{
	/*
	vector<GemType> _socketSlots;
	vector<int> _socketLevels;
	*/

	//	name, in quotes
	string t = char(it->_name.size()) + it->_name;
	t += char(it->_nickname.size()) + it->_nickname;

	//	individual int properties
	t += (char)it->_amountLeft;
	t += (char)it->_armourCategory;
	t += (char)it->_category;
	t += (char)it->_chargesLeft;
	t += (char)it->_chargeRegeneration;
	t += (char)it->_containsSpell;
	t += (char)it->_damageTaken;
	t += (char)it->_enhancementLevel;
	t += (char)it->_gemType;
	t += (char)it->_isNewItem;
	t += (char)it->_isTwoHanded;
	t += (char)it->_markedAsValuable;
	t += (char)it->_material;
	t += (char)it->_maxDurability;
	t += (char)it->_rarity;
	t += (char)it->_spellLevel;
	t += (char)it->_tier;

	//	list of properties (constant size)
	for (unsigned i = 0; i < PROP__NONE; i++)
	{
		auto v = it->_Property[i] + PROP_VALUE_OFFSET;
		auto p = decompose_int(v);
		t += (char)(p.first);
		t += (char)(p.second);
		/*cout << "Decomposed " << v << " into " << p.first << "," << p.second;
		cout << "; Recomposed = " << compose_intpair(p.first, p.second) << endl;*/
	}

	//	list of enchants, preceded by no. of enchants
	t += char(it->_Enchants.size());
	for (unsigned i = 0; i < it->_Enchants.size(); i++)
	{
		t += (char)it->_Enchants[i];
		t += (char)it->_EnchantLevels[i];
	}

	//	list of gem types / gem levels
	t += (char)(it->_socketSlots.size());
	for (unsigned i = 0; i < it->_socketSlots.size(); i++)
	{
		t += (char)it->_socketSlots[i];
		t += (char)it->_socketLevels[i];
	}

	return t;
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
void savegame::save_to_file(ofstream* f, gamedataPtr gdata)
{
	auto p = gdata->_player;


	//		PLAYER DATA		//
	*f << (char)p->_level;
	*f << (char)p->_PerkLevel;
	*f << (char)p->getBaseAttribute(ATTR_DEXTERITY);
	*f << (char)p->getBaseAttribute(ATTR_STRENGTH);
	*f << (char)p->getBaseAttribute(ATTR_WILLPOWER);



	//		METADATA		//
	auto vpair = decompose_int(gdata->_xp);
	*f << (char)vpair.first;
	*f << (char)vpair.second;
	*f << (char)gdata->_attributePointsLeft;
	*f << (char)gdata->_perkPoints;
	*f << (char)gdata->_townPortalCharge;

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


	//	Finished.
	messages::add(gdata, "Saved game.");
}



//	Given a name.
void savegame::save_to_file(const string name, gamedataPtr gdata)
{
	auto f = ofstream(FILE_PATH + name);
	if (f.is_open())
		save_to_file(&f, gdata);
	else
		cout << "ERROR: Couldn't open file " << name << endl;
}
