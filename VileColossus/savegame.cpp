#include "savegame.h"



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


	//	Dynamic properties.
	for (unsigned i = 0; i < PROP__NONE; i++)
		it->setProperty(static_cast<ItemProperty>(i), f->get());


	//	Enchants.
	dlen = f->get();
	for (unsigned i = 0; i < dlen; i++)
		it->addEnchantment(static_cast<ItemEnchantment>(f->get()), f->get());


	return it;
}


void savegame::load_from_file(ifstream* f, gamedataPtr gdata)
{
	cout << "LOADING FILE..." << endl;
	while (!f->eof())
	{
		//	is there an item here?
		if (f->get() == 1)
		{
			cout << "  Loading item... ";
			auto it = load_equipment_item(f);
			cout << "Loaded item named " << it->getName() << endl;
			gdata->_map->addItem(it, gdata->_player->_pos);
		}
		else
			cout << "  Empty equipment slot.";
	}
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


//	Serialize and write to file.
void savegame::save_to_file(ofstream* f, gamedataPtr gdata)
{
	for (auto it : gdata->_player->getAllEquippedItems())
	{
		//	Test for existence of item in this slot. If none, push a 0; otherwise, push a 1.
		if (it == nullptr)
			*f << (char)0;
		else
		{
			*f << (char)1;
			*f << it->serialize();
		}
	}
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
