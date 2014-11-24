#include <gl\glew.h>
#include <Windows.h>
#include "Circle.h"

#define PI 3.14159265



void Circle::draw(){
	if (fadeDuration != 0.0){
		double ms = elapsed();
		color = (ms >= fadeDuration) ? WHITE : mix(startColor, WHITE, ms / fadeDuration);
		if (color == WHITE)
			faded = true;
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