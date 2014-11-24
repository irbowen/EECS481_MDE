#ifndef LOCATION_H
#define LOCATION_H

#include <gl\glew.h>
#include <Windows.h>
#include <string>
#include "color.h"
#include <mutex>
#include <chrono>
#include <utility>
#include "ColorSlideRing.h"

using namespace std::chrono;
using std::milli;
using std::pair;

//START LOCATION
class Location {
public:
	//~Location();
	ColorSlideRing target;
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

	static int NEXT_LOC_ID;
	int id;
};
//END LOCATION


#endif