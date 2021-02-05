#include "progression.h"


//	Advance a level, gain the requisite bonuses, restore all health.
void playerGainLevel(gamedataPtr gdata)
{
	gdata->_player->_level++;
	gdata->_attributePointsLeft += 3;
	gdata->_player->healToMax();
	messages::add(gdata, " #*** YOU HAVE ATTAINED LEVEL " + to_string(gdata->_player->_level) + "! ***", { COLOR_POSITIVE });
}


//	XP awarded for killing a monster. The amount depends on our relative level & the monster's tier.
void addKillXP(gamedataPtr gdata, monsterPtr target)
{
	//	Base value from monster tier
	int base_xp;
	switch (target->_tier)
	{
	case(1):	base_xp = XP_PER_LEVEL / 100; break;
	case(2):	base_xp = XP_PER_LEVEL / 50; break;
	case(3):	base_xp = XP_PER_LEVEL / 10; break;
	case(4):	base_xp = XP_PER_LEVEL / 5; break;
	default:
		base_xp = 0;
	}

	//	Gain XP faster at lower levels.
	if (gdata->_player->_level < 2)
		base_xp *= 5;
	else if (gdata->_player->_level < 3)
		base_xp *= 2;

	//	Monsters of higher level grant slightly more xp
	int lvl_diff = target->_level - gdata->_player->_level;
	if (lvl_diff > 0)
		base_xp += (float)base_xp * (float)lvl_diff * 0.1f;

	//	Monsters of a lower level grant progressively less xp
	else if (lvl_diff < 0)
	{
		switch (-lvl_diff)
		{
		case(1):	base_xp /= 2; break;
		case(2):	base_xp /= 4; break;
		case(3):	base_xp /= 8; break;
		case(4):	base_xp /= 20; break;
		case(5):	base_xp /= 100; break;
		default:
			base_xp = 0;
		}
	}

	//	Grant it, check for level-up
	gdata->_xp += base_xp;
	if (gdata->_xp >= XP_PER_LEVEL)
	{
		gdata->_xp -= XP_PER_LEVEL;
		playerGainLevel(gdata);
	}

	//	We also recharge our town portal ability as we gain xp
	gdata->_townPortalCharge = MIN(TOWN_PORTAL_CHARGE_REQ, gdata->_townPortalCharge + base_xp);
}


//	Raise the selected stat, if we have a point available.
void spendAttributePoint(gamedataPtr gdata)
{
	if (gdata->_attributePointsLeft > 0)
	{
		if (gdata->_idx < ATTR__MAX)
		{
			auto attr = static_cast<Attribute>(gdata->_idx);
			gdata->_player->raiseAttribute(attr);
			gdata->_attributePointsLeft--;
		}
	}
}


//	Spend attribute points
void openAttributeMenu(gamedataPtr gdata)
{
	gdata->_state = STATE_SPEND_ATTRIBUTE_POINTS;
	gdata->_idx = 0;
}


//	Tests whether our town portal is available to cast.
bool isTownPortalCharged(gamedataPtr gdata)
{
	return gdata->_townPortalCharge >= TOWN_PORTAL_CHARGE_REQ;
}
