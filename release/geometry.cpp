#include "geometry.h"
#include <cmath>

#define PI 3.14159265


std::pair<double, double> between(const std::pair<double, double>& p1, const std::pair<double, double>& p2, double prct){

	return{ p1.first + prct * (p2.first - p1.first),
		p1.second + prct * (p2.second - p1.second) };

}

std::pair<double, double> jump(const std::pair<double, double>& p, double d, int a){
	double theta = a * PI / 180;
	return{ p.first + d * cos(theta), p.second + d * sin(theta) };
}

double distance(const std::pair<double, double>& a, const std::pair<double, double>& b){
	double dist_x = b.first - a.first;
	double dist_y = b.second - a.second;
	return sqrt(dist_x*dist_x + dist_y*dist_y);
}