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
	PolygonGL(const vector<pair<int, int>>& inVert, const Color &c) : color{ c } { vertices = inVert; }
	void draw();
	bool inside(double, double);
	inline void translate(int dx, int dy){ for (auto& v : vertices) v.first += dx, v.second += dy; }
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
	inline void set(const pair<double, double>& pp1, const pair<double, double>& pp2) { p1 = pp1, p2 = pp2; }
	inline double length() { return ::distance(p1, p2); }
};

class LinePair {
	pair<double, double> p1;
	pair<double, double> p2;
	Line l1;
	Line l2;
public:
	LinePair(const pair<double, double>& p1, const pair<double, double>& p2, const Color& c1, const Color& c2) : l1{ p1, p1, c1, 5.0 }, l2{ p1, p2, c2, 5.0 }, p1{ p1 }, p2{ p2 } {}

	inline void setProgress(double prct) { auto mid = between(p1, p2, prct); l1.set(p1, mid), l2.set(mid, p2); }

	inline void draw() { l1.draw(), l2.draw(); }

	inline double length() { return l1.length() + l2.length(); }
};
//END OF LINE

#endif