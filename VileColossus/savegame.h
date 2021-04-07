#pragma once


#include <iostream>
#include <fstream>

#include "gamedata.h"
#include "messages.h"



namespace savegame
{

	const string FILE_PATH = "data/saves/";
	const string SAVE_FILE_DEFAULT_NAME = "save";


	//	Loading


	void read_into_string(ifstream& f, string* s, const unsigned len);
	void read_into_string(ifstream& f, string* s);

	uint32_t read_int(ifstream& f);
	size_t read_size_t(ifstream& f);
	bool read_bool(ifstream& f);

	itemPtr deserialize_item(ifstream& f);
	void read_item_list(ifstream& f, vector<itemPtr>* ilist);

	void load_from_file(ifstream& f, gamedataPtr gdata);
	void load_from_file(const string name, gamedataPtr gdata);

	void test_load(gamedataPtr gdata);



	//	Serializers

	void serialize_string(ofstream& f, const string s);
	void serialize_bool(ofstream& f, const bool b);
	void serialize_int(ofstream& f, const uint32_t i);
	void serialize_size_t(ofstream& f, const size_t sz);

	void serialize_item(ofstream& f, const itemPtr it);


	//	Saving

	void save_to_file(ofstream& f, gamedataPtr gdata);
	void save_to_file(const string name, gamedataPtr gdata);


	//	Testing for existence of save file

	bool exists();


	//	Deleting upon a permadeath experience

	void delete_save_file();

}