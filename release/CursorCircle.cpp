#include "CursorCircle.h"
#include "Color.h"
#include <utility>

using std::pair;

//START OF CURSORCIRCLE
CursorCircle::CursorCircle(double xx, double yy, double rr, const Color& cc, const pair<double, double>& start, double ms, double dCursor, int aCursor) : 
								Circle{ xx, yy, rr, cc }, 
								startPos{ start }, 
								endPos{ xx, yy },
								distanceToCursor{ dCursor },
								angleToCursor{ aCursor }{
	fade(ms);
	startR = r;
}

void CursorCircle::draw(){
	double ms = elapsed();

	r = startR * ms / fadeDuration;

	endPos = jump(startPos, distanceToCursor, angleToCursor--);

	auto pos = between(startPos, endPos, ms / fadeDuration);
	x = pos.first, y = pos.second;

	if (ms / fadeDuration >= 0.95)
		faded = true;

	Circle::draw();
}
//END OF CURSORCIRCLE