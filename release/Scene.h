#ifndef SCENE_H
#define SCENE_H

#include "graphics.h"
#include "Circle.h"
#include "Cursor.h"

//START OF SCENE
class Scene {
public:
	static LocPair locpair;
	static list<Location> locations;
	static vector<PolygonGL> polys;
	static vector<ColorSlideRing> rings;
	static vector<Point> points;
	static vector<Circle> circles;
	static vector<Line> lines;
	static vector<RotatingMultiCursor> fancyCursors;
	static CursorContainer cursors;

	//draft
	static vector<LocPair> locpairs;

	static void draw();

};
//END OF SCENE
#endif