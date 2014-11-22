#include <stdio.h>
#include <math.h>
#include "graphics.h"

using namespace std;

double dist(double x1, double y1, double x2, double y2)
{
	double dist_x = x2 - x1;
	double dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x)+(dist_y*dist_y));

	return length;
}

//checks if (x3,y3) is inbetween (x1,y1) and (x2,y2)
bool on_line(double x1, double y1, double x2, double y2, double x3, double y3)
{
	if ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2) - dist(x1, y1, x2, y2)) / dist(x1, y1, x2, y2) < 0.1)
		return true;
	else
		return false;
}

//void drawLine(double x1, double y1, double x2, double y2){
bool line(LocPair loc){
	const int MAX_X = 640;
	const int MAX_Y = 480;
	ColorSlideRing loc1 = loc.start;
	ColorSlideRing loc2 = loc.destination;
	int last_x = (int)loc1.getX();
	int last_y = (int)loc1.getY();
	int x1 = (int)loc1.getX();
	int y1 = (int)loc1.getY();
	int x2 = (int)loc2.getX();
	int y2 = (int)loc2.getY();
	
	// draw current track and check whether the line is finished
	while(dist(last_x,x2,last_y,y2)-(int)loc2.getR()>0){
		// Assume minDepth is the cursor

		//get cursor location (400,400) is example for now
		int cursor_x = 400;
		int cursor_y = 400;

		int x = cursor_x % MAX_X; // column
		int y = cursor_y / MAX_X; // row

		if (on_line(x1, y1, x2, y2, x, y))
		{
			//if(dist(x,y,x2,y2)<dist(last_x,last_y,x2,y2)){
				last_x = x;
				last_y = y;
				// OpenGL draw line
				Scene::lines.push_back({ { x1, y1 }, { last_x, last_y }, RED, 5.0 });
			//}
		}
		else
			return false;
	}
	return true;
}
