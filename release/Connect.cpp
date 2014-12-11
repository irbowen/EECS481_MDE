#include "Connect.h"
#include "geometry.h"
#include <iterator>

double Connect::minDistance = 50;

int Connect::resolution = 50;

static double startMin = 50;

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

	if (next_dot == dots.size())
		return true;

	auto nextDotLoc = std::make_pair(dots[next_dot].getX(), dots[next_dot].getY());

	bool gotPoint = false;

	if (::distance(points[cur], pt) <= minDistance){

		gotPoint = true;

		bool gotDot = false;
		if (points[cur++] == nextDotLoc){

			gotDot = true;
			dots[next_dot].target.setGoalProgress(1.0);

			if (++next_dot == dots.size()){
				return true;
			}

			// while *cur within radius of dot we just got, increment cur
			while (::distance(points[cur], nextDotLoc) <= dots[next_dot-1].getRadius())
				++cur;
		}

		if (!gotDot && next_dot != 0){
			// dots per detection bubble radius
			int dotsOffset = (int)(minDistance * (resolution / lines.at(dots[next_dot - 1].id).length()));

			double prog = ((cur + 1) % resolution) / (double)resolution;

			prog = prog < 0 ? 0 : (prog > 1 ? 1 : prog);
			lines.at(dots[next_dot - 1].id).setProgress(prog);
		}

	}

	if (cur == points.size())
		return true;

	auto nxt = std::make_pair(dots[next_dot].getX(), dots[next_dot].getY());

	if (points[cur] == nxt){
		minDistance = dots[next_dot].getRadius();
	}
	else
		minDistance = startMin;

	// while *cur is within radius of next_dot, increment cur until at next dot
	while (::distance(points[cur], nextDotLoc) <= dots[next_dot].getRadius() && points[cur] != nextDotLoc){
		if (++cur == points.size()){
			--cur;
			break;
		}
	}

	return false;
}

pair<double, double> Connect::curGoal() 
{ 
	return points.empty() ? std::make_pair(-50.0, -50.0) : (cur == points.size() ? std::make_pair(-50.0, -50.0) : points[cur]); 
}