#include "Connect.h"
#include "geometry.h"
#include <iterator>

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


bool Connect::processCursor(const pair<double, double>& pt){

	auto nextDotLoc = std::make_pair(next_dot->getX(), next_dot->getY());

	if (::distance(*cur, pt) <= minDistance){

		double prog = ((cur - points.begin() + 1) % resolution) / (double) resolution;

		if (next_dot != dots.begin())
			lines.at(std::prev(next_dot)->id).setProgress(prog);

		if (*cur++ == nextDotLoc){
			next_dot->target.setGoalProgress(1.0);

			if (++next_dot == dots.end()){
				return true;
			}

			// while *cur within radius of dot we just got, increment cur
			while (::distance(*cur, nextDotLoc) <= std::prev(next_dot)->getRadius())
				++cur;
		}
	}
	// while *cur is within radius of next_dot, increment cur until at next dot
	while (::distance(*cur, nextDotLoc) <= next_dot->getRadius() && *cur != nextDotLoc)
		++cur;

	return false;
}

double Connect::minDistance = 10;

int Connect::resolution = 50;
