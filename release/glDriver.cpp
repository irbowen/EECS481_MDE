#include <chrono>
#include <iostream>
#include <cmath>

#include "graphics.h"
#include "glDriver.h"
#include "glDriver_share.h"
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

bool shift_last = false;
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

	
	Scene::fancyCursors.push_back({ 320, 240, 50, 5, {GradientCircleCursor{ 370, 240, 75, colorScheme_rainbow, 100 },
														GradientCircleCursor{ 345, 283, 75, { ORANGE, YELLOW, GREEN, BLUE, PURPLE, RED }, 100 },
														GradientCircleCursor{ 296, 283, 75, { YELLOW, GREEN, BLUE, PURPLE, RED, ORANGE }, 100 },
														GradientCircleCursor{ 271, 240, 75, { GREEN, BLUE, PURPLE, RED, ORANGE, YELLOW }, 100 },
														GradientCircleCursor{ 295, 197, 75, { BLUE, PURPLE, RED, ORANGE, YELLOW, GREEN }, 100 },
														GradientCircleCursor{ 344, 197, 75, { PURPLE, RED, ORANGE, YELLOW, GREEN, BLUE }, 100 } 
		/*GradientCircleCursor{ 0, 0, 24, { GREEN, palette(GREEN), palette(GREEN) }, 100 },
		//new GradientCircleCursor{ 0, 0, 24, { GREEN, palette(GREEN), palette(GREEN) }, 100 },
		GradientCircleCursor{ 0, 0, 24, { GREEN, palette(GREEN), palette(GREEN) }, 100 }*/
													} 
															});


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

						if (!ctrl_last && (Game::mode != '!')){
							Scene::locations.clear();
							Scene::connects.clear();
							Game::num_active_spots = 0;
							Game::mode = Game::mode == 's' ? 'k' : 's';
						}
							


						
						auto& cursor = Scene::fancyCursors[0];


						//cursor.addCircle();

						if (keys[VK_UP])
							cursor.chY(-10);
						else if (keys[VK_DOWN])
							cursor.chY(10);
						if (keys[VK_RIGHT])
							cursor.chX(10);
						else if (keys[VK_LEFT])
							cursor.chX(-10);
							
						
					}

					ctrl_last = keys[VK_CONTROL];


					
					if (keys[VK_SHIFT] && !shift_last)
						Scene::gameCursor.rotateScheme();
					


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