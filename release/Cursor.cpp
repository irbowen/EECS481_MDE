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

	for (int a = 0; a < 360; a += 360 / cs.size())
		cursorAngle.push_back(a);
}

void RotatingMultiCursor::draw(){
	for (GradientCircleCursor& cursor : cs){
		cursor.setPos(jump({ x, y }, r, cursorAngle[&cursor - &*cs.begin()] += speed));
	}

	CursorContainer::draw();
}

void RotatingMultiCursor::addCircle() { 
	for (auto& x : cs) 
		circles.push_back(x.addCircle()); 
}

// END OF ROTATINGMULTICURSOR