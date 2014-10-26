#include "graphics.h"
#include <gl\glew.h>
#include <Windows.h>
#include <cmath>
#include <cstdlib>


#define PI 3.14159265


void Circle::draw(){
	glBegin(GL_TRIANGLE_FAN);

	glColor3f(color.r, color.b, color.g);

	glVertex2f(x, y);

	for (int angle = 0; angle <= 360; angle += 1){
		glVertex2f(x + sin(angle * PI / 180) * r,
			y + cos(angle * PI / 180) * r);
	}

	glEnd();
}

void CirclePath::addCircle(){
	Circle prev = circles.front();
	double theta = (rand() % 360) * PI / 180;
	double distanceToNew = prev.r;
	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);

	double dr = (rand() % 2) ? prev.r / 10 : -prev.r / 10;

	Color c = prev.color == RED ? BLUE : (prev.color == BLUE ? GREEN : (prev.color == GREEN ? RED : RED));

	Color r{ 1.0 / (rand() % 10), 1.0 / (rand() % 10), 1.0 / (rand() % 10) };

	circles.push_front({ dx + prev.x, dy + prev.y, dr + prev.r, r });

}

bool operator==(Color& a, Color& b) {
	return	a.r == b.r &&
		a.b == b.b &&
		a.g == b.g;
}