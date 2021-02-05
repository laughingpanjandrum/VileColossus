
#include <ctime>
#include "utility.h"


/*
Call this at program start to seed the random number generator
*/
void initrand()
{
	srand(time(0));
}


//	A += operator for intpair a1.
void intpair_add(intpair * a1, const intpair * a2)
{
	a1->first += a2->first;
	a1->second += a2->second;
}


/*
Returns random integer from a to b inclusive.
If a and b are the same, returns a.
If b < a, throws an error and returns 0.
*/
int randint(int a, int b)
{
	//If the range contains only one value, just return that value
	if (a == b)
		return a;

	//	error checking: b can't be less than a
	else if (b < a)
	{
		cout << "ERROR: Called randint with range " << a << ',' << b << endl;
		return 0;
	}

	// Otherwise return random
	return std::rand() % (b - a + 1) + a;
}

int dieRoll(int dice, int sides)
{
	int total = 0;
	while (dice-- > 0)
		total += randint(1, sides);
	return total;
}

/*
Returns random integer from 0 to num-1.
Mostly useful for picking random array elements using the array size.
*/
int randrange(int num)
{
	return randint(1, num) - 1;
}

bool coinflip()
{
	return randint(1, 2) == 1;
}

//	random roll: 1-in-x, returns true on hit.
bool roll_one_in(int x)
{
	if (x < 1)
		return false;
	return randint(1, x) == 1;
}


//	Returns true [odds]% of the time
bool roll_percent(int odds)
{
	return randint(1, 100) <= odds;
}


//	Rolls a random number in the specified range, which each result being equally likely.
//	Re-rolls if we get a zero (so zero is functionally excluded from the range)
int roll_range_no_zero(int a, int b)
{
	int c = 0;
	while (c == 0)
		c = randint(a, b);
	return c;
}


//	The vector is arranged in pairs: element,weight
//	Elements with higher weights are proportionally more likely to be rolled.
int weightedRandom(vector<int> vals)
{
	//	Sum up weights
	int maxwt = 0;
	for (unsigned i = 1; i < vals.size(); i += 2)
	{
		maxwt += vals[i];
		vals[i] = maxwt;
	}

	//	Make the roll
	int r = randint(1, maxwt);

	//	Find the corresponding value
	for (unsigned i = 1; i < vals.size(); i += 2)
	{
		if (r <= vals[i])
			return vals[i - 1];
	}

	//	Whoops!
	cout << "ERROR! Bad weighted random roll " << r << endl;
	return vals[0];
}


//		Returns a random 2d vector, size 1.
intpair getRandomVector()
{
	auto vec = intpair(randint(-1, 1), randint(-1, 1));
	while (vec == intpair(0, 0))
		vec = intpair(randint(-1, 1), randint(-1, 1));
	return vec;
}

/*
Returns
- minValue if n < minValue;
- maxValue if n < maxValue;
- otherwise, n.
*/
int constrain(int n, int minValue, int maxValue)
{
	if (n < minValue)
		n = minValue;
	else if (n > maxValue)
		n = maxValue;
	return n;
}

/*
Returns:
-1 if destination is less than ourPt
1 if destination is greater than ourPt
0 otherwise
*/
int get1dVector(int ourPt, int destination)
{
	if (destination < ourPt)
		return -1;
	else if (destination > ourPt)
		return 1;
	return 0;
}

intpair get2dVector(intpair v1, intpair v2)
{
	return intpair(get1dVector(v1.first, v2.first), get1dVector(v1.second, v2.second));
}

/*
Returns the given text with whitespace preceding it, so that it will be centred on a line of the given length.
*/
string centreText(std::string txt, int width)
{
	int sp = (width / 2) - (txt.size() / 2) - 1;
	for (sp; sp > 0; sp--)
		txt = " " + txt;
	return txt;
}


