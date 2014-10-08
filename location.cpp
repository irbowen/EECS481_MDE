#include "location.h"
#include <math.h>

bool Location::makeBigger(double increase) {
	radius += increase;
}

bool Location::makeSmaller(double decrease) {
	radius -= decrease;
}

bool Location::isInLocation(double x, double y) {
	double distance = sqrt(pow(x - center_x, 2) + pow(y - center_y, 2));
	if (distance > radius) {
		return false;
	}
	return true;
}

bool Location::setColor(int r, int b, int y) :
	red(r), blue(b), yellow(y) 
{}
