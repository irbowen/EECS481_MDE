//Isaac Bowen
//eecs481 fall2014

#include "game.h"
#include "DepthBasics.h"
#include "graphics.h"
#include <mutex>
#include <thread>
#include "color.h"
#include "location.h"
#include <fstream>
#include "LocPair.h"
#include "Scene.h"
#include <vector>
#include <queue>
#include <math.h>

#define MIN_BUBBLE_RADIUS 25
#define MAX_BUBBLE_RADIUS 50

#define CREATE_LOCATION_HIGHLIGHTER(x,y,r) RotatingMultiCursor{ x, y, r, 5, \
{\
	GradientCircleCursor{ 0, 0, 25, { GREEN, palette(GREEN), palette(GREEN) }, 100 },\
	GradientCircleCursor{ 0, 0, 25, { GREEN, palette(GREEN), palette(GREEN) }, 100 },\
	GradientCircleCursor{ 0, 0, 25, { GREEN, palette(GREEN), palette(GREEN) }, 100 }\
}\
}

static const vector<GradientCircleCursor> RAINBOW_CURSORS{ 
	GradientCircleCursor{ 370, 240, 75, colorScheme_rainbow, 100 },
	GradientCircleCursor{ 345, 283, 75, { ORANGE, YELLOW, GREEN, BLUE, PURPLE, RED }, 100 },
	GradientCircleCursor{ 296, 283, 75, { YELLOW, GREEN, BLUE, PURPLE, RED, ORANGE }, 100 },
	GradientCircleCursor{ 296, 283, 75, { GREEN, BLUE, PURPLE, RED, ORANGE, YELLOW }, 100 },
	GradientCircleCursor{ 296, 283, 75, { BLUE, PURPLE, RED, ORANGE, YELLOW, GREEN }, 100 },
	GradientCircleCursor{ 296, 283, 75, { PURPLE, RED, ORANGE, YELLOW, GREEN, BLUE }, 100 }
};

using std::mutex;

extern mutex LocationLock;

Color palette(Color& c);

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
#ifdef DEBUG
	std::cout << ss.str() << "\n";
#endif
}

void Game::printRemovedLocations() {
	std::ostringstream ss;
	ss << "Removed locations: ";
	for (const auto& it : Scene::locations) {
		if (!it.isOn()) {
			ss << it.toString();
		}
	}
#ifdef DEBUG
	std::cout << ss.str() << "\n";
#endif
}

double Game::checkPressure(int x, int y, int radius){
	double pressure = frame_data.at(x + y*MAX_X);
	if (y - radius / 2 >= 0 && frame_data.at(x + (y - radius / 2)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y - radius / 2)*MAX_X);
	if (y - radius / 4 >= 0 && frame_data.at(x + (y - radius / 4)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y - radius / 4)*MAX_X);
	if (y + radius / 2 < MAX_Y && frame_data.at(x + (y + radius / 2)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y + radius / 2)*MAX_X);
	if (y + radius / 4 < MAX_Y && frame_data.at(x + (y + radius / 4)*MAX_X) > pressure)
		pressure = frame_data.at(x + (y + radius / 4)*MAX_X);
	if (x - radius / 2 >= 0 && frame_data.at((x - radius / 2) + y*MAX_X) > pressure)
		pressure = frame_data.at((x - radius / 2) + y*MAX_X);
	if (x - radius / 4 >= 0 && frame_data.at((x - radius / 4) + y*MAX_X) > pressure)
		pressure = frame_data.at((x - radius / 4) + y*MAX_X);
	if (x + radius / 2 < MAX_X && frame_data.at((x + radius / 2) + y*MAX_X) > pressure)
		pressure = frame_data.at((x + radius / 2) + y*MAX_X);
	if (x + radius / 4 < MAX_X && frame_data.at((x + radius / 4) + y*MAX_X) > pressure)
		pressure = frame_data.at((x + radius / 4) + y*MAX_X);
	return pressure;
}

void Game::run(char mode) {

	while (!buffer_valid) {//sleep while kinect boots up
#ifdef DEBUG
		std::cout << "waiting for buffer" << std::endl;
#endif
		std::this_thread::sleep_for(std::chrono::milliseconds(5*SAMPLE_MILLISECONDS));

	}
	//intial_buffer = frame_data;

	/*
	std::ofstream out_file;
	out_file.open("out3.txt");
	for (int i = 0; i < MAX_Y; i++) {
		for (int j = 0; j < MAX_X; j++) {
			out_file << initial_buffer.at(i*MAX_X + j) << " ";
		}
		out_file << "\n";
	}
	out_file << std::endl;
	out_file.close();
	*/

	int i = 0;
	while (true) {
		// Play background sound
		//PlaySound(TEXT("sound.wav"), NULL, SND_LOOP || SND_ASYNC);
#ifdef DEBUG
		std::cout << "Currently on round " << i << std::endl;
#endif
		//Run Slide Ring Target Mode
		if (mode == 's') {
			runSlideRingMode(i);

		}
		//Run Kinect The Dots Mode
		else if (mode == 'k') {
			//Scene::locpairs.push_back(createRandomLocPair(50, 50, 300, 300));
			runConnectMode();
		}
		i++;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(SAMPLE_MILLISECONDS));
	}
}

