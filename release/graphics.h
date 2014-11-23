#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <vector>
#include <iostream>
#include <deque>
#include <utility>
#include <unordered_set>
#include <sstream>
#include <list>
#include "color.h"
#include "location.h"
#include "CursorCircle.h"

using std::vector;
using std::deque;
using std::unordered_set;
using std::pair;
using std::list;

//START OF RANDOMCIRCLECURSOR
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
//END OF RANDOMCIRCLECURSOR

//START OF CURSORCONTAINER
class CursorContainer {
public:
	list<CursorCircle> circles;
public:
	vector<RandomCircleCursor*> cs;

	void draw();

	inline void addCircle(int i) { circles.push_back(cs[i]->addCircle()); }
};
//END OF CURSORCONTAINER

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
//END OF SCENE
#endif