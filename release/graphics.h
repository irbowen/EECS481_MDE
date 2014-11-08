#include <vector>
#include <deque>

using std::vector;
using std::deque;

struct Color;

struct Color {
	double r, g, b;
	Color(double rr, double gg, double bb) : r{ rr }, g{ gg }, b{ bb } {}
};
#define RED Color{1.0f, 0.0f, 0.0f}
#define BLUE Color{0.0f, 0.0f, 1.0f}
#define GREEN Color{0.0f, 1.0f, 0.0f}


class Location {
public:
	double x, y;
	double r, rStart;
	double pressure, targetPressure;
	bool on;
	Color color;
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

/*
class Target {
	int x, y;
	int r, rStart;
	int pressure, targetPressure;
	bool on;
public:
	Target(int xx, int yy, int rr, int target) : x{ xx }, y{ yy }, r{ rr }, rStart{ rr }, targetPressure{ target }, on{ false } {}
	void draw(); // Draw circle if on
	inline void turnOn() { on = true; }
	inline void turnOff() { on = false; }
	void setPressure(int); // Also adjust r so it scales down as pressure->targetPressure
	// [pressure = 0 -> r = rStart ;; pressure = targetPressure -> r = rStart / k for some const k]
	bool hitTarget(); // Is pressure close enough to targetPressure?
};
*/

bool operator==(Color& a, Color& b);

class Circle {
	double x, y;
	double r;
	Color color;
	friend class CirclePath;
	friend class CircleSpiral;
public:
	Circle(double xx, double yy, double rr, Color cc) : x{ xx }, y{ yy }, r{ rr }, color{ cc } {}
	Circle(double xx, double yy, double rr, Color cc, int aa) : x{ xx }, y{ yy }, r{ rr }, color{ cc }, angle{ aa } {}
	void draw();
	int angle = 0;
};

class CirclePath {
protected:
	deque<Circle> circles;
public:
	CirclePath(double x, double y, double r, Color c) { circles.push_front({ x, y, r, c }); }
	void draw() { for (auto it = circles.rbegin(); it != circles.rend(); ++it){ it->draw(); } }

	// Add different sized circle to path in random direction
	virtual void addCircle(int);

	inline void removeCircle() { if (circles.size() > 1) circles.pop_front(); }
};

class CircleSpiral : public CirclePath {
	using CirclePath::CirclePath;
public:
	CircleSpiral(double x, double y, double r, Color c) : CirclePath{ x, y, r, c } {}
	void addCircle(int);
};
/*
class Scene {
public:
	vector<Target> targets;
	vector<CirclePath> paths;
	vector<CircleSpiral> spirals;


	Scene() {} // initialize size and location of fixed targets
	void draw() { for (CirclePath cp : paths) { cp.draw(); } for (CircleSpiral cs : spirals) { cs.draw(); } }

	void startPath(double x, double y, double r, Color c) { paths.push_back(CirclePath{ x, y, r, c }); }
	void startSpiral(double x, double y, double r, Color c) { spirals.push_back(CircleSpiral{ x, y, r, c }); }
};*/