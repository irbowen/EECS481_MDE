#include "game.h"

Game::Game(int num) {
	num_locations = num;
}

void Game::run(int num_players) {
	if (num_players == 1) {
		//set some values
	}
	else if (num_players == 2) {
		//set other values
	}
	else {
		std::cout << "Incorrect number of arguments";
		std::exit(1);
	}
	//wait for action
	//determine result
	//repeat?
	for (int i = 0; i < NUM_ROUNDS; i++) {
		//display location on screen
		createRandomLocation();
		bool no_input = true;
		while (no_input) {
			Location.makeBigger(INCREASE_FACTOR);
			pause();
			testForInput();
			//after some number of pauses, break and try a new one
		}
		if (no_input == false) {
			std::cout << "Congrats!";
			//Fancy visuals
		}
	}
}

void Game::startGame() {
	//intialize values
	//test if kinect is up
	//test to display something
	//determine one or two player
	//this.run();
}