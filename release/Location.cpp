#include <gl\glew.h>
#include <Windows.h>
#include "location.h"
#include <string>
#include <iostream>
#include <cmath>


//START OF LOCATION
Location::Location(double x_in, double y_in, double r_in, double pressure_in) : rStart{ r_in }, start_pressure{ pressure_in }{
	target = ColorSlideRing(x_in, y_in, r_in, WHITE, RED, GREEN);
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


//START OF LOCPAIR
LocPair::LocPair(double x1_in, double y1_in, double x2_in, double y2_in, double r_in, double pressure_in)
: start{ x1_in, y1_in, r_in, WHITE, RED, GREEN }, destination{ x2_in, y2_in, r_in, WHITE, RED, GREEN }, rStart{ r_in }, start_pressure{ pressure_in }
{
	std::cout << "Created a location at (x, y, r): " << x1_in << " " << y1_in << " " << r_in << std::endl;
	std::cout << "At depth: " << start_pressure << std::endl;
	//on = true;
}

bool LocPair::withinPressure(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 250 && deflection >= TARGET_PRESSURE - 250;
}

bool LocPair::line(LocPair loc){
	const int MAX_X = 640;
	const int MAX_Y = 480;
	ColorSlideRing loc1 = loc.start;
	ColorSlideRing loc2 = loc.destination;
	int last_x = (int)loc1.getX();
	int last_y = (int)loc1.getY();
	int x1 = (int)loc1.getX();
	int y1 = (int)loc1.getY();
	int x2 = (int)loc2.getX();
	int y2 = (int)loc2.getY();

	// draw current track and check whether the line is finished
	while (dist(last_x, x2, last_y, y2) - (int)loc2.getR()>0){
		// Assume minDepth is the cursor

		//get cursor location (400,400) is example for now
		int cursor_x = 400;
		int cursor_y = 400;

		int x = cursor_x % MAX_X; // column
		int y = cursor_y / MAX_X; // row

		if (on_line(x1, y1, x2, y2, x, y))
		{
			//if(dist(x,y,x2,y2)<dist(last_x,last_y,x2,y2)){
			last_x = x;
			last_y = y;
			// OpenGL draw line
			Scene::lines.push_back({ { x1, y1 }, { last_x, last_y }, RED, 5.0 });
			//}
		}
		else
			return false;
	}
	return true;
}

double LocPair::dist(int x1, int y1, int x2, int y2)
{
	int dist_x = x2 - x1;
	int dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x) + (dist_y*dist_y));

	return length;
}

bool LocPair::on_line(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2) - dist(x1, y1, x2, y2)) / dist(x1, y1, x2, y2) < 0.1)
		return true;
	else
		return false;
}

void LocPair::draw(){
	start.draw();
	destination.draw();
}
//END OF LOCPAIR