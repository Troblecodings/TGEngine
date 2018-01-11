#pragma once

#include "stdbase.hpp"
#include "Instance.hpp"
#include "Properties.hpp"
#include "Debug.hpp"
#include <string>

#ifdef _WIN32

#include <Windows.h>
#include <tchar.h>
#include "Win32Window.hpp"

#endif

extern uint32_t x, y, width, height, d_width, d_height;
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