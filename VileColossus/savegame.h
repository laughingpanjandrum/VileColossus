#pragma once


#include <iostream>
#include <fstream>

#include "gamedata.h"
#include "messages.h"



namespace savegame
{

	const string FILE_PATH = "data/saves/";


	//	Saving

	void save_to_file(ofstream* f, gamedataPtr gdata);
	void save_to_file(const string name, gamedataPtr gdata);

}