#include "display.h"



display::display()
{
	//	Initialize empty arrays of visible map tiles
	for (unsigned int x = 0; x < MAP_X_SIZE; x++)
	{
		vector<int> xglyphs;
		vector<colorType> xcols;
		vector<colorType> xbgcols;
		for (unsigned int y = 0; y < MAP_Y_SIZE; y++)
		{
			xglyphs.push_back(0);
			xcols.push_back(COLOR_BLACK);
			xbgcols.push_back(COLOR_BLACK);
		}
		_visibleGlyphs.push_back(xglyphs);
		_visibleColors.push_back(xcols);
		_visibleBgcolors.push_back(xbgcols);
	}

	//	percentile colour map
	int idx[] = { 0, 50, 100 };
	TCODColor col[] = { TCODColor::darkRed, TCODColor::darkYellow, TCODColor::darkGreen };
	TCODColor::genMap(_percentColors, 3, col, idx);

	//	Title screen
	_title = new vector<string>({
		"...######################################################......",
		"...............................................................",
		"...............................................................",
		"..####..x........#########################################.....",
		"........xx..x..................................................",
		"........xx..x..................................................",
		"#####...xx..x..........####################################....",
		"........xx..x..................................................",
		"........xx..x..x.x..xx.........................................",
		".#####..xx..x..x.x..x....###################################...",
		"........xx..x..x.x..x..........................................",
		"........xx..x..x.x..xx.........................................",
		"...####.xx..x..x.x..x.....###################################..",
		"........xx..x..x.xx.xx.........................................",
		"........xx.xx..................................................",
		".....##.xxxxx..............###################################.",
		".........xxx......xxxx.........................................",
		"..........x.....xxxx...........................................",
		"......##.......xxx............................#################",
		"...............xx....xxx.x..xxx.xx.xx.x.x.xx...................",
		"...............xx....x.x.x..x.x.x..x..x.x.x....................",
		"........####...xx....x.x.x..x.x.x..x..x.x.x....###############.",
		"...............xx....x.x.x..x.x.xx.xx.x.x.xx...................",
		"...............xx....x.x.x..x.x..x..x.x.x..x...................",
		".........####..xx....xxx.xx.xxx.xx.xx.xxx.xx....#############..",
		"...............xx..............................................",
		"...............xxx.............................................",
		"............##..xxxx.......#################################...",
		"..................xxxx.........................................",
		"...............................................................",
		"...........###############################################.....",
	});
}


//	Title card.
void display::title()
{
	for (unsigned x = 0; x < _title->size(); x++)
	{
		for (unsigned y = 0; y < _title->at(x).size(); y++)
		{
			int gl = 177;
			auto col = COLOR_BLACK;
			switch ((*_title)[x][y])
			{
			case('#'):	col = TCODColor::darkRed; break;
			case('x'):	col = TCODColor::red; break;
			case('.'):	gl = ' '; break;
			}
			
			float f = 1.3f * (float)(x + y) / (float)(_title->size() + _title->at(x).size());
			col.scaleHSV(1.0f, f);

			_win.writec(y + 10, x + 10, gl, col);
		}
	}

	writeFormatted(24, 46, "#1 @START A NEW GAME", { COLOR_LIGHT });
	writeFormatted(25, 48, "#2 @LOAD A SAVED GAME", { COLOR_LIGHT });
	
	writeFormatted(60, 63, "#ctrl-f @toggles fullscreen", { COLOR_LIGHT });
}


//	Pick a game mode.
void display::drawModeSelection()
{
	_win.write(5, 5, "SELECT A GAME MODE", COLOR_LIGHT);
	drawLineHorizontal(4, 6, 20, COLOR_LIGHT);

	drawBox(8, 8, 70, 7, COLOR_DARK);
	writeFormatted(10, 10, "[#1@] #CASUAL", { COLOR_LIGHT, TCODColor::green });
	_win.writeWrapped(13, 11, 60, "On death, you return to the surface; your equipment is damaged and its maximum durability is reduced, but you keep all your items.", COLOR_MEDIUM);

	drawBox(8, 18, 70, 7, COLOR_DARK);
	writeFormatted(10, 20, "[#2@] #NORMAL", { COLOR_LIGHT, COLOR_LIGHT });
	_win.writeWrapped(13, 21, 60, "On death, you return to the surface; your equipment is damaged and its maximum durability is reduced, and all non-equipped items in your inventory are lost.", COLOR_MEDIUM);

	drawBox(8, 28, 70, 6, COLOR_DARK);
	writeFormatted(10, 30, "[#3@] #PERMADEATH", { COLOR_LIGHT, TCODColor::red });
	_win.writeWrapped(13, 31, 60, "On death, the game ends and your character is lost forever.", COLOR_MEDIUM);
}


//	All game controls.
void display::drawControls()
{
	_win.write(3, 3, "MOVEMENT", COLOR_WHITE);

	_win.write(4, 5, "numpad", COLOR_MEDIUM);
	_win.write(4, 6, "7 8 9", COLOR_LIGHT);
	_win.write(4, 7, "4   6", COLOR_LIGHT);
	_win.write(4, 8, "1 2 3", COLOR_MEDIUM);

	_win.write(15, 5, "vi-keys", COLOR_MEDIUM);
	_win.write(15, 6, "y k u", COLOR_LIGHT);
	_win.write(15, 7, "h   l", COLOR_LIGHT);
	_win.write(15, 8, "b j n", COLOR_LIGHT);

	_win.write(26, 6, "\\ | /", COLOR_LIGHT);
	_win.write(26, 7, "-   -", COLOR_LIGHT);
	_win.write(26, 8, "/ | \\", COLOR_LIGHT);

	writeFormatted(4, 10, "#ctrl+direction @to move several times in a row.", { COLOR_LIGHT });
	writeFormatted(4, 11, "#z or 5 @to wait for one turn.", { COLOR_LIGHT });

	writeFormatted(4, 12, "#Bump into @certain objects to interact with them.", { COLOR_LIGHT });


	_win.write(3, 15, "COMBAT", COLOR_WHITE);

	writeFormatted(4, 17, "#Bump into enemies @to attack in melee.", { COLOR_LIGHT });
	writeFormatted(4, 18, "#shift+direction @performs a ranged attack, if available.", { COLOR_LIGHT });
	writeFormatted(4, 20, "#TAB @to auto-attack an adjacent enemy.", { COLOR_LIGHT });

	writeFormatted(4, 22, "#q @to quaff your equipped flask.", { COLOR_LIGHT });
	writeFormatted(4, 24, "#w @to swap to your secondary weapons.", { COLOR_LIGHT });


	_win.write(3, 27, "MENUS AND ITEMS", COLOR_WHITE);

	writeFormatted(4, 29, "#e @View or change equipment.", { COLOR_LIGHT });
	writeFormatted(4, 30, "#i @View carried items.", { COLOR_LIGHT });
	writeFormatted(4, 31, "#m @View spell information.", { COLOR_LIGHT });
	writeFormatted(4, 32, "#a @View and spend attribute points.", { COLOR_LIGHT });

	writeFormatted(4, 34, "#shift+a @View detailed character sheet.", { COLOR_LIGHT });


	_win.write(3, 37, "MAP INTERACTION", COLOR_WHITE);

	writeFormatted(4, 39, "#g @Get items from the floor.", { COLOR_LIGHT });

	writeFormatted(4, 41, "#SPACE @to use a staircase or portal you're standing on.", { COLOR_LIGHT });
	writeFormatted(4, 42, "#shift+t @to teleport to the surface, if your portal is charged.", { COLOR_LIGHT });


	_win.write(3, 45, "LOOKING AROUND", COLOR_WHITE);

	writeFormatted(4, 47, "#x @Enter look mode.", { COLOR_LIGHT });

	_win.write(4, 49, "While in look mode:", COLOR_MEDIUM);
	writeFormatted(5, 50, "#directional keys @move the cursor around.", { COLOR_LIGHT });
	writeFormatted(5, 51, "#TAB @cycles between visible enemies.", { COLOR_LIGHT });
}


//	Character info
void display::drawCharacterSheet(gamedataPtr gdata)
{
	auto p = gdata->_player;
	int x = 6, y = 4;

	//	BASE ATTRIBUTES

	drawBox(x - 2, ++y, 26, 12, COLOR_DARK);
	_win.write(x, y, "BASE ATTRIBUTES", COLOR_LIGHT);

	y += 2;
	for (unsigned i = 0; i < ATTR__MAX; i++)
	{
		auto attr = static_cast<Attribute>(i);
		int base = p->getBaseAttribute(attr);
		auto val = p->getDerivedAttribute(attr);

		//	base value of the stat
		drawBox(x, y, 3, 2, COLOR_DARK);
		_win.write(x + 1, y + 1, extendInteger(base, 2), COLOR_WHITE);
		_win.write(x + 4, y + 1, getAttributeName(attr), COLOR_MEDIUM);
		y += 3;
	}


	drawBox(x + 18, 7, 3, 2, COLOR_DARK);
	_win.write(x + 19, 8, extendInteger(p->_level, 2), COLOR_LIGHT);
	_win.write(x + 18, 10, "XLVL", COLOR_MEDIUM);

	drawBox(x + 18, 12, 3, 2, COLOR_DARK);
	_win.write(x + 19, 13, extendInteger(p->_PerkLevel, 2), p->_PerkLevel > 0 ? COLOR_LIGHT : COLOR_DARK);
	_win.write(x + 18, 15, "ALVL", COLOR_MEDIUM);


	//	VITAL STATISTICS

	y += 2;
	drawBox(x - 2, ++y, 26, 36, COLOR_DARK);
	_win.write(x, y, "VITAL STATISTICS", COLOR_LIGHT);

	drawStatWithBox(x, y + 2, to_string(p->getMaxHealth()), "Health", COLOR_HEALTH);
	drawStatWithBox(x, y + 5, to_string(p->getMaxMagic()), "Magic", COLOR_MAGIC);
	drawStatWithBox(x, y + 9, to_string(p->getMoveEnergyCost()) + "%", "Move Delay", COLOR_MISC_STAT);
	drawStatWithBox(x, y + 12, to_string(p->getAttackEnergyCost()) + "%", "Attack Delay", COLOR_MISC_STAT);
	drawStatWithBox(x, y + 15, extendInteger(p->getVisionRadius(), 2), "Vision Radius", TCODColor::yellow);
	drawStatWithBox(x, y + 19, plusminus(p->getLeechOnKill()), "Life on Kill", COLOR_HEALTH);
	drawStatWithBox(x, y + 22, plusminus(p->getManaleech()), "Magic on Kill", COLOR_MAGIC);
	drawStatWithBox(x, y + 26, expressIntAsFloat(p->estimateDPS(), 1), "Avg Damage/Turn", TCODColor::darkCrimson);
	drawStatWithBox(x, y + 29, to_string(get_hit_chance(p->getAccuracy(), lootgen::getBaseDefenceForLevel(p->_level))) + "%", "Avg Hit Chance", COLOR_MISC_STAT);
	drawStatWithBox(x, y + 32, to_string(100 - get_hit_chance(lootgen::getBaseAccuracyForLevel(p->_level), p->getDefenceValue())) + "%", "Avg Defend Chance", COLOR_MISC_STAT);


	//	OFFENCE

	x = 33, y = 4;
	drawBox(x - 2, ++y, 26, 50, COLOR_DARK);
	_win.write(x, y, "OFFENSIVE", COLOR_LIGHT);

	int dmgbase = p->getWeaponDamage();
	int dmgvar = p->getDamageVariance();

	drawStatWithBox(x, y + 2, plusminus(p->getAccuracy()), "Accuracy", COLOR_MISC_STAT);
	drawStatWithBox(x, y + 5, to_string(dmgbase), "Weapon Damage", COLOR_HEALTH);
	drawStatWithBox(x, y + 8, extendInteger(p->getCriticalChance(), 2) + "%", "Critical Chance", TCODColor::crimson);
	drawStatWithBox(x, y + 11, plusminus(p->getCriticalMultiplier()) + "%", "Critical Damage", TCODColor::crimson);
	drawStatWithBox(x, y + 14, plusminus(p->getSpellPower()) + "%", "Spell Power", COLOR_MAGIC);

	y += 19;
	for (auto dt : SPECIAL_DAMAGE_TYPES)
	{
		int dam = p->getWeaponDamageOfType(dt);
		int aff = p->getElementalAffinity(dt);

		dam = adjustByPercent(dam, aff);
		auto min_dmg = adjustByPercent(1, aff);
		if (dam > 0)	drawStatWithBox(x, y, to_string(min_dmg) + "-" + to_string(dam), getDamageTypeName(dt) + " Damage", getDamageTypeColor(dt));
		else			drawStatWithBox(x, y, "00", getDamageTypeName(dt) + " Damage", COLOR_DARK);

		//	affinity is already factored in above
		drawStatWithBox(x, y + 3, plusminus(aff) + "%", getDamageTypeName(dt) + " Bonus", (aff > 0) ? getDamageTypeColor(dt) : COLOR_DARK);
		y += 7;
	}


	//	DEFENSIVE
	x = 60, y = 4;
	drawBox(x - 2, ++y, 26, 24, COLOR_DARK);
	_win.write(x, y, "DEFENSIVE", COLOR_LIGHT);
	
	drawStatWithBox(x, y + 2, extendInteger(p->getDefenceValue(), 3), "Defence Value", COLOR_MISC_STAT);
	drawStatWithBox(x, y + 5, extendInteger(p->getArmourValue(), 3), "Armour Value", COLOR_MISC_STAT);

	y += 10;
	for (auto dt : SPECIAL_DAMAGE_TYPES)
	{
		int res = p->getResistance(dt);
		drawStatWithBox(x, y, extendInteger(res, 2) + "%", getDamageTypeName(dt) + " Resist", (res > 0) ? getDamageTypeColor(dt) : COLOR_DARK);
		y += 3;
	}


	//	MISCELLANEOUS
	y += 3;
	drawBox(x - 2, ++y, 26, 24, COLOR_DARK);
	_win.write(x, y, "MISCELLANEOUS", COLOR_LIGHT);
	
	drawStatWithBox(x, y + 2, plusminus(p->getGreedBonus()) + "%", "Fragments Gained", TCODColor::gold);
	drawStatWithBox(x, y + 5, plusminus(p->getReprisalDamage()), "Reprisal Damage", COLOR_HEALTH);
	drawStatWithBox(x, y + 8, to_string(p->getRiposteChance()) + "%", "Riposte Chance", COLOR_LIGHT);
	drawStatWithBox(x, y + 11, plusminus(p->getCleaveDamageBonus()), "Cleave Damage", TCODColor::crimson);
	drawStatWithBox(x, y + 14, to_string(p->getStaggerChance()) + "%", "Stagger Chance", COLOR_MISC_STAT);
	drawStatWithBox(x, y + 17, to_string(p->getWrathOnKillChance()) + "%", "Wrath on Kill", COLOR_LIGHT);
	drawStatWithBox(x, y + 20, plusminus(p->getWrathDamageBonus()) + "%", "Wrath Damage", TCODColor::crimson);


	//	GAME MODE
	_win.write(1, 1, "Game Mode:", COLOR_DARK);
	switch (gdata->_mode)
	{
	case(GameMode::CASUAL):		_win.write(12, 1, "Casual", TCODColor::darkGreen); break;
	case(GameMode::NORMAL):		_win.write(12, 1, "Normal", COLOR_LIGHT); break;
	case(GameMode::PERMADEATH):	_win.write(12, 1, "Permadeath", TCODColor::darkRed); break;
	}
}