void Game::runSlideRingMode(int i) {
	
	//Location random = createRandomLocation(5);

	LocationLock.lock();
	if ((num_active_spots <= num_triggered_spots/4) && (num_active_spots < 4)) {
		double r_scale = log(num_active_spots + 3);
#ifdef DEBUG
		std::cout << "Num active: " << num_active_spots << " " << r_scale << std::endl;
#endif
		Scene::locations.push_back(createRandomLocation(Location::MAX_RADIUS/++num_active_spots + 20));
	}
	//	double pressure = checkPressure((int)loc_it.getX(), (int)loc_it.getY(), (int)loc_it.getRadius());
	//	loc_it.setPressure(pressure);

	for (auto& loc_it : Scene::locations) {
		if (!loc_it.isOn()) { //off, don't bother
			continue;
		}

		double x = 0, y = 0, pressure = 0;




		for (const auto& pt : getCursorPoints()) {
			x = pt.first;
			y = pt.second;
			if (loc_it.contains(x, y)) { //if cursor is inside location
				pressure = frame_data.at((unsigned)(y*MAX_X + x));

				loc_it.setPressure(pressure);
				PlaySound(TEXT("bubbles_sfx.wav"), NULL, SND_FILENAME || SND_ASYNC || SND_NOSTOP);//play a first sound
				if (loc_it.exactMatch(pressure)) {
#ifdef DEBUG
					std::cout << "Triggered exact match\n";
#endif
					loc_it.num_rounds_correct++;
					loc_it.prev_correct_round = i;
					if (loc_it.num_rounds_correct > 3) {
						PlaySound(NULL, 0, 0); //killz background sound
						PlaySound(TEXT("applause3.wav"), NULL, SND_FILENAME || SND_ASYNC || SND_NOSTOP);//play a second sound
#ifdef DEBUG
						std::cout << "Matches at " << loc_it.getX() << " " << loc_it.getY() << " pressure: " << pressure << std::endl;
#endif
						loc_it.fade(1000);
						loc_it.turnOff();
						num_triggered_spots++;
						num_active_spots--;
						loc_it.num_rounds_correct = 0;

						// trigger rainbow bubbles
						Scene::targetHighlighters[loc_it.id].chCursors(RAINBOW_CURSORS);

						Scene::targetHighlighters[loc_it.id].exploding = true;
						Scene::targetHighlighters[loc_it.id].explodeStart = high_resolution_clock::now();

						break;

					}
					else if (i - loc_it.prev_correct_round > 1){
						loc_it.num_rounds_correct = 0;
					}
				}
			}
			else {
				loc_it.setPressure(loc_it.start_pressure);
			}

		}
		if (double progress = loc_it.getPercentage(pressure)){
			Scene::targetHighlighters[loc_it.id].setCircleRadius((int)(MIN_BUBBLE_RADIUS + progress*(MAX_BUBBLE_RADIUS - MIN_BUBBLE_RADIUS)));
			Scene::targetHighlighters[loc_it.id].addCircle();
		}
	}

	// handle growing in constructor & Location::draw instead.
	/*
	for (auto& loc_it : Scene::locations) {//Increase size of all existing locations
		if (loc_it.isOn()) {
			loc_it.makeBigger(INCREASE_FACTOR);
		}
		Scene::targetHighlighters[loc_it.id].setR((int)loc_it.getRadius());
	}
	*/

	LocationLock.unlock();
}

