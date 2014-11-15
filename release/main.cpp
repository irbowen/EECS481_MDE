//Isaac Bowen
//eecs481 fall2014

#include "game.h"
#include "DepthBasics.h"
#include "Resource.h"
#include "stdafx.h"
#include <strsafe.h>
#include <thread>

void startGame(Game g) {
	g.startGame();
}

void startKinect(CDepthBasics kinect, HINSTANCE hInstance, int nCmdShow) {
	kinect.Run(hInstance, nCmdShow);
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	int glDriver();
	CDepthBasics kinect;
	Game g(kinect);

	std::thread glThread(glDriver), gameThread(startGame, g);


	kinect.Run(hInstance, nCmdShow);
	gameThread.join();
	glThread.join();
	//kinectThread.join();

	return 0;
}