//	Quick summary of player stats at the bottom of the screen.
void display::drawCharacterSummary(gamedataPtr gdata)
{
	int x = 4, y = 51;
	drawBox(2, 50, 85, 12, COLOR_DARK);
	auto p = gdata->_player;

	writeFormatted(x, ++y, "[#" + extendInteger(p->getMaxHealth(), 3) + "@] #Health", { COLOR_HEALTH, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getMaxMagic(), 3) + "@] Magic", { COLOR_MAGIC, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getDefenceValue(), 3) + "@] #Defence Value", { COLOR_MISC_STAT, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getArmourValue(), 3) + "@] #Armour Value", { COLOR_MISC_STAT, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getReprisalDamage(), 3) + "@] #Reprisal Damage", { COLOR_HEALTH, COLOR_LIGHT });
	writeFormatted(x, y + 2, "[#" + expressIntAsFloat(p->estimateDPS(), 1) + "@] #Avg Damage/Turn", { COLOR_HEALTH, COLOR_LIGHT });

	x = 30; y = 51;
	writeFormatted(x, ++y, "[#" + extendInteger(p->getAccuracy(), 3) + "@] #Accuracy", { COLOR_MISC_STAT, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getWeaponDamage(), 3) + "@] #Damage", { COLOR_HEALTH, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getCriticalChance(), 3) + "@] #Critical Chance", { TCODColor::lightCrimson, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getCriticalMultiplier(), 3) + "@] #Critical Damage", { TCODColor::crimson, COLOR_LIGHT });
	writeFormatted(x, ++y, "[#" + extendInteger(p->getSpellPower(), 3) + "@] Spell Power", { TCODColor::lightBlue, COLOR_LIGHT });
	writeFormatted(x, y + 2, "[#" + to_string(get_hit_chance(p->getAccuracy(), lootgen::getBaseDefenceForLevel(p->_level))) + "%@] #Avg Hit Chance", { COLOR_MISC_STAT, COLOR_LIGHT });

	x = 56; y = 51;
	for (auto dt : SPECIAL_DAMAGE_TYPES)
	{
		auto res = p->getResistance(dt);
		writeFormatted(x, ++y, "[#" + extendInteger(res, 2) + "%@] #" + getDamageTypeName(dt) + " Resist", { getDamageTypeColor(dt), COLOR_LIGHT });
	}
	writeFormatted(x, y + 3, "[#" + to_string(100 - get_hit_chance(lootgen::getBaseAccuracyForLevel(p->_level), p->getDefenceValue())) + "%@] #Avg Defend Chance", { COLOR_MISC_STAT, COLOR_LIGHT });
}


//	Quick summary of a given monster, in the message box
void display::drawMonsterSummary(gamedataPtr gdata, monsterPtr mon)
{
	int x = 4, y = 53;

	//	name
	_win.writec(x, y, mon->getGlyph(), mon->getColor());
	_win.write(x + 2, y, mon->getName(), mon->getColor());
	writeFormatted(x + 40, y, "Level #" + to_string(mon->_level), { COLOR_LIGHT });
	drawProgressDots(x + 50, y, mon->_tier, 5, TCODColor::gold);
	
	//	flags
	++y;
	string flag_txt = "";
	if (mon->isUndead())					flag_txt += "[Undead] ";
	if (mon->isFlying())					flag_txt += "[Flying] ";
	if (mon->hasFlag("minion"))				flag_txt += "[Minion] ";
	if (mon->hasFlag("berserker"))			flag_txt += "[Berserker] ";
	if (mon->hasFlag("slow"))				flag_txt += "[Slow] ";
	if (mon->hasFlag("teleports"))			flag_txt += "[Teleports] ";
	if (mon->hasFlag("ethereal"))			flag_txt += "[Ethereal] ";
	if (mon->hasFlag("arcane_attack"))		flag_txt += "[Arcane Attack] ";
	if (mon->hasFlag("electric_attack"))	flag_txt += "[Electric Attack] ";
	if (mon->hasFlag("fire_attack"))		flag_txt += "[Fire Attack] ";
	if (mon->hasFlag("poison_attack"))		flag_txt += "[Poison Attack] ";
	if (mon->hasFlag("spawner"))			flag_txt += "[Spawns Monsters] ";
	if (mon->hasFlag("casts_arcane_bolt"))	flag_txt += "[Casts Arcane Bolt] ";
	if (mon->hasFlag("casts_firebolt"))		flag_txt += "[Casts Firebolt] ";
	if (mon->hasFlag("casts_fireblast"))	flag_txt += "[Casts Fireblast] ";
	if (mon->hasFlag("casts_lightning"))	flag_txt += "[Casts Lightning] ";
	if (mon->hasFlag("spit_sludge"))		flag_txt += "[Spits Sludge] ";
	_win.write(x + 2, y, flag_txt, COLOR_DARK);


	//	basic info
	y += 2;
	string txt = "HEALTH #" + to_string(mon->getMaxHealth()) + " @: DEFENCE #" + to_string(mon->getDefenceValue()) + " @: PROT #" + to_string(mon->getArmourValue());
	if (!mon->hasFlag("no_attack"))
		txt += " @: ACCURACY #" + plusminus(mon->getAccuracy()) + " @: DAMAGE #" + to_string(mon->getWeaponDamage());
	writeFormatted(x, y, txt, { COLOR_HEALTH, COLOR_MISC_STAT, COLOR_MISC_STAT, COLOR_MISC_STAT, COLOR_HEALTH, });


	//	chance to hit
	int hit_them = get_hit_chance(gdata->_player->getAccuracy(), mon->getDefenceValue());
	int hit_us = get_hit_chance(mon->getAccuracy(), gdata->_player->getDefenceValue());
	writeFormatted(x, ++y, "#" + to_string(hit_them) + "% @to hit them; #" + to_string(hit_us) + "% @they hit us", { COLOR_HIGHLIGHT_POS, COLOR_HIGHLIGHT_NEG });


	//	relative damage
	int dam_to_us = calculate_average_damage(mon, gdata->_player);
	int dam_to_them = calculate_average_damage(gdata->_player, mon);
	writeFormatted(x, ++y, "#" + to_string(dam_to_them) + " @avg damage to them; #" + to_string(dam_to_us) + " @avg damage to us", { COLOR_HIGHLIGHT_POS, COLOR_HIGHLIGHT_NEG });


	//	resistances
	y += 2;
	for (auto dt : SPECIAL_DAMAGE_TYPES)
	{
		auto res = mon->getResistance(dt);
		string txt;
		if (res >= 100)
			txt = "Immune " + getDamageTypeName(dt);
		else if (res > 0)
			txt = "Resists " + getDamageTypeName(dt);
		if (!txt.empty())
		{
			_win.write(x, y, txt, getDamageTypeColor(dt));
			x += txt.size() + 1;
		}
	}


	//	status effects
	x = 4, y += 1;
	for (unsigned i = 0; i < STATUS__MAX; i++)
	{
		auto st = static_cast<StatusEffect>(i);
		if (mon->hasStatusEffect(st))
		{
			auto name = getStatusEffectName(st);
			_win.write(x, y, name, getStatusEffectColor(st));
			x += name.size() + 1;
		}
	}
}