Location Game::createRandomLocation(double final_radius) {
	//std::cout << "Scale factor: " << radius_scale_factor << std::endl;
	int x_location, y_location;
	bool valid = false;

	do {//check to make sure its not off the screen
		x_location = rand() % MAX_X;
		y_location = rand() % MAX_Y;
		if (x_location <= final_radius || x_location >= (MAX_X - final_radius)) {
			continue;//bad location, do the loop again
		}
		if (y_location <= final_radius || y_location >= (MAX_Y - final_radius)) {
			continue;//bad location, do the loop again
		}
		if (initial_buffer.at(MAX_X*y_location + x_location) <= 0) {
#ifdef DEBUG
			std::cout << "The depth is: " << initial_buffer.at(MAX_X*y_location + x_location) << " at " << x_location << ", " << y_location << " " <<  std::endl;
#endif
			continue;
		}
		if (Scene::locations.size() == 0) {
			valid = true;
			break;
		}
		auto& loc_it = Scene::locations.begin();
		for (; loc_it != Scene::locations.end(); ++loc_it) {//now check that it doesn't overlap with any already created
			if (loc_it->isOn()) {
				double distance = loc_it->distance(x_location, y_location);
				if (distance < loc_it->endRadius*2) {
					break;//bad location, times 2 just to be safe
				}
			}
		}
		if (loc_it == Scene::locations.end()) {
			valid = true;//if no issues with other locatiosn, location is good
		}
	} while (!valid);
	//double new_radius = start_radius / radius_scale_factor;
	//std::cout << "New radius is: " << new_radius << std::endl;
	
	Location temp =  Location(x_location, y_location, final_radius, initial_buffer.at(MAX_X*y_location + x_location));
	highlightLock.lock();
	Scene::targetHighlighters.insert({ temp.id, CREATE_LOCATION_HIGHLIGHTER((double)x_location, (double)y_location, final_radius * 8 / 7) });
	highlightLock.unlock();

	return temp;

	/*while ((x_location <= max_radius || abs(x_location - MAX_X) <= max_radius
	|| y_location <= max_radius || abs(y_location - MAX_Y) <= max_radius) || valid == false
	|| initial_buffer.at(MAX_X*y_location + x_location) == 0);*/

	/*	if (distance >= (2 * max_radius)) {//to avoid overlap
	count_not++;

	}*/
	//if (distance < radius || distance < loc_it->getRadius()) {//overlaps with another location
	//	continue;
	//}
	// If not overlap with all the circle, that is valid
	/*	if (count_on == count_not){
	valid = true;
	}*/
	
}

Location Game::createLocation(int xx, int yy, double radius_scale_factor) {
#ifdef DEBUG
	std::cout << "Scale factor: " << radius_scale_factor << std::endl;
#endif
	int x_location, y_location;
	bool valid = false;

	do {//check to make sure its not off the screen
		x_location = xx;
		y_location = yy;
		if (x_location <= MAX_RADIUS || x_location >= (MAX_X - MAX_RADIUS)) {
			continue;//bad location, do the loop again
		}
		if (y_location <= MAX_RADIUS || y_location >= (MAX_Y - MAX_RADIUS)) {
			continue;//bad location, do the loop again
		}
		if (initial_buffer.at(MAX_X*y_location + x_location) <= 0) {
#ifdef DEBUG
			std::cout << "The depth is: " << initial_buffer.at(MAX_X*y_location + x_location) << " at " << x_location << ", " << y_location << " " << std::endl;
#endif
			continue;
		}
		if (Scene::locations.size() == 0) {
			valid = true;
			break;
		}
		auto& loc_it = Scene::locations.begin();
		for (; loc_it != Scene::locations.end(); ++loc_it) {//now check that it doesn't overlap with any already created
			if (loc_it->isOn()) {
				double distance = loc_it->distance(x_location, y_location);
				if (distance < loc_it->getRadius() * 3) {
					break;//bad location, times 2 just to be safe
				}
			}
		}
		if (loc_it == Scene::locations.end()) {
			valid = true;//if no issues with other locatiosn, location is good
		}
	} while (!valid);
	double new_radius = start_radius / radius_scale_factor;
#ifdef DEBUG
	std::cout << "New radius is: " << new_radius << std::endl;
#endif
	return Location(x_location, y_location, new_radius, initial_buffer.at(MAX_X*y_location + x_location));
}

void Game::startGame() {
	run('s');
}

/*
vector<Location> vecs;
//determine how many locations to create
if (vecs.size() == 1 || vecs.size() == 2) {
	//location is good
	//call constructor
}


bool bad_loc = true;
while (bad_loc) {
	if (vecs.size() == 3) {
		Location _new_location = createRandomLocation(Location::MAX_RADIUS/++num_active_spots + 20);
		auto first_loc_pair = std::make_pair(vecs.at(0).getX(), vecs.at(0).getY());
		auto second_loc_pair std::make_pair(vecs.at(1).getX(), vecs.at(1).getY())
		for (auto i = 0.0; i < 100.0; i++) {
			auto return_pair = between(first_loc_pair, second_loc_pair, i);
			if (_new_location.contains(return_pair.first, return_pair.second)) {
				bad_loc = true;
				break;
			}
		}
		if (i == 100) {
			bad_loc = false;
		}
	}
}
*/
