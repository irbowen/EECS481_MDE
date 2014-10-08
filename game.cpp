//Isaac Bowen
//eecs481 fall2014

#include "game.h"

Game::Game() {
	error = false;
	std::cout << "Game object created";
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
	for (int i = 0; i < NUM_ROUNDS; i++) {
		//display location on screen
		Location display_location = createRandomLocation();
		bool no_input = true;
		//wait for action
		for (int count = 0; no_input; count++) {
			//gradually make the circle larger while its not being hit
			Location.makeBigger(INCREASE_FACTOR);
			if (testForInput()) {
				int x = getXInput();
				int y = getYInput();
				int z = getZInput();
				if (display_location.contains(x, y)) {
					no_input = false;
					break;
				}
			}
			pause(); //slow the loop down if sample from kinect/makeymakey is too high
			if (count > BREAK_FACTOR) {
				break;
			}
		}
		if (no_input == false) {
			std::cout << "Congrats!";
			awardPoints();
			playLights();
			Colors();
			Sounds();
			//Fancy visuals
		}
	}
}

void Game::startGame() {
	//intialize values
	//test if kinect & projector are good
	//determine one or two player
	int num_players = Video.displayOneOrTwoPlayer();
	run(num_players);
}

bool Game::successfulExit() {
	return !error;
}