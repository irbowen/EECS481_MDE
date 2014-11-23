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
#include "color.h"
#include "location.h"



using namespace std::chrono;
using std::milli;
using std::vector;
using std::deque;
using std::unordered_set;
using std::pair;
using std::list;



// START OF CIRCLE
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
//END OF CIRCLE

//START OF CURSORCIRCLE
class CursorCircle : public Circle {
	double startR;
	pair<double, double> startPos;
	pair<double, double> endPos;
public:
	CursorCircle(double xx, double yy, double rr, const Color& cc, const pair<double, double>& end, double ms);
	void draw();
};
//END OF CURSORCIRCLE


//START OF COLORSLIDECIRCLE
class ColorSlideCircle : public Circle {
	Color startColor;
	Color endColor;
public:
	ColorSlideCircle(double xx, double yy, double rr, Color cStart, Color cEnd) : Circle{ xx, yy, rr, cStart }, startColor{ cStart }, endColor{ cEnd } {}
	void setGoalProgress(double percent);
};
//END OF COLORSLIDECIRCLE


//START OF COLORSLIDERING
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
//END OF COLORSLIDERING


//START OF RANDOMCIRCLECURSOR
class RandomCircleCursor {
public:
	list<CursorCircle> circles;
	vector<vector<Color>*>::iterator colorScheme;
	int x, y, r;
public:
	RandomCircleCursor(int x, int y, int r) : x{ x }, y{ y }, r{ r }, colorScheme{ colorSchemes.begin() } {}
	void draw();
	// Add different sized circle to path in random direction
	void addCircle();
	inline virtual Color nextColor() { return (*colorScheme)->at(rand() % (*colorScheme)->size()); }
	inline void chY(int dy) { y += dy; }
	inline void chX(int dx) { x += dx; }
	inline void setPos(int xx, int yy) { x = xx, y = yy; }
	inline void rotateScheme() { ++colorScheme; if (colorScheme == colorSchemes.end()) colorScheme = colorSchemes.begin(); }
};
//END OF RANDOMCIRCLECURSOR


//START OF GRADIENTCIRCLECURSOR
class GradientCircleCursor : public RandomCircleCursor {
	ColorWheel gradient;

public:
	GradientCircleCursor(int x, int y, int r, const vector<Color>& g, int resolution) : RandomCircleCursor{ x, y, r }, gradient{ g, resolution } {}
	inline Color nextColor() { return gradient.next(); }
};
//END OF GRADIENTCIRCLECURSOR


//START OF POLYGONGL
class PolygonGL {
private:
	vector<std::pair<int, int>> vertices;
	Color color;
public:
	PolygonGL(vector<pair<int, int>> inVert, Color c) : color{ c } { vertices = inVert; }
	void draw();
	bool inside(double, double);
};
//END OF POLYGONGL


//START OF POINT
class Point {
	double x, y;
	Color color;
public:
	Point(double xx, double yy, Color c) : x{ xx }, y{ yy }, color{ c } {}
	void draw();
};
//END OF POINT

//START OF LINE
class Line {
	pair<double, double> p1;
	pair<double, double> p2;
	Color color;
	double thickness;
public:
	Line(const pair<double, double>& pp1, const pair<double, double>& pp2, const Color& c, double thk) : p1{ pp1 }, p2{ pp2 }, color{ c }, thickness{ thk } {}
	void draw();
};
//END OF LINE

class Location;
class LocPair;
//START OF SCENE
class Scene {
public:
	static vector<Location> locations;
	static vector<RandomCircleCursor> cursors;
	//static vector<CircleSpiral> spirals;
	static vector<PolygonGL> polys;
	static vector<ColorSlideRing> rings;
	static vector<Point> points;
	static vector<Circle> circles;
	static vector<Line> lines;


	//draft
	static vector<LocPair> locpairs;

	Scene() {} // initialize size and location of fixed targets
	static void draw();

	/*void startPath(double x, double y, double r, Color c) { paths.push_back(CirclePath{ x, y, r, c }); }
	void startSpiral(double x, double y, double r, Color c) { spirals.push_back(CircleSpiral{ x, y, r, c }); }*/
};
//END OF SCENE




#endif