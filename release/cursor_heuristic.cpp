#include "cursor_heuristic.h"
#include "DepthBasics.h"
#include <cmath>
#include <thread>
#include <deque>
#include <iterator>
#include <iostream>

#define MIN_CURSOR_SEPARATION 250
#define MAX_CURSORS 4
#define CURSOR_HISTORY_DEPTH 10
#define CURSOR_NOISE_THRESH 150
#define PREVIOUS_CURSOR_DEPTH 10

// % of the past CURSOR_HISTORY_DEPTH frames there must be
// a cursor within CURSOR_NOISE_THRESH
#define CURSOR_BLINK_TOLERANCE 0.8

using std::deque;
using std::next;

const int PARTITION_WIDTH = 16;
const int PARTITION_HEIGHT = 16;
const int GRID_WIDTH = 640 / PARTITION_WIDTH;
const int GRID_HEIGHT = 480 / PARTITION_HEIGHT;

mutex cursorLock;

static deque<vector<pair<double, double>>> prevCursors;

static set<Cursor> sortedCursors;

static deque<vector<pair<double, double>>> cursorHistory;

bool operator<(const Cursor& a, const Cursor& b){
	return a.depth < b.depth;
}


static double distance(const pair<double, double>& p1, const pair<double, double>& p2){

	double dx = p2.first - p1.first;
	double dy = p2.second - p1.second;

	return sqrt(dx*dx + dy*dy);
}

// running average of cursor points
vector<pair<double, double>> getCursorPoints(){

	if (cursorHistory.size() != CURSOR_HISTORY_DEPTH)
		return vector < pair < double, double > > {};

	vector<pair<double, double>> rtn;

	cursorLock.lock();
	auto cpy = cursorHistory;
	auto prevFrames = prevCursors;
	cursorLock.unlock();

	for (const auto& frame : prevFrames){

		for (const auto& prev : frame){

			bool blinked = true;

			for (const auto& candidate : cpy.front()){
				if (distance({ prev.first, prev.second },
				{ candidate.first, candidate.second })
				<= CURSOR_NOISE_THRESH){
					blinked = false;
					break;
				}
			}

			if (blinked)
				cpy.front().push_back({ prev.first, prev.second });
		}

	}

	for (const auto& candidate : cpy.front()){

		double xTot = candidate.first,
			   yTot = candidate.second;

		int matched = 0;

		for (auto& frame = next(cpy.begin()); frame != cpy.end(); ++frame){

			for (const auto& cursor : *frame){

				if (distance({ cursor.first, cursor.second },
							 { candidate.first, candidate.second })
							 <= CURSOR_NOISE_THRESH){
					xTot += cursor.first;
					yTot += cursor.second;
					++matched;
					break;
				}
				
			}

		}

		if (matched >= (int)(CURSOR_BLINK_TOLERANCE * (cpy.size() - 1)))
			rtn.push_back({ xTot / matched,
							yTot / matched });
	}
	/*
	// handle blinking cursor case [cursor missing from latest frame but present in others]
	for (const auto& candidate : *next(cpy.begin())){

		double xTot = candidate.first,
			   yTot = candidate.second;

		for (const auto& placed : rtn){
			if (distance({ xTot, yTot },
			{ placed.first, placed.second })
			< CURSOR_NOISE_THRESH)
				continue;
		}

		int matched = 0;

		for (auto& frame = next(cpy.begin()); frame != cpy.end(); ++frame){

			for (const auto& cursor : *frame){

				if (distance({ cursor.first, cursor.second },
				{ candidate.first, candidate.second })
				<= CURSOR_NOISE_THRESH){
					xTot += cursor.first;
					yTot += cursor.second;
					++matched;
					break;
				}

			}

		}

		if (matched >= (int)(CURSOR_BLINK_TOLERANCE * (cpy.size() - 1)))
			rtn.push_back({ xTot / matched,
			yTot / matched });
	}
	*/

	cursorLock.lock();
	prevCursors.push_front(rtn);
	if (prevCursors.size() == PREVIOUS_CURSOR_DEPTH)
		prevCursors.pop_back();
	cursorLock.unlock();

	return rtn;
}

