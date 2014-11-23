#ifndef COLORSLIDECIRCLE_H
#define COLORSLIDECIRCLE_H
#include "color.h"
#include "Circle.h"

//START OF COLORSLIDECIRCLE
class ColorSlideCircle : public Circle {
	Color startColor;
	Color endColor;
public:
	ColorSlideCircle(double xx, double yy, double rr, Color cStart, Color cEnd) : Circle{ xx, yy, rr, cStart }, startColor{ cStart }, endColor{ cEnd } {}
	void setGoalProgress(double percent);
};
//END OF COLORSLIDECIRCLE

#endif