//Isaac Bowen
//eecs481 fall2014

#include "game.h"

void startGame() {
	Game g;
	g.startGame();
}

int main(int argc, char* argv[]) {
	std::thread gameT(startGame);
	gameT.join();
	return 0;
}