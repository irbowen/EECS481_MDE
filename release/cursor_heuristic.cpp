#include "cursor_heuristic.h"
#include "DepthBasics.h"
#include <cmath>

#define MIN_CURSOR_SEPARATION 100

const int PARTITION_WIDTH = 32;
const int PARTITION_HEIGHT = 32;
const int GRID_WIDTH = 640 / PARTITION_WIDTH;
const int GRID_HEIGHT = 480 / PARTITION_HEIGHT;

mutex cursorLock;


set<Cursor> sortedCursors;

bool operator<(const Cursor& a, const Cursor& b){
	return a.depth < b.depth;
}


static double distance(const pair<double, double>& p1, const pair<double, double>& p2){

	double dx = p2.first - p1.first;
	double dy = p2.second - p1.second;

	return sqrt(dx*dx + dy*dy);
}

vector<pair<double, double>> getCursorPoints(){
	
	if (sortedCursors.empty())
		return vector < pair<double, double> > {};

	cursorLock.lock();
	auto cpy = sortedCursors;
	cursorLock.unlock();

	vector<pair<double, double>> rtn;
	rtn.push_back({ cpy.begin()->x, cpy.begin()->y });

	for (const auto& cursor : cpy){

		bool add = true;

		for (const auto& placed : rtn){
			if (distance({ cursor.x, cursor.y }, { placed.first, placed.second }) <= MIN_CURSOR_SEPARATION){
				add = false;
				break;
			}
		}

		if (add)
			rtn.push_back({cursor.x, cursor.y});

	}

	return rtn;
}

int cursorThread(){

	while (true){
	
		std::vector<std::vector<CursorDepthSquare>> gridMins(GRID_WIDTH, std::vector<CursorDepthSquare>(GRID_HEIGHT));
		for (int j = 0; j < GRID_HEIGHT * GRID_WIDTH; j++){
			CursorDepthSquare minVal = { -1, minDepth, true };
			for (int k = 0; k < PARTITION_HEIGHT * PARTITION_WIDTH; k++){
				int l = ABSOLUTE_INDEX(j, k);
				int curVal = (int)(frame_data[l] - initial_buffer[l]);
				if (curVal < minVal.depth && frame_data[l]){

					minVal = { l, (int)frame_data[l], false };


					/*
					bool lessThanSurr = true;
					if (l > 640 && curVal >= frame_data[l - 640] - initial_buffer[l - 640])
					lessThanSurr = false;
					if (lessThanSurr && l < 640 * 480 - 640 && curVal >= frame_data[l + 640] - initial_buffer[l + 640])
					lessThanSurr = false;
					if (lessThanSurr && l % 640 && curVal >= frame_data[l - 1] - initial_buffer[l - 1])
					lessThanSurr = false;
					if (lessThanSurr && (l % 640 != 639) && curVal >= frame_data[l + 1] - initial_buffer[l + 1])
					lessThanSurr = false;
					if (lessThanSurr){
					minVal = { l, frame_data[l], false };
					}
					*/
				}
			}
			gridMins[j % GRID_WIDTH][j / GRID_WIDTH] = minVal;
			/*cursorLock.lock();
			if (minVal.index != -1)
			Scene::debugCursors.push_back({ minVal.index % 640, minVal.index / 640, 20 });
			cursorLock.unlock();*/
		}

		
		set<Cursor> temp;

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
						temp.insert({ minimumPoint.index % 640, minimumPoint.index / 640, minimumPoint.depth});
					}
				}

			}
		}

		cursorLock.lock();
		sortedCursors = temp;
		cursorLock.unlock();

	}
}
