#pragma once

#include "TGEngine.hpp"
#include "Instance.hpp"
#include "Properties.hpp"
#include "Debug.hpp"

extern uint32_t x, y, width, height;
extern GLFWmonitor* monitor;
extern GLFWwindow* window;

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