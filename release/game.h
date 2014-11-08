//Isaac Bowen
//eecs481 fall2014

#include <vector>
#include <chrono>
#include <thread>
#include <time.h>
#include "location.h"

class Game {
	const int NUM_ROUNDS = 10;
	const int INCREASE_FACTOR = 1;
	const int BREAK_FACTOR = 20;
	const int MAX_X = 100;
	const int MAX_Y = 100;
	const int MAX_NUM_SPOTS = 10;
	const int SAMPLE_MILLISECONDS = 100;
	const int POINTS_PER_CORRECT = 100;
	int num_points;
	int start_radius = 5;
	int num_active_spots = 0;
	std::vector<Location> loc_list;
	Location createRandomLocation();
public:
	bool input = false;
	Game();
	void run();
	void startGame();
};