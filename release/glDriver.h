#ifndef GLDRIVER_H
#define GLDRIVER_H
#include <gl\glew.h>
#include <Windows.h>
#include "glDriver_share.h"
#include "Scene.h"


#define XRES 640	
#define YRES 480

#define FPS 60

#define FULLSCREEN true

extern HGLRC hRC;
extern HDC hDC;
extern HWND hWnd;
extern HINSTANCE hInstance;


extern bool active;


extern bool fullscreen;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int InitGL(GLvoid){
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClearDepth(1.0f);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	return TRUE;
}

GLvoid KillGLWindow(GLvoid){
	if (fullscreen){
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	if (hRC){
		if (!wglMakeCurrent(NULL, NULL)){
			MessageBox(NULL, L"Release of DC and RC Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC)){
			MessageBox(NULL, L"Release Rendering Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;
	}

	if (hDC && !ReleaseDC(hWnd, hDC)){
		MessageBox(NULL, L"Release Device Context Failed.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	if (hWnd && !DestroyWindow(hWnd)){
		MessageBox(NULL, L"Could Not Release hWnd.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}

	if (!UnregisterClass(L"OpenGL", hInstance)){
		MessageBox(NULL, L"Could Not Unregister Class.", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}


GLvoid ReSizeGLScene(GLsizei width, GLsizei height){
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag){
	GLuint PixelFormat;

	WNDCLASS wc;

	DWORD dwExStyle;
	DWORD dwStyle;

	RECT WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

	fullscreen = fullscreenflag;

	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"OpenGL";

	if (!RegisterClass(&wc)){
		MessageBox(NULL, L"Failed to register the window class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (fullscreen){
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
			if (MessageBox(NULL, L"The requested fullscreen mode is not supported by\n your video card. use windowed mode instead?", L"NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				fullscreen = false;
			else{
				MessageBox(NULL, L"Program will now close.", L"ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	if (fullscreen){
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(FALSE);
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);


	if (!(hWnd = CreateWindowExA(dwExStyle,
		"OpenGL",
		title,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
		dwStyle,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		hInstance,
		NULL))){

		KillGLWindow();
		MessageBox(NULL, L"Window creation error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if (!(hDC = GetDC(hWnd))){
		KillGLWindow();
		MessageBox(NULL, L"Can't create a gl device context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))){
		KillGLWindow();
		MessageBox(NULL, L"Can't fiend a suitable pixel format.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd)){
		KillGLWindow();
		MessageBox(NULL, L"Can't set the pixel format", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC))){
		KillGLWindow();
		MessageBox(NULL, L"Can't create a GL rendering context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC)){
		KillGLWindow();
		MessageBox(NULL, L"Can't activate the GL rendering context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ReSizeGLScene(width, height);

	if (!InitGL()){
		KillGLWindow();
		MessageBox(NULL, L"Initialization failed.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;

}

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam){

	switch (uMsg){
	case WM_ACTIVATE: {
		if (!HIWORD(wParam))
			active = TRUE;
		else
			active = FALSE;

		return 0;
	}
	case WM_SYSCOMMAND: {
		switch (wParam){
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}
	case WM_CLOSE: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN: {
		keys[wParam] = TRUE;
		return 0;
	}
	case WM_KEYUP: {
		keys[wParam] = FALSE;
		return 0;
	}
	case WM_SIZE: {
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




// Regulate fps using timer
class FPSregulator {
	high_resolution_clock::time_point last;
	double millsPerFrame;
public:
	FPSregulator(int fps) {
		last = high_resolution_clock::now();
		millsPerFrame = 1.0 / fps * 1000;
	}
	// Milliseconds since last frame (or construction)
	inline bool frameReady() {
		double elapsed = duration_cast<duration<double, milli>>(high_resolution_clock::now() - last).count();
		if (elapsed >= millsPerFrame){
			last = high_resolution_clock::now();
			return true;
		}
		return false;
	}
};

int DrawGLScene(GLvoid){
	glClear(GL_COLOR_BUFFER_BIT);

	const int xSize = XRES, ySize = YRES;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, xSize, ySize, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);

	// DO DRAWING

	Scene::draw();

	return TRUE;
}
#endif