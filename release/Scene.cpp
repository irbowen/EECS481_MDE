#include "Scene.h"
#include "DepthBasics.h"
#include "cursor_heuristic.h"

using std::mutex;
using std::cout;
using std::endl;

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
vector<DebugCursor> Scene::debugCursors;
unordered_map<int, RotatingMultiCursor> Scene::targetHighlighters;
GameCursor Scene::gameCursor;

void Scene::draw(){

	debugCursors.clear();

	for (const auto& pt : getCursorPoints()){

		gameCursor.setPos({ pt.first, pt.second });
		auto pressure = frame_data.at((unsigned)(pt.second*640+ pt.first));

		// normalized to shallowest possible cursor
		auto forward = initial_buffer.at(640*pt.second + pt.first) - pressure + minDepth;
		gameCursor.cursor.setR(gameCursor.cursor.startR + forward / 10);

		gameCursor.addCircle();
	}


	for (auto& x : debugCursors) x.draw();

	cursors.draw();
	locpair.draw();



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

	for (auto& x : targetHighlighters) x.second.draw();

	gameCursor.draw();


}