//	Spend attribute points to improve stats.
void display::drawAttributePoints(gamedataPtr gdata)
{
	drawBox(2, 2, 20, 15, COLOR_DARK);
	drawBox(23, 2, 40, 15, COLOR_DARK);
	writeFormatted(3, 2, "LEVEL #" + to_string(gdata->_player->_level), { COLOR_LIGHT });

	int x = 4, y = 4;
	auto col = (gdata->_attributePointsLeft > 0) ? COLOR_POSITIVE : COLOR_DARK;
	writeFormatted(x, y, "#POINTS LEFT [#" + extendInteger(gdata->_attributePointsLeft, 2) + "#]", { COLOR_LIGHT, col, COLOR_LIGHT, });

	//	list stats
	auto p = gdata->_player;
	y += 2;
	for (unsigned i = 0; i < ATTR__MAX; i++)
	{
		//	selection
		auto selected = i == gdata->_idx && (!gdata->_player->canGainPerks() || gdata->_attributePointsLeft > 0);
		if (selected)
			_win.writec(x - 1, y + 1, '>', COLOR_HIGHLIGHT);

		//	attribute
		auto attr = static_cast<Attribute>(i);
		int base = p->getBaseAttribute(attr);

		//	base value of the stat; turn green if capped
		drawBox(x, y, 3, 2, COLOR_DARK);
		_win.write(x + 1, y + 1, extendInteger(base, 2), (base < STAT_CAP) ? COLOR_WHITE : COLOR_POSITIVE);
		if (selected)
			_win.write(x + 5, y + 1, getAttributeName(attr), COLOR_BLACK, COLOR_MEDIUM);
		else
			_win.write(x + 4, y + 1, getAttributeName(attr), COLOR_MEDIUM);
		y += 3;

		//	description, if selected
		int rx = 31, ry = 4;
		if (selected)
		{
			switch (attr)
			{
			case(ATTR_DEXTERITY):

				drawStatWithBox(rx, ry, to_string(p->getAccuracy()), "Global Accuracy", COLOR_MISC_STAT);
				_win.write(rx - 5, ry + 1, "+1", COLOR_POSITIVE);
				
				drawStatWithBox(rx, ry + 3, to_string(p->getDefenceValue()), "Defence Value", COLOR_MISC_STAT);
				_win.write(rx - 5, ry + 4, "+0.5", COLOR_POSITIVE);

				drawStatWithBox(rx, ry + 6, to_string(p->getCriticalMultiplier()) + "%", "Critical Damage", TCODColor::crimson);
				_win.write(rx - 5, ry + 7, "+2%", COLOR_POSITIVE);

				drawStatWithBox(rx, ry + 9, to_string(p->getResistance(DTYPE_ELECTRIC)) + "%", "Electric Resist", getDamageTypeColor(DTYPE_ELECTRIC));
				if (base <= 50)
					_win.write(rx - 5, ry + 10, "+0.5%", COLOR_POSITIVE);

				break;


			case(ATTR_STRENGTH):
				
				drawStatWithBox(rx, ry, to_string(p->getMaxHealth()), "Health", COLOR_HEALTH);
				_win.write(rx - 5, ry + 1, "+2", COLOR_POSITIVE);

				drawStatWithBox(rx, ry + 3, to_string(p->getWeaponDamage()), "Weapon Damage", COLOR_HEALTH);
				_win.write(rx - 5, ry + 4, "+0.5", COLOR_POSITIVE);

				drawStatWithBox(rx, ry + 6, to_string(p->getResistance(DTYPE_POISON)) + "%", "Poison Resist", getDamageTypeColor(DTYPE_POISON));
				drawStatWithBox(rx, ry + 9, to_string(p->getResistance(DTYPE_FIRE)) + "%", "Fire Resist", getDamageTypeColor(DTYPE_FIRE));
				if (base <= 50)
				{
					_win.write(rx - 5, ry + 7, "+0.5%", COLOR_POSITIVE);
					_win.write(rx - 5, ry + 10, "+0.5%", COLOR_POSITIVE);
				}

				break;


			case(ATTR_WILLPOWER):

				drawStatWithBox(rx, ry, to_string(p->getMaxMagic()), "Magic", COLOR_MAGIC);
				_win.write(rx - 5, ry + 1, "+0.5", COLOR_POSITIVE);

				drawStatWithBox(rx, ry + 3, plusminus(p->getSpellPower()) + "%", "Spell Power", COLOR_MAGIC);
				_win.write(rx - 5, ry + 4, "+5%", COLOR_POSITIVE);

				drawStatWithBox(rx, ry + 6, to_string(p->getMaxSpellsKnown()), "Max Spells", TCODColor::lightBlue);
				if (base < 25)
					_win.write(rx - 5, ry + 7, "+0.2", COLOR_POSITIVE);
				
				drawStatWithBox(rx, ry + 9, to_string(p->getResistance(DTYPE_ARCANE)) + "%", "Arcane Resist", getDamageTypeColor(DTYPE_ARCANE));
				if (base <= 50)
					_win.write(rx - 5, ry + 10, "+0.5%", COLOR_POSITIVE);

				break;
			}
		}
	}


	//	perks
	y += 6;
	drawBox(x - 2, y - 1, 61, 16, COLOR_DARK);
	if (gdata->_player->_PerkLevel > 0)
		writeFormatted(x - 1, y - 1, "ASCENDANT LEVEL #" + to_string(gdata->_player->_PerkLevel), { COLOR_LIGHT });

	_win.write(x, ++y, "ASCENDANT ABILITIES", COLOR_LIGHT);
	if (!gdata->_player->canGainPerks())
		_win.write(x + 25, y, "[Unlocked after Level 30]", TCODColor::darkGrey);
	else if (gdata->_attributePointsLeft < 1)
	{
		col = (gdata->_perkPoints > 0) ? COLOR_POSITIVE : COLOR_DARK;
		writeFormatted(x + 25, y, "#POINTS LEFT [#" + extendInteger(gdata->_perkPoints, 2) + "#]", { COLOR_LIGHT, col, COLOR_LIGHT, });
	}

	//	List of perks
	y += 1; x += 1;
	for (unsigned i = 0; i < PERK__MAX; i++)
	{
		auto pk = static_cast<Perk>(i);
		y++;

		//	Highlighting
		if (gdata->_idx == i && gdata->_player->canGainPerks() && gdata->_attributePointsLeft < 1)
			_win.writec(x - 1, y, '>', COLOR_WHITE);

		//	Perk name and bonus
		_win.write(x, y, plusminus(getPerkBonusPerRank(pk)), p->canGainPerks() ? COLOR_POSITIVE : TCODColor::darkGrey);
		_win.write(x + 5, y, getPerkName(pk), COLOR_MEDIUM);

		//	Total bonus attained
		auto pcol = p->getPerkRank(pk) > 0 ? COLOR_POSITIVE : COLOR_DARK;
		writeFormatted(x + 25, y, "[#" + plusminus(p->getPerkBonus(pk)) + "@]", { pcol });

		//	No. of ranks
		drawProgressDots(x + 35, y, p->getPerkRank(pk), getPerkMaxLevel(pk), COLOR_HIGHLIGHT);
	}


	//	summary of character
	drawCharacterSummary(gdata);
}



//	List of items we can pick up
void display::drawItemPickup(gamedataPtr gdata)
{
	int x = 4, y = 4;
	drawBox(2, 2, 42, 50, COLOR_DARK);
	_win.write(x, y, "What do you wish to pick up?", COLOR_LIGHT);
	y++;

	for (unsigned i = 0; i < gdata->_currentItemList.size(); i++)
	{
		//	is the item selected
		y++;
		auto it = gdata->_currentItemList[i];
		bool selected = i == gdata->_idx;
		if (selected)
			_win.writec(x, y, '>', COLOR_WHITE);

		//	mark if it has unknown enchantments
		if (doesItemHaveUnknownEnchants(gdata, it))
			_win.writec(x - 1, y, '*', TCODColor::gold);

		//	the item itself
		_win.writec(x + 1, y, it->getGlyph(), it->getColor());

		if (selected)
			_win.write(x + 3, y, it->getName(), COLOR_BLACK, it->getColor());
		else
			_win.write(x + 3, y, it->getName(), it->getColor());
	}


	//	Description of selected
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		//	the item itself
		auto it = gdata->_currentItemList[gdata->_idx];
		drawItemInfo(gdata, it, 47, 4);

		//	equipped items of this type, if any
		auto eq = gdata->_player->getItemInSlot(getSlotForCategory(it->_category));
		if (eq != nullptr)
		{
			drawItemInfo(gdata, eq, 47, 25, it);
			_win.write(46, 24, "EQUIPPED", COLOR_WHITE);
		}
	}
	drawInventoryCapacity(gdata, 4, 50);
}


//	List of equipped items.
void display::drawEquipment(gamedataPtr gdata)
{
	drawBox(2, 2, 42, 19, COLOR_DARK);
	int x = 4, y = 4;

	_win.write(x, y, "Currently equipped:", COLOR_LIGHT);
	y++;

	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		auto slot = static_cast<EquipmentSlot>(i);
		_win.write(x, ++y, getEquipmentSlotName(slot), COLOR_DARK);

		//	selection
		bool selected = (i == gdata->_idx && gdata->_state == STATE_VIEW_EQUIPMENT) || (slot == gdata->_selectedSlot);
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_WHITE);
		else if (gdata->_player->hasNewForSlot(slot))
			_win.writec(x - 1, y, '!', COLOR_POSITIVE);

		//	item
		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr)
		{
			if (selected)
			{
				_win.writec(x + 9, y, it->getGlyph(), it->getColor());
				_win.write(x + 11, y, it->getName(), COLOR_BLACK, it->getColor());
			}
			else
			{
				_win.writec(x + 8, y, it->getGlyph(), it->getColor());
				_win.write(x + 10, y, it->getName(), it->getColor());
			}
		}
		else
			_win.write(x + 15, y, "---", TCODColor::darkGrey);
	}

	//	Controls
	writeFormatted(4, y + 2, "#ENTER @Select an item to equip here", { COLOR_LIGHT });
	writeFormatted(4, y + 3, "#  U   @Unequip this item", { COLOR_LIGHT });

	//	Our current flask
	y += 3;
	if (gdata->_player->_currentFlask != nullptr)
	{
		drawItemInfo(gdata, gdata->_player->_currentFlask, 4, y + 5);
		_win.write(4, y + 3, "CURRENT FLASK", COLOR_LIGHT);
	}


	//	Description of selected
	if (gdata->_idx < SLOT__NONE)
	{
		auto it = gdata->_player->getItemInSlot(static_cast<EquipmentSlot>(gdata->_idx));
		if (it != nullptr)
			drawItemInfo(gdata, it, 47, 4);
	}

	drawCharacterSummary(gdata);
}


//	Items we're carrying with us
void display::drawInventory(gamedataPtr gdata)
{
	int x = 4, y = 4;

	//	Carried items
	drawBox(2, 2, 42, 40, COLOR_DARK);
	_win.write(x, y, "Carried with you:", COLOR_LIGHT);
	_win.write(x + 37, y, "Sz", COLOR_DARK);
	y++;

	//	list of item carried
	for (unsigned i = 0; i < gdata->_currentItemList.size(); i++)
	{
		y += 1;
		bool selected = i == gdata->_idx;

		//	item (highlight if selected)
		auto it = gdata->_currentItemList[i];
		if (selected)
		{
			_win.writec(x, y, '>', COLOR_HIGHLIGHT);
			it->_isNewItem = false;
		}
		else if (it->_isNewItem)
			_win.writec(x, y, '!', COLOR_POSITIVE);


		//	specially marked
		if (it->_markedAsValuable)
			_win.writec(x - 1, y, '#', TCODColor::gold);

		//	item title
		if (selected)
		{
			_win.writec(x + 2, y, it->getGlyph(), it->getColor());
			_win.write(x + 4, y, it->getName(), COLOR_BLACK, it->getColor());
		}
		else
		{
			_win.writec(x + 1, y, it->getGlyph(), it->getColor());
			_win.write(x + 3, y, it->getName(), it->getColor());
		}

		//	weight
		for (unsigned i = 0; i < it->getSize(); i++)
			_win.writec(x + 37 + i, y, 254, COLOR_LIGHT);
	}

	//	the item selected AND the item in the slot we're viewing, if any
	itemPtr it = nullptr;
	itemPtr eq = nullptr;

	//	get the two items
	if (gdata->_idx < gdata->_currentItemList.size())
		it = gdata->_currentItemList[gdata->_idx];
	if (gdata->_selectedSlot != SLOT__NONE)
		eq = gdata->_player->getItemInSlot(gdata->_selectedSlot);
	else if (it != nullptr && it->_category == ITEM_FLASK)
		eq = gdata->_player->_currentFlask;

	//	draw their descriptions
	if (it != nullptr)
		drawItemInfo(gdata, it, 47, 4, eq);
	if (eq != nullptr)
	{
		drawItemInfo(gdata, eq, 47, 25);
		_win.write(45, 23, "EQUIPPED:", COLOR_WHITE);
	}


	//	capacity
	drawInventoryCapacity(gdata, 4, 40);

	//	character info
	if (gdata->_state == STATE_VIEW_INVENTORY_IN_STASH)
	{
		writeFormatted(2, 44, "# TAB  @View stash", { COLOR_LIGHT, COLOR_LIGHT });
		writeFormatted(2, 45, "#  t   @Transfer item to stash", { COLOR_LIGHT, COLOR_LIGHT });
		writeFormatted(2, 46, "#  T   @Transfer all to stash", { COLOR_LIGHT, COLOR_LIGHT });
	}
	writeFormatted(2, 47, "#  v   @View detailed description", { COLOR_LIGHT });
	writeFormatted(2, 48, "#  M   @Mark/unmark as valuable item", { COLOR_LIGHT });
	drawCharacterSummary(gdata);
}


