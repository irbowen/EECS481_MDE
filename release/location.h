#ifndef LOCATION_H
#define LOCATION_H

#include <gl\glew.h>
#include <Windows.h>
#include "graphics.h"
#include <string>
#include "color.h"
#include <mutex>


class ColorSlideRing;
//START LOCATION
class Location {
public:
	ColorSlideRing &target;
	double rStart;
	double TARGET_PRESSURE = 500;
	const double MAX_RADIUS = 50;
	double pressure, start_pressure;
	bool on;
	int num_rounds_correct = 0, prev_correct_round = 0;
	Color color;
	Location(double, double, double, double);
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
	bool withinPressure(double);
	bool exactMatch(double);
	double getPercentage(double);
	double distance(double, double);
	void turnOn() { on = true; };
	void turnOff() { on = false; };
	bool isOn() const { return on; };
	void setPressure(double);
	double getRadius() const;
	double getX() const;
	double getY() const;
	void draw();
	std::string toString() const;

	void fade(double ms);
};
//END LOCATION

//START LOCPAIR
class LocPair{
public:
	bool locked = false;
	//bool on;
	double TARGET_PRESSURE = 500;
	ColorSlideRing& start;
	ColorSlideRing& destination;
	double rStart;
	double start_pressure;
	LocPair(double, double, double, double, double, double);
	void draw();
	bool line(LocPair loc);
	bool withinPressure(double input);
	double dist(int x1, int y1, int x2, int y2);
	bool on_line(int x1, int y1, int x2, int y2, int x3, int y3);
};
//END LOCPAIR

#endif