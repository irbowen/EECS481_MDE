//Isaac Bowen
//eecs481 fall2014

#include "game.h"
#include "DepthBasics.h"
#include "graphics.h"

Game::Game() {
	srand(5);
}

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
			vector<double> pressure_buffer = kinect->getframe();
			for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {
				int x = loc_it->x;
				int y = loc_it->y;
				std::cout << "x: " << x << " y " << y << std::endl;
				double pressure = pressure_buffer.at(y*MAX_X+x);
				//double pressure = 0;
				if (loc_it->withinPressure(pressure)) {
					//	loc_it->changeColorByPercentage();
					num_active_spots--;
					//if (loc_it->perfectPressure(pressure)) {
					loc_it->turnOff();
					//}
					//LIGHTS, SOUNDS, POINTS
					//1.  Include the following header files in this order: 
						//#include "windows.h", #include "mmsystem.h"
					//2.  follow the following steps to add winmm.lib to the linker (assuming Visual Studio 2010):
						//a.  Right click the project name in the Solution Explorer and select "Property".
						//b. On the left pane of the Property window, select "Linker" and then "Input"
						//c. On the right pane, type winmm.lib in the "Additional Dependencies" row.
						//d.  Click "Apply" and then "OK".
					//PlaySound(TEXT("sound.wav"), NULL, SND_FILENAME);
				}
			}
			//REDRAW
			count = 0;
		}
		for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {
			loc_it->makeBigger(INCREASE_FACTOR);
		}
		count++;
		//REDRAW
		if (count > BREAK_FACTOR) {//it its too hard, just draw another
			break;
		}
		std::cout << "On pass " << count << std::endl;
		//Sleep for x number of miliseconds.  slow down the loop, dont sample kinect too much
		std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLE_MILLISECONDS));
	}
}

Location Game::createRandomLocation() {
	double radius = start_radius;
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
	std::cout << "x,y: " << x_location << " " << y_location;
	return randomLoc;
}

void Game::startGame() {
	run();
}