//	The ANVIL lets us repair and modify equipment
void display::drawAnvil(gamedataPtr gdata)
{
	int x = 4, y = 4;

	//	Carried items
	drawBox(2, 2, 42, 40, COLOR_DARK);
	_win.write(x, y, "Carried with you:", COLOR_LIGHT);
	_win.write(x + 37, y, "Sz", COLOR_DARK);
	y++;


	//	list of item carried
	for (unsigned i = 0; i < gdata->_currentItemList.size(); i++)
	{
		y += 1;
		bool selected = i == gdata->_idx;

		//	marks
		auto it = gdata->_currentItemList[i];
		if (selected)
		{
			_win.writec(x, y, '>', COLOR_HIGHLIGHT);
			it->_isNewItem = false;
		}
		if (it->_markedAsValuable)
			_win.writec(x - 1, y, '#', TCODColor::gold);

		//	item title
		if (selected)
		{
			_win.writec(x + 2, y, it->getGlyph(), it->getColor());
			_win.write(x + 4, y, it->getName(), COLOR_BLACK, it->getColor());
		}
		else
		{
			_win.writec(x + 1, y, it->getGlyph(), it->getColor());
			_win.write(x + 3, y, it->getName(), it->getColor());
		}
	}


	//	description of selected
	if (gdata->_idx < gdata->_currentItemList.size())
	{
		drawBox(45, 22, 35, 8, COLOR_DARK);
		auto it = gdata->_currentItemList[gdata->_idx];
		drawItemInfo(gdata, it, 47, 4);

		//	Repair?
		int repair_cost = it->getRepairCost();
		if (repair_cost > 0)
			writeFormatted(47, 24, "#r @Repair for #" + to_string(repair_cost) + " @fragments", { COLOR_LIGHT, TCODColor::gold });

		//	Annihilate?
		writeFormatted(47, 26, "#D @Dismantle for materials", { COLOR_LIGHT });
		//writeFormatted(47, 28, "#x @Extract enchantments", { COLOR_LIGHT });
	}


	//	Other bits.
	drawStashedMaterials(gdata, 47, 34);
	drawMessages(gdata);
}



void display::drawEnchantedForge(gamedataPtr gdata)
{
	drawBox(2, 2, 40, 31, COLOR_DARK);
	_win.write(3, 2, "ENCHANTED FORGE", COLOR_LIGHT);
	int x = 4, y = 4;


	//	List equipment
	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		auto slot = static_cast<EquipmentSlot>(i);
		_win.write(x, ++y, getEquipmentSlotName(slot), COLOR_DARK);

		bool selected = i == gdata->_idx;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);

		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr)
		{
			if (selected)
			{
				_win.writec(x + 9, y, it->getGlyph(), it->getColor());
				_win.write(x + 11, y, it->getName(), COLOR_BLACK, it->getColor());
			}
			else
			{
				_win.writec(x + 8, y, it->getGlyph(), it->getColor());
				_win.write(x + 10, y, it->getName(), it->getColor());
			}
		}
		else
			_win.write(x + 15, y, "---", TCODColor::darkGrey);
	}


	//	Description of selected
	y += 5;
	if (gdata->_idx < SLOT__NONE)
	{
		auto slot = static_cast<EquipmentSlot>(gdata->_idx);
		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr)
		{
			drawItemInfo(gdata, it, 47, 4);
			writeFormatted(x, ++y, "#r @Repair for #" + to_string(it->getRepairCost()) + " @fragments", { COLOR_LIGHT, TCODColor::gold });

			++y;
			if (it->_enhancementLevel < it->getMaxEnhancementLevel())
			{
				writeFormatted(x, ++y, "#e @Add enchantment", { COLOR_LIGHT });
				auto mat_cost = it->getEnhanceCost();
				auto mat_type = it->getEnhanceMaterial();
				writeFormatted(x + 3, ++y, "Costs #x" + to_string(mat_cost) + " " + getMaterialTypeName(mat_type), { getMaterialTypeColor(mat_type) });
			}

			++y;
			writeFormatted(x, ++y, "#f @Increase max durability", { COLOR_LIGHT });
			int cost = it->getReinforceCost();
			writeFormatted(x + 3, ++y, "Costs #x" + to_string(cost) + " fragments", { getMaterialTypeColor(MaterialType::FRAGMENTS) });
		}
	}


	//	Materials
	drawStashedMaterials(gdata, 47, 25);

	//	other stuff we can do
	writeFormatted(4, 45, "#R @Spend #" + to_string(getCostToRepairEquipment(gdata)) + " @fragments to repair all equipped items", { COLOR_LIGHT, TCODColor::gold });
	drawMessages(gdata);
}


//	List of enchantments we can add to an item.
void display::drawEnchantmentSelect(gamedataPtr gdata)
{
	drawBox(2, 2, 35, 40, COLOR_DARK);
	_win.write(3, 2, "Known Enchantments", COLOR_LIGHT);

	//	For testing validity
	auto valid_options = lootgen::getEnchantmentsForItemCategory(gdata->_viewingItem->_category);

	//	Enchantments
	int x = 4, y = 3;
	for (unsigned i = 0; i < gdata->_knownEnchants.size(); i++)
	{
		++y;
		bool selected = i == gdata->_idx;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);

		//	highlight whether enchantment is valid
		auto en = gdata->_knownEnchants[i];
		auto basecol = (find(valid_options.begin(), valid_options.end(), en) != valid_options.end()) ? TCODColor::gold : TCODColor::darkGrey;
		if (selected)
			_win.write(x, y, getItemEnchantmentDescription(en), COLOR_BLACK, basecol);
		else
			_win.write(x, y, getItemEnchantmentDescription(en), basecol);
	}

	//	Selected item
	drawItemInfo(gdata, gdata->_viewingItem, 45, 4);
	drawMessages(gdata);
}


//	Mess with our flask.
void display::drawAlchemy(gamedataPtr gdata)
{
	drawItemInfo(gdata, gdata->_player->_currentFlask, 4, 4);

	drawBox(48, 2, 35, 6, COLOR_DARK);
	writeFormatted(50, 4, "#e @Enhance healing potential", { COLOR_LIGHT });
	auto mat_cost = gdata->_player->_currentFlask->getEnhanceCost();
	auto mat_type = gdata->_player->_currentFlask->getEnhanceMaterial();
	writeFormatted(52, 5, "Requires #x" + to_string(mat_cost) + " " + getMaterialTypeName(mat_type), { getMaterialTypeColor(mat_type) });
	writeFormatted(52, 6, " and #" + to_string(getFlaskEnhanceCost(gdata, gdata->_player->_currentFlask)) + " fragments", { getMaterialTypeColor(MaterialType::FRAGMENTS) });

	drawStashedMaterials(gdata, 50, 13);
	drawMessages(gdata);
}


//	Generate new items by spending materials.
void display::drawDemonforge(gamedataPtr gdata)
{
	//	Base
	drawBox(2, 2, 50, 10, COLOR_DARK);
	_win.write(3, 2, "THE DEMONFORGE", TCODColor::orange);
	_win.write(41, 2, "Tier " + to_string(gdata->_demonforgeTier), TCODColor::white);


	//	Forges

	auto cost = getCostToForgeItem(3);
	//drawProgressDots(4, 4, 3, 4, TCODColor::gold);
	writeFormatted(4, 4, "#1 @Forge a random item for #" + to_string(cost.second) + " " + getMaterialTypeName(cost.first), { COLOR_LIGHT, getMaterialTypeColor(cost.first) });

	cost = getCostToForgeItem(4);
	//drawProgressDots(4, 5, 4, 4, TCODColor::gold);
	writeFormatted(4, 5, "#2 @Forge a random item for #" + to_string(cost.second) + " " + getMaterialTypeName(cost.first), { COLOR_LIGHT, getMaterialTypeColor(cost.first) });


	//	Transmutations
	writeFormatted(4, 7, "#3 @Transmute #500 glowing goo @to #100 luminous dust", { COLOR_LIGHT, getMaterialTypeColor(MaterialType::MAGIC_DUST), getMaterialTypeColor(MaterialType::GLOWING_POWDER) });
	writeFormatted(4, 8, "#4 @Transmute #500 luminous dust @to #1 radiant ash", { COLOR_LIGHT, getMaterialTypeColor(MaterialType::GLOWING_POWDER), getMaterialTypeColor(MaterialType::RADIANT_ASH) });


	//	Other fabrications
	writeFormatted(4, 9, "#5 @Create a #notched cube @for #250 luminous dust", { COLOR_LIGHT, getMaterialTypeColor(MaterialType::NOTCHED_CUBE), getMaterialTypeColor(MaterialType::GLOWING_POWDER) });


	//	Upgrades
	switch (gdata->_demonforgeTier)
	{
	case(1):	writeFormatted(4, 12, "#U @Upgrade forge to Tier 2 for #500 glowing goo", { COLOR_LIGHT, getMaterialTypeColor(MaterialType::MAGIC_DUST) }); break;
	case(2):	writeFormatted(4, 12, "#U @Upgrade forge to Tier 3 for #250 luminous dust", { COLOR_LIGHT, getMaterialTypeColor(MaterialType::GLOWING_POWDER) }); break;
	}

	
	//	OTHER STUFF
	drawStashedMaterials(gdata, 55, 4);
	drawMessages(gdata);
}


//	Allows us to socket/unsocket gems.
void display::drawGemstonePress(gamedataPtr gdata)
{
	drawBox(2, 2, 40, 25, COLOR_DARK);
	_win.write(3, 2, "GEMSTONE PRESS", COLOR_LIGHT);
	int x = 4, y = 4;


	//	List equipment
	for (unsigned i = 0; i < SLOT__NONE; i++)
	{
		auto slot = static_cast<EquipmentSlot>(i);
		_win.write(x, ++y, getEquipmentSlotName(slot), COLOR_DARK);

		bool selected = i == gdata->_idx;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);

		//	item info, if one is equipped here
		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr)
		{
			if (selected)
			{
				_win.writec(x + 9, y, it->getGlyph(), it->getColor());
				_win.write(x + 11, y, it->getName(), COLOR_BLACK, it->getColor());
			}
			else
			{
				_win.writec(x + 8, y, it->getGlyph(), it->getColor());
				_win.write(x + 10, y, it->getName(), it->getColor());
			}
		}
		else
			_win.write(x + 15, y, "---", TCODColor::darkGrey);
	}


	//	Optional actions
	y = 30;
	if (gdata->_idx < SLOT__NONE)
	{
		auto slot = static_cast<EquipmentSlot>(gdata->_idx);
		auto it = gdata->_player->getItemInSlot(slot);
		if (it != nullptr)
		{
			drawItemInfo(gdata, it, 47, 4);
			if (it->hasFreeSocket())
				writeFormatted(x, ++y, "#ENTER @Add a gem to an empty socket", { COLOR_LIGHT });

			if (canAddGemSlot(gdata, it))
			{
				++y;
				writeFormatted(x, ++y, "#  e @Use a Notched Cube to add a socket", { COLOR_LIGHT });
			}

			++y;
			writeFormatted(x, ++y, "#  x @Remove all socketed gems", { COLOR_LIGHT });
			++y;
			writeFormatted(x, ++y, "#  f @Fabricate gems", { COLOR_LIGHT });
		}
	}


	//	Materials
	drawStashedGemstones(gdata, 47, 25);
}


