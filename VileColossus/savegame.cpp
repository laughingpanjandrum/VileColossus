#include "savegame.h"




//	Serialize and write to file.
void savegame::save_to_file(ofstream* f, gamedataPtr gdata)
{
	for (auto it : gdata->_player->getAllEquippedItems())
	{
		if (it != nullptr)
			*f << it->serialize() + ',';
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
