//Isaac Bowen
//eecs481 fall2014
//Base class for running the game
#include <iostream>

class Game {
	const int NUM_ROUNDS = 10;
	const int INCREASE_FACTOR = 2;
	int num_locations;
public:
	Game(int);
	void run(int);
	void startGame();
	bool createRandomLocation();
};