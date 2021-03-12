#pragma once


#include <iostream>
#include <fstream>

#include "gamedata.h"
#include "messages.h"



namespace savegame
{

	const string FILE_PATH = "data/saves/";
	const string SAVE_FILE_DEFAULT_NAME = "save";


	//	Decompose an int into two parts, neither of which is greater than 255.
	intpair decompose_int(const int v);
	int compose_intpair(const int v1, const int v2);

	const string get_int_as_pair(const int v);
	void push_int_as_pair(ofstream* f, const int v);
	int load_composite_int(ifstream* f);


	//	Loading

	void load_from_file(ifstream& f, gamedataPtr gdata);
	void load_from_file(const string name, gamedataPtr gdata);


	void read_into_string(ifstream& f, string* s, const unsigned len);
	void read_into_string(ifstream& f, string* s);

	int read_int(ifstream& f);

	itemPtr deserialize_item(ifstream& f);

	void test_load(gamedataPtr gdata);



	//	Serializers

	void serialize_string(ofstream& f, const string s);
	void serialize_int(ofstream& f, const int i);
	void serialize_size_t(ofstream& f, const size_t sz);

	void serialize_to_file(ofstream& f, const itemPtr it);


	//	Saving

	void save_to_file(ofstream& f, gamedataPtr gdata);
	void save_to_file(const string name, gamedataPtr gdata);

}