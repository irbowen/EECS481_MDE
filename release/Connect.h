#ifndef CONNECT_H
#define CONNECT_H
#include "location.h"
#include "graphics.h"
#include <unordered_map>

using std::unordered_map;

class Connect {
	
	
	vector<pair<double, double>> points;
	static int resolution;
	vector<pair<double, double>>::iterator cur;
	vector<Location>::iterator next_dot;
	static double minDistance;
public:
	//Connect();
	vector<Location> dots;
	unordered_map<int, LinePair> lines;
	bool empty();
	Location front();
	//Connect(const vector<Location>&);
	bool processCursor(const pair<double, double>&);
	inline void draw(){ for (auto& x : lines) x.second.draw(); for (auto& x : dots) x.draw(); }
};
#endif