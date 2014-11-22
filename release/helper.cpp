#include <stdio.h>
#include <math.h>

using namespace std;

double dist(double x1, double y1, double x2, double y2)
{
	double dist_x = x2 - x1;
	double dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x) + (dist_y*dist_y));

	return length;
}

//checks if (x3,y3) is inbetween (x1,y1) and (x2,y2)
//will need to do %-base, because a regular line doesn't have thickness
//line displayed on the fabric will have some thickness
double on_line(double x1, double y1, double x2, double y2, double x3, double y3)
{
	return ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2)) == dist(x1, y1, x2, y2));
}