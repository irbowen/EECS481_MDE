#include <vector>
#include <deque>

using std::vector;
using std::deque;

struct Color;

#define RED Color{1.0f, 0.0f, 0.0f}
#define BLUE Color{0.0f, 1.0f, 0.0f}
#define GREEN Color{0.0f, 0.0f, 1.0f}


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

struct Color {
	double r, b, g;
	Color(double rr, double bb, double gg) : r{ rr }, b{ bb }, g{ gg } {}
};

bool operator==(Color& a, Color& b);

class Circle {
	double x, y;
	double r;
	Color color;
	friend class CirclePath;
public:
	Circle(double xx, double yy, double rr, Color cc) : x{ xx }, y{ yy }, r{ rr }, color { cc } {}
	void draw();
};

class CirclePath {
	deque<Circle> circles;
public:
	CirclePath(double x, double y, double r, Color c) { circles.push_front({ x, y, r, c }); }
	void draw() { for (auto it = circles.rbegin(); it != circles.rend(); ++it){ it->draw(); } }

	// Add different sized circle to path in random direction
	void addCircle();

	inline void removeCircle() { if (circles.size() > 1) circles.pop_front(); }
};

class Scene {
public:
	vector<Target> targets;
	vector<CirclePath> paths;


	Scene() {} // initialize size and location of fixed targets
	void draw() { for (CirclePath cp : paths) { cp.draw(); } }

	void startPath(double x, double y, double r, Color c) { paths.push_back(CirclePath{ x, y, r, c }); }

};