//	Select a gemstone to socket.
void display::drawGemstoneSelect(gamedataPtr gdata)
{
	drawBox(2, 2, 35, 40, COLOR_DARK);
	int x = 4, y = 4;
	for (unsigned i = 0; i < gdata->_stashedGems.size(); i++)
	{
		y++;
		auto selected = gdata->_idx == i;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);

		auto it = gdata->_stashedGems[i];
		if (selected)
			_win.write(x + 1, y, it->getName(), COLOR_BLACK, it->getColor());
		else
			_win.write(x, y, it->getName(), it->getColor());

		//	description of gem
		if (selected)
			drawItemInfo(gdata, it, 45, 25, gdata->_viewingItem);
	}

	//	description of item we're socketing something into
	drawItemInfo(gdata, gdata->_viewingItem, 45, 4);
}


//	Shows all gemstones we can create.
void display::drawGemstoneFabricator(gamedataPtr gdata)
{
	drawBox(2, 2, 35, 30, COLOR_DARK);
	_win.write(3, 2, "GEMSTONE FABRICATOR", COLOR_LIGHT);
	int x = 4, y = 4;
	for (unsigned i = 0; i < gdata->_currentItemList.size(); i++)
	{
		++y;
		bool selected = i == gdata->_idx;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);

		//	selected item
		auto it = gdata->_currentItemList[i];
		auto col = it->getColor();
		if (!hasMaterial(gdata, MaterialType::FRAGMENTS, getGemstoneFabricateCost(it)))
			col = TCODColor::darkGrey;
		if (selected)
		{
			//	name of gem
			_win.write(x + 3, y, it->getName(), COLOR_BLACK, col);

			//	requirements to fabricate
			writeFormatted(4, 35, "#ENTER @Fabricate gemstone", { COLOR_LIGHT });
			writeFormatted(5, 36, "Requires #x3 " + getGemTypeFullName(it->_gemType, it->_enhancementLevel - 1), { it->getColor() });
			writeFormatted(5, 37, "Costs #" + to_string(getGemstoneFabricateCost(it)) + " fragments", { TCODColor::gold });
		}
		else
			_win.write(x + 2, y, it->getName(), col);
	}

	//	Other stashed stuff
	drawStashedGemstones(gdata, 47, 4);
	drawStashedMaterials(gdata, 47, 34);
	drawMessages(gdata);
}


//	Pick spells to equip.
void display::drawRuneImprinter(gamedataPtr gdata)
{
	drawBox(2, 2, 40, 48, COLOR_DARK);
	_win.write(3, 2, "SELECT RUNES TO IMPRINT", COLOR_LIGHT);

	//	Spell runes
	int x = 5, y = 3;
	for (unsigned i = 0; i < gdata->_currentItemList.size(); i++)
	{
		//	test if this rune is selected
		++y;
		bool selected = gdata->_idx == i;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);


		//	the spell rune
		auto it = gdata->_currentItemList[i];
		_win.writec(x, y, it->getGlyph(), it->getColor());
		if (gdata->_player->isRuneEquipped(it))
			_win.writec(x - 2, y, '#', COLOR_WHITE);


		//	actual rune name
		if (selected)
		{
			//	highlight rune
			_win.write(x + 3, y, it->getName(), COLOR_BLACK, it->getColor());
			drawSpellInfo(gdata, it->_containsSpell, it->_spellLevel, 47, 4);
			drawItemEnchantments(gdata, it, 47, 18);

			//	Other options
			if (gdata->_player->isRuneEquipped(it))
				writeFormatted(47, 24, "#u @Unequip this rune", { COLOR_LIGHT });
			writeFormatted(47, 25, "#e @Increase spellrune level to #" + to_string(it->_spellLevel + 1), { COLOR_LIGHT, COLOR_POSITIVE });

			//	Requirements
			auto mat = it->_spellLevel >= 10 ? MaterialType::BRIGHT_RUNE : MaterialType::RUNE_SHARD;
			writeFormatted(49, 26, "(Requires #" + to_string(it->getEnhanceCost()) + " " + getMaterialTypeName(mat) + "@)", { getMaterialTypeColor(mat) });
			if (getSpellMPCost(it->_containsSpell, it->_spellLevel + 1) > getSpellMPCost(it->_containsSpell, it->_spellLevel))
				writeFormatted(47, 27, "  (#MP Cost +1@)", { TCODColor::lightBlue });
		}
		else
			_win.write(x + 2, y, it->getName(), it->getColor());
	}


	//	Spells known
	x = 47, y = 30;
	drawBox(45, 29, 35, 8, COLOR_DARK);
	_win.write(46, 29, "Spells Memorized", COLOR_LIGHT);
	writeFormatted(57, 37, "#U @Un-inscribe all runes", { COLOR_LIGHT });
	for (auto sp : gdata->_player->getAllSpellsKnown())
		_win.write(x, ++y, getSpellNameFull(sp, gdata->_player->getSpellLevel(sp)), getSpellColor(sp));


	//	no of slots
	const int maxspells = gdata->_player->getMaxSpellsKnown();
	unsigned s = 0;
	for (s = 0; s < gdata->_player->getAllSpellsKnown().size(); s++)
		_win.writec(80 + s - maxspells, 29, 254, TCODColor::lightBlue);
	for (s; s < maxspells; s++)
		_win.writec(80 + s - maxspells, 29, 255, TCODColor::darkGrey);


	//	mats/messages
	drawStashedMaterials(gdata, 47, 42);
	drawMessages(gdata);
}



void display::drawStash(gamedataPtr gdata)
{
	drawBox(2, 2, 35, 61, COLOR_DARK);
	_win.write(3, 2, "YOUR STASH", COLOR_LIGHT);
	int x = 4, y = 4;

	for (unsigned i = 0; i < gdata->_currentItemList.size(); i++)
	{
		y += 1;
		bool selected = i == gdata->_idx;

		//	highlight selected
		auto it = gdata->_currentItemList[i];
		if (selected)
		{
			_win.writec(x, y, '>', COLOR_HIGHLIGHT);
			it->_isNewItem = false;
		}

		//	marked as valuable
		if (it->_markedAsValuable)
			_win.writec(x - 1, y, '#', TCODColor::gold);

		//	item title
		if (selected)
		{
			_win.writec(x + 2, y, it->getGlyph(), it->getColor());
			_win.write(x + 4, y, it->getName(), COLOR_BLACK, it->getColor());
		}
		else
		{
			_win.writec(x + 1, y, it->getGlyph(), it->getColor());
			_win.write(x + 3, y, it->getName(), it->getColor());
		}
	}

	//	Selected item description, if any
	if (gdata->_idx < gdata->_currentItemList.size())
		drawItemInfo(gdata, gdata->_currentItemList[gdata->_idx], 40, 4);

	//	Stashed materials.
	x = 40; y = 25;
	drawStashedMaterials(gdata, 40, 25);
	drawStashedGemstones(gdata, 40, 37);
	
	//	Control options
	x = 4;  y = 58;
	writeFormatted(x, ++y, "#TAB   @Open inventory", { COLOR_LIGHT, COLOR_LIGHT });
	writeFormatted(x, ++y, " #g    @Transfer to inventory", { COLOR_LIGHT, COLOR_LIGHT });
	writeFormatted(x, ++y, " #M    @Mark as valuable item", { COLOR_LIGHT });
}



//	Lists all the spells we know.
void display::drawSpellList(gamedataPtr gdata)
{
	drawBox(2, 2, 35, 20, COLOR_DARK);
	_win.write(3, 2, "SPELLS KNOWN", COLOR_LIGHT);
	int x = 4, y = 3;

	auto spells = gdata->_player->getAllSpellsKnown();
	for (unsigned i = 0; i < spells.size(); i++)
	{
		++y;
		auto selected = i == gdata->_idx;
		if (selected)
			_win.writec(x - 1, y, '>', COLOR_HIGHLIGHT);

		//	the spell
		auto sp = spells.at(i);
		int lvl = gdata->_player->getSpellLevel(sp);


		//	spell name
		if (selected)
		{
			_win.write(x + 4, y, getSpellNameFull(sp, lvl), COLOR_BLACK, getSpellColor(sp));
			drawSpellInfo(gdata, sp, lvl, 47, 4);
		}
		else
			_win.write(x + 3, y, getSpellNameFull(sp, lvl), getSpellColor(sp));
	}

	drawMessages(gdata);
}


//	Info about a particular spell (NOT the rune/item that grants the spell, but the spell's actual effect when cast)
void display::drawSpellInfo(gamedataPtr gdata, const Spell sp, const int lvl, int atx, int aty)
{
	drawBox(atx - 2, aty - 2, 35, 20, COLOR_DARK);

	//	name & basic description
	_win.write(atx, aty, getSpellNameFull(sp, lvl), getSpellColor(sp));
	_win.write(atx, ++aty, "Tier " + to_string(getSpellTier(sp)) + " Spell", COLOR_DARK);
	aty = _win.writeWrapped(atx, ++aty, 32, getSpellDescription(sp), COLOR_MEDIUM);

	//	MP cost
	aty += 1;
	writeFormatted(atx, ++aty, "MP Cost #" + to_string(getSpellMPCost(sp, lvl)), { COLOR_MAGIC });

	//	base damage
	aty += 1;
	if (doesSpellInflictDamage(sp))
	{
		//	base damage
		auto dam = getSpellDamage(sp, lvl);
		auto dtype = getSpellDamageType(sp);
		writeFormatted(atx, ++aty, "Base Damage #" + to_string(dam.first) + "-" + to_string(dam.second) + " " + getDamageTypeName(dtype), { getDamageTypeColor(dtype) });

		//	adjusted (spell power)
		auto spellpower = gdata->_player->getSpellPower();
		auto affinity = gdata->_player->getElementalAffinity(dtype);
		dam.first = adjustByPercent(dam.first, spellpower + affinity);
		dam.second = adjustByPercent(dam.second, spellpower + affinity);
		writeFormatted(atx, ++aty, "Adjusted    #" + to_string(dam.first) + "-" + to_string(dam.second) + " " + getDamageTypeName(dtype), { getDamageTypeColor(dtype) });
	}

	//	Special damage from effects
	if (sp == Spell::SMITE_EVIL)
		writeFormatted(atx, ++aty, "Smite Damage #" + to_string(gdata->_player->getSmiteEvilDamage()), { TCODColor::gold });
	else if (sp == Spell::ARCANE_PULSE)
	{
		auto dam = gdata->_player->getArcanePulseDamage();
		writeFormatted(atx, ++aty, "Pulse Damage #1-" + to_string(dam), { getDamageTypeColor(DTYPE_ARCANE) });
		writeFormatted(atx, ++aty, "Adjusted     #1-" + to_string(adjustByPercent(dam, gdata->_player->getElementalAffinity(DTYPE_ARCANE))), { getDamageTypeColor(DTYPE_ARCANE) });
	}
	else if (sp == Spell::STATIC_FIELD)
	{
		auto dam = gdata->_player->getStaticFieldDamage();
		writeFormatted(atx, ++aty, "Damage   #" + to_string(dam), { getDamageTypeColor(DTYPE_ELECTRIC) });
		writeFormatted(atx, ++aty, "Adjusted #" + to_string(adjustByPercent(dam, gdata->_player->getElementalAffinity(DTYPE_ELECTRIC))), { getDamageTypeColor(DTYPE_ELECTRIC) });
	}
	else if (sp == Spell::VENOMFANG)
	{
		auto dam = gdata->_player->getVenomfangDamage();
		writeFormatted(atx, ++aty, "Poison Damage #" + to_string(dam), { getDamageTypeColor(DTYPE_POISON) });
		writeFormatted(atx, ++aty, "Adjusted      #" + to_string(adjustByPercent(dam, gdata->_player->getElementalAffinity(DTYPE_POISON))), { getDamageTypeColor(DTYPE_POISON) });
	}

	//	range
	if (isSpellTargeted(sp))
	{
		auto sp_range = getSpellRange(sp, lvl);
		writeFormatted(atx, ++aty, "Attack Range #" + to_string(sp_range), { COLOR_LIGHT });
	}

	//	duration
	if (doesSpellHaveDuration(sp))
	{
		auto dur = getSpellDuration(sp, lvl);
		writeFormatted(atx, ++aty, "Lasts #" + to_string(dur) + " @turns.", { COLOR_LIGHT });
	}
}



