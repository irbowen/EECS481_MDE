#ifndef CIRCLE_H
#define CIRCLE_H

#include <gl\glew.h>
#include <Windows.h>
#include <string>
#include "color.h"
#include <chrono>
#include <utility>
#include "geometry.h"

using namespace std::chrono;
using std::milli;

// START OF CIRCLE
class Circle {
protected:
	high_resolution_clock::time_point fadeStart;
	Color startColor;

public:
	double fadeDuration = 0.0;
	inline double elapsed() const { return duration_cast<duration<double, milli>>(high_resolution_clock::now() - fadeStart).count(); }
	double x, y;
	double r;
	Color color;
	Circle(double xx, double yy, double rr, Color cc) : x{ xx }, y{ yy }, r{ rr }, color{ cc } {}
	virtual void draw();
	void fade(double);
	bool faded = false;
};
//END OF CIRCLE

#endif