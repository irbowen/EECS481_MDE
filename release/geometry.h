#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <utility>

#define PI 3.14159265

// Given an initial point, a distance, and an angle in degrees, returns a new point
std::pair<double, double> jump(const std::pair<double, double>& p, double d, int a);

std::pair<double, double> between(const std::pair<double, double>& p1, const std::pair<double, double>& p2, double prct);

double distance(const std::pair<double, double>& a, const std::pair<double, double>& b);

#endif