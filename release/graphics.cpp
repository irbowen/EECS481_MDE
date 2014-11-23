#include "graphics.h"
#include <gl\glew.h>
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <mutex>
#include <iterator>
#include "color.h"

#define PI 3.14159265

//Isaac Bowen
//eecs481 fall2014

#include <math.h>
#include <algorithm>

using std::mutex;
using std::cout;
using std::endl;
using std::sort;
using std::merge;

mutex LocationLock;
vector<LocPair> Scene::locpairs;
vector<Location> Scene::locations;
vector<PolygonGL> Scene::polys;
vector<ColorSlideRing> Scene::rings;
vector<Point> Scene::points;
vector<Circle> Scene::circles;
vector<Line> Scene::lines;
LocPair Scene::locpair(-1, -1, -1, -1, -1, -1);
CursorContainer Scene::cursors;

namespace {
	std::pair<double, double> between(const std::pair<double, double>& p1, const std::pair<double, double>& p2, double prct){
		
		return {p1.first + prct * (p2.first - p1.first),
				p1.second + prct * (p2.second - p1.second)};

	}
}

//START OF CIRCLE
void Circle::draw(){
	if (fadeDuration != 0.0){
		double ms = elapsed();
		color = (ms >= fadeDuration) ? WHITE : mix(startColor, WHITE, ms / fadeDuration);
	}

	glBegin(GL_TRIANGLE_FAN);
	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	glVertex2f((GLfloat)x, (GLfloat)y);

	for (int angle = 0; angle <= 360; angle += 2){
		glVertex2f((GLfloat)(x + sin(angle * PI / 180) * r),
			(GLfloat)(y + cos(angle * PI / 180) * r));
	}
	glEnd();
}

void Circle::fade(double ms){
	fadeStart = high_resolution_clock::now();
	fadeDuration = ms;
	startColor = color;
}
//END OF CIRCLE

//START OF CURSORCIRCLE
CursorCircle::CursorCircle(double xx, double yy, double rr, const Color& cc, const std::pair<double, double>& end, double ms) : Circle{ xx, yy, rr, cc }, startPos{ xx, yy }, endPos{ end } {
	fade(ms);
	startR = r;
	startPos = { x, y };
}

void CursorCircle::draw(){
	double ms = elapsed();

	r = startR * ms / fadeDuration;
	auto pos = between(endPos, startPos, ms / fadeDuration);
	x = pos.first, y = pos.second;

	if (color == WHITE)
		return;

	Circle::draw();
}
//END OF CURSORCIRCLE

//START OF COLORSLIDECIRCLE
void ColorSlideCircle::setGoalProgress(double percent){
	color = mix(startColor, endColor, percent);
}
//END OF COLORSLIDECIRCLE

// START OF RANDOMCIRCLECURSOR
CursorCircle RandomCircleCursor::addCircle(){
	double theta = (rand() % 360) * PI / 180;
	double distanceToNew = 3*r/2/(rand()%5 + 1);
	double dx = distanceToNew * cos(theta);
	double dy = distanceToNew * sin(theta);
	double newR = r / (rand() % 4 + 2);

	Color color = nextColor();

	return{ dx + x, dy + y, newR, color, { x, y }, (double)(rand() % 1000 + 500) };
}

void CursorContainer::draw() {
	for (auto& circle : circles)
			circle.draw();
	circles.remove_if([](const CursorCircle& c) { return c.elapsed() / c.fadeDuration >= 0.95; });
}
//END OF RANDOMCIRCLECURSOR

//START OF POLYGONGL
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
//END OF POLYGONGL

//START OF POINT
void Point::draw(){
	glBegin(GL_POINTS);
	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	glVertex2f((GLfloat)x, (GLfloat)y);
	glEnd();
}
//END oF POINT

//START OF SCENE
void Scene::draw(){
	cursors.draw();
	locpair.draw();

	for (auto& x : polys) x.draw();
	for (auto& x : rings) x.draw();
	for (auto& x : points) x.draw();

	LocationLock.lock();
	for (auto& x : locations) x.draw();
	LocationLock.unlock();

	for (auto& x : circles) x.draw();
	for (auto& x : locpairs) x.draw();
	for (auto& x : lines) x.draw();
}
//END OF SCENE

//START OF LINE
void Line::draw(){
	glLineWidth((GLfloat)thickness);
	glBegin(GL_LINES);

	glColor3f((GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);
	glVertex2f((GLfloat)p1.first, (GLfloat)p1.second);
	glVertex2f((GLfloat)p2.first, (GLfloat)p2.second);

	glEnd();
	glLineWidth(1.0);
}
//END OF LINE