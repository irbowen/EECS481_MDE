#include <chrono>
#include <iostream>

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


	Scene::cursors.push_back(GradientCircleCursor{ 320, 240, 75, colorScheme_rainbow, 100});

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


						for (int i = 0; i < 5; ++i)
							Scene::cursors[0].addCircle();

						if (keys[VK_UP])
							Scene::cursors[0].chY(-10);
						else if (keys[VK_DOWN])
							Scene::cursors[0].chY(10);
						if (keys[VK_RIGHT])
							Scene::cursors[0].chX(10);
						else if (keys[VK_LEFT])
							Scene::cursors[0].chX(-10);
					}

					if (keys[VK_SHIFT] && !shift_last)
						Scene::cursors[0].rotateScheme();

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