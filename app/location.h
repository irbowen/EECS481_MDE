//Isaac Bowen
//eecs481 fall2014

#include <iostream>
class Location {
public:
	double x, y;
	double r;
	double pressure;
	bool visible;
	Location(double, double, double);
	~Location();
	void draw();
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
	bool withinPressure(double);
	double distance(double, double);
};