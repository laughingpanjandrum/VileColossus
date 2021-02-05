#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;



//Types
typedef pair<int, int> intpair;

void	intpair_add(intpair* a1, const intpair* a2);


void	initrand();
int		randint(int a, int b);
int		dieRoll(int dice, int sides);
int		randrange(int num);
bool	coinflip();
bool	roll_one_in(int x);
bool	roll_percent(int odds);
int		roll_range_no_zero(int a, int b);
int		weightedRandom(vector<int> vals);

intpair	getRandomVector();

int		constrain(int n, int minValue, int maxValue);
int		get1dVector(int ourPt, int destination);
intpair	get2dVector(intpair v1, intpair v2);
void	universalMenuScroll(int * idx, int vec, int maxValue);

string	centreText(string txt, int width);
string	plusminus(int value);
string	roundFloatToString(float f, int digits);
string	extendInteger(int i, int digits);
string	expressIntAsFloat(int val, int decimalPoint);
bool	isFirstAlphabetically(string txt1, string txt2);

pair<string, string> splitLine(string line, char splitAt = '=');
string	stripWhitespace(string line);
bool	isWhitespace(char c);
bool	isVowel(char c);

int		getHitPercent_d20(int bns, int tgt);
int		getHitPercent_2d10(int bns, int tgt);


//	chance to get a result of [index+1] or better on a 2d10 roll
const int ODDS_2D10[] = { 100, 100, 99, 97, 94, 90, 85, 79, 72, 64, 55, 45, 36, 28, 21, 15, 10, 6, 3, 1 };