//	Details of a given item.
//	If a second item 'compareTo' is given, indicates stat differences between the two items.
void display::drawItemInfo(gamedataPtr gdata, itemPtr it, int atx, int aty, itemPtr compareTo)
{
	int topy = aty;

	//	name & glyph
	_win.writec(atx, aty, it->getGlyph(), it->getColor());
	_win.write(atx + 2, aty, it->getName(), it->getColor());
	

	//	level and tier
	auto cat_txt = getItemCategoryName(it->_category) + " #[" + to_string(it->_enhancementLevel) + "/" + to_string(it->getMaxEnhancementLevel()) + "]";
	if (it->_isTwoHanded)
		cat_txt = "2h " + cat_txt;
	writeFormatted(atx, ++aty, cat_txt, { COLOR_DARK, COLOR_MEDIUM });
	drawProgressDots(atx + 20, aty, it->_rarity, 4, TCODColor::gold);


	//	durability, if relevant
	if (it->subjectToDurabilityLoss())
	{
		int dleft = it->_maxDurability - it->_damageTaken;
		auto dcol = (dleft <= 5) ? COLOR_NEGATIVE : COLOR_LIGHT;
		writeFormatted(atx + 29, aty, "CND: #" + to_string(dleft) + "@/" + to_string(it->_maxDurability), { dcol });
	}


	//	armour type
	if (it->isArmourPiece())
		_win.write(atx, ++aty, getArmourCategoryName(it->_armourCategory) + " Armour", COLOR_DARK);


	//	info about gems
	if (it->_category == ITEM_GEM)
	{
		ItemCategory cat = ITEM__MAX;
		if (gdata->_viewingItem != nullptr) cat = gdata->_viewingItem->_category;
		drawGemTypeEffects(gdata, it->_gemType, it->_enhancementLevel, atx, aty + 2, cat);
	}


	//	info about materials
	else if (it->_category == ITEM_CONSUMABLE || it->_category == ITEM_MATERIAL)
	{
		aty = drawMaterialDescription(it->_material, atx, ++aty);
	}


	//	list of properties
	++aty;
	for (unsigned i = 0; i < PROP__NONE; i++)
	{
		//	we don't display EVERY property
		auto prop = static_cast<ItemProperty>(i);
		if (!hide_item_property(prop))
		{
			//	stat of this item
			auto val = it->getProperty(prop);

			//	test comparison
			if (compareTo != nullptr)
			{
				auto cval = compareTo->getProperty(prop);
				if (val != 0)
				{
					//	stat value
					_win.write(atx + 5, ++aty, formatItemProperty(prop, val), COLOR_LIGHT);
					_win.write(atx + 12, aty, getItemPropertyName(prop), COLOR_MEDIUM);

					//	difference in values
					auto diff = val - cval;
					if (diff != 0)
					{
						auto col = (diff < 0) ? COLOR_NEGATIVE : COLOR_POSITIVE;
						_win.write(atx, aty, plusminus(diff), col);
					}
				}
			}

			//	no comparison; just draw the stat
			else if (val != 0)
			{
				_win.write(atx + 1, ++aty, formatItemProperty(prop, val), COLOR_LIGHT);
				_win.write(atx + 8, aty, getItemPropertyName(prop), COLOR_MEDIUM);
				//drawDottedLinePair(atx + 1, ++aty, atx + 16, getItemPropertyName(prop), formatItemProperty(prop, val), COLOR_LIGHT, COLOR_MEDIUM);
			}
		}
	}


	//	resistances (displayed separately, only for armour)
	if (it->isArmourPiece())
	{
		int tx = atx + 8;
		aty += 2;
		_win.write(atx, aty, "Resists", COLOR_MEDIUM);
		for (unsigned i = 0; i < lootgen::RESIST_PROPS.size(); i++)
		{
			const int val = it->getProperty(lootgen::RESIST_PROPS[i]);
			if (val > 0)
				writeFormatted(tx, aty, "[#" + extendInteger(val, 2) + "%@]", { getDamageTypeColor(SPECIAL_DAMAGE_TYPES[i]) });
			else
				writeFormatted(tx, aty, "[#00%@]", { COLOR_DARK });
			tx += 5;
		}
	}


	//	list of enchantments
	aty++;
	aty = drawItemEnchantments(gdata, it, atx, aty);


	//	list of sockets, full or no
	auto gems = it->getAllSocketedGemTypes();
	for (unsigned i = 0; i < gems->size(); i++)
	{
		auto col = getGemTypeColor(gems->at(i));
		_win.writec(atx, ++aty, 229, col);
		if (gems->at(i) != GemType::__NONE)
			_win.write(atx + 2, aty, getGemTypeFullName(gems->at(i), it->getSocketLevel(i)), col);
		else
			_win.write(atx + 2, aty, "Empty Socket", TCODColor::grey);
	}

	//	info about spells
	if (it->_containsSpell != Spell::__NONE)
		writeFormatted(atx + 2, ++aty, "#" + getSpellName(it->_containsSpell) + " @+" + to_string(it->_spellLevel), { getSpellColor(it->_containsSpell) });
	if (it->_category == ITEM_SPELLRUNE)
	{
		aty += 1;
		aty = _win.writeWrapped(atx, ++aty, 30, "Use the Rune Imprinter to learn spell runes.", COLOR_MEDIUM);
	}

	//	box item in
	drawBox(atx - 2, topy - 2, 42, 19, COLOR_DARK);
}


//	Item info takes over the whole screen.
void display::drawItemInfoDetailed(gamedataPtr gdata, itemPtr it)
{
	//	Basic info
	drawItemInfo(gdata, it, 4, 4);
	
	
	//	Enchantment details
	int x = 5, y = 24;
	for (auto en : *it->getAllEnchantments())
	{
		auto txt = getItemEnchantmentVerbose(en, it->getEnchantmentValue(en));
		_win.write(x - 1, ++y, getItemEnchantmentDescription(en), TCODColor::gold);
		y = _win.writeWrapped(x, ++y, 40, txt, COLOR_MEDIUM);
		y += 2;
	}
	drawBox(2, 23, 42, y - 22, COLOR_DARK);


	//	Gem descriptions
	x = 50; y = 3;
	auto gems = it->getAllSocketedGemTypes();
	for (unsigned i = 0; i < gems->size(); i++)
	{
		auto col = getGemTypeColor(gems->at(i));
		_win.writec(x, ++y, 229, col);
		if (gems->at(i) != GemType::__NONE)
		{
			drawBox(48, y - 2, 35, 8, COLOR_DARK);
			auto lvl = it->getSocketLevel(i);
			_win.write(x + 2, y, getGemTypeFullName(gems->at(i), lvl), col);
			drawGemTypeEffects(gdata, gems->at(i), lvl, x, ++y, it->_category);
			y += 8;
		}
	}
}


//	Effects of a gem of the given type, for each type of socket.
void display::drawGemTypeEffects(gamedataPtr gdata, const GemType gem, const int tier, int atx, int aty, const ItemCategory hcat)
{
	//	outlines for each
	string weapon, armour, jewel;
	switch (gem)
	{
	case(GemType::BLACKSTONE):
		armour = plusminus(tier * 2) + " #Armour Value";
		jewel = plusminus(tier * 25) + "% #Critical Damage";
		weapon = plusminus(tier * 4) + " #Physical Damage";
		break;

	case(GemType::BOLTSTONE):
		armour = plusminus(tier * 5) + "% #Electric Resistance";
		jewel = plusminus(tier * 25) + "% #Spell Power";
		weapon = plusminus(tier * 3) + " #Electric Damage";
		break;

	case(GemType::FLAMESTONE):
		armour = plusminus(tier * 5) + "% #Fire Resistance";
		jewel = plusminus(tier * 20) + " #Max Health";
		weapon = plusminus(tier * 3) + " #Fire Damage";
		break;

	case(GemType::SILVERSTONE):
		armour = plusminus(tier * 5) + "% #Arcane Resistance";
		jewel = plusminus(tier) + " #Max Magic";
		weapon = plusminus(tier * 3) + " #Arcane Damage";
		break;

	case(GemType::SPIDERSTONE):
		armour = plusminus(tier * 5) + "% #Poison Resistance";
		jewel = plusminus(tier) + "% #Critical Chance";
		weapon = plusminus(tier * 3) + " #Poison Damage";
		break;
	}

	
	//	should we highlight one of them?
	auto ha = (hcat == ITEM_BOOTS || hcat == ITEM_BRACERS || hcat == ITEM_CHESTPIECE || hcat == ITEM_GLOVES || hcat == ITEM_HELMET || hcat == ITEM_SHOULDERS);
	auto hj = (hcat == ITEM_AMULET || hcat == ITEM_RING);
	auto hw = (hcat == ITEM_SHIELD || hcat == ITEM_WEAPON);


	//	actually write the effects
	_win.write(atx, aty, "Effect if socketed into:", COLOR_LIGHT);
	writeFormatted(atx, ++aty, "#ARMOUR #" + armour, { ha ? COLOR_LIGHT : COLOR_DARK, COLOR_POSITIVE, COLOR_LIGHT, });
	writeFormatted(atx, ++aty, "#JEWEL  #" + jewel, { hj ? COLOR_LIGHT : COLOR_DARK, COLOR_POSITIVE, COLOR_LIGHT });
	writeFormatted(atx, ++aty, "#WEAPON #" + weapon, { hw ? COLOR_LIGHT : COLOR_DARK, COLOR_POSITIVE, COLOR_LIGHT });
}


//	Returns the last y-coord we drew on
int display::drawMaterialDescription(const MaterialType mat, int atx, int aty)
{
	switch (mat)
	{
		//	Materials
	case(MaterialType::BRIGHT_RUNE):		aty = _win.writeWrapped(atx, ++aty, 40, "Used to enhance spell runes up to level 10.", COLOR_MEDIUM); break;
	case(MaterialType::FRAGMENTS):			aty = _win.writeWrapped(atx, ++aty, 40, "The most common material, used for a variety of purposes - including repairing equipment.", COLOR_MEDIUM); break;
	case(MaterialType::GLASS_SHARD):		aty = _win.writeWrapped(atx, ++aty, 40, "Used to increase the healing potential of flasks.", COLOR_MEDIUM); break;
	case(MaterialType::GLOWING_POWDER):		aty = _win.writeWrapped(atx, ++aty, 40, "A special material that can be used to add a 4th enchantment to some items. Gained by dismantling equipment.", COLOR_MEDIUM); break;
	case(MaterialType::MAGIC_DUST):			aty = _win.writeWrapped(atx, ++aty, 40, "A material that can be used to add a 3rd enchantment to some items. Gained by dismantling equipment.", COLOR_MEDIUM); break;
	case(MaterialType::RADIANT_ASH):		aty = _win.writeWrapped(atx, ++aty, 40, "The rarest of materials. Used to add a 5th enchantment to legendary items.", COLOR_MEDIUM); break;
	case(MaterialType::RUNE_SHARD):			aty = _win.writeWrapped(atx, ++aty, 40, "Used to enhance spell runes past level 10.", COLOR_MEDIUM); break;

		//	Consumables
	case(MaterialType::NOTCHED_CUBE):		aty = _win.writeWrapped(atx, ++aty, 40, "Use at the gem press. Can add a gem slot to an item that has none.", COLOR_MEDIUM); break;
	}
	return aty;
}


//	Shows how many free inventory slots we have.
void display::drawInventoryCapacity(gamedataPtr gdata, int x, int y)
{
	_win.write(x, y, "CAPACITY", COLOR_LIGHT);
	int used = getInventorySlotsUsed(gdata);
	for (unsigned i = 0; i < gdata->_player->getMaxInventorySlots(); i++)
	{
		if (i < used)
			_win.writec(x + i + 9, y, 254, COLOR_LIGHT);
		else
			_win.writec(x + i + 9, y, 255, COLOR_DARK);
	}
}


