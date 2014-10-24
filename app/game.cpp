//Isaac Bowen
//eecs481 fall2014

#include "game.h"

Game::Game() {
	//empty for now
}

void Game::run() {
	for (int i = 0; i < NUM_ROUNDS; i++) {
		int count = 0;
		if (num_active_spots <= MAX_NUM_SPOTS) {
			loc_list.push_back(createRandomLocation());
		}
		while (true) {
			if (input) {//test for input from kinect
				//get x y z information
				int x, y, z;
				x = y = z = 0;
				for (Location loc : loc_list) {
					if (loc.contains(x, y) && loc.withinPressure(z)) {
						//LIGHTS, SOUNDS, POINTS
					}
				}
				//REDRAW
				count = 0;
			}
			else {
				for (Location loc : loc_list) {
					loc.makeBigger(INCREASE_FACTOR);
				}
				count++;
				//REDRAW
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
	do {//check to make sure its not off the screen
		x_location = rand() % MAX_X;
		y_location = rand() % MAX_Y;
		for (Location loc : loc_list) {//now check that it doesn't overlap with any already created
			double distance = loc.distance(x_location, y_location);
			if (distance < radius || distance < loc.r) {
				continue;
			}
		}
	} while (abs(x_location - MAX_X) >= radius || (abs(y_location - MAX_Y >= radius)));
	Location randomLoc(x_location, y_location, radius);
	return randomLoc;
}

void Game::startGame() {
	run();
}
