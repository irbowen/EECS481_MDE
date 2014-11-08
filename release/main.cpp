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

// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	std::thread glThread(glDriver);
	std::thread gameThread(startGame);
	gameThread.join();
	glThread.join();
	return 0;
}
