//Isaac Bowen
//eecs481 fall2014

class Location {
public:
	double x, y;
	double r;
	double pressure;
	Location(double, double, double);
	void draw();
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
	bool withinPressure(double);
	double distance(double, double);
};