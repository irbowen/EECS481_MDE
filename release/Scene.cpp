#include "Scene.h"
#include "DepthBasics.h"
#include "cursor_heuristic.h"
#include <unordered_set>

using std::mutex;
using std::cout;
using std::endl;
using std::unordered_set;

mutex LocationLock;
vector<LocPair> Scene::locpairs;
list<Location> Scene::locations;
vector<PolygonGL> Scene::polys;
vector<ColorSlideRing> Scene::rings;
vector<Point> Scene::points;
list<Circle> Scene::circles;
vector<Line> Scene::lines;
LocPair Scene::locpair(-1, -1, -1, -1, -1, -1);
CursorContainer Scene::cursors;
vector<RotatingMultiCursor> Scene::fancyCursors;
vector<DebugCursor> Scene::debugCursors;
unordered_map<int, RotatingMultiCursor> Scene::targetHighlighters;
GameCursor Scene::gameCursor;

mutex highlightLock;

unordered_set<int> deadLocations;

void Scene::draw(){

	debugCursors.clear();

	
	if (pullRegistered()){
		int x = pull_index % 640;
		int y = pull_index / 640;
		gameCursor.rotateScheme();
		auto colors = gameCursor.cursor.getColors();

		int angle = 0;

		for (const auto& color : colors){

			auto pos = jump({ x, y }, 50, angle += 360 / colors.size());

			circles.push_back({pos.first, pos.second, 50, color});

			circles.back().fade(2000);
		}
	}

	for (const auto& pt : getCursorPoints()){
		gameCursor.setPos({ pt.first, pt.second });
		auto pressure = frame_data.at((unsigned)(pt.second*640+ pt.first));

		// normalized to shallowest possible cursor
		auto forward = initial_buffer.at(640*pt.second + pt.first) - pressure + minDepth;
		auto r = gameCursor.cursor.startR + forward / 10;
		r = r > 2 * gameCursor.cursor.startR ? 2 * gameCursor.cursor.startR : (r < 0 ? 0 : r);
		gameCursor.cursor.setR(r > 2 * gameCursor.cursor.startR ? 2 * gameCursor.cursor.startR : r);

		gameCursor.addCircle(1500.0);
	}


	for (auto& x : debugCursors) x.draw();

	cursors.draw();
	locpair.draw();



	for (auto& x : fancyCursors) x.draw();

	for (auto& x : polys) x.draw();
	for (auto& x : rings) x.draw();
	for (auto& x : points) x.draw();

	LocationLock.lock();
	locations.remove_if([](const Location& l){ if (l.target.ring.faded) deadLocations.insert(l.id); return l.target.ring.faded; });
	for (auto& x : locations) x.draw();
	LocationLock.unlock();

	circles.remove_if([](const Circle& c){ return c.faded; });
	for (auto& x : circles) x.draw();

	for (auto& x : locpairs) x.draw();
	for (auto& x : lines) x.draw();

	highlightLock.lock();
	for (auto it = targetHighlighters.begin(); it != targetHighlighters.end();){

		auto ms = it->second.elapsed();

		if (it->second.exploding && ms >= RotatingMultiCursor::explodeDuration){
			it = targetHighlighters.erase(it);
			continue;
		}

		if (it->second.exploding && ms < RotatingMultiCursor::explodeDuration / 2)
			it->second.addCircle(RotatingMultiCursor::explodeDuration / 2);
		
		it->second.draw();
		++it;
	}
	highlightLock.unlock();

	gameCursor.draw();


}
