#ifndef COLORSLIDERING_H
#define COLORSLIDERING_H

#include "ColorSlideCircle.h"
#include "color.h"

//START OF COLORSLIDERING
class ColorSlideRing {
public:
	ColorSlideCircle ring;
	ColorSlideCircle center;
	ColorSlideRing(double x, double y, double r, Color centerStart, Color ringStart, Color end) : ring{ x, y, r, ringStart, end }, center{ x, y, r * 0.9, centerStart, end } {}
	inline void setGoalProgress(double percent){ ring.setGoalProgress(percent), center.setGoalProgress(percent); }
	inline void setR(double r) { ring.r = r, center.r = r * 0.9; }
	inline double getR() const { return ring.r; }
	inline double getX() const { return ring.x; }
	inline double getY() const { return ring.y; }
	inline void draw() { ring.draw(), center.draw(); }
	inline void fade(double ms) { ring.fade(ms), center.fade(ms); }
};
//END OF COLORSLIDERING

#endif