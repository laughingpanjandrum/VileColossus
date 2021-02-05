#pragma once

#include <string>
#include <vector>
#include <memory>
#include "color_constants.h"
#include "utility.h"

using namespace std;


class animation
{
public:

	animation();
	~animation() {}


	//	Creation

	void addPointToCurrentFrame(const intpair pt, const int glyph, const colorType col);
	void addFrame();
	void prependFrame();


	//	Getting animation data

	const vector<intpair> getPoints() const { return _pts[_at]; }
	const colorType getColor(const int idx) const { return _colors[_at][idx]; }
	const int getGlyph(const int idx) const { return _glyphs[_at][idx]; }


	//	Progressing

	void tick();
	bool isComplete() const { return _at >= _pts.size(); }


private:

	vector<vector<intpair>> _pts;
	vector<vector<int>> _glyphs;
	vector<vector<colorType>> _colors;

	int _at;

};
typedef shared_ptr<animation> animationPtr;



//	Generating specific types of animation.
animationPtr anim_BulletPath(const vector<intpair> path, const colorType col);
animationPtr anim_Explosion(const intpair ctr, const int size, const int gl, const colorType col);
animationPtr anim_FlashGlyph(const intpair pt, const int glyph, const colorType col);
animationPtr anim_MovingText(const intpair pt, const string txt, const colorType col, const intpair vec);
animationPtr anim_Projectile(const vector<intpair> path, const int glyph, const colorType col);


animationPtr padAnimationFront(animationPtr anim, int frames);


//	Glyph representing a line pointing from p1 to p2.
const int getGlyphForVector(intpair p1, intpair p2);