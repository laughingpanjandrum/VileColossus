#pragma once

#include "inputHandler.h"
#include "animation.h"
#include "color_constants.h"
#include "manageItems.h"
#include "window.h"
#include "gamedata.h"
#include "savegame.h"
#include "visibility.h"


#define MSG_COLOR_CHANGE_MARK '#'
#define MSG_DEFAULT_COLOR_MARK '@'


#define BRIGHTEN_AMOUNT 1.4f


#define MAP_X_SIZE 45
#define MAP_Y_SIZE 45
#define MAP_X_OFFSET 3
#define MAP_Y_OFFSET 3




const vector<string> HELP_SCREEN_CATEGORIES = {
	"OVERVIEW",
	"FLASKS",
	"BASIC COMBAT STATS",
	"SPECIAL COMBAT STATS",
	"SPECIAL DAMAGE TYPES",
	"MAGIC",
	"EXPERIENCE AND LEVELS",
	"ENHANCING EQUIPMENT",
};



class display
{
public:

	display();
	~display() {}


	void title();
	void drawModeSelection();

	void toggleFullscreen() { _win.toggle_fullscreen(); }

	void drawControls();
	void drawHelpScreen(gamedataPtr gdata);


	//	Character information

	void drawCharacterSheet(gamedataPtr gdata);
	void drawCharacterSummary(gamedataPtr gdata);

	void drawMonsterSummary(gamedataPtr gdata, monsterPtr mon);

	void drawAttributePoints(gamedataPtr gdata);


	//	Item information

	void drawItemPickup(gamedataPtr gdata);

	void drawEquipment(gamedataPtr gdata);
	void drawInventory(gamedataPtr gdata);

	void drawItemInfo(gamedataPtr gdata, itemPtr it, int atx, int aty, itemPtr compareTo = nullptr);
	void drawItemInfoDetailed(gamedataPtr gdata, itemPtr it);

	void drawGemTypeEffects(gamedataPtr gdata, const GemType gem, const int tier, int atx, int aty, const ItemCategory highlightCategory = ITEM__MAX);
	int drawMaterialDescription(const MaterialType mat, int atx, int aty);
	
	void drawInventoryCapacity(gamedataPtr gdata, int x, int y);
	int drawItemEnchantments(gamedataPtr gdata, itemPtr it, int atx, int aty);

	void drawStashedGemstones(gamedataPtr gdata, int x, int y);
	void drawStashedMaterials(gamedataPtr gdata, int x, int y);


	//	Crafting/stashing/etc

	void drawAnvil(gamedataPtr gdata);
	
	void drawEnchantedForge(gamedataPtr gdata);
	void drawEnchantmentSelect(gamedataPtr gdata);
	
	void drawAlchemy(gamedataPtr gdata);

	void drawDemonforge(gamedataPtr gdata);
	
	void drawGemstonePress(gamedataPtr gdata);
	void drawGemstoneSelect(gamedataPtr gdata);
	void drawGemstoneFabricator(gamedataPtr gdata);

	void drawRuneImprinter(gamedataPtr gdata);

	void drawStash(gamedataPtr gdata);


	//	MAGIC

	void drawSpellList(gamedataPtr gdata);
	void drawSpellInfo(gamedataPtr gdata, const Spell sp, const int lvl, int atx, int aty);


	//	Map drawing & the main game interface

	void drawMonsterHighlights(gamedataPtr gdata);

	void updateVisibleMapData(gamedataPtr gdata);
	void drawMainInterface(gamedataPtr gdata);

	void drawMessages(gamedataPtr gdata);
	void drawSidebar(gamedataPtr gdata);


	//	Rituals (abyssal portals to the final areas)

	void drawRitualAltar(gamedataPtr gdata);


	//	Coordinate conversion

	bool isPointOnDisplay(const intpair pt);
	intpair mapToDisplayCoord(const intpair ctr, int x, int y);
	intpair displayToMapCoord(const intpair ctr, int x, int y);


	//	Special colours

	colorType getPercentColor(int per) const;


	//	Animations

	void tickAnimations(gamedataPtr gdata);


	//	Utility

	void drawBox(const int x, const int y, const int w, const int h, colorType col);
	void drawInterfaceBoxes();

	void writeFormatted(int atx, int aty, string txt, vector<colorType> colors, float darkenFactor = 0.0f);

	void drawStatWithBox(int x, int y, string val, string title, colorType statcol);
	void drawDottedLinePair(int x, int y, int x2, const string title, const string val, colorType val_col = COLOR_LIGHT, colorType title_col = COLOR_MEDIUM);
	void drawProgressDots(int x, int y, int filled, int maxval, colorType col);
	void drawFilledBar(int x, int y, int filled, int maxval, colorType col, string text);

	void drawLineHorizontal(int x, int y, int width, colorType color);
	void drawLineVertical(int x, int y, int height, colorType color);

	void clear() { _win.clear(); }
	void refresh() { _win.refresh(); }
	void waitForKeypress() { _win.waitForKeypress(); }

	string getstr(int x, int y, string prompt) { return _win.getstr(x, y, prompt); }


private:

	window _win;

	//	Baked visible map data
	vector<vector<int>> _visibleGlyphs;
	vector<vector<colorType>> _visibleColors;
	vector<vector<colorType>> _visibleBgcolors;

	TCODColor _percentColors[101];


	//	Title screen
	const vector<string>* _title;


	//	Helpers
	int get_hit_chance(int acc, int def);
	int calculate_average_damage(const creaturePtr attacker, const creaturePtr target);

	bool hide_item_property(const ItemProperty prop) const;

};

