#include "animation.h"


animation::animation()
{

}


//	Insert a new data point into the current animation frame.
//	This is a coloured glyph that will be displayed at the given map coordinate when this frame is reached.
//	The frame modified is the MOST RECENT frame.
//	Animations are NOT created with any initial frames, so use addFrame() at least once before calling this!
void animation::addPointToCurrentFrame(const intpair pt, const int glyph, const colorType col)
{
	if (_pts.size() == 0)
		cout << "ERROR: Tried to add data to an animation with no frames!" << endl;
	_pts.back().push_back(pt);
	_glyphs.back().push_back(glyph);
	_colors.back().push_back(col);
}


//	Insert a new empty frame.
void animation::addFrame()
{
	_pts.push_back(vector<intpair>());
	_colors.push_back(vector<colorType>());
	_glyphs.push_back(vector<int>());
}


//	Insert an empty frame at the FRONT of the animation.
void animation::prependFrame()
{
	_pts.insert(_pts.begin(), vector<intpair>());
	_colors.insert(_colors.begin(), vector<colorType>());
	_glyphs.insert(_glyphs.begin(), vector<int>());
}


//	Advance to the next frame.
void animation::tick()
{
	if (_at < _pts.size())
		_at++;
}



//	Displays a single glyph for a few frames.
animationPtr anim_FlashGlyph(const intpair pt, const int glyph, const colorType col)
{
	auto a = animationPtr(new animation());
	for (unsigned i = 0; i < 3; i++)
	{
		a->addFrame();
		a->addPointToCurrentFrame(pt, glyph, col);
	}
	return a;
}


//	Text appears on the map, first drawn at the given point, then moving away along the given vector.
animationPtr anim_MovingText(const intpair pt, const string txt, const colorType col, const intpair vec)
{
	auto a = animationPtr(new animation());
	auto left = pt;
	for (unsigned i = 0; i < 3; i++)
	{
		//	add each character from the string
		for (unsigned n = 0; n < 3; n++)
		{
			a->addFrame();
			for (unsigned c = 0; c < txt.size(); c++)
			{
				auto at = intpair(left.first + c, left.second);
				a->addPointToCurrentFrame(at, txt[c], col);
			}
		}

		//	move along the vector
		intpair_add(&left, &vec);
	}
	return a;
}


//	Like 'bulletPath', but shows a fixed glyph rather than a line pointing to the target.
animationPtr anim_Projectile(const vector<intpair> path, const int glyph, const colorType col)
{
	auto a = animationPtr(new animation());

	//	Add each point to the animation.
	for (unsigned i = 0; i < path.size(); i++)
	{
		a->addFrame();
		a->addPointToCurrentFrame(path[i], glyph, col);
	}

	return a;
}


//	Adds empty framges to the start of the animation.
animationPtr padAnimationFront(animationPtr anim, int frames)
{
	while (frames-- > 0)
		anim->prependFrame();
	return anim;
}


//	Glyph travels along the given path. The glyph used for each point is a line pointing at the next point.
animationPtr anim_BulletPath(const vector<intpair> path, const colorType col)
{
	auto a = animationPtr(new animation());

	//	Add each point to the animation.
	for (unsigned i = 0; i < path.size(); i++)
	{
		//	Glyph pointing to the next point.
		int gl;
		if (i < path.size() - 1)
			gl = getGlyphForVector(path[i], path[i + 1]);

		//	If we're at the end of the path, the glyph points to the previous point instead
		else if (i > 0)
			gl = getGlyphForVector(path[i - 1], path[i]);

		//	If there is no next OR previous point (ie the path is length 1), just use a dash
		else
			gl = '-';

		//	Add the frame.
		a->addFrame();
		a->addPointToCurrentFrame(path[i], gl, col);
	}

	return a;
}

animationPtr anim_Explosion(const intpair ctr, const int size, const int gl, const colorType col)
{
	auto a = animationPtr(new animation());
	auto ccol = col;
	for (unsigned r = 0; r < size; r++)
	{
		a->addFrame();
		for (int x = ctr.first - r; x <= ctr.first + r; x++)
		{
			a->addPointToCurrentFrame(intpair(x, ctr.second), gl, ccol);
			a->addPointToCurrentFrame(intpair(x, ctr.second + r), gl, ccol);
		}
		for (int y = ctr.second - r; y <= ctr.second + r; y++)
		{
			a->addPointToCurrentFrame(intpair(ctr.first, y), gl, ccol);
			a->addPointToCurrentFrame(intpair(ctr.first + r, y), gl, ccol);
		}
		ccol.scaleHSV(1.0f, 1.1f);
	}
	return a;
}




/*
\|/
- -
/|\
*/
const int getGlyphForVector(intpair p1, intpair p2)
{
	if (p1.first < p2.first)
	{
		if		(p1.second < p2.second)	return '\\';
		else if (p1.second > p2.second)	return '/';
		else							return 196;
	}
	else if (p1.first > p2.first)
	{
		if		(p1.second < p2.second)	return '/';
		else if (p1.second > p2.second)	return '\\';
		else							return 196;
	}
	else
	{
		if (p1.second == p2.second)		return '*';
		else							return 179;
	}
}
