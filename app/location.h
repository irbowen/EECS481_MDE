//Isaac Bowen
//eecs481 fall2014

class Location {
	double center_x, center_y;
	double radius;
public:
	Location(double, double, double);
	void draw();
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
};