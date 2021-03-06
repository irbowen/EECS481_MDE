//------------------------------------------------------------------------------
// <copyright file="DepthBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "resource.h"
#include "Windows.h"
#include "NuiApi.h"
#include "ImageRenderer.h"
#include "global.h"
#include <vector>
#include <mutex>
#include <atomic>

using std::atomic;

extern std::vector<int> frame_data;
extern std::vector<int> initial_buffer;
//extern int max_depth;
//extern int max_index;
extern bool buffer_valid;
extern const int minDepth;
extern std::mutex cursorLock;
extern atomic<bool> newFrameReady;
extern bool fabricPulled;
extern bool fabricPulledLast;
bool pullRegistered();
extern int pull_index;

class CDepthBasics
{
    static const int        cDepthWidth  = 640;
    static const int        cDepthHeight = 480;
    static const int        cBytesPerPixel = 4;

    static const int        cStatusMessageMaxLen = MAX_PATH*2;

	//NUI_DEPTH_IMAGE_PIXEL * frame_data = NULL;

	//std::vector<double> frame_data;
	//double frame_data[307200];

public:
    /// <summary>
    /// Constructor
    /// </summary>
    CDepthBasics();

    /// <summary>
    /// Destructor
    /// </summary>
    ~CDepthBasics();

    /// <summary>
    /// Handles window messages, passes most to the class instance to handle
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Handle windows messages for a class instance
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Creates the main window and begins processing
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
    int                     Run(HINSTANCE hInstance, int nCmdShow);
	//std::vector<double> getframe();

private:
    HWND                    m_hWnd;

    bool                    m_bNearMode;

    // Current Kinect
    INuiSensor*             m_pNuiSensor;

    // Direct2D
    ImageRenderer*          m_pDrawDepth;
    ID2D1Factory*           m_pD2DFactory;
    
    HANDLE                  m_pDepthStreamHandle;
    HANDLE                  m_hNextDepthFrameEvent;

    BYTE*                   m_depthRGBX;

    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Create the first connected Kinect found 
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 CreateFirstConnected();

    /// <summary>
    /// Handle new depth data
    /// </summary>
    void                    ProcessDepth();

    /// <summary>
    /// Set the status bar message
    /// </summary>
    /// <param name="szMessage">message to display</param>
    void                    SetStatusMessage(WCHAR* szMessage);

};