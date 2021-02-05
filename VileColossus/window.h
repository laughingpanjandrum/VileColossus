
#include <string>
#include "libtcod.hpp"

using namespace std;


//Colour constants
#define SCREEN_BACKGROUND_COLOR TCODColor::black


#ifndef WINDOW_H
#define WINDOW_H

class window
{
public:

	window();
	window(string name, int xsize, int ysize);

	//Debug
	void			drawFont();

	//Console
	TCODConsole*	mainConsole;
	
	//Writing
	void			writec(int x, int y, int c, TCODColor col, TCODColor bgcol = SCREEN_BACKGROUND_COLOR, bool toAltConsole = false);
	void			write(int x, int y, string txt, TCODColor col, TCODColor bgcol = SCREEN_BACKGROUND_COLOR, bool toAltConsole = false);
	int				writeWrapped(int x, int y, unsigned int lineLen, std::string txt, 
		TCODColor col, TCODColor bgcol = SCREEN_BACKGROUND_COLOR,
		bool toAltConsole = false);
	
	//Clearing/refreshing
	void			clearRegion(int x, int y, int w, int h);
	void			clear();
	void			refresh();
	
	//Utility
	TCODColor		mixColors(TCODColor col1, TCODColor col2, float perc);
	bool			isPunctuator(char c);
	
	//Input
	string			getstr(int x, int y, std::string prompt = "~");
	TCOD_key_t		getkey();
	TCOD_key_t		waitForKeypress();
	
	//Special shape-drawing functions
	void			drawBox(int leftx, int topy, int width, int height, TCODColor col);

	//Image drawing
	//void drawImage(imageSharedPtr img, int atx, int aty);
	//void drawImage(imagePtr img, int atx, int aty);

private:

	//Constants used in drawing
	int BOTTOM_LEFT_CORNER = 200;
	int TOP_LEFT_CORNER = 201;
	int TOP_RIGHT_CORNER = 187;
	int BOTTOM_RIGHT_CORNER = 188;
	int VERTICAL_BAR = 186;
	int HORIZONTAL_BAR = 205;

};

#endif

