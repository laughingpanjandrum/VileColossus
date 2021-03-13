#pragma once

#include "animation.h"
#include "gridmap.h"
#include "mapgen.h"
#include "player.h"


#define MESSAGE_COUNT_MAX 14



enum class GameMode { CASUAL, NORMAL, PERMADEATH, };


enum GameState
{
	STATE_TITLE, STATE_SELECT_MODE,
	STATE_NORMAL,
	STATE_SAVE_GAME,
	STATE_CHARACTER_SHEET,
	STATE_LOOK, STATE_VIEW_ITEM_DETAILS,
	STATE_VIEW_INVENTORY, STATE_VIEW_EQUIPMENT, 
	STATE_VIEW_STASH, STATE_VIEW_INVENTORY_IN_STASH,
	STATE_ANVIL, STATE_FORGE, STATE_SELECT_ENCHANTMENT, STATE_ALCHEMY, STATE_DEMONFORGE,
	STATE_GEMSTONE_PRESS, STATE_SELECT_GEM, STATE_FABRICATE_GEMS,
	STATE_PICKING_UP_ITEMS,
	STATE_SPELL_LIST, STATE_RUNE_ENHANCER,
	STATE_AIMING_SPELL, STATE_LEARN_SPELL,
	STATE_SPEND_ATTRIBUTE_POINTS,
	STATE_ACKNOWLEDGE_DEATH,
	STATE_HIGHLIGHT_ENEMIES, STATE_HIGHLIGHT_MAPTILES,
};

struct message
{
	message() {}
	message(string txt, vector<colorType> colors) : _txt(txt), _colors(colors) {}
	string _txt;
	vector<colorType> _colors;
};


struct gamedata
{
	//	Basic data
	playerPtr _player;
	gridmapPtr _map;
	gridmapPtr _homeBase;
	gridmapPtr _helltemple;

	//	State
	GameState _state;
	int _idx;
	EquipmentSlot _selectedSlot;
	int _aimingSpellIdx;

	//	Map metadata
	int _depth;

	//	Progression
	int _xp = 0;
	int _attributePointsLeft = 0;
	int _perkPoints = 0;
	int _townPortalCharge = 0;

	//	Inventory
	vector<itemPtr> _carriedItems;
	vector<itemPtr> _currentItemList;
	vector<itemPtr> _stashItems;
	itemPtr _viewingItem;

	//	Other stashed stuff
	vector<ItemEnchantment> _knownEnchants;
	vector<itemPtr> _stashedMaterials;
	vector<itemPtr> _stashedGems;

	//	Messages/animations
	vector<message> _messages;
	vector<animationPtr> _animations;

	//	Auto-movement
	intpair _automoveVec;
	int _automoveDist;

	//	Cursor
	vector<creaturePtr> _visibleMonsters;
	intpair _cursorPt;

	//	Game progress
	int _demonforgeTier = 1;
	mapgen::game_progress _gameProgress;

	//	Game mode
	GameMode _mode;

	//	Debug flags
	bool _omniscient = false;
	bool _invincible = false;
	bool _autokill = false;
};
typedef shared_ptr<gamedata> gamedataPtr;


inline void addAnimation(gamedataPtr gdata, animationPtr anim)
{
	gdata->_animations.push_back(anim);
}