#include "graphics.h"
#include <gl\glew.h>
#include <Windows.h>
#include <cmath>
#include <cstdlib>


#define PI 3.14159265

//Isaac Bowen
//eecs481 fall2014

#include "graphics.h"
#include <math.h>
//Isaac Location code
Location::Location(double x_in, double y_in, double r_in, double pressure_in) : x{ x_in }, y{ y_in }, r{ r_in }, targetPressure{ pressure_in }{
	std::cout << "Created a location at (x, y, r): " << x << " " << y << " " << r << std::endl;
	on = true;
}

void Location::makeBigger(double increase) {
	r += increase;
}

void Location::makeSmaller(double decrease) {
	r -= decrease;
}

bool Location::contains(double x_in, double y_in) {
	return distance(x_in, y_in) < r;
}

//Returns true if the percentage pressure is
bool Location::withinPressure(double input) {
	double percentage = input / targetPressure;
	return percentage > .5;
}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(x - x_in, 2) + pow(y - y_in, 2));
}

void Location::setPressure(double in) {
	pressure = in;
}
//Isaac Locatin code

void Circle::draw(){
	glBegin(GL_TRIANGLE_FAN);

	glColor3f(color.r, color.b, color.g);
	
	x += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	y += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	
	glVertex2f(x, y);

	for (int angle = 0; angle <= 360; angle += 2){
		glVertex2f(x + sin(angle * PI / 180) * r,
			y + cos(angle * PI / 180) * r);
	}

	glEnd();
}

void CirclePath::addCircle(int depth){
	Circle prev = circles.front();
	double theta = (rand() % 360) * PI / 180;
	double distanceToNew = prev.r/2;
	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);

	float ratio = (1500 - depth - 500) / 1000;

	double base_r = 100;

	double dr = (rand() % 2) ? prev.r / 10 : -prev.r / 10;

	Color c = prev.color == RED ? BLUE : (prev.color == BLUE ? GREEN : (prev.color == GREEN ? RED : RED));

	Color r{ 1.0 / (rand() % 10), 1.0 / (rand() % 2), 1.0 / (rand() % 10) };

	circles.push_front({ dx + prev.x, dy + prev.y, prev.r + dr, r});

}

void CircleSpiral::addCircle(int depth){
	Circle prev = circles.front();
	static int angle = 0;
	double theta = (prev.angle + 5) * PI / 180;
	//double theta = (rand() % 360) * PI / 180;
	double distanceToNew = prev.r / 4;
	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);

	int normalized = (depth - 1100);

	double base_r = 100;

	double dr = (rand() % 2) ? prev.r / 10 : -prev.r / 10;

	Color c = prev.color == RED ? BLUE : (prev.color == BLUE ? GREEN : (prev.color == GREEN ? RED : RED));

	Color r{ 1.0 / (rand() % 10), 1.0 / (rand() % 10), 1.0 / (rand() % 2) };

	circles.push_front({ dx + prev.x, dy + prev.y, 100 - 0.2 * normalized, r, prev.angle + 5 });
}

bool operator==(Color& a, Color& b) {
	return	a.r == b.r &&
		a.b == b.b &&
		a.g == b.g;
}
