#include "CursorCircle.h"
#include "Color.h"
#include <utility>

using std::pair;

//START OF CURSORCIRCLE
CursorCircle::CursorCircle(double xx, double yy, double rr, const Color& cc, const std::pair<double, double>& end, double ms) : Circle{ xx, yy, rr, cc }, startPos{ xx, yy }, endPos{ end } {
	fade(ms);
	startR = r;
	startPos = { x, y };
}

void CursorCircle::draw(){
	double ms = elapsed();

	r = startR * ms / fadeDuration;
	auto pos = between(endPos, startPos, ms / fadeDuration);
	x = pos.first, y = pos.second;

	if (color == WHITE)
		return;

	Circle::draw();
}
//END OF CURSORCIRCLE