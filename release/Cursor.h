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

	inline int getX() { return x; };
	inline int getY() { return y; };

	inline void draw() { square.draw(); }
};
//END OF DEBUGCURSOR


//START OF RANDOMCIRCLECURSOR
class RandomCircleCursor {
	vector<vector<Color>*>::iterator colorScheme;
	int x, y, r;

	friend class RotatingMultiCursor;

public:

	int startR;

	RandomCircleCursor(int x, int y, int r) : x{ x }, y{ y }, r{ r }, startR{ r }, colorScheme{ colorSchemes.begin() } {}
	RandomCircleCursor() {}
	CursorCircle addCircle();
	CursorCircle addCircle(double);

	inline virtual Color nextColor() { return (*colorScheme)->at(rand() % (*colorScheme)->size()); }
	inline void chY(int dy) { y += dy; }
	inline void chX(int dx) { x += dx; }
	inline void setR(int rr) { r = rr; }
	inline void setPos(const std::pair<double, double>& p) { x = (int)p.first, y = (int)p.second; }
	std::pair<double, double> getPos() { return{ x, y }; }
	inline void rotateScheme() { ++colorScheme; if (colorScheme == colorSchemes.end()) colorScheme = colorSchemes.begin(); }
};
//END OF RANDOMCIRCLECURSOR

//START OF GRADIENTCIRCLECURSOR
class GradientCircleCursor : public RandomCircleCursor {
	ColorWheel gradient;

	friend class GameCursor;

public:
	GradientCircleCursor(int x, int y, int r, const vector<Color>& g, int resolution) : RandomCircleCursor{ x, y, r }, gradient{ g, resolution } {}
	GradientCircleCursor(int x, int y, int r, const ColorWheel& cw) : RandomCircleCursor{ x, y, r }, gradient{ cw } {}
	inline Color nextColor() { return gradient.next(); }
	inline void chColor(const ColorWheel& cw) { gradient.chGradient(cw.gradient); }
	inline vector<Color> getColors() { return gradient.getColors(); }
};
//END OF GRADIENTCIRCLECURSOR

class GameCursor {
	int i;
	list<CursorCircle> circles;
public:
	GradientCircleCursor cursor;
	GameCursor() : cursor{ 320, 240, 35, *CursorGradients.front() }, i{ 0 } {}
	inline Color nextColor() { return cursor.gradient.next(); }
	inline void rotateScheme(){ if (++i == CursorGradients.size()) i = 0; cursor.chColor(*CursorGradients[i]); }

	inline void addCircle() { circles.push_back(cursor.addCircle()); }
	inline void addCircle(double ms) { circles.push_back(cursor.addCircle(ms)); }

	void draw();

	inline void chY(int dy) { cursor.chY(dy); }
	inline void chX(int dx) { cursor.chX(dx); }
	inline void setPos(const std::pair<double, double>& p) { cursor.setPos(p); }
	inline std::pair<double, double> getPos() { return cursor.getPos(); }
};


// SUPER-CURSORS - ie, containers of multiple cursors
/*
class GameCursors {
	list<CursorCircle> circles;
public:
	vector<GameCursor> cs;

	//void draw();

	inline void addCircle(int i) { circles.push_back(cs[i].addCircle()); }
};
*/
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
	inline void addCircle(int i, double ms) { circles.push_back(cs[i].addCircle(ms)); }

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
	void addCircle(double);

	void draw() override;

	void update();
	void initAngles();

	high_resolution_clock::time_point explodeStart;


	inline double elapsed() const { return duration_cast<duration<double, milli>>(high_resolution_clock::now() - explodeStart).count(); }

	bool exploding = false;

	static double explodeDuration;

	void chCursors(const vector<GradientCircleCursor>&);
};
//END OF ROTATINGMULTICURSOR

#endif