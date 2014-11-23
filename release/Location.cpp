#include <gl\glew.h>
#include <Windows.h>
#include "location.h"
#include <string>
#include <iostream>
#include <cmath>
#include <sstream>

#define PI 3.14159265

//START OF LOCATION
Location::Location(double x_in, double y_in, double r_in, double pressure_in) : rStart{ r_in }, start_pressure{ pressure_in }, target{ x_in, y_in, r_in, WHITE, RED, GREEN }{
	std::cout << "Created a location at (x, y, r, z): " << x_in << " " << y_in << " " << r_in << pressure_in << std::endl;
	turnOn();
}

void Location::makeBigger(double increase) {
	if (target.getR() < MAX_RADIUS)
		target.setR(target.getR() + increase);
}

std::string Location::toString() const {
	std::ostringstream ss;
	ss << "X: " << getX() << "Y: " << getY() << " R: " << getRadius() << " start_pressure: " << start_pressure << "\n";
	return ss.str();
}

void Location::makeSmaller(double decrease) {
	target.setR(target.getR() - decrease);
}

bool Location::contains(double x_in, double y_in) {
	return distance(x_in, y_in) < target.getR();
}

double Location::getPercentage(double input) {
	double deflection = abs(input - start_pressure);
	return (deflection / TARGET_PRESSURE <= 1.0 &&
		deflection / TARGET_PRESSURE >= 0.0) ?
		deflection / TARGET_PRESSURE : 0.0;
}

bool Location::withinPressure(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 250 && deflection >= TARGET_PRESSURE - 250;
}

bool Location::exactMatch(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 200 && deflection >= TARGET_PRESSURE - 200;

}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(target.getX() - x_in, 2) + pow(target.getY() - y_in, 2));
}

void Location::setPressure(double in) {
	pressure = in;
	if (in == 0)
		target.setGoalProgress(0);
	else
		target.setGoalProgress(getPercentage(in));
}

double Location::getRadius() const{
	return target.getR();
}

double Location::getX() const{
	return target.getX();
}

double Location::getY() const{
	return target.getY();
}

void Location::draw(){
	target.draw();
}

void Location::fade(double ms){
	target.fade(ms);
}
//END OF LOCATION


//END OF LOCPAIR