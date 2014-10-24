//Isaac Bowen
//eecs481 fall2014

#include "location.h"
#include <math.h>

void Location::draw() {}

Location::Location(double x_in, double y_in, double r_in) : x{ x_in }, y{ y_in }, r{ r_in } {
	std::cout << "Created a location at (x, y, r): " << x << " " << y << " " << r << std::endl;
	visible = true;
}

Location::~Location() {
	//std::cout << "Deleteing location at (x, y, r): " << x << " " << y << " " << r << std::endl;
}

void Location::makeBigger(double increase) {
	r += increase;
}

void Location::makeSmaller(double decrease) {
	r -= decrease;
}

bool Location::contains(double x_in, double y_in) {
	return distance(x_in, y_in) < r;
}

bool Location::withinPressure(double input) {
	return true;//TODO
}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(x - x_in, 2) + pow(y - y_in, 2));
}
