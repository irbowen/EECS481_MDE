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

void ColorSlideCircle::setGoalProgress(double percent){
	color.r = startColor.r + (endColor.r - startColor.r) * percent;
	color.g = startColor.g + (endColor.g - startColor.g) * percent;
	color.b = startColor.b + (endColor.b - startColor.b) * percent;
}


//Isaac Location code
Location::Location(double x_in, double y_in, double r_in, double pressure_in) : x{ x_in }, y{ y_in }, r{ r_in }, start_pressure{ pressure_in }{
	std::cout << "Created a location at (x, y, r): " << x << " " << y << " " << r << std::endl;
	std::cout << "At depth: " << start_pressure << std::endl;
	on = true;
}

void Location::makeBigger(double increase) {
	r += increase;
}

std::string Location::toString() {
	std::ostringstream ss;
	ss << "X: " << x << "Y: " << " R: " << r << " start_pressure: " << start_pressure << "\n";
	return ss.str();
}

void Location::makeSmaller(double decrease) {
	r -= decrease;
}

bool Location::contains(double x_in, double y_in) {
	return distance(x_in, y_in) < r;
}

double Location::getPercentage(double input) {
	double deflection = abs(input - start_pressure);
	return deflection / TARGET_PRESSURE;
}

bool Location::withinPressure(double input) {
	double deflection = abs(input - start_pressure);
	return abs(deflection - TARGET_PRESSURE) < 500;
}

bool Location::exactMatch(double input) {
	double deflection = abs(input - start_pressure);
	return abs(deflection - TARGET_PRESSURE) < 250;
}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(x - x_in, 2) + pow(y - y_in, 2));
}

void Location::setPressure(double in) {
	start_pressure = in;
}
//Isaac Locatin code

void Circle::draw(){
	glBegin(GL_TRIANGLE_FAN);

	glColor3f(color.r, color.g, color.b);
	
	/*
	x += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	y += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	*/

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

void PolygonGL::draw(){
	glBegin(GL_POLYGON);
	
	glColor3f(color.r, color.g, color.b);
	
	for(int i = 0; i < vertices.size(); i++){
		glVertex2f(vertices[i].first, vertices[i].second);
	}

	glEnd();
}

bool PolygonGL::inside(double x, double y){
	bool rtn = false;
	for (int i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++){

		double xi = vertices[i].first;
		double xj = vertices[j].first;
		double yi = vertices[i].second;
		double yj = vertices[j].second;

		if (((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi) + xi))
			rtn = !rtn;


	} 

	return rtn;
}

void Point::draw(){
	glBegin(GL_POINTS);

	glColor3f(color.r, color.g, color.b);

	glVertex2f(x, y);

	glEnd();
}

