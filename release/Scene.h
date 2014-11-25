#ifndef SCENE_H
#define SCENE_H

#include "graphics.h"
#include "Circle.h"
#include "Cursor.h"

#include <unordered_map>

using std::unordered_map;

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

	static vector<DebugCursor> debugCursors;

	static GameCursors gameCursors;

	// loc-id -> highlighter
	static unordered_map<int, RotatingMultiCursor> targetHighlighters;

	//draft
	static vector<LocPair> locpairs;

	static void draw();

};
//END OF SCENE
#endif