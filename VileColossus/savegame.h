#pragma once


#include <iostream>
#include <fstream>

#include "gamedata.h"
#include "messages.h"



namespace savegame
{

	const string FILE_PATH = "data/saves/";


	//	Decompose an int into two parts, neither of which is greater than 255.
	intpair decompose_int(const int v);
	int compose_intpair(const int v1, const int v2);


	//	Loading

	itemPtr load_equipment_item(ifstream* f);
	void loadItemList(vector<itemPtr>* ilist, ifstream* f);

	void load_from_file(ifstream* f, gamedataPtr gdata);
	void load_from_file(const string name, gamedataPtr gdata);



	//	Saving

	const string serialize_item(const itemPtr it);

	void save_to_file(ofstream* f, gamedataPtr gdata);
	void save_to_file(const string name, gamedataPtr gdata);

}