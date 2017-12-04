#pragma once

#include "TGEngine.hpp"
#include "Instance.hpp"
#include "Properties.hpp"
#include "Debug.hpp"

#ifdef _WIN32

#include <Windows.h>
#include <tchar.h>

struct Window {
	HWND __impl_window;

	bool shouldclose();
};

#define GET_SIZE(x, y) const HWND hDesktop = GetDesktopWindow();\
RECT desktop;\
GetWindowRect(hDesktop, &desktop);\
x = desktop.right;\
y = desktop.bottom;

#endif

extern uint32_t x, y, width, height, d_width, d_height;
extern Window window;
extern VkSurfaceKHR surface;

/*
 * Creates a window based on the properties
 */
SINCE(0, 0, 1)
void createWindow(nio::Properties properties);

/*
 * Creates the window surface
 *   -> MUST BE CALLED AFTER createWindow and createInstance
 */
SINCE(0, 0, 1)
void createWindowSurface();

SINCE(0, 0, 1)
void destroyWindow();

/*
 * Don't use
 */
SINCE(0, 0, 1)
void getMonitor();