#ifndef CURSOR_H
#define CURSOR_H

#include "graphics.h"

//START OF DEBUGCURSOR
class DebugCursor {
	PolygonGL square;
	int x, y;
public:
	DebugCursor(int xx, int yy, int sz) : x{ xx }, y{ yy }, square{ { {xx - sz/2, yy - sz/2}, {xx + sz/2, yy - sz/2}, {xx + sz/2, yy + sz/2}, {xx - sz/2, yy + sz/2} }, BLUE}{}

	inline void setPos(int xx, int yy) { square.translate(xx - x, yy - y), x = xx, y = yy; }

	inline void draw() { square.draw(); }
};
//END OF DEBUGCURSOR


//START OF RANDOMCIRCLECURSOR
class RandomCircleCursor {
	vector<vector<Color>*>::iterator colorScheme;
	int x, y, r;

	friend class RotatingMultiCursor;

public:
	RandomCircleCursor(int x, int y, int r) : x{ x }, y{ y }, r{ r }, colorScheme{ colorSchemes.begin() } {}
	CursorCircle addCircle();

	inline virtual Color nextColor() { return (*colorScheme)->at(rand() % (*colorScheme)->size()); }
	inline void chY(int dy) { y += dy; }
	inline void chX(int dx) { x += dx; }
	inline void setPos(const std::pair<double, double>& p) { x = (int)p.first, y = (int)p.second; }
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

// SUPER-CURSORS - ie, containers of multiple cursors

//START OF CURSORCONTAINER
class CursorContainer {
protected:
	list<CursorCircle> circles;
public:
	vector<GradientCircleCursor> cs;

	CursorContainer(const vector<GradientCircleCursor>& in) { cs = in; }
	CursorContainer(){}

	virtual void draw();

	inline void addCircle(int i) { circles.push_back(cs[i].addCircle()); }

	//inline ~CursorContainer() { for (auto& x : cs) delete x; }
};
//END OF CURSORCONTAINER

//START OF ROTATINGMULTICURSOR
class RotatingMultiCursor : public CursorContainer {
	double x, y, r;
	int speed;
	vector<int> cursorAngle;
public:
	RotatingMultiCursor(double, double, double, int, const vector<GradientCircleCursor>&);
	RotatingMultiCursor(){}

	inline void chY(int dy) { y += dy; }
	inline void chX(int dx) { x += dx; }
	inline void setR(int rr) { r = rr; }
	inline void setPos(const std::pair<double, double>& p) { x = (int)p.first, y = (int)p.second; }

	inline void setCircleRadius(int r) { for (auto& cursor : cs) cursor.r = r; }

	inline void addCircle(int i) = delete;
	void addCircle();

	void draw() override;
};
//END OF ROTATINGMULTICURSOR

#endif