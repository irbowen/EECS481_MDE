//Isaac Bowen
//eecs481 fall2014

#include <thread>
#include "game.h"

void gamez() {
	for (int i = 0; i < 2; i++) {
		std::cout << "Check out this other multithreading bro\n";
	}
}

void workz() {
	for (int i = 0; i < 2; i++) {
		std::cout << "Check on this multithreading brah\n";
	}
}

void startGame() {
	Game g;
	g.startGame();
}

int main(int argc, char* argv[]) {
	//testing the water things multithreading
	std::thread gT(gamez);
	std::thread wT(workz);
	std::thread gameT(startGame);
	gT.join();
	wT.join();
	gameT.join();
	return 0;
}