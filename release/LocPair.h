#ifndef LOCPAIR_H
#define LOCPAIR_H

#include "Color.h"
#include "ColorSlideRing.h"


//START LOCPAIR
class LocPair{
public:
	LocPair();
	LocPair(double, double, double, double, double, double);

	bool locked = false;
	//bool on;
	double TARGET_PRESSURE = 500;
	double pressure;
	ColorSlideRing start;
	ColorSlideRing destination;
	Color start_color;
	Color dest_color;
	void start_setPressure(double);
	double start_getPercentage(double);
	void dest_setPressure(double);
	double dest_getPercentage(double);
	double rStart;
	double start_pressure;
	void draw();
	//bool line();
	bool withinPressure(double input);
	//double dist(int x1, int y1, int x2, int y2);
	//bool on_line(int x1, int y1, int x2, int y2, int x3, int y3);
};
//END LOCPAIR


#endif