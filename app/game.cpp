//Isaac Bowen
//eecs481 fall2014

#include "game.h"

Game::Game() {
	//empty for now
}

void Game::run() {
	for (int i = 0; i < NUM_ROUNDS; i++) {
		Location loc = createRandomLocation();
		int count = 0;
		while (true) {
			if (input) {//test for input from kinect
				//get x y z information
				//draw image
				count = 0;
			}
			else {
				loc.makeBigger(INCREASE_FACTOR);
				count++;
				//redraw image
				if (count > BREAK_FACTOR) {//it its too hard, just draw another
					break;
				}
			}
			Sleep(1000);//Sleep for x number of miliseconds.  slow down the loop, dont sample kinect too much
		}
	}
}

Location Game::createRandomLocation() {
	int radius = start_radius;
	int x_location, y_location;
	do {
		x_location = rand() % MAX_X;
		y_location = rand() % MAX_Y;
	} while (abs(x_location - MAX_X) >= radius || (abs(y_location - MAX_Y >= radius)));
	std::cout << "The final x: " << x_location;
	std::cout << "The final y: " << y_location;
	int dummy;
	std::cin >> dummy;
	Location loc(x_location, y_location, radius);
	return loc;
}

void Game::startGame() {
	run();
}
