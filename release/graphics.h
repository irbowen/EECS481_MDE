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
#include "LocPair.h"
#include "CursorCircle.h"

using std::vector;
using std::deque;
using std::unordered_set;
using std::pair;
using std::list;

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

#endif