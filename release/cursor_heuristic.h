#ifndef CURSOR_HEURISTIC_H
#define CURSOR_HEURISTIC_H

#include <vector>
#include <utility>
#include <mutex>
#include <set>

using std::mutex;
using std::pair;
using std::vector;
using std::set;

#define ABSOLUTE_INDEX(i,j) (((i / GRID_WIDTH) * PARTITION_HEIGHT * 640) + ((i % GRID_WIDTH) * PARTITION_WIDTH) + (j % PARTITION_WIDTH) + ((j / PARTITION_WIDTH) * 640))

struct CursorDepthSquare{
	int index;
	int depth;
	bool checked;
};

struct Cursor {
	int x, y, depth;
};

bool operator<(const Cursor& a, const Cursor& b);


extern vector<pair<double, double>> cursorPoints;

extern const int PARTITION_WIDTH;
extern const int PARTITION_HEIGHT;
extern const int GRID_WIDTH;
extern const int GRID_HEIGHT;

vector<pair<double, double>> getCursorPoints();

int cursorThread();
#endif