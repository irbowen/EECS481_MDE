//Isaac Bowen
//eecs481 fall2014

#include <iostream>
class Location {
	int x, y;
	int r, rStart;
	int pressure, targetPressure;
	bool on;
public:
	Location(double, double, double, double);
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
