//Isaac Bowen
//eecs481 fall2014
//Base class for storting targets

class Location {
	//center point
	double center_x, center_y;
	double radius;
	//color
	int red, blue, yellow;
public:
	Location(double, double, double);

	bool makeBigger(double);

	bool makeSmaller(double);

	bool isInLocation(double, double);

	bool setColor(int, int, int);
};