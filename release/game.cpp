//Isaac Bowen
//eecs481 fall2014

#include "game.h"
#include "DepthBasics.h"



Game::Game(CDepthBasics& kinect_in) {
	kinect = &kinect_in;
	//std::thread kinectThread(startKinect);
	srand(5);
}

void Game::run() {
	for (int i = 0; i < NUM_ROUNDS; i++) {
		std::cout << "Currently on round " << i << std::endl;
		int count = 0;
		if (num_active_spots <= MAX_NUM_SPOTS) {
			loc_list.push_back(createRandomLocation());
			num_active_spots++;
		}
		while (true) {
			vector<vector<double>> pressure_buffer = kinect->getframe();
			for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {
				double x = loc_it->x;
				double y = loc_it->y;
				double pressure = pressure_buffer.at(x).at(y);
				if (loc_it->withinPressure(pressure)) {
				//	loc_it->changeColorByPercentage();
					num_active_spots--;
					//if (loc_it->perfectPressure(pressure)) {
						loc_it->turnOff();
					//}
					//LIGHTS, SOUNDS, POINTS
				}
			}
			//REDRAW
			count = 0;
		}
		else {
			for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {
				loc_it->makeBigger(INCREASE_FACTOR);
			}
			count++;
			//REDRAW
			if (count > BREAK_FACTOR) {//it its too hard, just draw another
				break;
			}
			std::cout << "On pass " << count << std::endl;
		}
		//Sleep for x number of miliseconds.  slow down the loop, dont sample kinect too much
		std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLE_MILLISECONDS));
}

Location Game::createRandomLocation() {
	int radius = start_radius;
	int x_location, y_location;
	do {//check to make sure its not off the screen
		x_location = rand() % MAX_X;
		y_location = rand() % MAX_Y;
		std::cout << "X and Y: " << x_location << " " << y_location << std::endl;
		for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {//now check that it doesn't overlap with any already created
			if (loc_it->isOn()) {
				double distance = loc_it->distance(x_location, y_location);
				if (distance < radius || distance < loc_it->getRadius()) {
					//std::cout << "Overlap with previous location" << std::endl;
					continue;
				}
			}
		}
	} while (abs(x_location - MAX_X) <= radius || abs(y_location - MAX_Y) <= radius);
	Location randomLoc(x_location, y_location, radius, 5);
	return randomLoc;
}

void Game::startGame() {
	run();
}
