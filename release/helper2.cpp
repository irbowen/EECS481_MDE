#include <stdio.h>
#include <math.h>

using namespace std;

double dist(int x1, int y1, int x2, int y2)
{
	int dist_x = x2 - x1;
	int dist_y = y2 - y1;

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
	while(dist(last_x,x2,last_y,y2)-(int)loc2.getRadius()>0){
		// Assume minDepth is the cursor
		int x = minDepth_index % MAX_X; // column
		int y = minDepth_index / MAX_X; // row

		if (on_line(x1, y1, x2, y2, x, y))
			if(abs(dist(x,y,x2,y2)-dist(last_x,last_y,x2,y2))<1){
				last_x = x;
				last_y = y;
				// OpenGL draw line
				Scene::lines.push_back({{x1,y1},{last_x,last_y}, GREEN, 5.0});
			}
			else
				return false;
		}
		else
			return false;
	}
	return true;
}

double Game::checkPressure(LocPair loc){
	int x = (int)loc.start.getX();
	int y = (int)loc.start.getY();
	int radius = (int)loc.start.getR();
	double pressure = frame_data.at(x + y*MAX_X);
	if (y - radius / 2 >= 0 && frame_data.at(x + (y - radius / 2)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y - radius / 2)*MAX_X);
	if (y - radius >= 0 && frame_data.at(x + (y - radius)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y - radius)*MAX_X);
	if (y + radius / 2 < MAX_Y && frame_data.at(x + (y + radius / 2)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y + radius / 2)*MAX_X);
	if (y + radius<MAX_Y && frame_data.at(x + (y + radius)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y + radius)*MAX_X);
	if (x - radius / 2 >= 0 && frame_data.at((x - radius / 2) + y*MAX_X) > pressure)
		pressure = frame_data.at((x - radius / 2) + y*MAX_X);
	if (x - radius >= 0 && frame_data.at((x - radius) + y*MAX_X) > pressure)
		pressure = frame_data.at((x - radius) + y*MAX_X);
	if (x + radius / 2 < MAX_X && frame_data.at((x + radius / 2) + y*MAX_X) > pressure)
		pressure = frame_data.at((x + radius / 2) + y*MAX_X);
	if (x + radius<MAX_X && frame_data.at((x + radius) + y*MAX_X) > pressure)
		pressure = frame_data.at((x + radius) + y*MAX_X);
	return pressure;
}

else if (mode == 'k')
{
	int count = 0;
	// create a locpair already
		//1. check pressure at start ring
		double pressure = checkPressure(locpair);
		while(!withinPressure(pressure)){
			pressure = checkPressure(locpair);
		}
		//2. check if start ring is "locked-in" (ready to draw the line)
		//		a. if start ring is not locked in keep checking for locked in
		
		// OpenGL draw help line
		Scene::lines.push_back({locpair.start.getX(),locpair.start.getY()},
			{locpair.destination.getX(),locpair.destination.getX()}, RED, 3.0});

		//3. if start ring is locked in keep track of where she is pressing
		//		a. 
		while(!line(locpair)){
			count++;
		  	if (count>10)
		    		break;
		}

	}
}
