//Isaac Bowen
//eecs481 fall2014

#include <iostream>
#include <vector>
#include <windows.h>
#include "location.h"

class Game {
	const int NUM_ROUNDS = 10;
	const int INCREASE_FACTOR = 2;
	const int BREAK_FACTOR = 20;
	const int MAX_X = 100;
	const int MAX_Y = 100;
	const int MAX_NUM_SPOTS = 2;
	int start_radius = 10;
	int num_active_spots = 0;
	std::vector<Location> loc_list;
	Location createRandomLocation();
public:
	bool input = false;
	Game();
	void run();
	void startGame();
};