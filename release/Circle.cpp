#include <gl\glew.h>
#include <Windows.h>
#include "Circle.h"

#define PI 3.14159265



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


std::pair<double, double> between(const std::pair<double, double>& p1, const std::pair<double, double>& p2, double prct){

	return{ p1.first + prct * (p2.first - p1.first),
		p1.second + prct * (p2.second - p1.second) };

}
//END OF CIRCLE