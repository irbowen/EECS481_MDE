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

std::ostream& operator<<(std::ostream& os, const Color& c){
	std::cout << c.r << ' ' << c.g << ' ' << c.b;
	return os;
}


//Isaac Location code
Location::Location(double x_in, double y_in, double r_in, double pressure_in) : target{ x_in, y_in, r_in, WHITE, RED, GREEN }, rStart{ r_in }, start_pressure{ pressure_in }{
	std::cout << "Created a location at (x, y, r): " << x_in << " " << y_in << " " << r_in << std::endl;
	std::cout << "At depth: " << start_pressure << std::endl;
	turnOn();

}

void Location::makeBigger(double increase) {
	if (target.getR() < MAX_RADIUS)
		target.setR(target.getR() + increase);
}

std::string Location::toString() {
	std::ostringstream ss;
	ss << "X: " << getX() << "Y: " << getY() << " R: " << getRadius() << " start_pressure: " << start_pressure << "\n";
	return ss.str();
}

void Location::makeSmaller(double decrease) {
	target.setR(target.getR() - decrease);
}

bool Location::contains(double x_in, double y_in) {
	return distance(x_in, y_in) < target.getR();
}

double Location::getPercentage(double input) {
	double deflection = abs(input - start_pressure);
	return (deflection / TARGET_PRESSURE <= 1.0 &&
			deflection / TARGET_PRESSURE >= 0.0) ?
				deflection / TARGET_PRESSURE : 0.0;
}

bool Location::withinPressure(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 250 && deflection >= TARGET_PRESSURE - 250;
}

bool Location::exactMatch(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 200 && deflection >= TARGET_PRESSURE - 200;

}

double Location::distance(double x_in, double y_in) {
	return sqrt(pow(target.getX() - x_in, 2) + pow(target.getY() - y_in, 2));
}

void Location::setPressure(double in) {
	pressure = in;
	if (in == 0)
		target.setGoalProgress(0);
	else
		target.setGoalProgress(getPercentage(in));
}
//Isaac Locatin code

void ColorSlideCircle::setGoalProgress(double percent){
	color.r = startColor.r + (endColor.r - startColor.r) * percent;
	color.g = startColor.g + (endColor.g - startColor.g) * percent;
	color.b = startColor.b + (endColor.b - startColor.b) * percent;
}

void Circle::draw(){
	glBegin(GL_TRIANGLE_FAN);

	if (fadeDuration != 0.0){
		double elapsed = duration_cast<duration<double, milli>>(high_resolution_clock::now() - fadeStart).count();
		// TODO+++
	}


	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	
	/*
	x += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	y += (1.0 * (rand() % 10)) / (rand() % 10 + (rand() % 2 ? 1 : -10));
	*/

	glVertex2f((GLfloat)x, (GLfloat)y);

	for (int angle = 0; angle <= 360; angle += 2){
		glVertex2f((GLfloat) (x + sin(angle * PI / 180) * r),
				   (GLfloat) (y + cos(angle * PI / 180) * r));
	}

	glEnd();
}

void Circle::fade(double ms){
	fadeStart = high_resolution_clock::now();
	fadeDuration = ms;
	startColor = color;
}


void CirclePath::addCircle(int depth){
	Circle prev = circles.front();
	double theta = (rand() % 360) * PI / 180;
	double distanceToNew = prev.r/2;
	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);

	double ratio = (1500 - depth - 500) / 1000.0;

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
	
	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	
	for(unsigned i = 0; i < vertices.size(); i++){
		glVertex2f((GLfloat)vertices[i].first, (GLfloat)vertices[i].second);
	}

	glEnd();
}

bool PolygonGL::inside(double x, double y){
	bool rtn = false;
	for (unsigned i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++){

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

	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);

	glVertex2f((GLfloat)x, (GLfloat)y);

	glEnd();
}

void Scene::draw() {
	for (CirclePath cp : paths) { cp.draw(); }
	for (CircleSpiral cs : spirals) { cs.draw(); }
	for (auto x : polys) x.draw();
	for (auto x : rings) x.draw();
	for (auto x : points) x.draw();
	for (auto x : locations) x.draw();
}


vector<Location> Scene::locations;
vector<CirclePath> Scene::paths;
vector<CircleSpiral> Scene::spirals;
vector<PolygonGL> Scene::polys;
vector<ColorSlideRing> Scene::rings;
vector<Point> Scene::points;