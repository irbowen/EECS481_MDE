//Isaac Bowen
//eecs481 fall2014

#include "game.h"
#include "DepthBasics.h"
#include "graphics.h"
#include <mutex>
#include <thread>
#include "color.h"
#include "location.h"
#include <fstream>
#include "LocPair.h"
#include "Scene.h"
#include <vector>
#include <queue>
#include <math.h>

using std::mutex;

Color palette(Color& c);

void Game::runConnectMode()
{
	//Scene::locpairs.push_back(createRandomLocPair(50, 50, 300, 300));
	//Scene::locpair = createRandomLocPair(50, 50, 300, 300);


	//1. check pressure at start ring

	double pressure = checkPressure((int)Scene::locpair.start.getX(), (int)Scene::locpair.start.getY(), (int)Scene::locpair.start.getR());

	std::cout << "Pressure is " << pressure << std::endl;
	//2. check if start ring is "locked-in" (ready to draw the line)
	//		a. if start ring is not locked in keep checking for locked in
	if (!Scene::locpair.withinPressure(pressure))
	{
		//std::cout << "pressure req not met" << std::endl;
		//pressure = checkPressure((int)Scene::locpair.start.getX(), (int)Scene::locpair.start.getY(), (int)Scene::locpair.start.getR());
		//std::cout << "Inside pressure is " << pressure << std::endl;
	}

	else if (Scene::locpair.withinPressure(pressure))
	{
		Scene::locpair.locked = true;
		Scene::lines.push_back({ { (int)Scene::locpair.start.getX(), (int)Scene::locpair.start.getY() },
		{ (int)Scene::locpair.dest.getX(), (int)Scene::locpair.dest.getY() }, RED, 5.0 });
		//3. if start ring is locked in keep track of the cursor (Ara's Hand)
		//		a. 
		if (Scene::locpair.locked == true)
		{
			int i = 0;
			while (line(Scene::locpair))
			{
				if (i++ > 10) break;
			}
		}
	}
}

LocPair Game::createRandomLocPair(int opt_x1, int opt_y1, int opt_x2, int opt_y2)
{
	int start_x, start_y, dest_x, dest_y;

	start_x = rand() % MAX_X;
	start_y = rand() % MAX_Y;

	dest_x = rand() % MAX_X;
	dest_y = rand() % MAX_Y;

	if (opt_x1 > -1 && opt_y1 > -1 && opt_x2 > -1 && opt_y2 > -1)
	{
		start_x = opt_x1;
		start_y = opt_y1;

		dest_x = opt_x2;
		dest_y = opt_y2;
	}

	return LocPair(start_x, start_y, dest_x, dest_y, start_radius, initial_buffer.at(MAX_X*start_y + start_x));
}

double Game::dist(int x1, int y1, int x2, int y2)
{
	int dist_x = x2 - x1;
	int dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x) + (dist_y*dist_y));

	return length;
}

bool Game::on_line(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2) - dist(x1, y1, x2, y2)) / dist(x1, y1, x2, y2) < 0.1)
		return true;
	else
		return false;
}

bool Game::line(LocPair& locpair)
{
	const int MAX_X = 640;
	const int MAX_Y = 480;
	ColorSlideRing &loc1 = locpair.start;
	ColorSlideRing &loc2 = locpair.dest;
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
			//OpenGL draw line
			Scene::lines.push_back({ { x1, y1 }, { last_x, last_y }, RED, 5.0 });
			//}
		}
		else
			return false;
	}
	return true;
}