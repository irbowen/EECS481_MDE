//Isaac Bowen
//eecs481 fall2014

#include "game.h"
#include "DepthBasics.h"
#include "graphics.h"
#include <mutex>
#include <thread>

using std::mutex;

extern mutex LocationLock;

Game::Game() {
	srand(5);
}

Game::Game(CDepthBasics& kinect_in) {
	kinect = &kinect_in;
	srand(5);
}

//std::vector<Location> Game::loc_list;
//std::vector<Location> Scene::locations;

void Game::printRemainingLocations() {
	std::ostringstream ss;
	ss << "Remaing locations: ";
	for (const auto& it : Scene::locations) {
		if (it.isOn()) {
			ss << it.toString();
		}
	}
	std::cout << ss.str() << "\n";
}

void Game::printRemovedLocations() {
	std::ostringstream ss;
	ss << "Removed locations: ";
	for (const auto& it : Scene::locations) {
		if (!it.isOn()) {
			ss << it.toString();
		}
	}
	std::cout << ss.str() << "\n";
}

double Game::checkPressure(Location loc){
	int x = (int)loc.getX();
	int y = (int)loc.getY();
	int radius = (int)loc.getRadius();
	double pressure = frame_data.at(x + y*MAX_X);
	if (y - radius / 2 >= 0 && frame_data.at(x + (y - radius / 2)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y - radius / 2)*MAX_X);
	if (y - radius >= 0 && frame_data.at(x + (y - radius)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y - radius)*MAX_X);
	if (y + radius / 2 < MAX_Y && frame_data.at(x + (y + radius / 2)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y + radius / 2)*MAX_X);
	if (y + radius<MAX_Y && frame_data.at(x + (y + radius)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y + radius)*MAX_X);
	if (x - radius / 2 >= 0 && frame_data.at((x - radius / 2) + y*MAX_X) > pressure)
		pressure = frame_data.at((x - radius / 2) + y*MAX_X);
	if (x - radius >= 0 && frame_data.at((x - radius) + y*MAX_X) > pressure)
		pressure = frame_data.at((x - radius) + y*MAX_X);
	if (x + radius / 2 < MAX_X && frame_data.at((x + radius / 2) + y*MAX_X) > pressure)
		pressure = frame_data.at((x + radius / 2) + y*MAX_X);
	if (x + radius<MAX_X && frame_data.at((x + radius) + y*MAX_X) > pressure)
		pressure = frame_data.at((x + radius) + y*MAX_X);
	return pressure;
}

void Game::run() {
	while (!buffer_valid) {//sleep while kinect boots up
		std::cout << "waiting for buffer" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5*SAMPLE_MILLISECONDS));
	}
	for (int i = 0; i < NUM_ROUNDS; i++) {
		// Play background sound
		//PlaySound(TEXT("sound.wav"), NULL, SND_LOOP || SND_ASYNC);
		std::cout << "Currently on round " << i << std::endl;
		int count = 0;
		if (num_active_spots <= MAX_NUM_SPOTS) {
			Scene::locations.push_back(createRandomLocation());
			num_active_spots++;
		}


		
		LocationLock.lock();

		for (auto& loc_it : Scene::locations) {
			double pressure = checkPressure(loc_it);
			loc_it.setPressure(pressure);
			if (loc_it.isOn() && loc_it.withinPressure(pressure)) {//if the pressure is within the range
				if (loc_it.exactMatch(pressure)) {
					loc_it.num_rounds_correct++;
					loc_it.prev_correct_round = i;
					PlaySound(TEXT("jamesbond.wav"), NULL, SND_FILENAME || SND_ASYNC);//play a first sound
					if (loc_it.num_rounds_correct > 1) {
						// Stop background sound
						//PlaySound(NULL, 0, 0);
						PlaySound(TEXT("jamesbond.wav"), NULL, SND_FILENAME || SND_ASYNC);//play a second sound
						std::cout << "Matches at " << loc_it.getX() << " " << loc_it.getY() << " pressure: " << pressure << std::endl;
						printRemainingLocations();
						printRemovedLocations();
						num_active_spots--;
						loc_it.turnOff();
						loc_it.fade(1000);
						loc_it.num_rounds_correct = 0;
					}
					else if (i - loc_it.prev_correct_round > 1){
						loc_it.num_rounds_correct = 0;
					}
				}
			}
		}
		for (auto& loc_it : Scene::locations) {//Increase size of all existing locations
			loc_it.makeBigger(INCREASE_FACTOR);
			//redraw location
		}
		LocationLock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLE_MILLISECONDS));
	}
}

Location Game::createRandomLocation() {
	double radius = start_radius;
	int x_location, y_location;
	bool valid = false;
	do {//check to make sure its not off the screen
		x_location = rand() % MAX_X;
		y_location = rand() % MAX_Y;

		//	std::cout << "X and Y: " << x_location << " " << y_location << std::endl;
		if (Scene::locations.size() == 0) {
			valid = true;
		}
		// To detect overlap
		int count_on=0;
		int count_not=0;

		

		for (auto& loc_it : Scene::locations) {//now check that it doesn't overlap with any already created
			if (loc_it.isOn()) {
				count_on++;
				double distance = loc_it.distance(x_location, y_location);
				if (distance >= (radius + loc_it.getRadius())) {//to avoid overlap
					// check every circle 
					count_not++;
					//valid=true;
					//break;
				}
				//if (distance < radius || distance < loc_it->getRadius()) {//overlaps with another location
				//	continue;
				//}
			}
		}
		// If not overlap with all the circle, that is valid
		if (count_on==count_not){
			valid=true;
		}
		//} while (abs(x_location - MAX_X) <= radius || abs(y_location - MAX_Y) <= radius);
	} while ((x_location <= radius || abs(x_location - MAX_X) <= radius
		|| y_location <= radius || abs(y_location - MAX_Y) <= radius) || valid == false);

	std::cout << "x,y: " << x_location << " " << y_location;

	return Location(x_location, y_location, radius, frame_data.at(MAX_X*y_location + x_location));
}

void Game::startGame() {
	run();
}
