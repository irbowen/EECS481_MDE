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
	srand(5);
}

void Game::printRemainingLocations() {
	std::ostringstream ss;
	ss << "Remaing locations: ";
	for (auto it = loc_list.begin(); it != loc_list.end(); ++it) {
		if (it->isOn()) {
			ss << it->toString();
		}
	}
	std::cout << ss.str() << "\n";
}

void Game::printRemovedLocations() {
	std::ostringstream ss;
	ss << "Removed locations: ";
	for (auto it = loc_list.begin(); it != loc_list.end(); ++it) {
		if (!it->isOn()) {
			ss << it->toString();
		}
	}
	std::cout << ss.str() << "\n";
}

void Game::run() {
	std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLE_MILLISECONDS*10));
	for (int i = 0; i < NUM_ROUNDS; i++) {
		std::cout << "Currently on round " << i << std::endl;
		int count = 0;
		if (num_active_spots <= MAX_NUM_SPOTS) {
			loc_list.push_back(createRandomLocation());
			num_active_spots++;
		}
		for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {
			int x = loc_it->x;
			int y = loc_it->y;
			double pressure = frame_data.at(y*MAX_X + x);//check locations around this spot
			for (auto f_it = frame_data.begin(); f_it != frame_data.end(); ++f_it) {//prints out the fram
				//hard to see because cmd is only 80 chars wide!
		//		std::cout << *f_it << " ";
			}
			if (loc_it->isOn() && loc_it->withinPressure(pressure)) {//if the pressure is within the range
				//change color
				if (true) {//if it matches 'excatly'
					std::cout << "Matches at " << x << " " << y << " pressure: " << pressure  << std::endl;
					printRemainingLocations();
					printRemovedLocations();
					num_active_spots--;
					loc_it->turnOff();
				}
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
		for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {//Increase size of all existing locations
			loc_it->makeBigger(INCREASE_FACTOR);
			//redraw location
		}
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
		//bool valid=false;
	//	std::cout << "X and Y: " << x_location << " " << y_location << std::endl;
		for (auto loc_it = loc_list.begin(); loc_it != loc_list.end(); ++loc_it) {//now check that it doesn't overlap with any already created
			if (loc_it->isOn()) {
				double distance = loc_it->distance(x_location, y_location);
				//if (distance >= (radius + loc_it->getRadius()) {//to avoid overlap
					//valid=true;
					//break;
				//}
				if (distance < radius || distance < loc_it->getRadius()) {//overlaps with another location
					continue;
				}
			}
		}
	} while (abs(x_location - MAX_X) <= radius || abs(y_location - MAX_Y) <= radius);
	// while ( (x_location <= radius || abs(x_location - MAX_X) <= radius 
		//|| y_location <= radius || abs(y_location - MAX_Y) <= radius) || valid==false);
	Location randomLoc(x_location, y_location, radius, frame_data.at(MAX_X*y_location + x_location));
	//std::cout << "x,y: " << x_location << " " << y_location;
	return randomLoc;
}

void Game::startGame() {
	run();
}
