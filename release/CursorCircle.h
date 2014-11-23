#ifndef CURSORCIRCLE_H
#define CURSORCIRCLE_H

#include <utility>
#include "Circle.h"

using std::pair;

//START OF CURSORCIRCLE
class CursorCircle : public Circle {
	double startR;
	pair<double, double> startPos;
	pair<double, double> endPos;
public:
	CursorCircle(double xx, double yy, double rr, const Color& cc, const pair<double, double>& end, double ms);
	void draw();
};
//END OF CURSORCIRCLE

#endif