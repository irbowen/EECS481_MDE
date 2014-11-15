#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <iostream>
#include <deque>
#include <utility>
#include <unordered_set>

using std::vector;
using std::deque;
using std::unordered_set;

struct Color {
	double r, g, b;
	Color(double rr, double gg, double bb) : r{ rr }, g{ gg }, b{ bb } {};
	Color() {};
};

std::ostream& operator<<(std::ostream& os, const Color& c);

#define RED Color{1.0f, 0.0f, 0.0f}
#define BLUE Color{0.0f, 0.0f, 1.0f}
#define GREEN Color{0.0f, 1.0f, 0.0f}

#define CYAN Color{0.0, 1.0, 1.0}
#define MAGENTA Color{1.0, 0.0, 1.0}
#define YELLOW Color{1.0, 1.0, 0.0}

#define PINK Color{1.0, 0.51, 0.75}
#define ORANGE Color{0.98, 0.45, 0.02}
#define TEAL Color{0.01, 0.58, 0.53}

#define PURPLE Color{0.5, 0.12, 0.61}
#define TURQUOISE Color{0.02, 0.76, 0.67}

#define WHITE Color{1.0, 1.0, 1.0}


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
public:
	double x, y;
	double r;
	Color color;
	Circle(double xx, double yy, double rr, Color cc) : x{ xx }, y{ yy }, r{ rr }, color{ cc } {}
	Circle(double xx, double yy, double rr, Color cc, int aa) : x{ xx }, y{ yy }, r{ rr }, color{ cc }, angle{ aa } {}
	void draw();
	int angle = 0;
};

class ColorSlideCircle : public Circle {
	Color startColor;
	Color endColor;
public:
	ColorSlideCircle(double xx, double yy, double rr, Color cStart, Color cEnd) : Circle{ xx, yy, rr, cStart }, startColor{ cStart }, endColor{ cEnd } {}

	void setGoalProgress(double percent);

};

class ColorSlideRing {
public:

	ColorSlideCircle ring;
	ColorSlideCircle center;
	ColorSlideRing(double x, double y, double r, Color centerStart, Color ringStart, Color end) : ring{ x, y, r, ringStart, end }, center{ x, y, r * 0.9, centerStart, end } {}

	inline void setGoalProgress(double percent){ ring.setGoalProgress(percent), center.setGoalProgress(percent); }

	inline void setR(double r) { ring.r = r, center.r = r * 0.9; }

	inline double getR(){ return ring.r; }

	inline double getX(){ return ring.x; }

	inline double getY(){ return ring.y; }

	inline void draw() { ring.draw(), center.draw(); }
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
	//using CirclePath::CirclePath;
public:
	CircleSpiral(double x, double y, double r, Color c) : CirclePath{ x, y, r, c } {}
	void addCircle(int);
};

class PolygonGL {
private:
	vector<std::pair<int, int>> vertices;
	Color color;
public:
	PolygonGL(vector<std::pair<int, int>> inVert, Color c) : color{ c } { vertices = inVert; }
	void draw();
	bool inside(double, double);
};

class Point {
	double x, y;
	Color color;
public:
	Point(double xx, double yy, Color c) : x{ xx }, y{ yy }, color{ c } {}
	void draw();
};

class Location;

class Scene {
public:
	unordered_set<Location*> targets;
	vector<CirclePath> paths;
	vector<CircleSpiral> spirals;
	vector<PolygonGL> polys;
	vector<ColorSlideRing> rings;
	vector<Point> points;


	Scene() {} // initialize size and location of fixed targets
	void draw();

	void startPath(double x, double y, double r, Color c) { paths.push_back(CirclePath{ x, y, r, c }); }
	void startSpiral(double x, double y, double r, Color c) { spirals.push_back(CircleSpiral{ x, y, r, c }); }
};

extern Scene scene;

class Location {
public:
	ColorSlideRing target;
	double rStart;
	const double TARGET_PRESSURE = 500;
	double pressure;
	double start_pressure;
	bool on;
	Color color;
	Location(double, double, double, double);
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
	bool withinPressure(double);
	bool exactMatch(double);
	double getPercentage(double);
	double distance(double, double);
	void turnOn() { on = true; scene.targets.insert(this); };
	void turnOff() { on = false; scene.targets.erase(this); };
	bool isOn() { return on; };
	void setPressure(double);
	double getRadius() { return target.getR(); };
	double getX() { return target.getX(); }
	double getY() { return target.getY(); }
	void draw() { target.draw(); }
};

#endif