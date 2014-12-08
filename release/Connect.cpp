#include "Connect.h"
#include "geometry.h"

Connect::Connect(const vector<Location>& in) {
	dots = in;
	for (int i = 0; i < dots.size() - 1; ++i){
		
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
	if (::distance(*cur, pt) <= minDistance){

		double prog = ((cur - points.begin() + 1) % resolution) / (double) resolution;

		lines[next_dot->id].setProgress(prog);

		if (*cur == std::make_pair(next_dot->getX(), next_dot->getY())){
			// got next location, fill in

			// while *cur within radius of next_dot (aka 'this dot' still), increment cur

			if (++next_dot == dots.end()){
				// beat whole thing, return true
			}
		}



	}
	// while *cur is within radius of next_dot, increment cur until at next dot
}

double Connect::minDistance = 10;

int Connect::resolution = 50;
