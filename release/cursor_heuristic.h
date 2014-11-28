// God-heuristic for finding cursor points given depth data

//// A: Get cursors for single input frame
// 1) divide frame into grid, finding min of each square
// 2) find minimum of each contiguous chunk of min-squares
// 3) sort those minimums
// 4) take a certain number of cursors from top of sorted set, excluding pts too close to those already placed

//// B: Compute smoothed cursor movement
// 1) Keep a sliding window of frames created in [A]
// 2) Compute averages of cursor positions that appear in the latest frame as well as most other history frames

//// C: reduce 'blinking' (when cursor doesn't appear in latest, but has been on screen recently)
// 1) Keep a sliding window of frames created in [B]
// 2) If any cursors in the window aren't close to any in the latest frame, add them to the latest frame before doing [B]
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

extern const int PARTITION_WIDTH;
extern const int PARTITION_HEIGHT;
extern const int GRID_WIDTH;
extern const int GRID_HEIGHT;

vector<pair<double, double>> getCursorPoints();

int cursorThread();

#endif