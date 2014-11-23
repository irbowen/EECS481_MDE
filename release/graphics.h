#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <iostream>
#include <deque>
#include <utility>
#include <unordered_set>
#include <sstream>
#include <chrono>
#include <list>


using namespace std::chrono;

using std::milli;

using std::vector;
using std::deque;
using std::unordered_set;
using std::pair;

using std::list;

struct Color {
	double r, g, b;
	Color(double rr, double gg, double bb) : r{ rr }, g{ gg }, b{ bb } {}
	Color(int rr, int gg, int bb) : r{ rr / 255.0 }, g{ gg / 255.0 }, b{ bb / 255.0 } {}
	Color() {}
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


// all predefined colors
extern vector<Color> colors;

// predefined color schemes
extern vector<Color> colorScheme_bleu;
extern vector<Color> colorScheme_desert;
extern vector<Color> colorScheme_ocean;
extern vector<Color> colorScheme_goldfish;
extern vector<Color> colorScheme_melon;
extern vector<Color> colorScheme_emo;

extern vector<Color> colorScheme_rainbow;

// pointers to predefined schemes
extern vector<vector<Color>*> colorSchemes;

#define WHITE Color{1.0, 1.0, 1.0}

bool operator==(const Color& a, const Color& b);

class Circle {
protected:
	high_resolution_clock::time_point fadeStart;
	Color startColor;

public:

	double fadeDuration = 0.0;
	inline double elapsed() const { return duration_cast<duration<double, milli>>(high_resolution_clock::now() - fadeStart).count(); }

	double x, y;
	double r;
	Color color;

	Circle(double xx, double yy, double rr, Color cc) : x{ xx }, y{ yy }, r{ rr }, color{ cc } {}

	virtual void draw();
	void fade(double);
};

class CursorCircle : public Circle {
	double startR;
	pair<double, double> startPos;
	pair<double, double> endPos;
public:
	CursorCircle(double xx, double yy, double rr, const Color& cc, const pair<double, double>& end, double ms);
	void draw();
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

	inline double getR() const { return ring.r; }

	inline double getX() const { return ring.x; }

	inline double getY() const { return ring.y; }

	inline void draw() { ring.draw(), center.draw(); }

	inline void fade(double ms) { ring.fade(ms), center.fade(ms); }
};

class ColorWheel {

	int ticksPerColor, ticks;

	vector<Color> gradient;
	int cur_i;

public:

	// resolution of 1 means that calls to next() will cycle through colors in gradient. Higher resolutions add inbetween colors.
	ColorWheel(const vector<Color>& colors, int resolution) : ticksPerColor{ resolution }, ticks{ 0 }, cur_i{ 0 } { gradient = colors; }

	Color next();
};


class RandomCircleCursor {
public:
	list<CursorCircle> circles;
	vector<vector<Color>*>::iterator colorScheme;
	int x, y, r;
public:
	RandomCircleCursor(int x, int y, int r) : x{ x }, y{ y }, r{ r }, colorScheme{ colorSchemes.begin() } {}

	CursorCircle addCircle();

	inline virtual Color nextColor() { return (*colorScheme)->at(rand() % (*colorScheme)->size()); }

	inline void chY(int dy) { y += dy; }

	inline void chX(int dx) { x += dx; }

	inline void setPos(int xx, int yy) { x = xx, y = yy; }

	inline void rotateScheme() { ++colorScheme; if (colorScheme == colorSchemes.end()) colorScheme = colorSchemes.begin(); }
};

class CursorContainer {
public:
	list<CursorCircle> circles;
public:
	vector<RandomCircleCursor*> cs;

	void draw();

	inline void addCircle(int i) { circles.push_back(cs[i]->addCircle()); }
};

class GradientCircleCursor : public RandomCircleCursor {
	
	ColorWheel gradient;

public:
	GradientCircleCursor(int x, int y, int r, const vector<Color>& g, int resolution) : RandomCircleCursor{ x, y, r }, gradient{ g, resolution } {}
	inline Color nextColor() { return gradient.next(); }
};

/*
class CircleSpiral : public CirclePath {
	//using CirclePath::CirclePath;
public:
	CircleSpiral(double x, double y, double r, Color c) : CirclePath{ x, y, r, c } {}
	void addCircle(int);
};
*/


class PolygonGL {
private:
	vector<std::pair<int, int>> vertices;
	Color color;
public:
	PolygonGL(vector<pair<int, int>> inVert, Color c) : color{ c } { vertices = inVert; }
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

class Line {
	pair<double, double> p1;
	pair<double, double> p2;
	Color color;
	double thickness;
public:
	Line(const pair<double, double>& pp1, const pair<double, double>& pp2, const Color& c, double thk) : p1{ pp1 }, p2{ pp2 }, color{ c }, thickness{ thk } {}
	void draw();
};

class Location;


//draft
class LocPair;

class Scene {
public:
	static LocPair locpair;
	static vector<Location> locations;
	//static vector<CircleSpiral> spirals;
	static vector<PolygonGL> polys;
	static vector<ColorSlideRing> rings;
	static vector<Point> points;
	static vector<Circle> circles;
	static vector<Line> lines;
	static CursorContainer cursors;


	//draft
	static vector<LocPair> locpairs;

	Scene() {} // initialize size and location of fixed targets
	static void draw();

	/*void startPath(double x, double y, double r, Color c) { paths.push_back(CirclePath{ x, y, r, c }); }
	void startSpiral(double x, double y, double r, Color c) { spirals.push_back(CircleSpiral{ x, y, r, c }); }*/
};

class Location {
public:
	ColorSlideRing target;
	double rStart;
	double TARGET_PRESSURE = 500;
	const double MAX_RADIUS = 50;
	double pressure, start_pressure;
	bool on;
	int num_rounds_correct = 0, prev_correct_round = 0;
	Color color;
	Location(double, double, double, double);
	void makeBigger(double);
	void makeSmaller(double);
	bool contains(double, double);
	bool withinPressure(double);
	bool exactMatch(double);
	double getPercentage(double);
	double distance(double, double);
	void turnOn() { on = true; };
	void turnOff() { on = false; };
	bool isOn() const { return on; };
	void setPressure(double);
	double getRadius() const { return target.getR(); };
	double getX() const { return target.getX(); }
	double getY() const { return target.getY(); }
	void draw() { target.draw(); }
	std::string toString() const;

	inline void fade(double ms){ target.fade(ms); }
};

class LocPair
{
public:

	bool locked = false;
	//bool on;
	double TARGET_PRESSURE = 500;

	double pressure;

	ColorSlideRing start;
	ColorSlideRing destination;

	Color start_color;
	Color dest_color;

	void start_setPressure(double);
	double start_getPercentage(double);

	void dest_setPressure(double);
	double dest_getPercentage(double);

	double rStart;
	double start_pressure;
	LocPair();
	LocPair(double, double, double, double, double, double);
	void draw() { start.draw(); destination.draw(); }

	bool line();

	bool withinPressure(double input);

	double dist(int x1, int y1, int x2, int y2);

	bool on_line(int x1, int y1, int x2, int y2, int x3, int y3);


};

#endif