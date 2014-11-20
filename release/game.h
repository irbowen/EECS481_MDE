#ifndef GAME_H
#define GAME_H

#include <vector>
#include <chrono>
#include <thread>
#include <time.h>
#include <sstream>
//For playing sound
#include "Windows.h"
#include "Mmsystem.h"
#include "DepthBasics.h"
#include "graphics.h"



class Game {
	const int NUM_ROUNDS = 1000;
	const double INCREASE_FACTOR = 4;
	const int BREAK_FACTOR = 20;
	const int MAX_X = 640;
	const int MAX_Y = 480;
	const int MAX_NUM_SPOTS = 15;
	const int SAMPLE_MILLISECONDS = 100;
	double start_radius = 20;
	int num_active_spots = 0;
	Location createRandomLocation();
	double checkPressure(Location loc);

	CDepthBasics* kinect;
public:
//	static std::vector<Location> loc_list;
	Game();
	Game(CDepthBasics& kinect);
	void run();
	void startGame();
	void startKinect();
	void printRemainingLocations();
	void printRemovedLocations();
};
#endif
