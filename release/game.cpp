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

#define MIN_BUBBLE_RADIUS 25
#define MAX_BUBBLE_RADIUS 50

//static vector<RandomCircle

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
		std::cout << "waiting for buffer" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(5*SAMPLE_MILLISECONDS));
	}
	//intial_buffer = frame_data;
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
	int i = 0;
	while (true) {
		// Play background sound
		//PlaySound(TEXT("sound.wav"), NULL, SND_LOOP || SND_ASYNC);
		std::cout << "Currently on round " << i << std::endl;
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
	LocationLock.lock();
	if (num_active_spots <= log(num_triggered_spots + 2)) {
		Scene::locations.push_back(createRandomLocation(-1, -1));
		auto& last = Scene::locations.back();
		last.target.setR(last.target.getR() * (1 / log(num_triggered_spots+2)));
		num_active_spots++;
	}
	cursorLock.lock();
	Scene::debugCursors.clear();
	for (int j = 0; j < minDepth_index.size(); j++){
		Scene::debugCursors.push_back({minDepth_index[j] % 640, minDepth_index[j] / 640, 20});
	}
	cursorLock.unlock();
	for (auto& loc_it : Scene::locations) {
		double pressure = checkPressure((int)loc_it.getX(), (int)loc_it.getY(), (int)loc_it.getRadius());
		loc_it.setPressure(pressure);

		if (double progress = loc_it.getPercentage(pressure)){
			Scene::targetHighlighters[loc_it.id].setCircleRadius(MIN_BUBBLE_RADIUS + progress*(MAX_BUBBLE_RADIUS - MIN_BUBBLE_RADIUS));
			Scene::targetHighlighters[loc_it.id].addCircle();
		}

		if (loc_it.isOn() && loc_it.withinPressure(pressure)) {//if the pressure is within the range
			std::cout << "Within pressure\n";
			if (loc_it.exactMatch(pressure)) 
			{
				loc_it.num_rounds_correct++;
				loc_it.prev_correct_round = i;
				PlaySound(TEXT("jamesbond.wav"), NULL, SND_FILENAME || SND_ASYNC);//play a first sound
				if (loc_it.num_rounds_correct > 1) {
					// Stop background sound
					//PlaySound(NULL, 0, 0);
					PlaySound(TEXT("jamesbond.wav"), NULL, SND_FILENAME || SND_ASYNC);//play a second sound
					std::cout << "Matches at " << loc_it.getX() << " " << loc_it.getY() << " pressure: " << pressure << std::endl;
				//	printRemainingLocations();
					//printRemovedLocations();
					num_active_spots--;
					loc_it.turnOff();
					loc_it.fade(1000);

					loc_it.num_rounds_correct = 0;
					num_triggered_spots++;
				}
			}
			else if (i - loc_it.prev_correct_round > 1){
				loc_it.num_rounds_correct = 0;
			}
		}
	}
	for (auto& loc_it : Scene::locations) {//Increase size of all existing locations
		loc_it.makeBigger(INCREASE_FACTOR);
		//redraw location
	}
	LocationLock.unlock();
}

double Game::dist(int x1, int y1, int x2, int y2)
{
	int dist_x = x2 - x1;
	int dist_y = y2 - y1;

	double length = sqrt((dist_x*dist_x) + (dist_y*dist_y));

	return length;
}

bool Game::on_line(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if ((dist(x3, y3, x1, y1) + dist(x3, y3, x2, y2) - dist(x1, y1, x2, y2)) / dist(x1, y1, x2, y2) < 0.1)
		return true;
	else
		return false;
}

bool Game::line(LocPair& locpair)
{
	const int MAX_X = 640;
	const int MAX_Y = 480;
	ColorSlideRing &loc1 = locpair.start;
	ColorSlideRing &loc2 = locpair.destination;
	int last_x = (int)loc1.getX();
	int last_y = (int)loc1.getY();
	int x1 = (int)loc1.getX();
	int y1 = (int)loc1.getY();
	int x2 = (int)loc2.getX();
	int y2 = (int)loc2.getY();

	// draw current track and check whether the line is finished
	while (dist(last_x, x2, last_y, y2) - (int)loc2.getR()>0){
		// Assume minDepth is the cursor

		//get cursor location (400,400) is example for now
		int cursor_x = 400;
		int cursor_y = 400;

		int x = cursor_x % MAX_X; // column
		int y = cursor_y / MAX_X; // row

		if (on_line(x1, y1, x2, y2, x, y))
		{
			//if(dist(x,y,x2,y2)<dist(last_x,last_y,x2,y2)){
			last_x = x;
			last_y = y;
			//OpenGL draw line
			Scene::lines.push_back({ { x1, y1 }, { last_x, last_y }, RED, 5.0 });
			//}
		}
		else
			return false;
	}
	return true;
}


