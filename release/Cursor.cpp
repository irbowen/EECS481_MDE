#include "Cursor.h"


// START OF RANDOMCIRCLECURSOR
CursorCircle RandomCircleCursor::addCircle(){
	int angle = rand() % 360;

	double distanceToNew = 3 * r / 2 / (rand() % 5 + 1);
	double newR = r / (rand() % 4 + 2);

	auto pt = jump({ x, y }, distanceToNew, angle);

	Color color = nextColor();

	return{ pt.first, pt.second, newR, color, { x, y }, (double)(rand() % 1000 + 500), distanceToNew, angle };
}
CursorCircle RandomCircleCursor::addCircle(double ms){
	int angle = rand() % 360;

	double distanceToNew = 3 * r / 2 / (rand() % 5 + 1);
	double newR = r / (rand() % 4 + 2);

	auto pt = jump({ x, y }, distanceToNew, angle);

	Color color = nextColor();

	return{ pt.first, pt.second, newR, color, { x, y }, ms, distanceToNew, angle };
}
//END OF RANDOMCIRCLECURSOR

void CursorContainer::draw() {
	circles.remove_if([](const CursorCircle& c) { return c.faded; });
	for (auto& circle : circles)
		circle.draw();
}

// START OF ROTATINGMULTICURSOR
RotatingMultiCursor::RotatingMultiCursor(double xx, double yy, double rr, int s, const vector<GradientCircleCursor>& in) : 
											CursorContainer{in},
											x{ xx },
											y{ yy },
											r{ rr },
											speed{ s }{
	initAngles();
}

void RotatingMultiCursor::update(){
	for (GradientCircleCursor& cursor : cs){
		cursor.setPos(jump({ x, y }, r, cursorAngle[&cursor - &*cs.begin()] += speed));
	}
}

void RotatingMultiCursor::draw(){
	update();

	CursorContainer::draw();
}

void RotatingMultiCursor::addCircle() { 
	for (auto& x : cs) 
		circles.push_back(x.addCircle()); 
}

void RotatingMultiCursor::addCircle(double ms) {
	for (auto& x : cs)
		circles.push_back(x.addCircle(ms));
}


void RotatingMultiCursor::initAngles() {
	for (int a = 0; a < 360; a += 360 / cs.size())
		cursorAngle.push_back(a);
}

void RotatingMultiCursor::chCursors(const vector<GradientCircleCursor>& cs_in){
	cs = cs_in;
	initAngles();
	update();
}

// END OF ROTATINGMULTICURSOR


void GameCursor::draw() {
	circles.remove_if([](const CursorCircle& c) { return c.faded; });
	for (auto& circle : circles)
		circle.draw();
}

