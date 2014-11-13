#include <vector>
#include <chrono>
#include <thread>
#include <time.h>
//For playing sound
//#include "Windows.h"
//#include "Mmsystem.h"
#include "DepthBasics.h"
#include "graphics.h"

class Game {
	const int NUM_ROUNDS = 1000;
	const int INCREASE_FACTOR = 1;
	const int BREAK_FACTOR = 20;
	const int MAX_X = 640;
	const int MAX_Y = 480;
	const int MAX_NUM_SPOTS = 20;
	const int SAMPLE_MILLISECONDS = 100;
	double start_radius = 5;
	int num_active_spots = 0;
	std::vector<Location> loc_list;
	Location createRandomLocation();
	CDepthBasics* kinect;
public:
	Game();
	Game(CDepthBasics& kinect);
	void run();
	void startGame();
	void startKinect();
};
