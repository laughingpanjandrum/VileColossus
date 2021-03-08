#pragma once


#include <iostream>
#include <fstream>

#include "gamedata.h"
#include "messages.h"



namespace savegame
{

	const string FILE_PATH = "data/saves/";



	//	Loading

	itemPtr load_equipment_item(ifstream* f);

	void load_from_file(ifstream* f, gamedataPtr gdata);
	void load_from_file(const string name, gamedataPtr gdata);



	//	Saving

	void save_to_file(ofstream* f, gamedataPtr gdata);
	void save_to_file(const string name, gamedataPtr gdata);

}