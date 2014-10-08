//Isaac Bowen
//eecs481 fall2014

#include <iostream>

class Game {
	const int NUM_ROUNDS = 10;
	const int INCREASE_FACTOR = 2;
	const int BREAK_FACTOR = 20;
	bool error;
public:
	Game();
	void run(int);
	void startGame();
	bool createRandomLocation();
	bool successfulExit();
};