#ifndef GAME_H
#define GAME_H

#include <vector>
#include <chrono>
#include <thread>
#include <time.h>
#include <sstream>
#include "Windows.h"
#include "Mmsystem.h"
#include "DepthBasics.h"
#include "graphics.h"

struct CursorDepth{
	int index;
	int depth;
	bool checked;
};

class Game {
	const int NUM_ROUNDS = 1000;
	const double INCREASE_FACTOR = 4;
	const int BREAK_FACTOR = 20;
	const int MAX_X = 640;
	const int MAX_Y = 480;
	const int MAX_NUM_SPOTS = 3;
	const int MAX_RADIUS = 50;
	const int SAMPLE_MILLISECONDS = 100;
	double start_radius = 20;
	//std::vector<double> intial_buffer;
	int num_active_spots = 0, num_triggered_spots = 0;
	Location createRandomLocation(double radius_scale_factor);

	Location createLocation(int xx, int yy, double radius_scale_factor);
	LocPair createRandomLocPair(int opt_x1 = -1, int opt_y1 = -1, int opt_x2 = -1, int opt_y2 = -1);
	double checkPressure(int x, int y, int radius);
	CDepthBasics* kinect;
public:
	Game();
	Game(CDepthBasics& kinect);
	void run(char mode);						//s = slidering | k = kinect-the-dots
	void startGame();
	void startKinect();
	void printRemainingLocations();
	void printRemovedLocations();
	void runSlideRingMode(int);

	//for connectmode
	void runConnectMode();
	bool line(LocPair& locpair);
	double dist(int x1, int y1, int x2, int y2);
	bool on_line(int x1, int y1, int x2, int y2, int x3, int y3);
};
#endif