//	Draws ONLY the item's enchantments.
int display::drawItemEnchantments(gamedataPtr gdata, itemPtr it, int atx, int aty)
{
	//	filled slots
	auto en_list = it->getAllEnchantments();
	for (auto en : *en_list)
	{
		_win.writec(atx, ++aty, 4, TCODColor::gold);
		writeFormatted(atx + 2, aty, formatItemEnchantment(en, it->getEnchantmentValue(en)), { COLOR_POSITIVE });
	}


	//	empty slots (for certain categories)
	if (it->_category != ITEM_CONSUMABLE && it->_category != ITEM_GEM && it->_category != ITEM_MATERIAL)
	{
		for (unsigned i = 0; i < it->getMaxEnhancementLevel() - it->_enhancementLevel; i++)
		{
			_win.writec(atx, ++aty, 4, TCODColor::darkGrey);
			_win.write(atx + 2, aty, "[Enchantment Slot]", TCODColor::darkGrey);
		}
	}

	return aty;
}


//	All the gems in our stash.
void display::drawStashedGemstones(gamedataPtr gdata, int x, int y)
{
	drawBox(x - 2, y - 2, 35, 28, COLOR_DARK);
	_win.write(x - 1, y - 2, "GEMSTONES", COLOR_LIGHT);
	for (auto gem : gdata->_stashedGems)
	{
		_win.write(x, y, gem->getName(), gem->getColor());
		y++;
	}
}



//	Materials in our stash.
void display::drawStashedMaterials(gamedataPtr gdata, int x, int y)
{
	drawBox(x - 2, y - 2, 35, 10, COLOR_DARK);
	_win.write(x - 1, y - 2, "MATERIALS", COLOR_LIGHT);
	for (auto mat : gdata->_stashedMaterials)
	{
		_win.write(x, y, mat->getName(), mat->getColor());
		y += 1;
	}
}


//	Tags all visible monsters.
void display::drawMonsterHighlights(gamedataPtr gdata)
{
	auto ctr = gdata->_player->_pos;

	//	Emphasize highlighted monster, if any.
	auto mon = gdata->_map->getCreature(gdata->_cursorPt);
	if (mon != nullptr && !mon->isPlayer())
	{
		auto dpt = mapToDisplayCoord(ctr, mon->_pos.first, mon->_pos.second);
		dpt.first += MAP_X_OFFSET;
		dpt.second += MAP_Y_OFFSET;
		_win.writec(dpt.first, dpt.second, mon->getGlyph(), mon->getColor(), COLOR_HIGHLIGHT);
		_win.writec(dpt.first + 1, dpt.second, 196, mon->getColor());
		_win.write(dpt.first + 2, dpt.second, mon->getName(), COLOR_BLACK, mon->getColor());
	}

	//	Draw all visible monsters.
	else
	{
		for (auto mon : gdata->_visibleMonsters)
		{
			auto dpt = mapToDisplayCoord(ctr, mon->_pos.first, mon->_pos.second);
			dpt.first += MAP_X_OFFSET;
			dpt.second += MAP_Y_OFFSET;
			_win.writec(dpt.first + 1, dpt.second, 196, mon->getColor());
			_win.write(dpt.first + 2, dpt.second, mon->getName(), COLOR_BLACK, mon->getColor());
		}
	}
}


//	Re-calculates what is visible on the map.
void display::updateVisibleMapData(gamedataPtr gdata)
{
	//	player character is at the centre of the screen.
	auto ctr = gdata->_player->_pos;
	gdata->_visibleMonsters.clear();
	int vis = getActualVisionRadius(gdata);


	//	Visible tiles on the map.
	for (int dx = 0; dx < MAP_X_SIZE; dx++)
	{
		for (int dy = 0; dy < MAP_Y_SIZE; dy++)
		{
			//	Default display values
			int gl = 0;
			colorType col = COLOR_BLACK;
			colorType bgcol = COLOR_BLACK;

			//	Is this in the map boundaries
			auto mpt = displayToMapCoord(ctr, dx, dy);
			if (gdata->_map->inBounds(mpt))
			{
				//	Distance to point
				auto dist = hypot(mpt.first - ctr.first, mpt.second - ctr.second);

				//	Derive into from the base tile.
				auto tl = gdata->_map->getTile(mpt);
				gl = getMaptileGlyph(tl);
				col = getMaptileColor(tl);
				bgcol = COLOR_BLACK;

				//	Is this point in FOV?
				if ((gdata->_map->isInFov(mpt) && dist <= vis) || gdata->_omniscient)
				{
					//	Remember tiles we're seen
					gdata->_map->addToMemoryMap(mpt.first, mpt.second);

					//	Surfaces override tiles
					auto sf = gdata->_map->getSurface(mpt);
					if (sf != Surface::__NONE)
					{
						if (drawMaptileOverSurface(tl))
							bgcol = getSurfaceColor(sf);
						else
						{
							gl = getSurfaceGlyph(sf);
							col = getSurfaceColor(sf);
						}
					}

					//	Flickering effect
					else if (doesMaptileFlicker(tl))
					{
						auto f = 1.0f + (float)randint(-3, 3) * 0.1f;
						col.scaleHSV(1.0f, f);
					}

					//	Darken tiles that are further away. Stairs don't get darkened because they are IMPORTANT.
					if (!isMaptileStairs(tl))
					{
						auto darken_factor = 1.0f - 0.3f * dist / (float)(vis - dist);
						if (darken_factor < 0.15f) darken_factor = 0.15f;
						col.scaleHSV(1.0f, darken_factor);
					}
				}

				//	if not, we can still see it if it's in the memory map
				else if (gdata->_map->inMemoryMap(mpt.first, mpt.second))
				{
					//	but it gets darkened (unless it's stairs)
					if (!isMaptileStairs(tl))
					{
						col.scaleHSV(1.0f, 0.15f);
						bgcol.scaleHSV(1.0f, 0.15f);
					}
				}

				//	otherwise, we can't see it at all.
				else
				{
					gl = ' ';
					col = COLOR_BLACK;
					bgcol = COLOR_BLACK;
				}
			}

			//	Update the map info.
			_visibleGlyphs[dx][dy] = gl;
			_visibleColors[dx][dy] = col;
			_visibleBgcolors[dx][dy] = bgcol;
		}
	}


	//	Add items.
	for (auto icp : *gdata->_map->getAllItems())
	{
		auto dpt = mapToDisplayCoord(ctr, icp->_pos.first, icp->_pos.second);
		if (isPointOnDisplay(dpt) && gdata->_map->isInFov(icp->_pos))
		{
			//	top item from this container determines its appearance
			auto it = icp->_items.front();
			_visibleGlyphs[dpt.first][dpt.second] = it->getGlyph();
			_visibleColors[dpt.first][dpt.second] = it->getColor();

			//	if the underlying tile is lava, reveal it
			if (gdata->_map->getTile(icp->_pos) == MT_LAVA)
				_visibleBgcolors[dpt.first][dpt.second] = getMaptileColor(MT_LAVA);
		}
	}


	//	Add creatures.
	for (auto cr : gdata->_map->getAllCreatures())
	{
		//	Test whether the monster is currently onscreen
		auto dpt = mapToDisplayCoord(ctr, cr->_pos.first, cr->_pos.second);
		if (isPointOnDisplay(dpt))
		{
			//	We can always see ourselves; test whether we can see monsters
			if ((getDistanceBetweenCreatures(gdata->_player, cr) <= vis && canPlayerSeeCreature(gdata, cr)) || gdata->_omniscient)
			{
				//	map data
				_visibleGlyphs[dpt.first][dpt.second] = cr->getGlyph();
				_visibleColors[dpt.first][dpt.second] = cr->getColor();

				//	remember monsters we see
				if (!cr->isPlayer())
					gdata->_visibleMonsters.push_back(cr);
			}
		}
	}


	//	Animations
	for (auto anim : gdata->_animations)
	{
		auto pts = anim->getPoints();
		for (unsigned i = 0; i < pts.size(); i++)
		{
			auto dpt = mapToDisplayCoord(ctr, pts[i].first, pts[i].second);
			if (isPointOnDisplay(dpt))
			{
				_visibleGlyphs[dpt.first][dpt.second] = anim->getGlyph(i);
				_visibleColors[dpt.first][dpt.second] = anim->getColor(i);
			}
		}
	}


	//	Cursor position
	if (inCursorState(gdata))
	{
		auto dpt = mapToDisplayCoord(ctr, gdata->_cursorPt.first, gdata->_cursorPt.second);
		_visibleBgcolors[dpt.first][dpt.second] = COLOR_HIGHLIGHT;
	}
}


//	The MAIN GAME INTERFACE, with the map and such.
//	Call 'updateVisibleMapData' before this to make sure the map is up-to-date. This just draws map data we remember.
void display::drawMainInterface(gamedataPtr gdata)
{
	drawInterfaceBoxes();
	_win.write(3, 2, gdata->_map->_name, COLOR_LIGHT);

	//	Draw the map
	bool danger_filter = gdata->_player->getHealthPercent() <= 20;
	for (unsigned x = 0; x < MAP_X_SIZE; x++)
	{
		for (unsigned y = 0; y < MAP_Y_SIZE; y++)
		{
			//	apply filters, if any
			auto col = _visibleColors[x][y];
			col.scaleHSV(1.0f, BRIGHTEN_AMOUNT);
			if (danger_filter)
				col = TCODColor::lerp(col, TCODColor::crimson, 0.3);

			//	actually show this
			_win.writec(x + MAP_X_OFFSET, y + MAP_Y_OFFSET, _visibleGlyphs[x][y], col, _visibleBgcolors[x][y]);
		}
	}

	//	Other interface elements
	drawSidebar(gdata);

	////	Hotkeyed spells
	int x = 3, y = 49;
	for (unsigned h = 0; h < gdata->_player->_ImprintedRunes.size(); h++)
	{
		auto sp = gdata->_player->_ImprintedRunes[h]->_containsSpell;
		if (sp != Spell::__NONE)
		{
			auto name = "[" + to_string(h + 1) + "] #" + getSpellName(sp);
			writeFormatted(x, y, name, { getSpellColor(sp) });
			x += name.size() + 2 ;
		}
	}

	//	Either messages go in this box, or we show monster info
	if (inCursorState(gdata))
	{
		//	tile here
		auto tl = gdata->_map->getTile(gdata->_cursorPt);
		_win.writec(4, 51, getMaptileGlyph(tl), getMaptileColor(tl));
		_win.write(6, 51, getMaptileName(tl), getMaptileColor(tl));

		//	special info?
		if (isSpecialMaptile(tl))
			_win.writeWrapped(6, 53, 75, getMaptileDescription(tl), COLOR_DARK);

		//	surface here, if any
		auto sf = gdata->_map->getSurface(gdata->_cursorPt);
		if (sf != Surface::__NONE)
		{
			_win.writec(30, 51, getSurfaceGlyph(sf), getSurfaceColor(sf));
			_win.write(32, 51, getSurfaceName(sf), getSurfaceColor(sf));
		}

		//	monster info
		auto mon = gdata->_map->getCreature(gdata->_cursorPt);
		if (mon != nullptr && !mon->isPlayer())
			drawMonsterSummary(gdata, static_pointer_cast<monster>(mon));
	}
	else
		drawMessages(gdata);
}


//	Display all messages at the appropriate screen position
void display::drawMessages(gamedataPtr gdata)
{
	int x = 3, y = 51;
	float f = 1.0f;
	for (auto m : gdata->_messages)
	{
		f -= 0.05f;
		writeFormatted(x, y++, m._txt, m._colors, f);
	}
}