static vector<pair<double, double>> getCursorPointsFrame(){

	if (sortedCursors.empty())
		return vector < pair<double, double> > {};
	
	vector<pair<double, double>> rtn;
	rtn.push_back({ sortedCursors.begin()->x, sortedCursors.begin()->y });

	for (const auto& cursor : sortedCursors){

		bool add = true;

		for (const auto& placed : rtn){

			if (distance({ cursor.x, cursor.y },
						 { placed.first, placed.second }) 
						 <= MIN_CURSOR_SEPARATION){

				add = false;
				break;
			}
		}

		if (add)
			rtn.push_back({cursor.x, cursor.y});

		if (rtn.size() == MAX_CURSORS)
			break;

	}

	return rtn;
}

int cursorThread(){

	while (true){

		// make sure consecutive processed frames are unique
		while (!newFrameReady)
			std::this_thread::yield();

		newFrameReady = false;
		sortedCursors.clear();
	
		std::vector<std::vector<CursorDepthSquare>> gridMins(GRID_WIDTH, std::vector<CursorDepthSquare>(GRID_HEIGHT));

		for (int j = 0; j < GRID_HEIGHT * GRID_WIDTH; j++){

			CursorDepthSquare minVal = { -1, minDepth, true };

			for (int k = 0; k < PARTITION_HEIGHT * PARTITION_WIDTH; k++){

				int l = ABSOLUTE_INDEX(j, k);
				int curVal = (int)(frame_data[l] - initial_buffer[l]);

				if (curVal < minVal.depth && frame_data[l])
					minVal = { l, (int)frame_data[l], false };

			}
			gridMins[j % GRID_WIDTH][j / GRID_WIDTH] = minVal;
		}

		for (int j = 0; j < GRID_WIDTH; j++){
			for (int k = 0; k < GRID_HEIGHT; k++){
				if (!gridMins[j][k].checked){
					CursorDepthSquare minimumPoint = gridMins[j][k];
					gridMins[j][k].checked = true;
					int l = j, temp_l = 0;
					int m = k, temp_m = 0; //so we can look through the array w/o messing everything up
					bool addCursor = true;
					while (1){
						bool foundMax = false;
						if (l && gridMins[l - 1][m].depth < minimumPoint.depth){
							minimumPoint = gridMins[l - 1][m];
							temp_l = l - 1;
							temp_m = m;
							foundMax = true;
							if (gridMins[l - 1][m].checked){
								addCursor = false;
							}
						}
						if (l < GRID_WIDTH - 1 && gridMins[l + 1][m].depth < minimumPoint.depth){
							minimumPoint = gridMins[l + 1][m];
							temp_l = l + 1;
							temp_m = m;
							foundMax = true;
							if (gridMins[l + 1][m].checked){
								addCursor = false;
							}
						}
						if (m && gridMins[l][m - 1].depth < minimumPoint.depth){
							minimumPoint = gridMins[l][m - 1];
							temp_l = l;
							temp_m = m - 1;
							foundMax = true;
							if (gridMins[l][m - 1].checked){
								addCursor = false;
							}
						}
						if (m < GRID_HEIGHT - 1 && gridMins[l][m + 1].depth < minimumPoint.depth){
							minimumPoint = gridMins[l][m + 1];
							temp_l = l;
							temp_m = m + 1;
							foundMax = true;
							if (gridMins[l][m + 1].checked){
								addCursor = false;
							}
						}
						if (!addCursor){
							break;
						}
						else{
							gridMins[temp_l][temp_m].checked = true;
							if (!foundMax){
								break;
							}
							foundMax = false;
							l = temp_l;
							m = temp_m;
						}
					}

					if (addCursor){
						sortedCursors.insert({ minimumPoint.index % 640, minimumPoint.index / 640, minimumPoint.depth});
					}
				}

			}
		}

		cursorLock.lock();
		cursorHistory.push_front(getCursorPointsFrame());
		if (cursorHistory.size() > CURSOR_HISTORY_DEPTH)
			cursorHistory.pop_back();
		cursorLock.unlock();
	}
}
