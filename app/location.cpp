//Isaac Bowen
//eecs481 fall2014

#include "location.h"
#include <math.h>

void Location::draw() {}

Location::Location(double x_in, double y_in, double r_in) : x{ x_in }, y{ y_in }, r{ r_in } {}

void Location::makeBigger(double increase) {
	r += increase;
}

void Location::makeSmaller(double decrease) {
	r -= decrease;
}

bool Location::contains(double x_in, double y_in) {
	if (distance(x_in, y_in) > r) {
		return false;
	}
	return true;
}
bool Location::withinPressure(double input) {
	return true;//for now
	//TODO
}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(x - loc.x, 2) + pow(y - loc.y, 2));
}