//	Sidebar showing player state.
void display::drawSidebar(gamedataPtr gdata)
{
	int x = 52, y = 3;
	auto p = gdata->_player;


	//	debugging
	//_win.write(52, 1, to_string(p->_actionEnergy), COLOR_WHITE);


	//	Debug flags
	if (gdata->_invincible)
		_win.write(51, 2, "GOD", COLOR_WHITE);
	if (gdata->_autokill)
		_win.write(55, 2, "AUTOKILL", COLOR_WHITE);


	//	Level

	_win.write(x, ++y, "LV " + to_string(p->_level), { COLOR_MEDIUM });
	if (gdata->_attributePointsLeft > 0)
		_win.writec(x - 1, y, '+', COLOR_POSITIVE);
	int xp_per = 100.0f * (float)gdata->_xp / (float)XP_PER_LEVEL;
	drawFilledBar(x + 6, y, xp_per / 5, 20, TCODColor::silver, ' ' + to_string(xp_per) + "%");
	y++;


	//	State

	_win.write(x, ++y, "HEALTH", p->willFlaskWasteHealing() ? COLOR_MEDIUM : COLOR_WHITE);
	auto hp_per = p->getHealthPercent();
	drawFilledBar(x + 6, y, hp_per / 5, 20, getPercentColor(hp_per), ' ' + to_string(p->getHealthLeft()) + "/" + to_string(p->getMaxHealth()));

	_win.write(x, ++y, "MAGIC", COLOR_MEDIUM);
	auto mp_per = p->getMagicPercent();
	drawFilledBar(x + 6, y, mp_per / 5, 20, COLOR_MAGIC, ' ' + to_string(p->getMagicLeft()) + "/" + to_string(p->getMaxMagic()));

	y += 2;
	
	_win.write(x, ++y, "  [T]", isTownPortalCharged(gdata) ? COLOR_WHITE : TCODColor::darkerGrey);
	drawFilledBar(x + 6, y, gdata->_townPortalCharge / (TOWN_PORTAL_CHARGE_REQ / 20), 20, TCODColor::darkBlue, " Portal Charge");


	//	Flask
	
	y += 2;
	drawLineHorizontal(x - 1, y, 36, COLOR_DARK);
	y += 2;

	if (p->_currentFlask != nullptr)
	{
		drawBox(x, y, 2, 2, COLOR_DARK);

		//	icon indicates progress to next charge
		auto scale = (float)p->_currentFlask->getRegenerationPercent() / 100.0f;
		auto fl_col = TCODColor::lerp(TCODColor::darkGrey, TCODColor::crimson, scale);
		//_win.write(x, y + 3, to_string(p->_currentFlask->getRegenerationPercent()) + "%", fl_col);
		_win.writec(x + 1, y + 1, p->_currentFlask->getGlyph(), fl_col);

		//	name of flask
		writeFormatted(x + 4, y + 1, "#" + p->_currentFlask->getName(), { p->_currentFlask->getColor() });
		drawProgressDots(x + 5, y + 2, p->_currentFlask->getChargesLeft(), p->_currentFlask->getMaxCharges(), TCODColor::crimson);
	}


	//	Equipment

	y += 4;
	drawLineHorizontal(x - 1, y, 36, COLOR_DARK);
	y += 2;
	
	for (auto it : p->getAllEquippedItems())
	{
		if (it != nullptr)
		{
			_win.writec(x, ++y, it->getGlyph(), it->getColor());
			_win.write(x + 2, y, it->getName(), it->getColor());
		}
		else
			_win.writec(x, ++y, '-', TCODColor::darkGrey);
	}


	//	Status effects
	y += 2;
	for (auto st : ALL_STATUS_EFFECTS)
	{
		if (p->hasStatusEffect(st))
		{
			_win.write(x, ++y, "[" + extendInteger(p->getStatusEffectDuration(st), 2) + "]", COLOR_MEDIUM);
			_win.write(x + 5, y, getStatusEffectName(st), getStatusEffectColor(st));
		}
	}
	for (unsigned i = 0; i < BUFF__MAX; i++)
	{
		auto bf = static_cast<BuffType>(i);
		if (p->hasBuff(bf))
		{
			_win.write(x, ++y, "[" + extendInteger(p->getBuffDuration(bf), 2) + "]", COLOR_MEDIUM);
			_win.write(x + 5, y, getBuffName(bf), COLOR_HIGHLIGHT_POS);
		}
	}
}



//	Checks that a given display coordinate is in the bounds of the display.
bool display::isPointOnDisplay(const intpair pt)
{
	return pt.first >= 0 && pt.first < MAP_X_SIZE && pt.second >= 0 && pt.second < MAP_Y_SIZE;
}


//	Inverse of the function below: this takes a MAP coordinate and returns the coordinate at which it should
//	appear on the DISPLAY. Both "ctr" and "x","y" are map coordinates.
//	Make sure to check that the returned coordinate is actually IN the display, as some map coordinates will
//	be outside the display area if the map is larger than the window.
intpair display::mapToDisplayCoord(const intpair ctr, int x, int y)
{
	intpair pt;
	pt.first = (MAP_X_SIZE / 2) + (x - ctr.first);
	pt.second = (MAP_Y_SIZE / 2) + (y - ctr.second);
	return pt;
}



//	This converts a coordinate from the DISPLAY GRID, starting at 0,0 in the top left of the display window
//	to its corresponding coordinate on the MAP, where the given point "ctr" appears at the CENTRE of the display.
//	"ctr" is in MAP coordinates. "x" and "y" are the DISPLAY coordinates to convert.
intpair display::displayToMapCoord(const intpair ctr, int x, int y)
{
	intpair pt;
	pt.first = x + (ctr.first - MAP_X_SIZE / 2);
	pt.second = y + (ctr.second - MAP_Y_SIZE / 2);
	return pt;
}


//	A colour that ranged from 0 to 100, spanning from red to yellow to green
colorType display::getPercentColor(int per) const
{
	//	cap at the limits of the percent range
	if		(per < 0) per = 0;
	else if (per > 100) per = 100;
	return _percentColors[per];
}


//	Advances animations, deletes any that have expired.
void display::tickAnimations(gamedataPtr gdata)
{
	vector<animationPtr> toRemove;
	for (auto anim : gdata->_animations)
	{
		anim->tick();
		if (anim->isComplete())
			toRemove.push_back(anim);
	}
	for (auto anim : toRemove)
	{
		auto f = find(gdata->_animations.begin(), gdata->_animations.end(), anim);
		gdata->_animations.erase(f);
	}
}


void display::drawBox(const int x, const int y, const int w, const int h, colorType col)
{
	//	vertical edges
	drawLineVertical(x, y, h, col);
	drawLineVertical(x + w, y, h, col);

	//	horizontal edges
	drawLineHorizontal(x, y, w, col);
	drawLineHorizontal(x, y + h, w, col);

	//	corners
	_win.writec(x + w, y, 191, col);
	_win.writec(x, y + h, 192, col);
	_win.writec(x + w, y + h, 217, col);
	_win.writec(x, y, 218, col);
}


//	The big boxes that outline the normal game interface
void display::drawInterfaceBoxes()
{
	drawBox(2, 2, 46, 46, COLOR_DARK);
	drawBox(50, 2, 37, 46, COLOR_DARK);
	drawBox(2, 50, 85, 12, COLOR_DARK);
}


//	Text with colour formatting. Initially uses a default colour.
//	Upon reaching a '#' character, switches to the next colour in the given colour list.
//	Upon reaching a '@' character, reverts to the default colour.
void display::writeFormatted(int atx, int aty, string txt, vector<colorType> colors, float darkenFactor)
{
	colorType col = COLOR_MEDIUM;
	int colIdx = 0;
	int xp = 0;

	//	if a darkening-factor is given, we adjust all colors by that factor
	if (darkenFactor != 0.0f)
		col.scaleHSV(1.0f, darkenFactor);

	//	draw the message
	for (int i = 0; i < txt.size(); i++)
	{
		if (txt[i] == MSG_COLOR_CHANGE_MARK)
		{
			if (colIdx < colors.size())
			{
				col = colors[colIdx];
				if (darkenFactor != 0.0f)
					col.scaleHSV(1.0f, darkenFactor);
				colIdx++;
			}

			//	hopefully switching to this awful colour will indicate that there's an error
			else
			{
				//cout << "ERROR: Badly formatted message '" + txt + "'" << endl;
				col = TCODColor::lightestPink;
			}
		}
		else if (txt[i] == MSG_DEFAULT_COLOR_MARK)
		{
			col = COLOR_MEDIUM;
			if (darkenFactor != 0.0f)
				col.scaleHSV(1.0f, darkenFactor);
		}
		else
			_win.writec(atx + xp++, aty, txt[i], col);
	}
}


//	A statistic in a little box, 
void display::drawStatWithBox(int x, int y, string val, string title, colorType statcol)
{
	drawBox(x, y, 6, 2, COLOR_DARK);
	_win.write(x + 1, y + 1, centreText(val, 6), statcol);
	_win.write(x + 7, y + 1, title, COLOR_MEDIUM);
}


//	Writes 'title' at x,y and 'val' and x2,y with dots connecting them
void display::drawDottedLinePair(int x, int y, int x2, const string title, const string val, colorType val_col, colorType title_col)
{
	//	the two values
	_win.write(x, y, title, title_col);
	_win.write(x2, y, val, val_col);

	//	the dots
	for (unsigned x3 = x + title.size(); x3 < x2; x3++)
		_win.writec(x3, y, 249, COLOR_DARK);
}


//	Dot-like glyphs form a progress bar, with current value 'filled', and a total of 'maxval' dots drawn.
//	'col' is the colour of filled dots; unfilled dots are grey.
void display::drawProgressDots(int x, int y, int filled, int maxval, colorType col)
{
	//	error testing - if filled is <0, we will hang here
	if (filled < 0)
		return;

	//	draw normally
	for (unsigned i = 0; i < filled; i++)
		_win.writec(x + i, y, 9, col);
	for (unsigned i = filled; i < maxval; i++)
		_win.writec(x + i, y, 9, TCODColor::darkGrey);
}


void display::drawFilledBar(int x, int y, int filled, int maxval, colorType col, string text)
{
	if (filled < 0 || maxval < 0) return;
	for (unsigned i = 0; i < filled; i++)
	{
		if (i < text.size())
			_win.writec(x + i, y, text[i], COLOR_BLACK, col);
		else
			_win.writec(x + i, y, ' ', COLOR_BLACK, col);
	}
	for (unsigned i = filled; i < maxval; i++)
	{
		if (i < text.size())
			_win.writec(x + i, y, text[i], col, TCODColor::darkerGrey);
		else
			_win.writec(x + i, y, ' ', col, TCODColor::darkerGrey);
	}
}


void display::drawLineHorizontal(int x, int y, int width, colorType color)
{
	for (int i = 0; i < width; i++)
		_win.writec(x + i, y, 196, color);
}

void display::drawLineVertical(int x, int y, int height, colorType color)
{
	for (int i = 0; i < height; i++)
		_win.writec(x, y + i, 179, color);
}


//	Percent chance to hit calculator for when we're inspecting a monster.
//	Returns the % chance that an attack with the given Accuracy vs the given Defence will hit.
int display::get_hit_chance(int acc, int def)
{
	int roll_req = (def - acc);
	int total = (20 - roll_req) * 5;
	if		(total < 5)		total = 5;
	else if (total > 100)	total = 100;
	return total;
}


//	Tallies up all types of damage attacker inflicts & accounts for defender's resistances.
int display::calculate_average_damage(const creaturePtr attacker, const creaturePtr target)
{
	//	physical damage
	int total = MAX(1, attacker->getWeaponDamage() - target->getArmourValue());

	//	special damage types
	for (auto dt : SPECIAL_DAMAGE_TYPES)
	{
		auto dam = attacker->getWeaponDamageOfType(dt);
		if (dam > 0)
		{
			auto res = target->getResistance(dt);
			dam -= (float)dam * (float)res / 100.0f;
			if (dam > 0) total += dam;
		}
	}

	return total;
}


//	Returns True if the property should not be displayed when listing an item's properties.
bool display::hide_item_property(const ItemProperty prop) const
{
	return prop == PROP_DAMAGE_VARIANCE || prop == PROP_RESIST_ARCANE || prop == PROP_RESIST_ELECTRIC || prop == PROP_RESIST_FIRE || prop == PROP_RESIST_POISON || prop == PROP_RIPOSTE_DAMAGE;
}
