#pragma once

#include <string>
#include <memory>

#include "color_constants.h"
#include "libtcod.hpp"
#include "utility.h"

using namespace std;



#define CHAR_SET_LOWERCASE "abcdefghijklmnopqrstuvwxyz"
#define CHAR_SET_UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"



enum buttonAction
{
	BTN_NONE,
};



class inputHandler
{
public:

	inputHandler();
	~inputHandler() {}

	//					Check for new events
	void				checkForEvents() { TCODSystem::checkForEvent(TCOD_EVENT_ANY, &_key, &_mouse); }
	void				checkForKeypress() { TCODSystem::checkForEvent(TCOD_EVENT_KEY_RELEASE, &_key, &_mouse); }
	void				waitForKeypress() { TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &_key, &_mouse, true); }

	//					modifier keys
	bool				isCtrlPressed() const { return _key.lctrl || _key.rctrl; }
	bool				isShiftKeyPressed() const { return _key.shift; }

	//					Checking key pressed
	bool				isKeyPressed() const { return _key.pressed; }
	bool				isKeyPressed(TCOD_keycode_t kp) const { return _key.vk == kp; }
	bool				isKeyPressed(char c) const { return _key.c == c; }

	//					Number keys
	bool				isNumberKey() const { return isNumberKey(_key.vk); }
	bool				isNumberKey(TCOD_keycode_t vk) const { return numberKeyToInt(vk) != -1; }
	int					numberKeyToInt(TCOD_keycode_t vk) const;
	int					numberKeyToInt() const { return numberKeyToInt(_key.vk); }

	//					Movement keys
	bool				isDirectionalKeyPressed() const;
	intpair				getVectorFromKeypress() const;

	//					Getting characters
	char				getKeypressCharacter() const { return _key.c; }
	bool				isKeypressACharacter() const;
	int					getIntFromKeypressChar() const;

	//					Mouse info
	intpair				getMouseCoord() const { return intpair(_mouse.cx, _mouse.cy); }


private:

	TCOD_key_t			_key;
	TCOD_mouse_t		_mouse;

};
typedef shared_ptr<inputHandler> inputHandlerPtr;
