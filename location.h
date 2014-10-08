//Isaac Bowen
//eecs481 fall2014

class Location {
	//center point
	double center_x, center_y;
	double radius;
	//color
	int red, blue, yellow;
public:
	Location(double, double, double);
	void draw();
	//return bool based on success/failure of operation
	bool makeBigger(double);
	bool makeSmaller(double);
	bool contains(double, double);
	bool setColor(int, int, int);
	bool gradualColorChange(int, int, int);
};