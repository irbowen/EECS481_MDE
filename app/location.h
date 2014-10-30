//Isaac Bowen
//eecs481 fall2014

#include <iostream>
class Location {
	int x, y;
	int r, rStart;
	int pressure, targetPressure;
	bool on;
public:
	Location(int, int, int, int);
	void draw();
	void makeBigger(int);
	void makeSmaller(int);
	bool contains(int, int);
	bool withinPressure(int);
	int distance(int, int);
	void turnOn() { on = true; };
	void turnOff() { on = false; };
	bool isOn() { return on; };
	void setPressure(int); // Also adjust r so it scales down as pressure->targetPressure
	// [pressure = 0 -> r = rStart ;; pressure = targetPressure -> r = rStart / k for some const k]
	int getRadius() { return r; };
};
