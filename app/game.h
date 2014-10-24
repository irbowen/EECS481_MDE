//Isaac Bowen
//eecs481 fall2014

#include <iostream>
#include <windows.h>
#include "location.h"

class Game {
	const int NUM_ROUNDS = 10;
	const int INCREASE_FACTOR = 2;
	const int BREAK_FACTOR = 20;
	const int MAX_X = 100;
	const int MAX_Y = 100;
	int start_radius = 10;
	bool error;
	Location createRandomLocation();
public:
	bool input = false;
	Game();
	void run();
	void startGame();
};