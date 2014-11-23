#include <chrono>
#include <iostream>
#include <cmath>

#include "graphics.h"
#include "glDriver.h"
#include "game.h"

using namespace std::chrono;

using std::milli;

using std::cout;
using std::endl;

HGLRC hRC = NULL;
HDC hDC = NULL;
HWND hWnd = NULL;
HINSTANCE hInstance;

bool active = true;
bool keys[256];

bool fullscreen = FULLSCREEN;

static bool shift_last = false;
static bool ctrl_last = false;

int glDriver(){
	MSG msg;
	BOOL done = FALSE;

	if (!CreateGLWindow("OpenGL framework", XRES, YRES, 16, fullscreen))
		return 0;

	FPSregulator timer(FPS);


	// Check hardware support for openGL line width
	/*
	GLfloat LineRange[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, LineRange);
	std::cout << "Minimum Line Width " << LineRange[0] << " -- ";
	std::cout << "Maximum Line Width " << LineRange[1] << std::endl;
	*/

	// Line ctor : Line(p1, p2, color, thickness)
	// Scene::lines.push_back({ {100,100}, {400,400}, BLUE, 5.0 });

	for (int i = 0; i < 360; i += 60){
		int d = 50;
		double theta = i * 3.14 / 180;

		int dx = d * cos(theta);
		int dy = d * sin(theta);

		cout << 320 + dx << ' ' << 240 + dy << endl;
	}

	Scene::cursors.cs.push_back(new GradientCircleCursor{ 370, 240, 75, colorScheme_rainbow, 100 });
	Scene::cursors.cs.push_back(new GradientCircleCursor{ 345, 283, 75, { ORANGE, YELLOW, GREEN, BLUE, PURPLE, RED }, 100 });
	Scene::cursors.cs.push_back(new GradientCircleCursor{ 296, 283, 75, { YELLOW, GREEN, BLUE, PURPLE, RED, ORANGE }, 100 });
	Scene::cursors.cs.push_back(new GradientCircleCursor{ 271, 240, 75, { GREEN, BLUE, PURPLE, RED, ORANGE, YELLOW }, 100 });
	Scene::cursors.cs.push_back(new GradientCircleCursor{ 295, 197, 75, { BLUE, PURPLE, RED, ORANGE, YELLOW, GREEN }, 100 });
	Scene::cursors.cs.push_back(new GradientCircleCursor{ 344, 197, 75, { PURPLE, RED, ORANGE, YELLOW, GREEN, BLUE }, 100 });


	while (!done){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT)
				done = TRUE;
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			if (active){

				if (keys[VK_ESCAPE])
					done = TRUE;
				else if (timer.frameReady()) {
	
					if (keys[VK_CONTROL]){

						ctrl_last = keys[VK_CONTROL];

						for (auto& cursor : Scene::cursors.cs){

							Scene::cursors.addCircle(&cursor - &*Scene::cursors.cs.begin());

							if (keys[VK_UP])
								cursor->chY(-10);
							else if (keys[VK_DOWN])
								cursor->chY(10);
							if (keys[VK_RIGHT])
								cursor->chX(10);
							else if (keys[VK_LEFT])
								cursor->chX(-10);

						}
					}

					/*
					if (keys[VK_SHIFT] && !shift_last)
						Scene::cursors[0].rotateScheme();
					*/


					shift_last = keys[VK_SHIFT];

					DrawGLScene();
					SwapBuffers(hDC);
				}

			}
		}
	}

	KillGLWindow();
	return (msg.wParam);
}