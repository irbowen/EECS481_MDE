#include "graphics.h"
#include <gl\glew.h>
#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <mutex>
#include <iterator>
#include "color.h"
#include "location.h"
#include "Circle.h"
#include "CursorCircle.h"
#include "LocPair.h"


//Isaac Bowen
//eecs481 fall2014

#include <math.h>
#include <algorithm>
#define PI 3.14159265


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