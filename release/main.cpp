//Isaac Bowen
//eecs481 fall2014

#include "game.h"

#include "stdafx.h"
#include <strsafe.h>
#include "DepthBasics.h"
#include "resource.h"
#include <thread>
#include <mutex>
#include <condition_variable>

void startGame(Game g) {
	g.startGame();
}

void startKinect(CDepthBasics kinect, HINSTANCE hInstance, int nCmdShow) {
	kinect.Run(hInstance, nCmdShow);
}

// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	int glDriver();
	CDepthBasics kinect;
	
	Game g(kinect);

	std::thread glThread(glDriver);
	std::thread gameThread(startGame, g);
	//std::thread kinectThread(startKinect, kinect, hInstance, nCmdShow);

	kinect.Run(hInstance, nCmdShow);

	gameThread.join();
	glThread.join();
	//kinectThread.join();

	return 0;
}
