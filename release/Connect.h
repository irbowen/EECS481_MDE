#ifndef CONNECT_H
#define CONNECT_H
#include "location.h"
#include "graphics.h"
#include <unordered_map>

using std::unordered_map;

class Connect {
	//vector<pair<double, double>> points;

	static int resolution;
	
	static double minDistance;
public:
	//Connect();
	//Connect(const vector<Location>&);

	//vector<pair<double, double>>::iterator cur;
	//vector<Location>::iterator next_dot;

	int cur;
	int next_dot;

	vector<pair<double, double>> points;
	vector<Location> dots;
	unordered_map<int, LinePair> lines;
	bool empty();
	Location front();
	
	bool processCursor(const pair<double, double>&);
	inline void draw(){ for (auto& x : lines) x.second.draw(); for (auto& x : dots) x.draw(); }
};
#endif