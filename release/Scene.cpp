#include "Scene.h"

using std::mutex;
using std::cout;
using std::endl;

#define NEVER_DO_THIS RotatingMultiCursor{ 320, 240, 50, 5, \
{\
	new GradientCircleCursor{ 0, 0, 24, { GREEN, palette(GREEN), palette(GREEN) }, 100 },\
		new GradientCircleCursor{ 0, 0, 24, { GREEN, palette(GREEN), palette(GREEN) }, 100 },\
		new GradientCircleCursor{ 0, 0, 24, { GREEN, palette(GREEN), palette(GREEN) }, 100 }\
}\
}

mutex LocationLock;
vector<LocPair> Scene::locpairs;
list<Location> Scene::locations;
vector<PolygonGL> Scene::polys;
vector<ColorSlideRing> Scene::rings;
vector<Point> Scene::points;
vector<Circle> Scene::circles;
vector<Line> Scene::lines;
LocPair Scene::locpair(-1, -1, -1, -1, -1, -1);
CursorContainer Scene::cursors;
vector<RotatingMultiCursor> Scene::fancyCursors;
DebugCursor Scene::debugCursor{ 320, 240, 50 };
///unordered_map<Location*, RotatingMultiCursor> Scene::targetHighlighters;

void Scene::draw(){

	debugCursor.draw();
	cursors.draw();
	locpair.draw();

	//for (auto& x : targetHighlighters) x.second.draw();

	for (auto& x : fancyCursors) x.draw();

	for (auto& x : polys) x.draw();
	for (auto& x : rings) x.draw();
	for (auto& x : points) x.draw();

	LocationLock.lock();
	locations.remove_if([](const Location& l){ return l.target.ring.faded; });
	for (auto& x : locations) x.draw();
	LocationLock.unlock();

	for (auto& x : circles) x.draw();
	for (auto& x : locpairs) x.draw();
	for (auto& x : lines) x.draw();
}
