#ifndef LOCPAIR_H
#define LOCPAIR_H

#include "location.h"
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
	ColorSlideRing dest;
	Color start_color;
	Color dest_color;

	bool withinPressure(double input);
	void start_setPressure(double);
	double start_getPercentage(double);
	void dest_setPressure(double);
	double dest_getPercentage(double);
	double rStart;
	double start_pressure;
	void draw();
};
//END LOCPAIR


#endif