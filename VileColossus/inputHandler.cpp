#include "inputHandler.h"


inputHandler::inputHandler()
{
	//_kb = getAllKeybindings();
}

//		Converts number key to int. Returns -1 if a number key is not given.
int inputHandler::numberKeyToInt(TCOD_keycode_t vk) const
{
	switch (vk)
	{
	case(TCODK_0):	return 0;
	case(TCODK_1):	return 1;
	case(TCODK_2):	return 2;
	case(TCODK_3):	return 3;
	case(TCODK_4):	return 4;
	case(TCODK_5):	return 5;
	case(TCODK_6):	return 6;
	case(TCODK_7):	return 7;
	case(TCODK_8):	return 8;
	case(TCODK_9):	return 9;
	}
	return -1;
}


bool inputHandler::isDirectionalKeyPressed() const
{
	return _key.c == 'h' || _key.c == 'j' || _key.c == 'k' || _key.c == 'l' || _key.c == 'y' || _key.c == 'u' || _key.c == 'b' || _key.c == 'n' ||
		_key.c == 'H' || _key.c == 'J' || _key.c == 'K' || _key.c == 'L' || _key.c == 'Y' || _key.c == 'U' || _key.c == 'B' || _key.c == 'N';
}

//		Returns movement vector based on the directional key currently pressed.
intpair inputHandler::getVectorFromKeypress() const
{
	switch (_key.c)
	{
	case('H'):
	case('h'):		return intpair(-1, 0);
	case('J'):
	case('j'):		return intpair(0, 1);
	case('K'):
	case('k'):		return intpair(0, -1);
	case('L'):
	case('l'):		return intpair(1, 0);
	case('Y'):
	case('y'):		return intpair(-1, -1);
	case('U'):
	case('u'):		return intpair(1, -1);
	case('B'):
	case('b'):		return intpair(-1, 1);
	case('N'):
	case('n'):		return intpair(1, 1);
	}
	cout << "ERROR: Key is not a direction." << endl;
	return intpair(0, 0);
}

bool inputHandler::isKeypressACharacter() const
{
	auto c = _key.c;
	if (c == ' ') return true;
	string lower = CHAR_SET_LOWERCASE;
	string upper = CHAR_SET_UPPERCASE;
	return find(lower.begin(), lower.end(), c) != lower.end() || find(upper.begin(), upper.end(), c) != upper.end();
}

int inputHandler::getIntFromKeypressChar() const
{
	auto c = _key.c;
	if (c >= 97 && c <= 122)
		return c - 97;
	else if (c >= 65 && c <= 90)
		return c - 39;
	else
		return -1;
}