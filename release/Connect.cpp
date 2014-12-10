#include "Connect.h"
#include "geometry.h"
#include <iterator>

/*
Connect::Connect(const vector<Location>& in) {
	dots = in;
	for (unsigned i = 0; i < dots.size() - 1; ++i){
		
		for (int j = 0; j < resolution; ++j)
			points.push_back(between({ dots[i].getX(), dots[i].getY() }, { dots[i+1].getX(), dots[i+1].getY()}, ((double)j) / 50));

		lines.insert({dots[i].id, {
				{ dots[i].getX(), dots[i].getY() },
				{ dots[i + 1].getX(), dots[i + 1].getY() },
				GREEN,
				RED
				} });
	}

	next_dot = dots.begin();
	cur = points.begin();

}
*/

bool Connect::empty()
{
	return dots.empty();
}

bool Connect::processCursor(const pair<double, double>& pt){

	auto nextDotLoc = std::make_pair(dots[next_dot].getX(), dots[next_dot].getY());

	if (::distance(points[cur], pt) <= minDistance){

		double prog = ((cur + 1) % resolution) / (double) resolution;

		if (next_dot != 0)
			lines.at(dots[next_dot-1].id).setProgress(prog);

		if (points[cur++] == nextDotLoc){
			dots[next_dot].target.setGoalProgress(1.0);

			if (++next_dot == dots.size()){
				return true;
			}

			// while *cur within radius of dot we just got, increment cur
			while (::distance(points[cur], nextDotLoc) <= dots[next_dot-1].getRadius())
				++cur;
		}
	}
	// while *cur is within radius of next_dot, increment cur until at next dot
	while (::distance(points[cur], nextDotLoc) <= dots[next_dot].getRadius() && points[cur] != nextDotLoc)
		++cur;

	return false;
}

double Connect::minDistance = 10;

int Connect::resolution = 50;
