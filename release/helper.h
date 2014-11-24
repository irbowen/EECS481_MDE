#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include "Scene.h"
#include "LocPair.h"
#include "location.h"

using namespace std;

double dist(int x1, int y1, int x2, int y2)
{
	int dist_x = x2 - x1;
	int dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x) + (dist_y*dist_y));

	return length;
}

//checks if (x3,y3) is inbetween (x1,y1) and (x2,y2)
bool on_line(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2) - dist(x1, y1, x2, y2)) / dist(x1, y1, x2, y2) < 0.1)
		return true;
	else
		return false;
}

//void drawLine(double x1, double y1, double x2, double y2){




void paircheckCursors()
{
	LocPair& pair = Scene::locpair;

	for (auto& cursor : Scene::debugCursors) 
	{
			int curs_x = cursor.getX();
			int curs_y = cursor.getY();
			if (pair.start.contains(curs_x, curs_y)) 
			{
				
			}

	}
}