/*
Returns given integer as string.
If positive or zero, returns in form "+n"
If negative, returns in form "-n"
*/
std::string plusminus(int value)
{
	std::string t = std::to_string(value);
	if (value >= 0)
		t = '+' + t;
	return t;
}




//	Rounds a float to the given no. of decimal places & returns it in string form.
std::string roundFloatToString(float num, int decimalPlaces)
{
	int v2 = num * pow(10, decimalPlaces);
	auto st = std::to_string(v2);
	if (num < 1)
		st = '0' + st;
	st.insert(st.end() - decimalPlaces, '.');
	return st;
}


//	Adds zeroes to the front of an int until it is the desired length.
string extendInteger(int i, int digits)
{
	string txt = to_string(i);
	while (txt.size() < digits)
		txt = '0' + txt;
	return txt;
}


//	Converts an int to a string and then inserts a decimal point after 'decimalPoint' characters.
//	For example, the arguments (15,1) would return the string "1.5"
//	And (200,2) returns "20.0"
string expressIntAsFloat(int val, int decimalPoint)
{
	string txt = to_string(val);
	while (txt.size() < decimalPoint + 1)
		txt += "0";
	txt.insert(txt.begin() + decimalPoint, '.');
	return txt;
}


//	Returns True if txt1 precedes txt2 alphabetically.
//	Also returns True if the two strings are identical.
bool isFirstAlphabetically(string txt1, string txt2)
{
	for (unsigned int i = 0; i < txt1.size(); i++)
	{
		//	If the two strings are otherwise identical, the shorter string comes first alphabetically.
		if (i >= txt2.size())		return false;

		//	Compare the current character.
		if (txt1[i] < txt2[i])		return true;
		else if (txt2[i] < txt1[i]) return false;
	}

	//	If we end up out here, txt1 may be shorter than txt2 but they are identical otherwise.
	return true;
}


//	Moves an index up or down, and loops around if it goes under zero or over/equal to the max value.
void universalMenuScroll(int * idx, int vec, int maxValue)
{
	*idx += vec;
	if (*idx < 0)				*idx = maxValue - 1;
	else if (*idx >= maxValue)	*idx = 0;
}


//	Splits the given string into two halves at the first occurrence of the given character.
pair<string, string> splitLine(string line, char splitAt)
{
	pair<string, string> data;
	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line[i] == splitAt)
		{
			data.first = line.substr(0, i);
			data.second = line.substr(i + 1, line.size());
			return data;
		}
	}

	//	If the splitting character does not appear, leave the second part empty.
	data.first = line;
	data.second = "";
	return data;
}


//		Removes whitespace from the string. (INCLUDING THE MIDDLE.)
string stripWhitespace(string line)
{
	string	stripped = "";
	for (int i = 0; i < line.size(); i++)
	{
		if (!isWhitespace(line[i]))
			stripped += line[i];
	}
	return stripped;
}

bool isWhitespace(char c)
{
	return c == ' ' || c == '\t';
}

bool isVowel(char c)
{
	return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
		c == 'A' || c == 'E' || c == 'I' || c == 'I' || c == 'U';
}


//	Percent chance to hit the given target on a d20 roll, with the given bonus to the roll.
//	Capped at 5% / 95%, since natural 1s always fail and natural 20s always succeed.
int getHitPercent_d20(int bns, int tgt)
{
	int mustRoll = 21 - (tgt - bns);
	int perc = mustRoll * 5;
	if (perc < 5) perc = 5;
	else if (perc > 95) perc = 95;
	return perc;
}


//	Percent chance that 2d10+bns >= tgt
int getHitPercent_2d10(int bns, int tgt)
{
	int mustRoll = tgt - bns;

	//	a target number of 21 or higher is impossible to hit; a target of 2 or less is impossible to miss
	if		(mustRoll >= 21)	return 0;
	else if (mustRoll <= 2)		return 100;

	//	otherwise, return the actual odds on a 2d10
	return ODDS_2D10[mustRoll - 1];
}

