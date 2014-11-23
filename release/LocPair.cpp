#include "LocPair.h"
#include <iostream>


//START OF LOCPAIR
LocPair::LocPair(double x1_in, double y1_in, double x2_in, double y2_in, double r_in, double pressure_in)
: start{ x1_in, y1_in, r_in, WHITE, RED, GREEN }, destination{ x2_in, y2_in, r_in, WHITE, RED, GREEN }, rStart{ r_in }, start_pressure{ pressure_in }
{
	std::cout << "Created a location at (x, y, r): " << x1_in << " " << y1_in << " " << r_in << std::endl;
	std::cout << "At depth: " << start_pressure << std::endl;
	//on = true;
}

bool LocPair::withinPressure(double input) {
	double deflection = abs(input - start_pressure);
	return deflection <= TARGET_PRESSURE + 250 && deflection >= TARGET_PRESSURE - 250;
}


bool LocPair::line(){
	const int MAX_X = 640;
	const int MAX_Y = 480;
	ColorSlideRing loc1 = start;
	ColorSlideRing loc2 = destination;
	int last_x = (int)loc1.getX();
	int last_y = (int)loc1.getY();
	int x1 = (int)loc1.getX();
	int y1 = (int)loc1.getY();
	int x2 = (int)loc2.getX();
	int y2 = (int)loc2.getY();

	// draw current track and check whether the line is finished
	while (dist(last_x, x2, last_y, y2) - (int)loc2.getR()>0){
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
			//Scene::lines.push_back({ { x1, y1 }, { last_x, last_y }, RED, 5.0 });
			//}
		}
		else
			return false;
	}
	return true;
}

double LocPair::dist(int x1, int y1, int x2, int y2)
{
	int dist_x = x2 - x1;
	int dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x) + (dist_y*dist_y));

	return length;
}

bool LocPair::on_line(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2) - dist(x1, y1, x2, y2)) / dist(x1, y1, x2, y2) < 0.1)
		return true;
	else
		return false;
}

void LocPair::draw(){
	start.draw();
	destination.draw();
}

double LocPair::start_getPercentage(double input) {
	double deflection = abs(input - start_pressure);
	return (deflection / TARGET_PRESSURE <= 1.0 &&
		deflection / TARGET_PRESSURE >= 0.0) ?
		deflection / TARGET_PRESSURE : 0.0;
}

void LocPair::start_setPressure(double in) {
	pressure = in;
	if (in == 0)
		start.setGoalProgress(0);
	else
		start.setGoalProgress(start_getPercentage(in));
}
