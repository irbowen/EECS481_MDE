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
#include "geometry.h"


//Isaac Bowen
//eecs481 fall2014

#include <math.h>
#include <algorithm>

using std::cout;
using std::endl;



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