void Game::runConnectMode()
{
	//Scene::locpairs.push_back(createRandomLocPair(50, 50, 300, 300));
	Scene::locpair = createRandomLocPair(50, 50, 300, 300);

		
		//1. check pressure at start ring
		
		double pressure = checkPressure((int)Scene::locpair.start.getX(), (int)Scene::locpair.start.getY(), (int)Scene::locpair.start.getR());

		std::cout << "Pressure is " << pressure << std::endl;
		//2. check if start ring is "locked-in" (ready to draw the line)
		//		a. if start ring is not locked in keep checking for locked in
		while (!Scene::locpair.withinPressure(pressure))
		{
			//std::cout << "pressure req not met" << std::endl;
			pressure = checkPressure((int)Scene::locpair.start.getX(), (int)Scene::locpair.start.getY(), (int)Scene::locpair.start.getR());
			//std::cout << "Inside pressure is " << pressure << std::endl;
		}

		Scene::locpair.locked = true;
		Scene::lines.push_back({ { (int)Scene::locpair.start.getX(), (int)Scene::locpair.start.getY() }, 
		{ (int)Scene::locpair.destination.getX(), (int)Scene::locpair.destination.getX() }, RED, 5.0 });
		//3. if start ring is locked in keep track of the cursor (Ara's Hand)
		//		a. 
		if (Scene::locpair.locked == true) 
		{
			int i = 0;
			while (line(Scene::locpair))
			{
				if (i++ > 10) break;
			}
		}
}

LocPair Game::createRandomLocPair(int opt_x1, int opt_y1, int opt_x2, int opt_y2)
{
	int start_x, start_y, dest_x, dest_y;

	start_x = rand() % MAX_X;
	start_y = rand() % MAX_Y;

	dest_x = rand() % MAX_X;
	dest_y = rand() % MAX_Y;

	if (opt_x1 > -1 && opt_y1 > -1 && opt_x2 > -1 && opt_y2 > -1)
	{
		start_x = opt_x1;
		start_y = opt_y1;

		dest_x = opt_x2;
		dest_y = opt_y2;
	}

	return LocPair(start_x, start_y, dest_x, dest_y, start_radius, initial_buffer.at(MAX_X*start_y + start_x));
}

Location Game::createRandomLocation(int opt_x, int opt_y) {
	double radius = start_radius;
	int x_location, y_location;
	bool valid = false;
	Location temp(0,0,0,0);
	int max_radius = (int)temp.MAX_RADIUS;

	
	do {//check to make sure its not off the screen
		x_location = rand() % MAX_X;
		y_location = rand() % MAX_Y;

		if (opt_x > -1 && opt_y > -1)
		{
			x_location = opt_x;
			y_location = opt_y;
		}

		//	std::cout << "X and Y: " << x_location << " " << y_location << std::endl;
		if (Scene::locations.size() == 0) {
			valid = true;
		}
		// To detect overlap
		int count_on = 0;
		int count_not = 0;


		for (auto& loc_it : Scene::locations) {//now check that it doesn't overlap with any already created
			if (loc_it.isOn()) {
				count_on++;
				double distance = loc_it.distance(x_location, y_location);
				if (distance >= (2 * max_radius)) {//to avoid overlap
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
		if (count_on == count_not){
			valid = true;
		}
		//} while (abs(x_location - MAX_X) <= radius || abs(y_location - MAX_Y) <= radius);
	} while ((x_location <= max_radius || abs(x_location - MAX_X) <= max_radius
		|| y_location <= max_radius || abs(y_location - MAX_Y) <= max_radius) || valid == false
		|| initial_buffer.at(MAX_X*y_location + x_location) == 0);

	std::cout << "x,y: " << x_location << " " << y_location;

	return Location(x_location, y_location, radius, initial_buffer.at(MAX_X*y_location + x_location));
	
}

void Game::startGame() {
	run('s');
}
