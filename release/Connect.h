#ifndef CONNECT_H
#define CONNECT_H
#include "location.h"
#include "graphics.h"
#include <unordered_map>

using std::unordered_map;

class Connect {
	vector<Location> dots;
	unordered_map<int, LinePair> lines;

	vector<pair<double, double>> points;

public:

	vector<pair<double, double>>::iterator cur;

	vector<Location>::iterator next_dot;

	Connect(const vector<Location>&);

	bool processCursor(const pair<double, double>&);

	static double minDistance;

	static int resolution;

	inline void draw(){ for (auto& x : lines) x.second.draw(); for (auto& x : dots) x.draw(); }
};
#endif