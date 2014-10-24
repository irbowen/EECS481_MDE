//Isaac Bowen
//eecs481 fall2014

#include "location.h"
#include <math.h>

void Location::draw() {}

Location::Location(double x, double y, double r) : 
	center_x{ x }, center_y{ y }, radius{ r } {}

void Location::makeBigger(double increase) {
	radius += increase;
}

void Location::makeSmaller(double decrease) {
	radius -= decrease;
}

bool Location::contains(double x, double y) {
	double distance = sqrt(pow(x - center_x, 2) + pow(y - center_y, 2));
	if (distance > radius) {
		return false;
	}
	return true;
}
