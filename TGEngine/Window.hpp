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
void createWindow(nio::Properties properties);

/*
 * Creates the window surface
 *   -> MUST BE CALLED AFTER createWindow and createInstance
 */
void createWindowSurface();

void destroyWindow();

/*
 * Don't use
 */
void getMonitor();