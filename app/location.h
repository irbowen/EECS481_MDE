//Isaac Bowen
//eecs481 fall2014

#include <iostream>
class Location {
	double x, y;
	double r, rStart;
	double pressure, targetPressure;
	bool on;
public:
	Location(double, double, double, double);
	~Location();
	void draw();
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
	bool withinPressure(double);
	double distance(double, double);
	void turnOn() { on = true; };
	void turnOff() { on = false; };
	bool isOn() { return on; };
	void setPressure(double); // Also adjust r so it scales down as pressure->targetPressure
	// [pressure = 0 -> r = rStart ;; pressure = targetPressure -> r = rStart / k for some const k]
	double getRadius() { return r; };
};